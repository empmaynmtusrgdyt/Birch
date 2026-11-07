/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// Тесты выбора языка интерфейса и подключения переводов.

#include <QFile>
#include <QtTest>

#include "core/CommandLine.h"
#include "core/Language.h"

using namespace birch::i18n;

namespace {

// Ширина строки в колонках терминала: иероглифы и полноширинные знаки — по две.
int columns(const QString& line) {
    int width = 0;
    for (const QChar c : line) {
        const char16_t u = c.unicode();
        const bool wide = (u >= 0x1100 && u <= 0x115F) || (u >= 0x2E80 && u <= 0xA4CF) || (u >= 0xAC00 && u <= 0xD7A3)
                          || (u >= 0xF900 && u <= 0xFAFF) || (u >= 0xFE30 && u <= 0xFE4F) || (u >= 0xFF00 && u <= 0xFF60)
                          || (u >= 0xFFE0 && u <= 0xFFE6);
        width += wide ? 2 : 1;
    }
    return width;
}

}  // namespace

class TestLanguage : public QObject {
    Q_OBJECT

private slots:
    void availableHasSourceAndEnglish() {
        const QStringList codes = availableLanguages();
        QVERIFY(codes.contains("ru"));
        QVERIFY(codes.contains("en"));
        QVERIFY(std::is_sorted(codes.begin(), codes.end()));
    }

    void choose_data() {
        QTest::addColumn<QString>("preferred");
        QTest::addColumn<QStringList>("system");
        QTest::addColumn<QString>("expected");
        const QStringList ruSystem{"ru-RU", "en-US"};
        QTest::newRow("система русская") << "" << ruSystem << "ru";
        QTest::newRow("система английская") << "" << QStringList{"en-GB"} << "en";
        QTest::newRow("система немецкая — английский") << "" << QStringList{"de-DE"} << "en";
        QTest::newRow("немецкий, потом русский") << "" << QStringList{"de-DE", "ru"} << "ru";
        QTest::newRow("подтеги через _") << "" << QStringList{"ru_RU.UTF-8"} << "ru";
        QTest::newRow("явный выбор важнее системы") << "en" << ruSystem << "en";
        QTest::newRow("явный выбор с регионом") << "en_US" << ruSystem << "en";
        QTest::newRow("неизвестный выбор — по системе") << "xx" << ruSystem << "ru";
        QTest::newRow("язык «C»") << "" << QStringList{"C"} << "en";
        QTest::newRow("пустой список") << "" << QStringList{} << "en";
    }
    void choose() {
        QFETCH(QString, preferred);
        QFETCH(QStringList, system);
        QFETCH(QString, expected);
        QCOMPARE(birch::i18n::choose(preferred, system, {"en", "ru"}), expected);
    }

    // Языки с регионом в имени файла (pt_BR, zh_CN) и подбор по языку и письменности.
    void chooseWithRegions_data() {
        QTest::addColumn<QString>("preferred");
        QTest::addColumn<QStringList>("system");
        QTest::addColumn<QString>("expected");
        QTest::newRow("pt-BR") << "" << QStringList{"pt-BR"} << "pt_BR";
        QTest::newRow("pt-PT — бразильский") << "" << QStringList{"pt-PT"} << "pt_BR";
        QTest::newRow("явный pt") << "pt" << QStringList{"ru-RU"} << "pt_BR";
        QTest::newRow("zh-Hans-CN") << "" << QStringList{"zh-Hans-CN"} << "zh_CN";
        QTest::newRow("zh-CN") << "" << QStringList{"zh-CN"} << "zh_CN";
        QTest::newRow("явный zh") << "zh" << QStringList{"en-US"} << "zh_CN";
        QTest::newRow("zh-Hant-TW — не упрощённый") << "" << QStringList{"zh-Hant-TW"} << "en";
        QTest::newRow("zh-Hant-TW с обрезанными тегами Qt") << "" << QStringList{"zh-Hant-TW", "zh-TW", "zh"} << "en";
        QTest::newRow("традиционный, потом явно упрощённый") << "" << QStringList{"zh-Hant-TW", "zh", "zh-Hans-CN"} << "zh_CN";
        QTest::newRow("pt-PT с обрезанным pt") << "" << QStringList{"pt-PT", "pt"} << "pt_BR";
        QTest::newRow("de-AT с обрезанным de") << "" << QStringList{"de-AT", "de"} << "de";
        QTest::newRow("zh-TW, потом японский") << "" << QStringList{"zh-TW", "ja-JP"} << "ja";
        QTest::newRow("fr-CA") << "" << QStringList{"fr-CA"} << "fr";
        QTest::newRow("de-AT") << "" << QStringList{"de-AT"} << "de";
        QTest::newRow("es-MX") << "" << QStringList{"es-MX"} << "es";
        QTest::newRow("es-419") << "" << QStringList{"es-419"} << "es";
        QTest::newRow("de_DE.UTF-8") << "" << QStringList{"de_DE.UTF-8"} << "de";
        QTest::newRow("явный pt_BR") << "pt_BR" << QStringList{"de-DE"} << "pt_BR";
    }
    void chooseWithRegions() {
        QFETCH(QString, preferred);
        QFETCH(QStringList, system);
        QFETCH(QString, expected);
        const QStringList available{"de", "en", "es", "fr", "ja", "pt_BR", "ru", "zh_CN"};
        QCOMPARE(birch::i18n::choose(preferred, system, available), expected);
    }

