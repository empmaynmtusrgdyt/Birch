// Тесты командной строки: разбор ключей, справка и версия в формате GNU,
// рендер схемы в файл и коды выхода.

#include <QFile>
#include <QImage>
#include <QTemporaryDir>
#include <QtTest>

#include "core/CommandLine.h"
#include "core/Theme.h"

using namespace birch;
using namespace birch::cli;

namespace {

Arguments parseArgs(const QStringList& args) { return parse(QStringList{QStringLiteral("birch")} + args); }

QString example(const QString& name) { return QStringLiteral(BIRCH_EXAMPLES_DIR "/") + name; }

}  // namespace

class TestCli : public QObject {
    Q_OBJECT

private slots:
    // --- разбор ключей ----------------------------------------------------------

    void noArgumentsOpensWindow() {
        const Arguments a = parseArgs({});
        QCOMPARE(a.mode, Mode::Gui);
        QVERIFY(a.input.isEmpty());
        QCOMPARE(parseArgs({"схема.birch"}).input, QStringLiteral("схема.birch"));
    }

    void helpAndVersionWin() {
        QCOMPARE(parseArgs({"--help"}).mode, Mode::Help);
        QCOMPARE(parseArgs({"-h"}).mode, Mode::Help);
        QCOMPARE(parseArgs({"--version"}).mode, Mode::Version);
        QCOMPARE(parseArgs({"-v"}).mode, Mode::Version);
        // Справка важнее остального, даже ошибок в других ключах.
        QCOMPARE(parseArgs({"-o", "x.png", "--colour", "-h"}).mode, Mode::Help);
    }

    void renderLongOptions() {
        const Arguments a = parseArgs({"схема.birch", "--output=out.PNG", "--theme=dark", "--monochrome",
                                       "--horizontal", "--transparent", "--scale", "3"});
        QCOMPARE(a.mode, Mode::Render);
        QCOMPARE(a.input, QStringLiteral("схема.birch"));
        QCOMPARE(a.output, QStringLiteral("out.PNG"));  // расширение — без учёта регистра
        QVERIFY(a.dark && a.monochrome && a.horizontal && a.transparent);
        QCOMPARE(a.scale, 3);
    }

    void renderShortOptions() {
        // Значение через пробел и слитно, ключи без значения — склеенные.
        const Arguments a = parseArgs({"-tdark", "-maH", "-s", "4", "-o", "out.svg", "схема.birch"});
        QCOMPARE(a.mode, Mode::Render);
        QVERIFY(a.dark && a.monochrome && a.transparent && a.horizontal);
        QCOMPARE(a.scale, 4);
        QCOMPARE(a.output, QStringLiteral("out.svg"));
    }

    void renderDefaults() {
        const Arguments a = parseArgs({"a.birch", "-o", "a.mmd"});
        QCOMPARE(a.mode, Mode::Render);
        QVERIFY(!a.dark && !a.monochrome && !a.horizontal && !a.transparent);
        QCOMPARE(a.scale, 2);
    }

    void stdinInput() {
        QCOMPARE(parseArgs({"-", "-o", "a.svg"}).input, QStringLiteral("-"));
        QCOMPARE(parseArgs({"-"}).mode, Mode::Error);  // в окно со стандартного ввода нельзя
    }

    void doubleDashEndsOptions() {
        const Arguments a = parseArgs({"-o", "a.png", "--", "-странное имя.birch"});
        QCOMPARE(a.mode, Mode::Render);
        QCOMPARE(a.input, QStringLiteral("-странное имя.birch"));
    }

    void errors_data() {
        QTest::addColumn<QStringList>("args");
        QTest::addColumn<QString>("message");
        QTest::newRow("неизвестный длинный") << QStringList{"--colour"} << "неизвестный ключ «--colour»";
        QTest::newRow("неизвестный короткий") << QStringList{"-x"} << "неизвестный ключ «-x»";
        QTest::newRow("нет значения") << QStringList{"a.birch", "-o"} << "ключу «-o» нужно значение";
        QTest::newRow("тема без -o") << QStringList{"-t", "dark", "a.birch"}
                                      << "ключ «--theme» работает только вместе с -o";
        QTest::newRow("Ч/Б без -o") << QStringList{"-m"} << "ключ «--monochrome» работает только вместе с -o";
        QTest::newRow("масштаб без -o") << QStringList{"--scale=3"} << "ключ «--scale» работает только вместе с -o";
        QTest::newRow("формат") << QStringList{"a.birch", "-o", "a.jpg"} << "неизвестный формат файла «a.jpg»";
        QTest::newRow("без расширения") << QStringList{"a.birch", "-o", "a"} << "неизвестный формат файла «a»";
        QTest::newRow("тема") << QStringList{"a.birch", "-o", "a.png", "--theme=blue"}
                              << "неверное значение «blue» для ключа «--theme»";
        QTest::newRow("масштаб 0") << QStringList{"a.birch", "-o", "a.png", "-s", "0"}
                                   << "неверное значение «0» для ключа «--scale»";
        QTest::newRow("масштаб текстом") << QStringList{"a.birch", "-o", "a.png", "-s", "два"}
                                         << "неверное значение «два»";
        QTest::newRow("масштаб велик") << QStringList{"a.birch", "-o", "a.png", "-s", "11"} << "от 1 до 10";
        QTest::newRow("нет файла") << QStringList{"-o", "a.png"} << "не указан файл схемы";
        QTest::newRow("два файла") << QStringList{"a.birch", "b.birch"} << "лишний аргумент «b.birch»";
    }

