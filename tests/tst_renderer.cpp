#include <QElapsedTimer>
#include <QFile>
#include <QFontMetricsF>
#include <QRegularExpression>
#include <QtTest>

#include "core/Parser.h"
#include "core/Renderer.h"

using namespace birch;

namespace {

Diagram parseOk(const QString& src) {
    const auto r = Parser{}.parse(src);
    Q_ASSERT(!r.hasErrors());
    return r.diagram;
}

// Горизонтальный размах фигуры узла с заданным <title> в SVG от Graphviz.
qreal nodeShapeWidth(const QByteArray& svg, const QString& title) {
    const QString s = QString::fromUtf8(svg);
    const qsizetype start = s.indexOf(QStringLiteral("<title>%1</title>").arg(title));
    if (start < 0) return -1;
    const qsizetype end = s.indexOf(QStringLiteral("</g>"), start);
    const QString block = s.mid(start, end - start);

    static const QRegularExpression shapeRe(QStringLiteral("(?:points|d)=\"([^\"]+)\""));
    static const QRegularExpression numRe(QStringLiteral("-?\\d+(?:\\.\\d+)?"));
    const auto m = shapeRe.match(block);
    if (!m.hasMatch()) return -1;

    qreal minX = 1e9, maxX = -1e9;
    bool isX = true;
    for (auto it = numRe.globalMatch(m.captured(1)); it.hasNext(); isX = !isX) {
        const qreal v = it.next().captured(0).toDouble();
        if (isX) {
            minX = qMin(minX, v);
            maxX = qMax(maxX, v);
        }
    }
    return maxX - minX;
}

void dump(const RenderResult& r, const QString& name) {
    const QString dir = qEnvironmentVariable("BIRCH_DUMP_DIR");
    if (dir.isEmpty()) return;
    r.image.save(dir + u'/' + name + QStringLiteral(".png"));
    QFile f(dir + u'/' + name + QStringLiteral(".svg"));
    if (f.open(QIODevice::WriteOnly)) f.write(r.svg);
}

QString exampleSource() {
    QFile f(QStringLiteral(BIRCH_EXAMPLES_DIR "/welcome.birch"));
    if (!f.open(QIODevice::ReadOnly)) return {};
    return QString::fromUtf8(f.readAll());
}

}  // namespace

class TestRenderer : public QObject {
    Q_OBJECT

private slots:
    void exampleLightAndDark() {
        Renderer renderer;
        const Diagram dia = parseOk(exampleSource());

        for (const Theme& theme : {Theme::light(), Theme::dark()}) {
            RenderOptions opt;
            opt.theme = theme;
            const auto r = renderer.render(dia, opt);
            QVERIFY2(r.ok(), qPrintable(r.error));
            QVERIFY(!r.image.isNull());
            QVERIFY(r.size.width() > 50 && r.size.height() > 50);
            QCOMPARE(r.image.width(), qCeil(r.size.width() * opt.scale));
            QVERIFY(r.svg.contains("<svg"));
            QVERIFY(r.svg.contains(QStringLiteral("Данные корректны?").toUtf8()));
            QVERIFY(r.svg.contains(Renderer::resolveFontFamily(theme).toUtf8()));
            QCOMPARE(r.image.pixelColor(1, 1), theme.background);
            dump(r, QStringLiteral("example-") + theme.name);
        }
    }

    void monochromeUsesOnlyBlackAndWhite_data() {
        QTest::addColumn<bool>("dark");
        QTest::newRow("light") << false;
        QTest::newRow("dark") << true;
    }

    void monochromeUsesOnlyBlackAndWhite() {
        QFETCH(bool, dark);
        Renderer renderer;
        RenderOptions opt;
        opt.theme = Theme::monochrome(dark);
        const auto r = renderer.render(parseOk(exampleSource()), opt);
        QVERIFY2(r.ok(), qPrintable(r.error));

        // Все цвета в SVG (fill, stroke) — только чёрный или белый.
        static const QRegularExpression colorRe(QStringLiteral("(?:fill|stroke)=\"(#[0-9a-fA-F]{6})\""));
        int colors = 0;
        for (auto it = colorRe.globalMatch(QString::fromUtf8(r.svg)); it.hasNext(); ++colors) {
            const QString c = it.next().captured(1).toLower();
            QVERIFY2(c == QStringLiteral("#000000") || c == QStringLiteral("#ffffff"), qPrintable(c));
        }
        QVERIFY(colors > 10);  // цвета действительно есть, регулярка не промахнулась

        QCOMPARE(r.image.pixelColor(1, 1), dark ? QColor(Qt::black) : QColor(Qt::white));
        dump(r, QStringLiteral("example-") + opt.theme.name);
    }

    void themeSelect() {
        QCOMPARE(Theme::select(false, false).name, Theme::light().name);
        QCOMPARE(Theme::select(true, false).name, Theme::dark().name);
        QCOMPARE(Theme::select(false, true).name, QStringLiteral("mono-light"));
        QCOMPARE(Theme::select(true, true).name, QStringLiteral("mono-dark"));
    }

