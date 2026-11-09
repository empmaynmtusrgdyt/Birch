/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "core/Examples.h"

#include <QDir>
#include <QFile>

#include "core/Language.h"

namespace birch::examples {

namespace {

// Сравниваем без учёта переводов строк Windows (в том числе в самих примерах,
// если git выложил их с \r\n) и пустых строк по краям.
QString normalized(QString text) {
    text.remove(u'\r');
    return text.trimmed();
}

// Прошлые версии стартового примера. Если такой текст сохранился в сеансе после
// обновления Birch, вместо него показывается нынешний пример.
const char* const kLegacyWelcome[] = {
    // до 0.3.3 включительно: «_» в тексте узла показывался как пробел
    "// Добро пожаловать в Birch!\n"
    "// [Процесс]   {Условие?}   (Старт/Конец)   подпись стрелки — в кавычках после узла\n"
    "\n"
    "(Старт) -> [Ввод_данных] -> {Данные корректны?};\n"
    "{Данные корректны?} -> [Сохранить] \"Да\" -> (Конец);\n"
    "{Данные корректны?} -> [Показать_ошибку] \"Нет\"\n"
    "    -> [Ввод_данных];",
    // с 0.4.0 до 0.7.0: с комментариями в начале
    "// Добро пожаловать в Birch!\n"
    "// [Процесс]   {Условие?}   (Старт/Конец)   подпись стрелки — в кавычках после узла\n"
    "\n"
    "(Старт) -> [Ввод данных] -> {Данные корректны?};\n"
    "{Данные корректны?} -> [Сохранить] \"Да\" -> (Конец);\n"
    "{Данные корректны?} -> [Показать ошибку] \"Нет\"\n"
    "    -> [Ввод данных];",
};

}  // namespace

QString path(const QString& dir, const QString& name, const QString& language) {
    if (language != QLatin1String(i18n::kSourceLanguage)) {
        for (const QString& code : {language, QString::fromLatin1(i18n::kFallbackLanguage)}) {
            const QString localized = QStringLiteral("%1/%2.%3.birch").arg(dir, name, code);
            if (QFile::exists(localized)) return localized;
        }
    }
    return QStringLiteral("%1/%2.birch").arg(dir, name);
}

QString untouchedName(const QString& dir, const QString& text) {
    const QString wanted = normalized(text);
    if (wanted.isEmpty()) return {};
    for (const char* legacy : kLegacyWelcome) {
        if (wanted == normalized(QString::fromUtf8(legacy))) return QStringLiteral("welcome");
    }
    const QStringList files = QDir(dir).entryList({QStringLiteral("*.birch")}, QDir::Files);
    for (const QString& file : files) {
        QFile f(dir + u'/' + file);
        if (f.open(QIODevice::ReadOnly) && normalized(QString::fromUtf8(f.readAll())) == wanted)
            return file.section(u'.', 0, 0);
    }
    return {};
}

}  // namespace birch::examples
