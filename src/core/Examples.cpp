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
    const QStringList files = QDir(dir).entryList({QStringLiteral("*.birch")}, QDir::Files);
    for (const QString& file : files) {
        QFile f(dir + u'/' + file);
        if (f.open(QIODevice::ReadOnly) && normalized(QString::fromUtf8(f.readAll())) == wanted)
            return file.section(u'.', 0, 0);
    }
    return {};
}

}  // namespace birch::examples
