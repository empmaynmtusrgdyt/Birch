/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "core/FileDrop.h"

#include <QFileInfo>
#include <QMimeData>
#include <QUrl>

namespace birch {

QString droppedDiagramFile(const QMimeData* mime) {
    if (!mime || !mime->hasUrls()) return {};
    for (const QUrl& url : mime->urls()) {
        if (!url.isLocalFile()) continue;
        const QFileInfo info(url.toLocalFile());
        const QString suffix = info.suffix().toLower();
        if (info.isFile() && (suffix == QLatin1String("birch") || suffix == QLatin1String("txt")))
            return info.absoluteFilePath();
    }
    return {};
}

}  // namespace birch