    void withoutEnglishFallsBackToSource() {
        QCOMPARE(birch::i18n::choose("", {"de"}, {"ru"}), QStringLiteral("ru"));
    }

    void nativeNames() {
        QCOMPARE(nativeName("ru"), QStringLiteral("Русский"));
        QCOMPARE(nativeName("en"), QStringLiteral("English"));
        QCOMPARE(nativeName("es"), QStringLiteral("Español"));
        QCOMPARE(nativeName("de"), QStringLiteral("Deutsch"));
        QCOMPARE(nativeName("fr"), QStringLiteral("Français"));
        QCOMPARE(nativeName("pt_BR"), QStringLiteral("Português (Brasil)"));
        QCOMPARE(nativeName("zh_CN"), QStringLiteral("简体中文"));
        QCOMPARE(nativeName("ja"), QStringLiteral("日本語"));
        QCOMPARE(nativeName("xx"), QStringLiteral("xx"));  // нет ни перевода, ни названия в Qt
    }

    // Каждый встроенный перевод подключается, переводит окно, а справка
    // командной строки помещается в 80 колонок терминала.
    void everyLanguageWorks_data() {
        QTest::addColumn<QString>("code");
        for (const QString& code : availableLanguages()) QTest::newRow(qPrintable(code)) << code;
    }
    void everyLanguageWorks() {
        QFETCH(QString, code);
        install(code);
        QCOMPARE(current(), code);
        QVERIFY(!nativeName(code).isEmpty());
        const QString file = QCoreApplication::translate("birch::MainWindow", "Файл");
        if (code == QLatin1String(kSourceLanguage)) QCOMPARE(file, QStringLiteral("Файл"));
        else QVERIFY2(file != QStringLiteral("Файл"), "меню «Файл» не переведено");
        const QStringList lines = birch::cli::helpText(QStringLiteral("birch")).split('\n');
        QVERIFY(lines.size() > 10);
        for (const QString& line : lines) QVERIFY2(columns(line) <= 80, qPrintable(code + ": " + line));
        install("en");
    }

    // Стандартные кнопки и меню Qt по-русски — если перевод Qt встроен в сборку.
    void qtStandardDialogsInRussian() {
        if (!QFile::exists(":/i18n/qtbase_ru.qm")) QSKIP("в этой сборке Qt нет qtbase_ru.qm");
        install("ru");
        QCOMPARE(current(), QStringLiteral("ru"));
        QCOMPARE(QCoreApplication::translate("QPlatformTheme", "Cancel"), QStringLiteral("Отмена"));
        install("en");
        QCOMPARE(QCoreApplication::translate("QPlatformTheme", "Cancel"), QStringLiteral("Cancel"));
    }
};

QTEST_MAIN(TestLanguage)
#include "tst_language.moc"