    void errors() {
        QFETCH(QStringList, args);
        QFETCH(QString, message);
        const Arguments a = parseArgs(args);
        QCOMPARE(a.mode, Mode::Error);
        QVERIFY2(a.error.contains(message), qPrintable(a.error));
    }

    void screenshotOptionsAreHiddenGuiOptions() {
        const Arguments a = parseArgs({"--screenshot", "s.png", "--screenshot-theme", "dark",
                                       "--screenshot-monochrome", "--find", "Данные", "a.birch"});
        QCOMPARE(a.mode, Mode::Gui);
        QCOMPARE(a.screenshot, QStringLiteral("s.png"));
        QCOMPARE(a.screenshotTheme, QStringLiteral("dark"));
        QVERIFY(a.screenshotMonochrome);
        QCOMPARE(a.find, QStringLiteral("Данные"));
        QVERIFY(!helpText("birch").contains(QStringLiteral("screenshot")));
        QVERIFY(!helpText("birch").contains(QStringLiteral("--find")));
    }

    void qtOptionsAreLeftToQt() {
        // -style fusion иначе разобралось бы как -s -t -y… и сломало разбор.
        const QStringList args{"birch",           "-style",   "fusion", "--platform=offscreen", "-reverse",
                               "-qwindowtitle",   "Заголовок", "-m",    "-o",                   "a.png",
                               "a.birch",         "--",       "-style"};
        const QStringList cleaned = withoutQtOptions(args);
        QCOMPARE(cleaned, (QStringList{"birch", "-m", "-o", "a.png", "a.birch", "--", "-style"}));
        QCOMPARE(parse(cleaned).mode, Mode::Error);  // «-style» после «--» — второй файл
        QCOMPARE(parse(withoutQtOptions({"birch", "-style", "fusion", "a.birch"})).mode, Mode::Gui);
    }

    void programNameAsInvoked() {
        QCOMPARE(programName("/usr/local/bin/birch"), QStringLiteral("birch"));
        QCOMPARE(programName("/Applications/Birch.app/Contents/MacOS/Birch"), QStringLiteral("Birch"));
        QCOMPARE(programName("C:/Program Files/Birch/bin/Birch.exe"), QStringLiteral("Birch"));
        QCOMPARE(programName("C:/Program Files/Birch/cmd/birch.com"), QStringLiteral("birch"));
        QCOMPARE(programName("birch"), QStringLiteral("birch"));  // набрано в cmd через birch.com
        QCOMPARE(programName(""), QStringLiteral("birch"));
    }

    // --- справка и версия --------------------------------------------------------

    void helpIsGnuStyle() {
        const QString help = helpText("birch");
        const QStringList lines = help.split(u'\n');
        QCOMPARE(lines.first(), QStringLiteral("Использование: birch [ПАРАМЕТР]... [ФАЙЛ]"));
        for (const QString& line : lines) QVERIFY2(line.size() <= 80, qPrintable(line));
        for (const char* option : {"-o, --output=ФАЙЛ", "-t, --theme=ТЕМА", "-m, --monochrome", "-H, --horizontal",
                                   "-a, --transparent", "-s, --scale=N", "-h, --help", "-v, --version"})
            QVERIFY2(help.contains(QString::fromUtf8(option)), option);
        QVERIFY(help.contains(QStringLiteral("Об ошибках сообщайте:")));
        QVERIFY(help.endsWith(u'\n'));
        QVERIFY(helpText("Birch").startsWith(QStringLiteral("Использование: Birch ")));
    }

    void versionIsGnuStyle() {
        const QStringList lines = versionText("1.2.3").split(u'\n');
        QCOMPARE(lines[0], QStringLiteral("birch (Birch) 1.2.3"));  // эту строку разбирают скрипты
        QCOMPARE(lines[1], QStringLiteral("Copyright (C) 2026 Marth Scoobert."));
        QVERIFY(lines.join(u'\n').contains(QStringLiteral("GPLv3")));
        QVERIFY(lines.join(u'\n').contains(QStringLiteral("Qt %1").arg(QString::fromLatin1(qVersion()))));
        QVERIFY(lines.join(u'\n').contains(QStringLiteral("Graphviz ")));
        for (const QString& line : lines) QVERIFY2(line.size() <= 80, qPrintable(line));
    }

