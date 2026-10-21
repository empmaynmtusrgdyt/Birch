#include <QApplication>
#include <QCommandLineParser>
#include <QFileOpenEvent>
#include <QIcon>

#include "ui/MainWindow.h"

namespace {

// macOS отдаёт открываемые из Finder файлы событием, а не аргументом командной строки.
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

    QCommandLineParser cli;
    cli.setApplicationDescription(QStringLiteral("Блок-схемы из текста"));
    cli.addHelpOption();
    cli.addVersionOption();
    cli.addPositionalArgument(QStringLiteral("file"), QStringLiteral("Файл .birch для открытия"));
    cli.process(app);

    birch::MainWindow window;
    FileOpenFilter fileOpenFilter(&window);
    app.installEventFilter(&fileOpenFilter);

    if (!cli.positionalArguments().isEmpty()) window.openFile(cli.positionalArguments().constFirst());
    window.show();
    return QApplication::exec();
}
