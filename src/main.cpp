/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <QApplication>
#include <QFileOpenEvent>
#include <QIcon>
#include <QLocale>
#include <QLoggingCategory>
#include <QSettings>

#include <QStyle>
#include <QFile>
#include <QTimer>

#include <cstdio>
#include <cstring>

#if defined(Q_OS_MACOS)
#include <mach-o/dyld.h>
#include <unistd.h>

#include <climits>
#include <cstdlib>
#include <string>
#endif
#if defined(Q_OS_LINUX)
#include <langinfo.h>

#include <clocale>
#endif

#include "core/CommandLine.h"
#include "core/Language.h"
#include "ui/MainWindow.h"

namespace {

// macOS отдаёт открываемые из Finder файлы событием, а не аргументом командной строки.
// На Windows и Linux файл приходит аргументом.
class FileOpenFilter : public QObject {
public:
    explicit FileOpenFilter(birch::MainWindow* window) : window_(window) {}

protected:
    bool eventFilter(QObject* watched, QEvent* event) override {
        if (event->type() == QEvent::FileOpen) {
            const QString path = static_cast<QFileOpenEvent*>(event)->file();
            if (!path.isEmpty()) window_->openFromSystem(path);
            return true;
        }
        return QObject::eventFilter(watched, event);
    }

private:
    birch::MainWindow* window_;
};

#if defined(Q_OS_MACOS)
// Запуск по ссылке (/usr/local/bin/birch → Birch.app/Contents/MacOS/Birch): Qt
// ищет свои плагины и бандл по пути запуска, то есть рядом со ссылкой, и не
// находит их. Перезапускаемся по настоящему пути; argv[0] остаётся «birch».
void execRealPathIfLinked(char* argv[]) {
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    std::string path(size, '\0');
    if (_NSGetExecutablePath(path.data(), &size) != 0) return;
    path.resize(std::strlen(path.c_str()));
    char real[PATH_MAX];
    if (realpath(path.c_str(), real) == nullptr || path == real) return;
    execv(real, argv);  // если не вышло — продолжаем как есть
}
#endif

// В контейнерах и cron на Linux часто локаль «C» (ASCII): Qt ругается на неё в
// stderr и неверно читает русские имена файлов. Для командной строки берём C.UTF-8.
void prepareLocale(birch::cli::Mode mode) {
#if defined(Q_OS_LINUX)
    if (mode == birch::cli::Mode::Gui) return;
    if (std::setlocale(LC_ALL, "") == nullptr || std::strcmp(nl_langinfo(CODESET), "UTF-8") != 0) {
        qputenv("LC_ALL", "C.UTF-8");
    }
#else
    Q_UNUSED(mode);
#endif
}

}  // namespace

