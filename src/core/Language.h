/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <QString>
#include <QStringList>

namespace birch::i18n {

// Язык интерфейса.
//
// Исходный язык строк в коде — русский. Остальные языки — это встроенные файлы
// перевода :/i18n/birch_<код>.qm (из translations/birch_<код>.ts). Новый язык
// добавляется одним файлом перевода: список языков строится по ним.
//
// Порядок выбора:
//   1. переменная среды BIRCH_LANGUAGE (для скриптов и проверок);
//   2. в окне — выбор в меню «Вид → Язык» (настройка ui/language);
//   3. язык системы; если его перевода нет — английский.
// Командная строка, как утилиты GNU, смотрит только на 1 и 3.

inline constexpr char kSourceLanguage[] = "ru";
inline constexpr char kFallbackLanguage[] = "en";
inline constexpr char kEnvironmentVariable[] = "BIRCH_LANGUAGE";
inline constexpr char kSettingsKey[] = "ui/language";

// Коды доступных языков: исходный и все встроенные переводы, по алфавиту.
QStringList availableLanguages();

// Название языка на нём самом: «Русский», «English».
QString nativeName(const QString& code);

// Выбирает язык. preferred — явный выбор (переменная среды или настройка),
// пустой — «как в системе»; uiLanguages — языки системы по порядку
// предпочтения (QLocale::system().uiLanguages()).
QString choose(const QString& preferred, const QStringList& uiLanguages, const QStringList& available);

// Язык для командной строки: BIRCH_LANGUAGE или язык системы.
QString systemChoice();

// Подключает переводы Birch и стандартных диалогов Qt. Нужен уже созданный
// QCoreApplication. Для исходного языка подключает только формы множественного числа.
void install(const QString& code);

// Язык, подключённый install(); пустой — если install() ещё не вызывали.
QString current();

}  // namespace birch::i18n
