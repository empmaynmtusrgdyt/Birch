/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <QString>

namespace birch::examples {

// Примеры схем: <имя>.birch на исходном языке и <имя>.<язык>.birch на остальных
// (welcome.birch, welcome.en.birch, frames.de.birch…). В программе они лежат в
// ресурсах :/examples, в тестах — в папке resources/examples.

// Путь к примеру на нужном языке. Если у языка нет своего примера — английский.
QString path(const QString& dir, const QString& name, const QString& language);

// Имя примера («welcome», «frames»), если text — один из примеров без изменений
// на любом языке; иначе пустая строка. Так после смены языка нетронутый пример
// заменяется тем же примером на новом языке, а свои схемы остаются как есть.
QString untouchedName(const QString& dir, const QString& text);

}  // namespace birch::examples
