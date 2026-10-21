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

struct Node {
    QString id;     // уникальный ключ (текст внутри скобок)
    QString label;  // отображаемый текст ("_" -> пробел)
    NodeShape shape = NodeShape::Process;
};

struct Edge {
    QString from;
    QString to;
    QString label;  // необязательная подпись: -> [B] "Да"
};

struct Diagram {
    QList<Node> nodes;  // в порядке первого появления
    QList<Edge> edges;

    bool isEmpty() const { return nodes.isEmpty(); }
};

}  // namespace birch
