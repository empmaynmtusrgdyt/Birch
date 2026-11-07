/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "core/Language.h"

#include <QCoreApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QLocale>
#include <QRegularExpression>
#include <QTranslator>

namespace birch::i18n {

namespace {

QString primary(const QString& tag) {
    // «ru-RU», «ru_RU», «en-Latn-US» → «ru», «en»
    return tag.section(QRegularExpression(QStringLiteral("[-_]")), 0, 0).toLower();
}

QString g_current;

}  // namespace

QStringList availableLanguages() {
    QStringList codes{QString::fromLatin1(kSourceLanguage)};
    const QStringList files = QDir(QStringLiteral(":/i18n")).entryList({QStringLiteral("birch_*.qm")});
    for (const QString& file : files) {
        const QString code = file.mid(6).chopped(3);  // birch_<код>.qm
        if (!codes.contains(code)) codes << code;
    }
    codes.sort();
    return codes;
}

QString nativeName(const QString& code) {
    // Qt называет «en» по региону по умолчанию — «American English»
    if (code == QLatin1String("en")) return QStringLiteral("English");
    const QString name = QLocale(code).nativeLanguageName();
    if (name.isEmpty()) return code;
    return name.left(1).toUpper() + name.mid(1);  // «русский» → «Русский»
}

QString choose(const QString& preferred, const QStringList& uiLanguages, const QStringList& available) {
    const QString wanted = primary(preferred);
    if (available.contains(wanted)) return wanted;
    for (const QString& tag : uiLanguages) {
        const QString code = primary(tag);
        if (available.contains(code)) return code;
    }
    const QString fallback = QString::fromLatin1(kFallbackLanguage);
    return available.contains(fallback) ? fallback : QString::fromLatin1(kSourceLanguage);
}

QString systemChoice() {
    return choose(qEnvironmentVariable(kEnvironmentVariable), QLocale::system().uiLanguages(),
                  availableLanguages());
}

void install(const QString& code) {
    static QTranslator birch;
    static QTranslator qt;
    QCoreApplication::removeTranslator(&birch);
    QCoreApplication::removeTranslator(&qt);

    // Для исходного языка файл содержит только формы множественного числа.
    if (birch.load(QStringLiteral(":/i18n/birch_%1.qm").arg(code))) QCoreApplication::installTranslator(&birch);

    // Стандартные диалоги и контекстные меню Qt: встроенный перевод, иначе — из Qt.
    if (qt.load(QStringLiteral(":/i18n/qtbase_%1.qm").arg(code))
        || qt.load(QStringLiteral("qtbase_%1").arg(code), QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        QCoreApplication::installTranslator(&qt);
    }
    g_current = code;
}

QString current() { return g_current; }

}  // namespace birch::i18n
