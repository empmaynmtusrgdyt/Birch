/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <QList>
#include <QString>

namespace birch {

// Модель блок-схемы, которую строит Parser и потребляет Renderer.
// Не зависит от Graphviz: парсер можно тестировать отдельно.

enum class NodeShape {
    Process,   // [Текст]   скруглённый прямоугольник
    Decision,  // {Текст}   ромб
    Terminal,  // (Текст)   «таблетка» (начало/конец)
};

// Рамка: несколько блоков, обведённых рамкой с заголовком (=Название= в тексте).
struct Group {
    QString name;
};

struct Node {
    QString id;     // уникальный ключ: у общего блока — его текст, у блока рамки — рамка и текст
    QString label;  // отображаемый текст
    NodeShape shape = NodeShape::Process;
    int group = -1;  // индекс в Diagram::groups; -1 — общий блок, вне рамок
};

struct Edge {
    QString from;
    QString to;
    QString label;  // необязательная подпись: -> [B] "Да"
};

struct Diagram {
    QList<Group> groups;  // в порядке первого заголовка; рамка без блоков не рисуется
    QList<Node> nodes;    // в порядке первого появления
    QList<Edge> edges;

    bool isEmpty() const { return nodes.isEmpty(); }
};

}  // namespace birch
