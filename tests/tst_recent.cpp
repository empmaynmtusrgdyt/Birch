/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// Тесты списка недавних файлов.

#include <QDir>
#include <QtTest>

#include "core/RecentFiles.h"

using namespace birch;

class TestRecent : public QObject {
    Q_OBJECT

    static QString abs(const QString& p) { return QDir::cleanPath(QDir::current().absoluteFilePath(p)); }

private slots:
    void newestFirst() {
        QStringList list = recent::add({}, "a.birch");
        list = recent::add(list, "b.birch");
        QCOMPARE(list, QStringList({abs("b.birch"), abs("a.birch")}));
    }

    void noDuplicatesEvenWithDifferentSpelling() {
        QStringList list = recent::add({}, "a.birch");
        list = recent::add(list, "b.birch");
        list = recent::add(list, "./x/../a.birch");
        QCOMPARE(list, QStringList({abs("a.birch"), abs("b.birch")}));
    }

    void limitedToMax() {
        QStringList list;
        for (int i = 0; i < 15; ++i) list = recent::add(list, QStringLiteral("f%1.birch").arg(i));
        QCOMPARE(list.size(), recent::kMaxFiles);
        QCOMPARE(list.first(), abs("f14.birch"));
        QCOMPARE(list.last(), abs("f5.birch"));
    }

    void removeFile() {
        QStringList list = recent::add(recent::add({}, "a.birch"), "b.birch");
        QCOMPARE(recent::remove(list, "a.birch"), QStringList({abs("b.birch")}));
        QCOMPARE(recent::add(list, ""), list);
    }

    void labelsAddFolderForSameNames() {
        const QStringList list{"/home/me/work/схема.birch", "/home/me/docs/схема.birch", "/home/me/x.birch"};
        QCOMPARE(recent::labels(list),
                 QStringList({"схема.birch — work", "схема.birch — docs", "x.birch"}));
    }
};

QTEST_MAIN(TestRecent)
#include "tst_recent.moc"
