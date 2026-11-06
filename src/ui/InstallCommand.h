/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

class QWidget;

namespace birch {

// Пункт меню «Установить команду birch…»: на macOS и в AppImage, где пакет
// сам команду в терминале не ставит (см. core/ShellCommand.h).
bool canInstallShellCommand();
void installShellCommand(QWidget* parent);

}  // namespace birch