int main(int argc, char* argv[]) {
    namespace cli = birch::cli;
    namespace i18n = birch::i18n;
#if defined(Q_OS_MACOS)
    execRealPathIfLinked(argv);
#endif

    // Ключи разбираются до создания приложения: --help и --version работают
    // мгновенно и без экрана, а -o рисует схему без окна.
    const QStringList rawArgs = cli::rawArguments(argc, argv);
    const QString program = cli::programName(rawArgs.value(0));
    cli::Arguments args = cli::parse(cli::withoutQtOptions(rawArgs));

    // Язык командной строки и окна выбирается до правки локали ниже: она меняет
    // переменные среды, по которым Qt определяет язык системы.
    const QString systemLanguage = i18n::systemChoice();
    prepareLocale(args.mode);

    switch (args.mode) {
    case cli::Mode::Help:
    case cli::Mode::Version:
    case cli::Mode::Error: {
        cli::attachParentConsole();
        // Окна нет, но переводам нужен QCoreApplication. Ключи разбираются ещё раз —
        // уже с переводом, чтобы сообщение об ошибке было на нужном языке.
        QCoreApplication app(argc, argv);
        i18n::install(systemLanguage);
        args = cli::parse(cli::withoutQtOptions(rawArgs));
        if (args.mode == cli::Mode::Help) {
            cli::printOut(cli::helpText(program));
            return cli::kExitOk;
        }
        if (args.mode == cli::Mode::Version) {
            cli::printOut(cli::versionText(QStringLiteral(BIRCH_VERSION)));
            return cli::kExitOk;
        }
        cli::printErr(cli::usageErrorText(program, args.error));
        return cli::kExitUsage;
    }
    case cli::Mode::Render: {
        cli::attachParentConsole();
        // Окно не нужно, но шрифты измеряет Qt. На macOS и Linux — без экрана
        // (работает и по SSH, и в CI); на Windows у offscreen нет системных шрифтов.
#if !defined(Q_OS_WIN)
        if (!qEnvironmentVariableIsSet("QT_QPA_PLATFORM")) qputenv("QT_QPA_PLATFORM", "offscreen");
#endif
        // Служебные сообщения Qt о шрифтах в режиме без экрана — не для пользователя.
        QLoggingCategory::setFilterRules(QStringLiteral("qt.qpa.*.warning=false"));
        QGuiApplication app(argc, argv);
        i18n::install(systemLanguage);
        QString errors;
        const int code = cli::render(args, program, errors);
        cli::printErr(errors);
        return code;
    }
    case cli::Mode::Gui:
        break;
    }

    QApplication app(argc, argv);
#if defined(Q_OS_WIN)
    // Стиль windowsvista (Windows 10) не умеет тёмную тему: при её выборе редактор
    // и меню оставались светлыми. Fusion поддерживает обе темы. На Windows 11
    // остаётся родной стиль windows11 — он переключает темы сам.
    if (QApplication::style()->name().compare(QLatin1String("windowsvista"), Qt::CaseInsensitive) == 0)
        QApplication::setStyle(QStringLiteral("fusion"));
#endif
    QApplication::setApplicationName(QStringLiteral("Birch"));
    QApplication::setApplicationDisplayName(QStringLiteral("Birch"));
    QApplication::setOrganizationName(QStringLiteral("Birch"));
    QApplication::setOrganizationDomain(QStringLiteral("birch.app"));
    QApplication::setApplicationVersion(QStringLiteral(BIRCH_VERSION));
    QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/app.svg")));
#if defined(Q_OS_LINUX)
    QGuiApplication::setDesktopFileName(QStringLiteral("birch"));  // связь окна с birch.desktop
#endif

    // Язык окна: BIRCH_LANGUAGE, иначе выбор в меню «Вид → Язык», иначе язык системы
    QString language = qEnvironmentVariable(i18n::kEnvironmentVariable);
    if (language.isEmpty()) language = QSettings().value(QLatin1String(i18n::kSettingsKey)).toString();
    i18n::install(i18n::choose(language, QLocale::system().uiLanguages(), i18n::availableLanguages()));

    birch::MainWindow window;
    FileOpenFilter fileOpenFilter(&window);
    app.installEventFilter(&fileOpenFilter);

    // Служебные ключи для снимков окна в CI.
    if (!args.screenshotTheme.isEmpty() && !window.setThemeByName(args.screenshotTheme)) {
        cli::printErr(cli::usageErrorText(program, QStringLiteral("неверное значение «%1» для ключа «--screenshot-theme»")
                                                       .arg(args.screenshotTheme)));
        return cli::kExitUsage;
    }
    if (args.screenshotMonochrome) window.setMonochrome(true);

    const QString screenshot = args.screenshot;
    if (!screenshot.isEmpty()) {
        window.setSessionSaving(false);
        QObject::connect(&window, &birch::MainWindow::previewRendered, &app,
                         [&](bool ok, const QString& error) {
                             if (!ok) {
                                 std::fprintf(stderr, "render error: %s\n", qPrintable(error));
                                 QCoreApplication::exit(1);
                                 return;
                             }
                             // Даём окну один цикл отрисоваться, затем снимаем его.
                             QTimer::singleShot(500, &window, [&] {
                                 const QImage image = window.grab().toImage();
                                 const bool saved = image.save(screenshot);
                                 std::printf("screenshot %s: %s\n", saved ? "saved" : "FAILED", qPrintable(screenshot));

                                 // Рядом — фактический цвет фона редактора на снимке (низ
                                 // области текста, где нет строк): по нему CI проверяет темы.
                                 const QRect editor = window.editorViewportRect();
                                 const qreal dpr = image.devicePixelRatio();
                                 const QPoint sample(qRound(editor.center().x() * dpr),
                                                     qRound((editor.bottom() - 12) * dpr));
                                 const QColor bg = image.pixelColor(sample);
                                 QFile info(screenshot + QStringLiteral(".txt"));
                                 if (info.open(QIODevice::WriteOnly | QIODevice::Text)) {
                                     info.write(QStringLiteral("editor_background=%1\nlightness=%2\nstyle=%3\n")
                                                    .arg(bg.name())
                                                    .arg(bg.lightness())
                                                    .arg(QApplication::style()->name())
                                                    .toUtf8());
                                     // Панель поиска помещается в своё место, ничего не наезжает.
                                     if (window.isFindBarOpen())
                                         info.write(QByteArray("find_bar_fits=") + (window.findBarFits() ? "1" : "0") + '\n');
                                 }
                                 QCoreApplication::exit(saved ? 0 : 1);
                             });
                         });
        QTimer::singleShot(30000, &app, [] {
            std::fprintf(stderr, "screenshot: timeout, no render\n");
            QCoreApplication::exit(2);
        });
    }

    if (!args.input.isEmpty()) window.openFile(args.input);
    window.show();
    cli::releaseConsoleLauncher();
    if (!args.find.isEmpty()) window.openReplace(args.find);
    return QApplication::exec();
}
