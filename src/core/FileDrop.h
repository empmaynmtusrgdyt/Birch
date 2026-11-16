/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <QString>

class QMimeData;

namespace birch {

// Файл схемы, который перетащили в окно: первый локальный файл .birch или .txt
// из перетаскиваемых. Пустая строка — если такого нет (тогда перетаскивание
// обычное: текст вставляется в редактор).
QString droppedDiagramFile(const QMimeData* mime);

}  // namespace birch
