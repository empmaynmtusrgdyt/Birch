/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// Тесты примеров схем: пример на языке интерфейса и узнавание нетронутого
// примера, чтобы после смены языка показать его на новом языке.

#include <QDir>
#include <QFile>
#include <QtTest>

#include "core/Examples.h"

using namespace birch;

namespace {

const QString kDir = QStringLiteral(BIRCH_EXAMPLES_DIR);

QString read(const QString& file) {
    QFile f(kDir + u'/' + file);
    return f.open(QIODevice::ReadOnly) ? QString::fromUtf8(f.readAll()) : QString();
}

}  // namespace

class TestExamples : public QObject {
    Q_OBJECT

private slots:
    void pathByLanguage_data() {
        QTest::addColumn<QString>("language");
        QTest::addColumn<QString>("file");
        QTest::newRow("исходный") << "ru" << "welcome.birch";
        QTest::newRow("английский") << "en" << "welcome.en.birch";
        QTest::newRow("немецкий") << "de" << "welcome.de.birch";
        QTest::newRow("с регионом") << "pt_BR" << "welcome.pt_BR.birch";
        QTest::newRow("без своего примера — английский") << "it" << "welcome.en.birch";
    }
    void pathByLanguage() {
        QFETCH(QString, language);
        QFETCH(QString, file);
        QCOMPARE(examples::path(kDir, "welcome", language), kDir + u'/' + file);
    }

    // Каждый пример на любом языке узнаётся, в том числе с переводами строк Windows.
    void recognizesEveryExample_data() {
        QTest::addColumn<QString>("file");
        for (const QString& file : QDir(kDir).entryList({"*.birch"}, QDir::Files)) QTest::newRow(qPrintable(file)) << file;
    }
    void recognizesEveryExample() {
        QFETCH(QString, file);
        const QString text = read(file).remove(u'\r');  // git на Windows может выложить файл с \r\n
        const QString name = file.section(u'.', 0, 0);
        QCOMPARE(examples::untouchedName(kDir, text), name);
        QCOMPARE(examples::untouchedName(kDir, QString(text).replace("\n", "\r\n")), name);
        QCOMPARE(examples::untouchedName(kDir, text + "\n\n"), name);
    }

    void ownTextIsNotAnExample() {
        QCOMPARE(examples::untouchedName(kDir, ""), QString());
        QCOMPARE(examples::untouchedName(kDir, "[A] -> [B]"), QString());
        // Пример с правкой — уже текст пользователя
        QCOMPARE(examples::untouchedName(kDir, read("welcome.birch") + "[Ещё шаг]"), QString());
    }
};

QTEST_MAIN(TestExamples)
#include "tst_examples.moc"
