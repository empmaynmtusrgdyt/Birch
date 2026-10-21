#include "core/Renderer.h"

#include <QBuffer>
#include <QFont>
#include <QFontDatabase>
#include <QFontMetricsF>
#include <QHash>
#include <QPainter>
#include <QScopeGuard>
#include <QSvgRenderer>
#include <QtMath>

#include <cmath>
#include <mutex>
#include <utility>

#include <gvc.h>
#include <gvplugin.h>
#include <gvplugin_textlayout.h>

#if defined(BIRCH_GRAPHVIZ_BUILTIN_PLUGINS)
// Плагины слинкованы напрямую: регистрируем их, без поиска config в рантайме.
// На Windows это данные из DLL — их нужно явно импортировать.
#if defined(_WIN32)
#define BIRCH_GV_IMPORT __declspec(dllimport)
#else
#define BIRCH_GV_IMPORT
#endif
extern "C" {
BIRCH_GV_IMPORT extern gvplugin_library_t gvplugin_dot_layout_LTX_library;
BIRCH_GV_IMPORT extern gvplugin_library_t gvplugin_core_LTX_library;
}
#endif

namespace birch {

namespace {

// Graphviz хранит глобальное состояние (в т.ч. обработчик ошибок) — один рендер за раз.
std::mutex& graphvizMutex() {
    static std::mutex m;
    return m;
}

// Строковые параметры cgraph объявлены как char*, хотя не изменяются (в 2.42 — и значения).
char* cstr(const char* s) { return const_cast<char*>(s); }

void setAttr(void* obj, const char* name, const QByteArray& value) {
    agsafeset(obj, cstr(name), cstr(value.constData()), cstr(""));
}

QByteArray color(const QColor& c) { return c.name(QColor::HexRgb).toUtf8(); }
QByteArray number(double v) { return QByteArray::number(v, 'f', 3); }

// Текст -> escString Graphviz:
//  '\' экранируется, иначе \N, \G и т.п. станут подстановками;
//  '&' экранируется, иначе Graphviz раскроет HTML-сущности (&amp; -> &, &nbsp; ...);
//  перенос строки -> "\n" (строка, выровненная по центру).
QByteArray escapeLabel(const QString& text) {
    QString out;
    out.reserve(text.size() + 8);
    for (const QChar c : text) {
        if (c == u'\\') out += QStringLiteral("\\\\");
        else if (c == u'&') out += QStringLiteral("&amp;");
        else if (c == u'\n') out += QStringLiteral("\\n");
        else out += c;
    }
    return out.toUtf8();
}

// ---------------------------------------------------------------------------
// Плагин textlayout: ширину текста меряет Qt тем же шрифтом, которым потом
// рисует SVG. Без него Graphviz оценивает ширину по таблице для латиницы.
// ---------------------------------------------------------------------------

constexpr int kMeasurePixelSize = 100;  // меряем крупным кеглем и масштабируем — без ошибок хинтинга

QFontMetricsF& metricsFor(const QString& family) {
    thread_local QHash<QString, QFontMetricsF> cache;
    auto it = cache.find(family);
    if (it == cache.end()) {
        QFont f(family);
        f.setPixelSize(kMeasurePixelSize);
        f.setHintingPreference(QFont::PreferNoHinting);
        it = cache.insert(family, QFontMetricsF(f));
    }
    return *it;
}

// Тип возврата берём из заголовка: bool в новых Graphviz, boolean в 2.4x.
using TextLayoutResult = decltype(std::declval<gvtextlayout_engine_t&>().textlayout(nullptr, nullptr));

TextLayoutResult qtTextLayout(textspan_t* span, char** fontpath) {
    if (fontpath) *fontpath = nullptr;
    if (!span || !span->str || !span->font) return false;

    const double size = span->font->size > 0 ? span->font->size : 14.0;
    const QString family = span->font->name ? QString::fromUtf8(span->font->name) : QString();
    const QFontMetricsF& fm = metricsFor(family);

    const double k = size / kMeasurePixelSize;
    span->size.x = fm.horizontalAdvance(QString::fromUtf8(span->str)) * k * 1.02;  // +2% запас
    // Высота строки и смещения — как у встроенной оценки Graphviz, чтобы
    // вертикальное выравнивание SVG-текста совпадало со стандартным.
    span->size.y = size * 1.2;
    span->yoffset_layout = 0.0;
    span->yoffset_centerline = 0.1 * size;
    span->layout = nullptr;
    span->free_layout = nullptr;
    return true;
}

gvtextlayout_engine_t gQtTextLayoutEngine = {qtTextLayout};

// Высокое качество, чтобы при динамической загрузке плагинов наш выигрывал у pango.
gvplugin_installed_t gQtTextLayoutTypes[] = {
    {0, "textlayout", 100, &gQtTextLayoutEngine, nullptr},
    {0, nullptr, 0, nullptr, nullptr},
};

gvplugin_api_t gQtTextLayoutApis[] = {
    {API_textlayout, gQtTextLayoutTypes},
    {static_cast<api_t>(0), nullptr},
};

gvplugin_library_t gQtTextLayoutLibrary = {cstr("birch_qt_textlayout"), gQtTextLayoutApis};

// ---------------------------------------------------------------------------
// Сбор сообщений об ошибках Graphviz вместо вывода в stderr
// ---------------------------------------------------------------------------

QString& graphvizErrors() {
    static QString errors;
    return errors;
}

int collectGraphvizError(char* msg) {
    if (msg) graphvizErrors() += QString::fromUtf8(msg);
    return 0;
}

template <typename F>
struct RenderDataLength;
template <typename R, typename A1, typename A2, typename A3, typename A4, typename L>
struct RenderDataLength<R (*)(A1, A2, A3, A4, L*)> {
    using type = L;
};

struct GraphDeleter {
    void operator()(Agraph_t* g) const { agclose(g); }
};
using GraphPtr = std::unique_ptr<Agraph_t, GraphDeleter>;

// ---------------------------------------------------------------------------
// Стили
// ---------------------------------------------------------------------------

void applyGraphStyle(Agraph_t* g, const RenderOptions& opt, const QByteArray& font) {
    const Theme& t = opt.theme;

    agattr(g, AGRAPH, cstr("rankdir"), cstr(opt.direction == LayoutDirection::LeftToRight ? "LR" : "TB"));
    agattr(g, AGRAPH, cstr("bgcolor"), cstr("transparent"));  // фон заливаем сами
    agattr(g, AGRAPH, cstr("pad"), cstr("0.3"));
    agattr(g, AGRAPH, cstr("nodesep"), cstr("0.45"));
    agattr(g, AGRAPH, cstr("ranksep"), cstr("0.5"));
    agattr(g, AGRAPH, cstr("splines"), cstr("spline"));
    agattr(g, AGRAPH, cstr("outputorder"), cstr("edgesfirst"));  // стрелки под узлами
    agattr(g, AGRAPH, cstr("fontname"), cstr(font.constData()));
    agattr(g, AGRAPH, cstr("charset"), cstr("UTF-8"));

    agattr(g, AGNODE, cstr("shape"), cstr("box"));
    agattr(g, AGNODE, cstr("style"), cstr("rounded,filled"));
    agattr(g, AGNODE, cstr("penwidth"), cstr("1.4"));
    agattr(g, AGNODE, cstr("fontname"), cstr(font.constData()));
    agattr(g, AGNODE, cstr("fontsize"), cstr(number(t.fontSize).constData()));
    agattr(g, AGNODE, cstr("margin"), cstr("0.22,0.11"));  // дюймы: воздух вокруг текста
    agattr(g, AGNODE, cstr("height"), cstr("0.46"));
    agattr(g, AGNODE, cstr("width"), cstr("1.1"));
    agattr(g, AGNODE, cstr("fillcolor"), cstr(color(t.process.fill).constData()));
    agattr(g, AGNODE, cstr("color"), cstr(color(t.process.stroke).constData()));
    agattr(g, AGNODE, cstr("fontcolor"), cstr(color(t.process.text).constData()));
    agattr(g, AGNODE, cstr("label"), cstr(""));

    agattr(g, AGEDGE, cstr("color"), cstr(color(t.edge).constData()));
    agattr(g, AGEDGE, cstr("penwidth"), cstr("1.3"));
    agattr(g, AGEDGE, cstr("arrowhead"), cstr("normal"));
    agattr(g, AGEDGE, cstr("arrowsize"), cstr("0.7"));
    agattr(g, AGEDGE, cstr("fontname"), cstr(font.constData()));
    agattr(g, AGEDGE, cstr("fontsize"), cstr(number(t.edgeFontSize).constData()));
    agattr(g, AGEDGE, cstr("fontcolor"), cstr(color(t.edgeText).constData()));
    agattr(g, AGEDGE, cstr("label"), cstr(""));
}

void applyNodeStyle(Agnode_t* n, const Node& node, const Theme& theme) {
    const NodeStyle& s = theme.style(node.shape);
    setAttr(n, "label", escapeLabel(node.label));
    setAttr(n, "fillcolor", color(s.fill));
    setAttr(n, "color", color(s.stroke));
    setAttr(n, "fontcolor", color(s.text));

    switch (node.shape) {
    case NodeShape::Decision:
        setAttr(n, "shape", "diamond");
        setAttr(n, "style", "filled");
        setAttr(n, "margin", "0.04,0.02");  // ромб и так в ~2 раза больше текста
        setAttr(n, "height", "0.7");
        break;
    case NodeShape::Terminal:
        // Скругление в Graphviz ограничено 1/3 стороны, поэтому «таблетку»
        // отличаем ещё и жирной обводкой.
        setAttr(n, "style", "rounded,filled");
        setAttr(n, "penwidth", "2.2");
        setAttr(n, "margin", "0.28,0.11");
        break;
    case NodeShape::Process:
        break;
    }
}

QString resolveFamily(const QStringList& families) {
    static std::mutex m;
    static QHash<QStringList, QString> cache;
    std::lock_guard lock(m);
    if (auto it = cache.constFind(families); it != cache.constEnd()) return *it;

    QString chosen;
    for (const QString& f : families) {
        if (QFontDatabase::hasFamily(f)) {
            chosen = f;
            break;
        }
    }
    if (chosen.isEmpty()) chosen = QFontDatabase::systemFont(QFontDatabase::GeneralFont).family();
    cache.insert(families, chosen);
    return chosen;
}

qreal clampScale(QSizeF size, qreal scale) {
    if (size.isEmpty()) return scale;
    const qreal maxSide = qMax(size.width(), size.height());
    if (maxSide * scale > Renderer::kMaxImageSide) scale = Renderer::kMaxImageSide / maxSide;
    const qreal pixels = size.width() * size.height() * scale * scale;
    if (pixels > static_cast<qreal>(Renderer::kMaxImagePixels))
        scale *= std::sqrt(static_cast<qreal>(Renderer::kMaxImagePixels) / pixels);
    return scale;
}

}  // namespace

// ---------------------------------------------------------------------------

struct Renderer::Impl {
    GVC_t* gvc = nullptr;
};

Renderer::Renderer() : d(std::make_unique<Impl>()) {
    // Плагины передаются при создании контекста: движок textlayout Graphviz
    // выбирает один раз внутри gvContextPlugins, gvAddLibrary позже не помогает.
    static const lt_symlist_t builtins[] = {
#if defined(BIRCH_GRAPHVIZ_BUILTIN_PLUGINS)
        {"gvplugin_dot_layout_LTX_library", &gvplugin_dot_layout_LTX_library},
        {"gvplugin_core_LTX_library", &gvplugin_core_LTX_library},
#endif
        {"gvplugin_birch_qt_textlayout_LTX_library", &gQtTextLayoutLibrary},
        {nullptr, nullptr},
    };
#if defined(BIRCH_GRAPHVIZ_BUILTIN_PLUGINS)
    constexpr int demandLoading = 0;  // всё нужное уже внутри приложения
#else
    constexpr int demandLoading = 1;  // остальные плагины — из установленного Graphviz
#endif
    std::lock_guard lock(graphvizMutex());
    d->gvc = gvContextPlugins(builtins, demandLoading);
}

Renderer::~Renderer() {
    std::lock_guard lock(graphvizMutex());
    if (d->gvc) gvFreeContext(d->gvc);
}

QString Renderer::resolveFontFamily(const Theme& theme) { return resolveFamily(theme.fontFamilies); }

RenderResult Renderer::render(const Diagram& diagram, const RenderOptions& options) {
    RenderResult result;
    if (diagram.isEmpty()) return result;
    if (!d->gvc) {
        result.error = QStringLiteral("Не удалось инициализировать Graphviz");
        return result;
    }

    const QByteArray font = resolveFontFamily(options.theme).toUtf8();
    QByteArray svg;
    {
        std::lock_guard lock(graphvizMutex());
        graphvizErrors().clear();
        const agusererrf prevErrf = agseterrf(collectGraphvizError);
        const auto restoreErrf = qScopeGuard([prevErrf] { agseterrf(prevErrf); });

        GraphPtr g(agopen(cstr("birch"), Agdirected, nullptr));
        if (!g) {
            result.error = QStringLiteral("Graphviz: не удалось создать граф");
            return result;
        }
        applyGraphStyle(g.get(), options, font);

        // Имена узлов — n0, n1…: текст может быть любым, а имя в cgraph — ключ.
        QHash<QString, Agnode_t*> nodes;
        nodes.reserve(diagram.nodes.size());
        for (qsizetype i = 0; i < diagram.nodes.size(); ++i) {
            const Node& node = diagram.nodes[i];
            const QByteArray name = "n" + QByteArray::number(i);
            Agnode_t* n = agnode(g.get(), cstr(name.constData()), 1);
            applyNodeStyle(n, node, options.theme);
            nodes.insert(node.id, n);
        }
        for (const Edge& edge : diagram.edges) {
            Agnode_t* from = nodes.value(edge.from);
            Agnode_t* to = nodes.value(edge.to);
            if (!from || !to) continue;  // парсер такого не выдаёт, но лучше перестраховаться
            Agedge_t* e = agedge(g.get(), from, to, nullptr, 1);
            if (e && !edge.label.isEmpty()) {
                // Пробелы по краям — визуальный отступ подписи от линии.
                setAttr(e, "label", escapeLabel(QStringLiteral(" %1 ").arg(edge.label)));
            }
        }

        if (gvLayout(d->gvc, g.get(), "dot") != 0) {
            result.error = QStringLiteral("Graphviz: ошибка раскладки. %1").arg(graphvizErrors().trimmed());
            return result;
        }
        const auto freeLayout = qScopeGuard([&] { gvFreeLayout(d->gvc, g.get()); });

        char* data = nullptr;
        // Тип длины отличается между версиями (unsigned int / size_t).
        RenderDataLength<decltype(&gvRenderData)>::type length = 0;
        if (gvRenderData(d->gvc, g.get(), "svg", &data, &length) != 0 || !data) {
            result.error = QStringLiteral("Graphviz: ошибка рендеринга SVG. %1").arg(graphvizErrors().trimmed());
            if (data) gvFreeRenderData(data);
            return result;
        }
        svg = QByteArray(data, static_cast<qsizetype>(length));
        gvFreeRenderData(data);
    }

    // SVG -> QImage. Размер берём из viewBox: он в pt, без пересчёта единиц QtSvg.
    QSvgRenderer svgRenderer(svg);
    if (!svgRenderer.isValid()) {
        result.error = QStringLiteral("Не удалось разобрать SVG от Graphviz");
        return result;
    }
    const QSizeF size = svgRenderer.viewBoxF().size();
    if (!options.rasterize) {
        result.svg = std::move(svg);
        result.size = size;
        return result;
    }
    const qreal scale = clampScale(size, options.scale > 0 ? options.scale : 1.0);
    const QSize pixels(qMax(1, qCeil(size.width() * scale)), qMax(1, qCeil(size.height() * scale)));

    QImage image(pixels, QImage::Format_ARGB32_Premultiplied);
    if (image.isNull()) {
        result.error = QStringLiteral("Недостаточно памяти для картинки %1×%2").arg(pixels.width()).arg(pixels.height());
        return result;
    }
    image.fill(options.transparentBackground ? QColor(Qt::transparent) : options.theme.background);
    {
        QPainter p(&image);
        p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
        svgRenderer.render(&p, QRectF(QPointF(0, 0), QSizeF(pixels)));
    }
    // 72 dpi × масштаб: Word/Pages вставят картинку в логическом размере, а не в 2 раза больше.
    const int dpm = qRound(72.0 * scale / 0.0254);
    image.setDotsPerMeterX(dpm);
    image.setDotsPerMeterY(dpm);
    image.setDevicePixelRatio(scale);

    result.svg = std::move(svg);
    result.image = std::move(image);
    result.size = size;
    return result;
}

QByteArray Renderer::toPng(const QImage& image) {
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    if (!image.save(&buffer, "PNG")) return {};
    return bytes;
}

QString Renderer::graphvizVersion() {
    std::lock_guard lock(graphvizMutex());
    GVC_t* gvc = gvContextPlugins(nullptr, 0);
    const QString v = QString::fromUtf8(gvcVersion(gvc));
    gvFreeContext(gvc);
    return v;
}

}  // namespace birch