    void usageErrorIsTwoLines() {
        QCOMPARE(usageErrorText("birch", "неизвестный ключ «--colour»"),
                 QStringLiteral("birch: неизвестный ключ «--colour»\n"
                                "Попробуйте «birch --help» для получения дополнительной информации.\n"));
    }

    // --- рендер -------------------------------------------------------------------

    void renderFormats_data() {
        QTest::addColumn<QString>("file");
        QTest::addColumn<QByteArray>("start");
        QTest::newRow("png") << "out.png" << QByteArray("\x89PNG");
        QTest::newRow("svg") << "out.svg" << QByteArray("<?xml");
        QTest::newRow("mmd") << "out.mmd" << QByteArray("flowchart TD");
    }

    void renderFormats() {
        QFETCH(QString, file);
        QFETCH(QByteArray, start);
        QTemporaryDir dir;
        const Arguments a = parseArgs({example("frames.birch"), "-o", dir.filePath(file)});
        QString errors;
        QCOMPARE(render(a, "birch", errors), int(kExitOk));
        QVERIFY2(errors.isEmpty(), qPrintable(errors));
        QFile f(dir.filePath(file));
        QVERIFY(f.open(QIODevice::ReadOnly));
        QVERIFY(f.readAll().startsWith(start));
    }

    void renderLooks() {
        QTemporaryDir dir;
        QString errors;
        const QString light = dir.filePath("light.png");
        const QString dark = dir.filePath("dark.png");
        const QString big = dir.filePath("big.png");
        QCOMPARE(render(parseArgs({example("welcome.birch"), "-o", light, "-s", "1"}), "birch", errors), 0);
        QCOMPARE(render(parseArgs({example("welcome.birch"), "-o", dark, "-s", "1", "-t", "dark"}), "birch", errors), 0);
        QCOMPARE(render(parseArgs({example("welcome.birch"), "-o", big}), "birch", errors), 0);
        const QImage l(light), d(dark), b(big);
        QCOMPARE(l.pixelColor(0, 0), Theme::light().background);
        QCOMPARE(d.pixelColor(0, 0), Theme::dark().background);
        QCOMPARE(b.width(), l.width() * 2);  // масштаб по умолчанию — 2

        const QString clear = dir.filePath("clear.png");
        QCOMPARE(render(parseArgs({example("welcome.birch"), "-o", clear, "-a"}), "birch", errors), 0);
        QCOMPARE(QImage(clear).pixelColor(0, 0).alpha(), 0);
    }

    void diagramErrorsWriteNothing() {
        QTemporaryDir dir;
        const QString src = dir.filePath("bad.birch");
        QFile f(src);
        QVERIFY(f.open(QIODevice::WriteOnly));
        f.write("[A] -> [B\n{X} -> [Y]\n[Y] -> (X)\n");
        f.close();
        QString errors;
        const QString out = dir.filePath("bad.png");
        QCOMPARE(render(parseArgs({src, "-o", out}), "birch", errors), int(kExitDiagramErrors));
        QVERIFY(!QFile::exists(out));
        QVERIFY2(errors.contains(src + QStringLiteral(":1:8: ошибка: Не закрыта скобка")), qPrintable(errors));
        QVERIFY(errors.contains(src + QStringLiteral(":3:8: предупреждение: ")));
    }

    void warningsStillRender() {
        QTemporaryDir dir;
        const QString src = dir.filePath("warn.birch");
        QFile f(src);
        QVERIFY(f.open(QIODevice::WriteOnly));
        f.write("{X} -> [Y]\n[Y] -> (X)\n");
        f.close();
        QString errors;
        QCOMPARE(render(parseArgs({src, "-o", dir.filePath("w.svg")}), "birch", errors), int(kExitOk));
        QVERIFY(errors.contains(QStringLiteral("предупреждение")));
        QVERIFY(QFile::exists(dir.filePath("w.svg")));
    }

    void emptyDiagram() {
        QTemporaryDir dir;
        QFile f(dir.filePath("empty.birch"));
        QVERIFY(f.open(QIODevice::WriteOnly));
        f.write("// только комментарий\n");
        f.close();
        QString errors;
        QCOMPARE(render(parseArgs({f.fileName(), "-o", dir.filePath("e.png")}), "birch", errors),
                 int(kExitDiagramErrors));
        QVERIFY(errors.contains(QStringLiteral("схема пуста")));
    }

    void ioErrors() {
        QTemporaryDir dir;
        QString errors;
        QCOMPARE(render(parseArgs({dir.filePath("нет такого.birch"), "-o", dir.filePath("a.png")}), "birch", errors),
                 int(kExitIoError));
        QVERIFY(errors.startsWith(QStringLiteral("birch: не удалось открыть «")));
        errors.clear();
        QCOMPARE(render(parseArgs({example("welcome.birch"), "-o", dir.filePath("нет/папки/a.png")}), "birch", errors),
                 int(kExitIoError));
        QVERIFY(errors.startsWith(QStringLiteral("birch: не удалось записать «")));
    }
};

QTEST_MAIN(TestCli)
#include "tst_cli.moc"
