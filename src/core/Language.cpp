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

// Подходящий тегу язык из доступных: тот же язык и та же письменность. Регион
// важен, только если есть выбор: «pt-PT» получит pt_BR, «zh-Hans-CN» — zh_CN, а
// «zh-Hant-TW» (традиционные иероглифы) упрощённый китайский не получит.
QString match(const QString& tag, const QStringList& available) {
    // «ru_RU.UTF-8», «sr_RS@latin» → «ru_RU», «sr_RS»
    const QString name = tag.section(QRegularExpression(QStringLiteral("[.@]")), 0, 0);
    if (name.isEmpty()) return {};
    const QLocale wanted(name);
    if (wanted.language() == QLocale::C || wanted.language() == QLocale::AnyLanguage) return {};
    QString sameLanguage;
    for (const QString& code : available) {
        const QLocale locale(code);
        if (locale.language() != wanted.language() || locale.script() != wanted.script()) continue;
        if (locale.territory() == wanted.territory()) return code;
        if (sameLanguage.isEmpty()) sameLanguage = code;
    }
    return sameLanguage;
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
    // Название задаёт сам перевод: у Qt оно бывает с регионом по умолчанию —
    // «American English», «español de España».
    //: Название языка этого перевода на нём самом: Deutsch, Español, 日本語
    static const char* const kOwnName = QT_TRANSLATE_NOOP("Language", "Русский");
    if (code == QLatin1String(kSourceLanguage)) return QString::fromUtf8(kOwnName);
    QTranslator translator;
    if (translator.load(QStringLiteral(":/i18n/birch_%1.qm").arg(code))) {
        const QString own = translator.translate("Language", kOwnName);
        if (!own.isEmpty()) return own;
    }
    const QString name = QLocale(code).nativeLanguageName();
    if (name.isEmpty()) return code;
    return name.left(1).toUpper() + name.mid(1);  // «русский» → «Русский»
}

QString choose(const QString& preferred, const QStringList& uiLanguages, const QStringList& available) {
    if (const QString code = match(preferred, available); !code.isEmpty()) return code;
    // Qt дописывает в конец списка обрезанные теги: «zh-Hant-TW», «zh-TW», «zh».
    // Обрезанный тег — не отдельный выбор пользователя: голый «zh» Qt считает
    // упрощённым китайским, а пользователь просил традиционный.
    const auto truncationOfEarlier = [&uiLanguages](qsizetype index) {
        const QString tag = uiLanguages[index];
        for (qsizetype i = 0; i < index; ++i) {
            const QString& earlier = uiLanguages[i];
            if (earlier.size() > tag.size() && earlier.startsWith(tag, Qt::CaseInsensitive)
                && (earlier[tag.size()] == u'-' || earlier[tag.size()] == u'_'))
                return true;
        }
        return false;
    };
    for (qsizetype i = 0; i < uiLanguages.size(); ++i) {
        if (truncationOfEarlier(i)) continue;
        if (const QString code = match(uiLanguages[i], available); !code.isEmpty()) return code;
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
