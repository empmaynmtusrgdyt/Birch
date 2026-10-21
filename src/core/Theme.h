#pragma once

#include <QColor>
#include <QString>
#include <QStringList>

#include "core/Diagram.h"

namespace birch {

struct NodeStyle {
    QColor fill;
    QColor stroke;
    QColor text;
};

// Визуальная тема схемы. Цвета передаются в Graphviz как атрибуты узлов и рёбер.
struct Theme {
    QString name;
    QColor background;
    QColor edge;
    QColor edgeText;
    NodeStyle process;
    NodeStyle decision;
    NodeStyle terminal;

    // Первый установленный в системе шрифт из списка используется и для
    // измерения текста, и для отрисовки, поэтому текст всегда помещается в узел.
    QStringList fontFamilies;
    double fontSize = 13.0;      // pt
    double edgeFontSize = 11.0;  // pt

    const NodeStyle& style(NodeShape shape) const;

    static Theme light();
    static Theme dark();
};

}  // namespace birch
