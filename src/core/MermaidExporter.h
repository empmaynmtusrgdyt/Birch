/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <QString>

#include "core/Diagram.h"

namespace birch {

// Переводит схему в текст Mermaid (https://mermaid.js.org) — его понимают
// GitHub, GitLab, Notion, Obsidian и многие вики: схема вставляется текстом
// и рисуется прямо на странице.
//
// Блоки становятся n1, n2…, рамки — subgraph g1, g2…: текст может быть любым,
// поэтому в идентификаторы он не годится. Цвета не переносятся — Mermaid
// раскрашивает схему по теме сайта.
struct MermaidOptions {
    bool leftToRight = false;  // flowchart LR вместо TD
};

QString toMermaid(const Diagram& diagram, const MermaidOptions& options = {});

// То же, обёрнутое в блок ```mermaid — для вставки в Markdown.
QString toMermaidMarkdown(const Diagram& diagram, const MermaidOptions& options = {});

}  // namespace birch
