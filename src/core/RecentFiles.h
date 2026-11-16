/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <QStringList>

namespace birch::recent {

// Список недавних файлов для меню «Файл → Недавние файлы»: последний — первым,
// без повторов, не длиннее kMaxFiles.
inline constexpr qsizetype kMaxFiles = 10;

// Добавляет путь в начало; тот же файл под другой записью пути не дублируется.
QStringList add(QStringList list, const QString& path);

// Убирает файл из списка.
QStringList remove(QStringList list, const QString& path);

// Подписи для меню: имя файла, а если имена совпадают — с папкой: «схема.birch — Документы».
QStringList labels(const QStringList& list);

}  // namespace birch::recent
