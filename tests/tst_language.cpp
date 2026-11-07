/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// Тесты выбора языка интерфейса и подключения переводов.

#include <QFile>
#include <QtTest>

#include "core/Language.h"

using namespace birch::i18n;

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

    void withoutEnglishFallsBackToSource() {
        QCOMPARE(birch::i18n::choose("", {"de"}, {"ru"}), QStringLiteral("ru"));
    }

    void nativeNames() {
        QCOMPARE(nativeName("ru"), QStringLiteral("Русский"));
        QCOMPARE(nativeName("en"), QStringLiteral("English"));
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
