/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "core/ShellCommand.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

namespace birch::shell {

LinkState linkState(const QString& link, const QString& target) {
    const QFileInfo info(link);
    if (!info.exists() && !info.isSymLink()) return LinkState::Missing;
    // canonicalFilePath раскрывает ссылки; у битой ссылки он пустой.
    const QString resolved = info.canonicalFilePath();
    if (!resolved.isEmpty() && resolved == QFileInfo(target).canonicalFilePath()) return LinkState::Ours;
    return LinkState::Other;
}

bool createLink(const QString& link, const QString& target, QString* error) {
    const QFileInfo info(link);
    const QString dir = info.absolutePath();
    if (!QDir().mkpath(dir)) {
        if (error) *error = QStringLiteral("не удалось создать папку «%1»").arg(dir);
        return false;
    }
    if (info.exists() || info.isSymLink()) {
        QFile old(link);
        if (!old.remove()) {
            if (error) *error = QStringLiteral("не удалось заменить «%1»: %2").arg(link, old.errorString());
            return false;
        }
    }
    QFile file(target);
    if (!file.link(link)) {
        if (error) *error = QStringLiteral("не удалось создать «%1»: %2").arg(link, file.errorString());
        return false;
    }
    return true;
}

bool directoryInPath(const QString& dir, const QString& pathVariable) {
    const QString wanted = QDir::cleanPath(dir);
    for (const QString& entry : pathVariable.split(u':', Qt::SkipEmptyParts)) {
        if (QDir::cleanPath(entry) == wanted) return true;
    }
    return false;
}

}  // namespace birch::shell
