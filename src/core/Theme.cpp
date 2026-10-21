#include "core/Theme.h"

namespace birch {

namespace {

QStringList defaultFonts() {
    return {
        QStringLiteral("Inter"),           // если установлен пользователем
        QStringLiteral("SF Pro Text"),     // если установлены SF-шрифты Apple
        QStringLiteral("Helvetica Neue"),  // есть в любой macOS
        QStringLiteral("Helvetica"),
    };
}

}  // namespace

const NodeStyle& Theme::style(NodeShape shape) const {
    switch (shape) {
    case NodeShape::Decision: return decision;
    case NodeShape::Terminal: return terminal;
    case NodeShape::Process: break;
    }
    return process;
}

Theme Theme::light() {
    Theme t;
    t.name = QStringLiteral("light");
    t.background = QColor(0xFAFAFB);
    t.edge = QColor(0x64748B);
    t.edgeText = QColor(0x475569);
    t.process = {QColor(0xE8F0FE), QColor(0x7BA7F2), QColor(0x1E293B)};   // голубой
    t.decision = {QColor(0xFEF3C7), QColor(0xF0B429), QColor(0x3D2C00)};  // янтарный
    t.terminal = {QColor(0xD8F5E6), QColor(0x34B37A), QColor(0x0B3B2A)};  // мятный
    t.fontFamilies = defaultFonts();
    return t;
}

Theme Theme::monochrome(bool dark) {
    const QColor ink = dark ? QColor(Qt::white) : QColor(Qt::black);
    const QColor paper = dark ? QColor(Qt::black) : QColor(Qt::white);
    const NodeStyle node{paper, ink, ink};

    Theme t;
    t.name = dark ? QStringLiteral("mono-dark") : QStringLiteral("mono-light");
    t.background = paper;
    t.edge = ink;
    t.edgeText = ink;
    t.process = node;
    t.decision = node;
    t.terminal = node;
    t.fontFamilies = defaultFonts();
    return t;
}

Theme Theme::select(bool dark, bool mono) {
    if (mono) return monochrome(dark);
    return dark ? Theme::dark() : Theme::light();
}

Theme Theme::dark() {
    Theme t;
    t.name = QStringLiteral("dark");
    t.background = QColor(0x1C1D21);
    t.edge = QColor(0x8B97AB);
    t.edgeText = QColor(0xB8C2D3);
    t.process = {QColor(0x1F3355), QColor(0x5B8DEF), QColor(0xE6EEFF)};
    t.decision = {QColor(0x46350F), QColor(0xE0A82E), QColor(0xFFF3D6)};
    t.terminal = {QColor(0x123C2C), QColor(0x3FBF88), QColor(0xD6F7E8)};
    t.fontFamilies = defaultFonts();
    return t;
}

}  // namespace birch