    void emptyDiagramGivesNullImage() {
        Renderer renderer;
        const auto r = renderer.render(Diagram{});
        QVERIFY(r.ok());
        QVERIFY(r.image.isNull());
    }

    void transparentBackground() {
        Renderer renderer;
        RenderOptions opt;
        opt.transparentBackground = true;
        const auto r = renderer.render(parseOk("[A] -> [B]"), opt);
        QVERIFY(r.ok());
        QCOMPARE(r.image.pixelColor(0, 0).alpha(), 0);
    }

    void specialCharactersAreEscaped() {
        Renderer renderer;
        // \\N в DSL -> текст "\N"; без экранирования Graphviz подставил бы имя узла.
        const auto r = renderer.render(parseOk("[<b>&amp; \"q\"] -> [путь\\\\N] \"a<b\""));
        QVERIFY2(r.ok(), qPrintable(r.error));
        QVERIFY(r.svg.contains("&lt;b&gt;&amp;amp;"));
        QVERIFY(r.svg.contains("путь\\N"));
        QVERIFY(r.svg.contains("a&lt;b"));
    }

    void multilineLabel() {
        Renderer renderer;
        const auto r = renderer.render(parseOk("[Первая\\nВторая]"));
        QVERIFY(r.ok());
        QVERIFY(r.svg.contains(QStringLiteral(">Первая<").toUtf8()));
        QVERIFY(r.svg.contains(QStringLiteral(">Вторая<").toUtf8()));
    }

    void textFitsInsideNode() {
        // Проверяет, что ширину текста меряет Qt (наш плагин), а не таблица Graphviz.
        Renderer renderer;
        const QString label = QStringLiteral("Широкая кириллическая подпись ЖЖЖЖ ШШШШ ЩЩЩЩ");
        RenderOptions opt;
        const auto r = renderer.render(parseOk(QStringLiteral("[%1]").arg(label)), opt);
        QVERIFY(r.ok());

        QFont f(Renderer::resolveFontFamily(opt.theme));
        f.setPixelSize(100);
        const qreal textWidth = QFontMetricsF(f).horizontalAdvance(label) * opt.theme.fontSize / 100.0;
        const qreal shapeWidth = nodeShapeWidth(r.svg, QStringLiteral("n0"));
        QVERIFY2(shapeWidth > textWidth + 20,
                 qPrintable(QStringLiteral("узел %1pt, текст %2pt").arg(shapeWidth).arg(textWidth)));
        QVERIFY2(shapeWidth < textWidth + 60, "слишком большой отступ — ширина считается неточно");
    }

    void leftToRight() {
        Renderer renderer;
        RenderOptions opt;
        opt.direction = LayoutDirection::LeftToRight;
        const auto r = renderer.render(parseOk("[A] -> [B] -> [C] -> [D] -> [E]"), opt);
        QVERIFY(r.ok());
        QVERIFY(r.size.width() > 2 * r.size.height());
    }

    void hugeScaleIsClamped() {
        Renderer renderer;
        RenderOptions opt;
        opt.scale = 1000;
        const auto r = renderer.render(parseOk("[A] -> [B]"), opt);
        QVERIFY(r.ok());
        QVERIFY(r.image.width() <= Renderer::kMaxImageSide);
        QVERIFY(r.image.height() <= Renderer::kMaxImageSide);
    }

    void largeGraph() {
        QString src;
        for (int i = 0; i < 300; ++i)
            src += QStringLiteral("[Узел_%1] -> [Узел_%2] \"%3\"\n").arg(i).arg((i * 7 + 3) % 300).arg(i);
        Renderer renderer;
        QElapsedTimer t;
        t.start();
        const auto r = renderer.render(parseOk(src));
        QVERIFY2(r.ok(), qPrintable(r.error));
        qInfo() << "300 узлов:" << t.elapsed() << "мс," << r.image.size();
    }

    void pngRoundTrip() {
        Renderer renderer;
        const auto r = renderer.render(parseOk("(Старт) -> {Да?} -> (Конец)"));
        const QByteArray png = Renderer::toPng(r.image);
        QVERIFY(png.startsWith("\x89PNG"));
        QImage back;
        QVERIFY(back.loadFromData(png, "PNG"));
        QCOMPARE(back.size(), r.image.size());
    }

    void repeatedRendersAreStable() {
        Renderer renderer;
        const Diagram dia = parseOk(exampleSource());
        QByteArray first;
        for (int i = 0; i < 30; ++i) {
            const auto r = renderer.render(dia);
            QVERIFY(r.ok());
            if (i == 0) first = r.svg;
            QCOMPARE(r.svg, first);  // раскладка детерминирована
        }
    }
};

QTEST_MAIN(TestRenderer)
#include "tst_renderer.moc"
