/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "core/RecentFiles.h"

#include <QDir>
#include <QFileInfo>

namespace birch::recent {

namespace {

// Один и тот же файл, даже если путь записан по-разному («a/../b», «./b»).
bool sameFile(const QString& a, const QString& b) {
    return QDir::cleanPath(QFileInfo(a).absoluteFilePath()) == QDir::cleanPath(QFileInfo(b).absoluteFilePath());
}

}  // namespace

QStringList add(QStringList list, const QString& path) {
    if (path.isEmpty()) return list;
    list = remove(std::move(list), path);
    list.prepend(QDir::cleanPath(QFileInfo(path).absoluteFilePath()));
    while (list.size() > kMaxFiles) list.removeLast();
    return list;
}

QStringList remove(QStringList list, const QString& path) {
    list.removeIf([&path](const QString& item) { return sameFile(item, path); });
    return list;
}

QStringList labels(const QStringList& list) {
    QStringList result;
    for (const QString& path : list) {
        const QFileInfo info(path);
        const QString name = info.fileName();
        const auto sameName = std::count_if(list.begin(), list.end(),
                                            [&name](const QString& p) { return QFileInfo(p).fileName() == name; });
        result << (sameName > 1 ? QStringLiteral("%1 — %2").arg(name, info.dir().dirName()) : name);
    }
    return result;
}

}  // namespace birch::recent
