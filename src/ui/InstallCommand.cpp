#include "ui/InstallCommand.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>

#include "core/ShellCommand.h"

namespace birch {

namespace {

struct Paths {
    QString link;    // где появится команда birch
    QString target;  // на что она ссылается
};

Paths shellCommandPaths() {
#if defined(Q_OS_MACOS)
    return {QStringLiteral("/usr/local/bin/birch"), QCoreApplication::applicationFilePath()};
#elif defined(Q_OS_LINUX)
    // Переменную APPIMAGE задаёт сам AppImage: путь к его файлу.
    const QString appImage = qEnvironmentVariable("APPIMAGE");
    if (!appImage.isEmpty()) return {QDir::homePath() + QStringLiteral("/.local/bin/birch"), appImage};
    return {};
#else
    return {};
#endif
}

QString tryText() {
    return QCoreApplication::translate("InstallCommand",
                                       "Откройте новое окно терминала и попробуйте:\n\n"
                                       "    birch --help\n"
                                       "    birch схема.birch -o схема.png");
}

void showInstalled(QWidget* parent, const Paths& paths) {
    QString text = QCoreApplication::translate("InstallCommand", "Команда birch установлена: %1.")
                       .arg(paths.link)
                 + QStringLiteral("\n\n") + tryText();
#if defined(Q_OS_LINUX)
    const QString binDir = QFileInfo(paths.link).absolutePath();
    if (!shell::directoryInPath(binDir, qEnvironmentVariable("PATH"))) {
        text += QCoreApplication::translate(
                    "InstallCommand",
                    "\n\nПапки ~/.local/bin пока нет в PATH. Обычно она появляется там после "
                    "повторного входа в систему. Если нет — добавьте в ~/.bashrc строку:\n\n"
                    "    export PATH=\"$HOME/.local/bin:$PATH\"");
    }
    text += QCoreApplication::translate("InstallCommand",
                                        "\n\nЕсли переместите файл AppImage, выберите этот пункт ещё раз.");
#endif
    QMessageBox::information(parent, QCoreApplication::translate("InstallCommand", "Команда birch"), text);
}

#if defined(Q_OS_MACOS)
// В /usr/local/bin обычно может писать только администратор: macOS спросит пароль.
void installWithAdminRights(QWidget* parent, const Paths& paths) {
    auto* process = new QProcess(parent);
    process->setProgram(QStringLiteral("/usr/bin/osascript"));
    // Пути передаются аргументами, а не вставляются в текст скрипта: так их не нужно экранировать.
    process->setArguments({
        QStringLiteral("-e"), QStringLiteral("on run argv"),
        QStringLiteral("-e"),
        QStringLiteral("do shell script \"mkdir -p /usr/local/bin && ln -sf \" & quoted form of item 1 of argv"
                       " & \" \" & quoted form of item 2 of argv with prompt \"%1\" with administrator privileges")
            .arg(QCoreApplication::translate("InstallCommand",
                                             "Birch установит команду birch в /usr/local/bin.")),
        QStringLiteral("-e"), QStringLiteral("end run"),
        paths.target, paths.link,
    });
    QObject::connect(process, &QProcess::finished, parent, [parent, process, paths](int code) {
        process->deleteLater();
        if (code == 0 && shell::linkState(paths.link, paths.target) == shell::LinkState::Ours) {
            showInstalled(parent, paths);
            return;
        }
        const QString err = QString::fromUtf8(process->readAllStandardError()).trimmed();
        if (err.contains(QLatin1String("-128"))) return;  // пользователь нажал «Отменить»
        QMessageBox::warning(parent, QCoreApplication::translate("InstallCommand", "Команда birch"),
                             QCoreApplication::translate("InstallCommand",
                                                         "Не удалось установить команду birch.\n\n%1")
                                 .arg(err));
    });
    process->start();
}
#endif

}  // namespace

bool canInstallShellCommand() { return !shellCommandPaths().link.isEmpty(); }

void installShellCommand(QWidget* parent) {
    const Paths paths = shellCommandPaths();
    if (paths.link.isEmpty()) return;
    const QString title = QCoreApplication::translate("InstallCommand", "Команда birch");

#if defined(Q_OS_MACOS)
    // Программа запущена прямо из образа .dmg или macOS переместила её во временную
    // папку (так бывает, пока приложение не открыли через «Открыть»): ссылка на
    // такой путь перестанет работать.
    if (paths.target.startsWith(QLatin1String("/Volumes/")) || paths.target.contains(QLatin1String("/AppTranslocation/"))) {
        QMessageBox::warning(parent, title,
                             QCoreApplication::translate(
                                 "InstallCommand",
                                 "Birch запущен не из папки «Программы», поэтому команда на него "
                                 "перестанет работать.\n\nПеретащите Birch в «Программы», откройте его "
                                 "оттуда и выберите этот пункт ещё раз. Если Birch уже там, выполните "
                                 "в Терминале:\n\n    xattr -dr com.apple.quarantine /Applications/Birch.app"));
        return;
    }
#endif

    switch (shell::linkState(paths.link, paths.target)) {
    case shell::LinkState::Ours:
        QMessageBox::information(parent, title,
                                 QCoreApplication::translate("InstallCommand", "Команда birch уже установлена: %1.")
                                         .arg(paths.link)
                                     + QStringLiteral("\n\n") + tryText());
        return;
    case shell::LinkState::Other:
        if (QMessageBox::question(parent, title,
                                  QCoreApplication::translate("InstallCommand",
                                                              "По пути %1 уже есть другой файл. "
                                                              "Заменить его командой Birch?")
                                      .arg(paths.link))
            != QMessageBox::Yes)
            return;
        break;
    case shell::LinkState::Missing:
        break;
    }

    QString error;
    if (shell::createLink(paths.link, paths.target, &error)) {
        showInstalled(parent, paths);
        return;
    }
#if defined(Q_OS_MACOS)
    installWithAdminRights(parent, paths);
#else
    QMessageBox::warning(parent, title,
                         QCoreApplication::translate("InstallCommand", "Не удалось установить команду birch: %1.")
                             .arg(error));
#endif
}

}  // namespace birch
