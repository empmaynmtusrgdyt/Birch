/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "core/CommandLine.h"

#include <QCommandLineParser>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QSaveFile>

#include <algorithm>
#include <cstdio>
#include <string>

#include "core/MermaidExporter.h"
#include "core/Parser.h"
#include "core/Renderer.h"

#if defined(Q_OS_WIN)
#include <io.h>
#include <windows.h>
#include <shellapi.h>
#endif

namespace birch::cli {

namespace {

// Ключи Qt, которые QApplication разбирает сам; true — у ключа есть значение.
// Qt принимает их и с одним, и с двумя дефисами, и в виде -style=fusion.
const QList<std::pair<QString, bool>>& qtOptions() {
    static const QList<std::pair<QString, bool>> options = {
        {QStringLiteral("style"), true},          {QStringLiteral("stylesheet"), true},
        {QStringLiteral("platform"), true},       {QStringLiteral("platformpluginpath"), true},
        {QStringLiteral("platformtheme"), true},  {QStringLiteral("plugin"), true},
        {QStringLiteral("qmljsdebugger"), true},  {QStringLiteral("qwindowgeometry"), true},
        {QStringLiteral("qwindowicon"), true},    {QStringLiteral("qwindowtitle"), true},
        {QStringLiteral("session"), true},        {QStringLiteral("display"), true},
        {QStringLiteral("geometry"), true},       {QStringLiteral("title"), true},
        {QStringLiteral("name"), true},           {QStringLiteral("reverse"), false},
        {QStringLiteral("widgetcount"), false},   {QStringLiteral("nograb"), false},
        {QStringLiteral("dograb"), false},
    };
    return options;
}

// Как ключ выглядит в сообщении: «-o» или «--output».
QString dashed(const QString& name) {
    return name.size() == 1 ? QStringLiteral("-") + name : QStringLiteral("--") + name;
}

void write(FILE* stream, const QString& text) {
#if defined(Q_OS_WIN)
    // В консоль — текстом UTF-16 через WriteConsoleW: так кириллица видна на любой
    // Windows и при любой кодовой странице. Консоль Windows 8.1 плохо понимает
    // UTF-8 даже с SetConsoleOutputCP(CP_UTF8) и показывает вместо кириллицы мусор.
    //
    // Консоль узнаём по типу файла, а не через GetConsoleMode: тому нужен доступ
    // на чтение, а CONOUT$ после freopen открыт только на запись — проверка не
    // проходила, и текст уходил в UTF-8. Тип «символьное устройство» бывает и у NUL,
    // поэтому если WriteConsoleW не сработал, пишем как в файл.
    std::fflush(stream);
    const intptr_t os = _get_osfhandle(_fileno(stream));
    const HANDLE console = reinterpret_cast<HANDLE>(os);
    if (os != -1 && GetFileType(console) == FILE_TYPE_CHAR) {
        const std::wstring wide = text.toStdWString();
        size_t pos = 0;
        bool ok = true;
        while (pos < wide.size()) {
            const DWORD chunk = static_cast<DWORD>(std::min<size_t>(wide.size() - pos, 8192));
            DWORD written = 0;
            if (!WriteConsoleW(console, wide.data() + pos, chunk, &written, nullptr) || written == 0) {
                ok = false;
                break;
            }
            pos += written;
        }
        if (ok || pos > 0) return;  // если часть уже в консоли — не повторяем её
    }
#endif
    // В файлы и каналы — UTF-8
    const QByteArray bytes = text.toUtf8();
    std::fwrite(bytes.constData(), 1, static_cast<size_t>(bytes.size()), stream);
    std::fflush(stream);
}

}  // namespace

// --- аргументы ---------------------------------------------------------------------

QStringList rawArguments(int argc, char* argv[]) {
    QStringList args;
#if defined(Q_OS_WIN)
    // argv на Windows — в кодировке ANSI, кириллица в путях может потеряться.
    int count = 0;
    if (LPWSTR* wide = CommandLineToArgvW(GetCommandLineW(), &count)) {
        for (int i = 0; i < count; ++i) args << QString::fromWCharArray(wide[i]);
        LocalFree(wide);
        return args;
    }
#endif
    for (int i = 0; i < argc; ++i) args << QString::fromLocal8Bit(argv[i]);
    return args;
}

QStringList withoutQtOptions(const QStringList& args) {
    QStringList out;
    for (int i = 0; i < args.size(); ++i) {
        const QString& arg = args[i];
        if (arg == QStringLiteral("--")) {  // дальше — только имена файлов
            out << args.mid(i);
            break;
        }
        if (i == 0 || !arg.startsWith(u'-') || arg == QStringLiteral("-")) {
            out << arg;
            continue;
        }
        QString name = arg.mid(arg.startsWith(QStringLiteral("--")) ? 2 : 1);
        const qsizetype eq = name.indexOf(u'=');
        if (eq >= 0) name.truncate(eq);
        bool skipped = false;
        for (const auto& [qtName, hasValue] : qtOptions()) {
            if (name != qtName) continue;
            if (hasValue && eq < 0) ++i;  // значение — следующим аргументом
            skipped = true;
            break;
        }
        if (!skipped) out << arg;
    }
    return out;
}

Arguments parse(const QStringList& args) {
    Arguments a;

    QCommandLineParser p;
    p.setSingleDashWordOptionMode(QCommandLineParser::ParseAsCompactedShortOptions);  // -ma = -m -a
    const QCommandLineOption output(QStringList{QStringLiteral("o"), QStringLiteral("output")}, QString(), QStringLiteral("file"));
    const QCommandLineOption theme(QStringList{QStringLiteral("t"), QStringLiteral("theme")}, QString(), QStringLiteral("theme"));
    const QCommandLineOption monochrome(QStringList{QStringLiteral("m"), QStringLiteral("monochrome")});
    const QCommandLineOption horizontal(QStringList{QStringLiteral("H"), QStringLiteral("horizontal")});
    const QCommandLineOption transparent(QStringList{QStringLiteral("a"), QStringLiteral("transparent")});
    const QCommandLineOption scale(QStringList{QStringLiteral("s"), QStringLiteral("scale")}, QString(), QStringLiteral("n"));
    const QCommandLineOption help(QStringList{QStringLiteral("h"), QStringLiteral("help")});
    const QCommandLineOption version(QStringList{QStringLiteral("v"), QStringLiteral("version")});
    const QCommandLineOption screenshot(QStringLiteral("screenshot"), QString(), QStringLiteral("file"));
    const QCommandLineOption screenshotTheme(QStringLiteral("screenshot-theme"), QString(), QStringLiteral("theme"));
    const QCommandLineOption screenshotMonochrome(QStringLiteral("screenshot-monochrome"));
    const QCommandLineOption find(QStringLiteral("find"), QString(), QStringLiteral("text"));
    p.addOptions({output, theme, monochrome, horizontal, transparent, scale, help, version, screenshot,
                  screenshotTheme, screenshotMonochrome, find});

    const bool ok = p.parse(args);
    auto fail = [&a](const QString& message) {
        a.mode = Mode::Error;
        a.error = message;
        return a;
    };

    // Справка и версия важнее всего остального, как в GNU.
    if (p.isSet(help)) {
        a.mode = Mode::Help;
        return a;
    }
    if (p.isSet(version)) {
        a.mode = Mode::Version;
        return a;
    }
    if (!ok) {
        const QStringList unknown = p.unknownOptionNames();
        if (!unknown.isEmpty()) return fail(QStringLiteral("неизвестный ключ «%1»").arg(dashed(unknown.first())));
        // Иначе не хватает значения: «Missing value after '-o'.»
        const QRegularExpressionMatch m = QRegularExpression(QStringLiteral("'([^']+)'")).match(p.errorText());
        return fail(QStringLiteral("ключу «%1» нужно значение").arg(m.hasMatch() ? m.captured(1) : QString()));
    }

    const QStringList files = p.positionalArguments();
    if (files.size() > 1) return fail(QStringLiteral("лишний аргумент «%1»: можно указать только один файл").arg(files[1]));
    a.input = files.value(0);

    a.screenshot = p.value(screenshot);
    a.screenshotTheme = p.value(screenshotTheme);
    a.screenshotMonochrome = p.isSet(screenshotMonochrome);
    a.find = p.value(find);

    if (!p.isSet(output)) {
        // Ключи вида картинки без -o молча не игнорируем: иначе непонятно, почему окно не изменилось.
        const std::pair<const QCommandLineOption*, const char*> pictureOptions[] = {
            {&theme, "--theme"},         {&monochrome, "--monochrome"}, {&horizontal, "--horizontal"},
            {&transparent, "--transparent"}, {&scale, "--scale"},
        };
        for (const auto& [option, name] : pictureOptions) {
            if (p.isSet(*option))
                return fail(QStringLiteral("ключ «%1» работает только вместе с -o").arg(QLatin1String(name)));
        }
        if (a.input == QStringLiteral("-"))
            return fail(QStringLiteral("чтение схемы со стандартного ввода («-») работает только вместе с -o"));
        a.mode = Mode::Gui;
        return a;
    }

    a.mode = Mode::Render;
    a.output = p.value(output);
    if (a.output.isEmpty()) return fail(QStringLiteral("ключу «-o» нужно значение"));
    const QString suffix = QFileInfo(a.output).suffix().toLower();
    if (suffix != QStringLiteral("png") && suffix != QStringLiteral("svg") && suffix != QStringLiteral("mmd"))
        return fail(QStringLiteral("неизвестный формат файла «%1»: поддерживаются .png, .svg и .mmd").arg(a.output));
    if (a.input.isEmpty()) return fail(QStringLiteral("не указан файл схемы"));

    if (p.isSet(theme)) {
        const QString t = p.value(theme);
        if (t != QStringLiteral("light") && t != QStringLiteral("dark"))
            return fail(QStringLiteral("неверное значение «%1» для ключа «--theme»: допустимы light и dark").arg(t));
        a.dark = t == QStringLiteral("dark");
    }
    if (p.isSet(scale)) {
        bool isNumber = false;
        const int n = p.value(scale).toInt(&isNumber);
        if (!isNumber || n < 1 || n > kMaxScale)
            return fail(QStringLiteral("неверное значение «%1» для ключа «--scale»: нужно целое число от 1 до %2")
                            .arg(p.value(scale))
                            .arg(kMaxScale));
        a.scale = n;
    }
    a.monochrome = p.isSet(monochrome);
    a.horizontal = p.isSet(horizontal);
    a.transparent = p.isSet(transparent);
    return a;
}

QString programName(const QString& argv0) {
    const QString name = QFileInfo(argv0).fileName();
    // Birch.exe напрямую или birch.com — консольная обёртка на Windows
    if (name.endsWith(QStringLiteral(".exe"), Qt::CaseInsensitive)
        || name.endsWith(QStringLiteral(".com"), Qt::CaseInsensitive))
        return name.chopped(4);
    return name.isEmpty() ? QStringLiteral("birch") : name;
}

// --- тексты ------------------------------------------------------------------------

QString helpText(const QString& program) {
    return QStringLiteral(
               "Использование: %1 [ПАРАМЕТР]... [ФАЙЛ]\n"
               "Открывает ФАЙЛ (.birch) в редакторе или, с ключом -o, рисует по нему\n"
               "блок-схему в файл, не открывая окна.\n"
               "Если ФАЙЛ — «-», текст схемы читается со стандартного ввода.\n"
               "\n"
               "Аргументы, обязательные для длинных ключей, обязательны и для коротких.\n"
               "  -o, --output=ФАЙЛ        сохранить схему в ФАЙЛ и выйти; формат по\n"
               "                             расширению: .png, .svg или .mmd (Mermaid)\n"
               "  -t, --theme=ТЕМА         тема: light (по умолчанию) или dark\n"
               "  -m, --monochrome         чёрно-белая схема\n"
               "  -H, --horizontal         располагать блоки слева направо\n"
               "  -a, --transparent        прозрачный фон (PNG и SVG)\n"
               "  -s, --scale=N            масштаб PNG от 1 до 10 (по умолчанию 2)\n"
               "  -h, --help               показать эту справку и выйти\n"
               "  -v, --version            показать информацию о версии и выйти\n"
               "\n"
               "Примеры:\n"
               "  %1 схема.birch                       открыть схему в редакторе\n"
               "  %1 схема.birch -o схема.png          нарисовать схему в PNG\n"
               "  %1 -t dark -s 3 схема.birch -o схема.png\n"
               "  cat схема.birch | %1 - -o схема.svg\n"
               "\n"
               "Коды выхода:\n"
               "  0  успешно\n"
               "  1  в тексте схемы есть ошибки или схема пуста\n"
               "  2  ошибка чтения или записи файла\n"
               "  3  неверные ключи командной строки\n"
               "\n"
               "Домашняя страница: <https://github.com/empmaynmtusrgdyt/Birch>\n"
               "Об ошибках сообщайте: <https://github.com/empmaynmtusrgdyt/Birch/issues>\n")
        .arg(program);
}

QString versionText(const QString& version) {
    return QStringLiteral(
               "birch (Birch) %1\n"
               "Copyright (C) 2026 Marth Scoobert.\n"
               "Лицензия MPL 2.0: Mozilla Public License 2.0 <https://mozilla.org/MPL/2.0/>\n"
               "Это свободное ПО: вы можете изменять и распространять его.\n"
               "НИКАКИХ ГАРАНТИЙ не предоставляется в пределах, допускаемых законом.\n"
               "\n"
               "Используются: Qt %2, Graphviz %3\n")
        .arg(version, QString::fromLatin1(qVersion()), Renderer::graphvizVersion());
}

QString usageErrorText(const QString& program, const QString& error) {
    return QStringLiteral("%1: %2\nПопробуйте «%1 --help» для получения дополнительной информации.\n")
        .arg(program, error);
}

// --- рендер ------------------------------------------------------------------------

int render(const Arguments& a, const QString& program, QString& errors) {
    const bool fromStdin = a.input == QStringLiteral("-");
    const QString sourceName = fromStdin ? QStringLiteral("<stdin>") : a.input;

    QFile in;
    const bool opened = fromStdin ? in.open(stdin, QIODevice::ReadOnly) : (in.setFileName(a.input), in.open(QIODevice::ReadOnly));
    if (!opened) {
        errors += QStringLiteral("%1: не удалось открыть «%2»: %3\n").arg(program, a.input, in.errorString());
        return kExitIoError;
    }
    const QString source = QString::fromUtf8(in.readAll());

    // Ошибки и предупреждения — в формате «файл:строка:столбец: …», его понимают редакторы и CI.
    const ParseResult parsed = Parser{}.parse(source);
    for (const Diagnostic& d : parsed.diagnostics) {
        errors += QStringLiteral("%1:%2:%3: %4: %5\n")
                      .arg(sourceName)
                      .arg(d.line)
                      .arg(d.column)
                      .arg(d.isError() ? QStringLiteral("ошибка") : QStringLiteral("предупреждение"), d.message);
    }
    // С ошибками файл не пишем: иначе скрипт молча получил бы неполную схему.
    if (parsed.hasErrors()) return kExitDiagramErrors;
    if (parsed.diagram.isEmpty()) {
        errors += QStringLiteral("%1: схема пуста — нечего рисовать\n").arg(sourceName);
        return kExitDiagramErrors;
    }

    const QString suffix = QFileInfo(a.output).suffix().toLower();
    QByteArray bytes;
    if (suffix == QStringLiteral("mmd")) {
        MermaidOptions opt;
        opt.leftToRight = a.horizontal;
        bytes = toMermaid(parsed.diagram, opt).toUtf8();
    } else {
        const bool png = suffix == QStringLiteral("png");
        RenderOptions opt;
        opt.theme = Theme::select(a.dark, a.monochrome);
        opt.direction = a.horizontal ? LayoutDirection::LeftToRight : LayoutDirection::TopToBottom;
        opt.transparentBackground = a.transparent;
        opt.scale = a.scale;
        opt.rasterize = png;
        Renderer renderer;
        const RenderResult r = renderer.render(parsed.diagram, opt);
        if (!r.ok()) {
            errors += QStringLiteral("%1: %2\n").arg(program, r.error);
            return kExitIoError;
        }
        bytes = png ? Renderer::toPng(r.image) : r.svg;
    }

    QSaveFile out(a.output);
    if (!out.open(QIODevice::WriteOnly) || out.write(bytes) != bytes.size() || !out.commit()) {
        errors += QStringLiteral("%1: не удалось записать «%2»: %3\n").arg(program, a.output, out.errorString());
        return kExitIoError;
    }
    return kExitOk;
}

// --- вывод -------------------------------------------------------------------------

void attachParentConsole() {
#if defined(Q_OS_WIN)
    static bool attached = false;
    if (attached) return;
    attached = true;
    // Если вывод перенаправлен в файл или канал, он и так работает.
    const auto hasStream = [](DWORD id) {
        const HANDLE h = GetStdHandle(id);
        return h != nullptr && h != INVALID_HANDLE_VALUE && GetFileType(h) != FILE_TYPE_UNKNOWN;
    };
    const bool out = hasStream(STD_OUTPUT_HANDLE);
    const bool err = hasStream(STD_ERROR_HANDLE);
    if ((!out || !err) && AttachConsole(ATTACH_PARENT_PROCESS)) {
        FILE* f = nullptr;
        if (!out) freopen_s(&f, "CONOUT$", "w", stdout);
        if (!err) freopen_s(&f, "CONOUT$", "w", stderr);
    }
#endif
}

void releaseConsoleLauncher() {
#if defined(Q_OS_WIN)
    // birch.com передаёт наследуемое событие через переменную среды.
    const QByteArray value = qgetenv("BIRCH_WINDOW_EVENT");
    if (value.isEmpty()) return;
    qunsetenv("BIRCH_WINDOW_EVENT");
    bool ok = false;
    const qulonglong handle = value.toULongLong(&ok);
    if (!ok) return;
    const HANDLE event = reinterpret_cast<HANDLE>(static_cast<uintptr_t>(handle));
    SetEvent(event);
    CloseHandle(event);
#endif
}

void printOut(const QString& text) { write(stdout, text); }
void printErr(const QString& text) { write(stderr, text); }

}  // namespace birch::cli
