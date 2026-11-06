#!/usr/bin/env bash
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Проверяет, что в начале каждого исходного файла Birch есть уведомление
# о лицензии MPL 2.0 (Exhibit A из текста лицензии).
set -uo pipefail
cd "$(dirname "$0")/../.."

missing=0
while IFS= read -r f; do
    if ! head -6 "$f" | grep -q 'https://mozilla.org/MPL/2.0/'; then
        echo "::error file=$f::нет уведомления о лицензии MPL 2.0 в начале файла"
        missing=1
    fi
done < <(git ls-files '*.h' '*.cpp' '*.cmake' '*.sh' '*.ps1' '*CMakeLists.txt' \
                       '.github/workflows/*.yml' 'packaging/arch/PKGBUILD')

[ "$missing" -eq 0 ] && echo "Уведомление о лицензии есть во всех исходных файлах"
exit "$missing"
