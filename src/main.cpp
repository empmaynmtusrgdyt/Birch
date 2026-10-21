#include <QApplication>
#include <QCommandLineParser>
#include <QFileOpenEvent>
#include <QIcon>
#include <QTimer>

#include <cstdio>

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

}  // namespace

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("Birch"));
    QApplication::setApplicationDisplayName(QStringLiteral("Birch"));
    QApplication::setOrganizationName(QStringLiteral("Birch"));
    QApplication::setOrganizationDomain(QStringLiteral("birch.app"));
    QApplication::setApplicationVersion(QStringLiteral(BIRCH_VERSION));
    QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/app.svg")));
#if defined(Q_OS_LINUX)
    QGuiApplication::setDesktopFileName(QStringLiteral("birch"));  // связь окна с birch.desktop
#endif

    QCommandLineParser cli;
    cli.setApplicationDescription(QStringLiteral("Блок-схемы из текста"));
    cli.addHelpOption();
    cli.addVersionOption();
    cli.addPositionalArgument(QStringLiteral("file"), QStringLiteral("Файл .birch для открытия"));
    // Смоук-тест для CI: дождаться первой отрисовки, сохранить снимок окна и выйти.
    QCommandLineOption screenshotOption(QStringLiteral("screenshot"),
                                        QStringLiteral("Сохранить снимок окна после отрисовки и выйти."),
                                        QStringLiteral("file.png"));
    screenshotOption.setFlags(QCommandLineOption::HiddenFromHelp);
    cli.addOption(screenshotOption);
    cli.process(app);

    birch::MainWindow window;
    FileOpenFilter fileOpenFilter(&window);
    app.installEventFilter(&fileOpenFilter);

    const QString screenshot = cli.value(screenshotOption);
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
                                 const bool saved = window.grab().save(screenshot);
                                 std::printf("screenshot %s: %s\n", saved ? "saved" : "FAILED", qPrintable(screenshot));
                                 QCoreApplication::exit(saved ? 0 : 1);
                             });
                         });
        QTimer::singleShot(30000, &app, [] {
            std::fprintf(stderr, "screenshot: timeout, no render\n");
            QCoreApplication::exit(2);
        });
    }

    if (!cli.positionalArguments().isEmpty()) window.openFile(cli.positionalArguments().constFirst());
    window.show();
    return QApplication::exec();
}
