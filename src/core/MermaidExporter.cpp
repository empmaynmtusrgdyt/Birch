#include "core/MermaidExporter.h"

#include <QHash>

namespace birch {

namespace {

// Текст в кавычках Mermaid. Особые символы — через коды Mermaid «#…;»:
// кавычка закрыла бы строку, «<» и «>» — HTML-разметка, «&» и «#» начинают коды.
QString quoted(const QString& text) {
    QString out;
    out.reserve(text.size() + 2);
    out += u'"';
    for (const QChar c : text) {
        switch (c.unicode()) {
        case u'"': out += QStringLiteral("#quot;"); break;
        case u'#': out += QStringLiteral("#35;"); break;
        case u'&': out += QStringLiteral("#amp;"); break;
        case u'<': out += QStringLiteral("#lt;"); break;
        case u'>': out += QStringLiteral("#gt;"); break;
        case u'\n': out += QStringLiteral("<br/>"); break;
        default: out += c;
        }
    }
    out += u'"';
    return out;
}

QString nodeDeclaration(const QString& id, const Node& node) {
    switch (node.shape) {
    case NodeShape::Decision: return QStringLiteral("%1{%2}").arg(id, quoted(node.label));
    case NodeShape::Terminal: return QStringLiteral("%1([%2])").arg(id, quoted(node.label));  // «таблетка»
    case NodeShape::Process: break;
    }
    return QStringLiteral("%1(%2)").arg(id, quoted(node.label));  // скруглённый прямоугольник
}

}  // namespace

QString toMermaid(const Diagram& diagram, const MermaidOptions& options) {
    QString out = options.leftToRight ? QStringLiteral("flowchart LR\n") : QStringLiteral("flowchart TD\n");
    const QString indent = QStringLiteral("    ");

    QHash<QString, QString> ids;  // Node::id -> n1, n2…
    for (int i = 0; i < diagram.nodes.size(); ++i)
        ids.insert(diagram.nodes[i].id, QStringLiteral("n%1").arg(i + 1));

    // Mermaid кладёт блок в subgraph, где он упомянут впервые. Поэтому сначала
    // общие блоки, потом рамки со своими блоками, и только в конце — стрелки.
    for (const Node& node : diagram.nodes) {
        if (node.group < 0) out += indent + nodeDeclaration(ids.value(node.id), node) + u'\n';
    }
    for (int g = 0; g < diagram.groups.size(); ++g) {
        QString body;
        for (const Node& node : diagram.nodes) {
            if (node.group == g) body += indent + indent + nodeDeclaration(ids.value(node.id), node) + u'\n';
        }
        if (body.isEmpty()) continue;  // пустую рамку не рисуем, как и в Birch
        out += indent + QStringLiteral("subgraph g%1[%2]\n").arg(g + 1).arg(quoted(diagram.groups[g].name));
        out += body;
        out += indent + QStringLiteral("end\n");
    }
    for (const Edge& edge : diagram.edges) {
        const QString from = ids.value(edge.from);
        const QString to = ids.value(edge.to);
        if (from.isEmpty() || to.isEmpty()) continue;
        out += indent + from;
        out += edge.label.isEmpty() ? QStringLiteral(" --> ") : QStringLiteral(" -->|%1| ").arg(quoted(edge.label));
        out += to + u'\n';
    }
    return out;
}

QString toMermaidMarkdown(const Diagram& diagram, const MermaidOptions& options) {
    return QStringLiteral("```mermaid\n") + toMermaid(diagram, options) + QStringLiteral("```\n");
}

}  // namespace birch
