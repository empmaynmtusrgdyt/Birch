#pragma once

class QWidget;

namespace birch {

// Пункт меню «Установить команду birch…»: на macOS и в AppImage, где пакет
// сам команду в терминале не ставит (см. core/ShellCommand.h).
bool canInstallShellCommand();
void installShellCommand(QWidget* parent);

}  // namespace birch
