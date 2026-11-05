#pragma once

#include <QString>

namespace birch::shell {

// Команда birch в терминале — символическая ссылка на программу: на macOS
// /usr/local/bin/birch → Birch.app/Contents/MacOS/Birch, в AppImage
// ~/.local/bin/birch → файл AppImage. Пакеты .deb, Arch и установщик Windows
// ставят команду сами, там это не нужно.

enum class LinkState {
    Missing,  // по этому пути ничего нет
    Ours,     // ссылка на эту программу
    Other,    // другой файл или ссылка в другое место (в том числе битая)
};

LinkState linkState(const QString& link, const QString& target);

// Создаёт папку и ссылку link → target, заменяя то, что лежало по этому пути.
// Если нет прав — false и текст ошибки.
bool createLink(const QString& link, const QString& target, QString* error);

// Есть ли папка в списке PATH (элементы через «:»).
bool directoryInPath(const QString& dir, const QString& pathVariable);

}  // namespace birch::shell
