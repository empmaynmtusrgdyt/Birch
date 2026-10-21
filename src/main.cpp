#include <QApplication>
#include <QCommandLineParser>
#include <QFileOpenEvent>
#include <QIcon>
#include <QStyle>
#include <QFile>
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
    QCommandLineOption themeOption(QStringLiteral("theme"), QStringLiteral("Тема на этот запуск: system, light, dark."),
                                   QStringLiteral("name"));
    themeOption.setFlags(QCommandLineOption::HiddenFromHelp);
    cli.addOption(themeOption);
    cli.process(app);

    birch::MainWindow window;
    FileOpenFilter fileOpenFilter(&window);
    app.installEventFilter(&fileOpenFilter);

    if (cli.isSet(themeOption) && !window.setThemeByName(cli.value(themeOption))) {
        std::fprintf(stderr, "unknown theme: %s\n", qPrintable(cli.value(themeOption)));
        return 3;
    }

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
                                 }
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
