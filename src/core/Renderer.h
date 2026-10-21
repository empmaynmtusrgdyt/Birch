#pragma once

#include <QByteArray>
#include <QImage>
#include <QSizeF>
#include <QString>

#include <memory>

#include "core/Diagram.h"
#include "core/Theme.h"

namespace birch {

enum class LayoutDirection { TopToBottom, LeftToRight };

struct RenderOptions {
    Theme theme = Theme::light();
    LayoutDirection direction = LayoutDirection::TopToBottom;
    qreal scale = 2.0;  // пикселей на pt: 2.0 = чёткая картинка на Retina / HiDPI
    bool transparentBackground = false;
    bool rasterize = true;  // false — только SVG (для векторного превью)
};

struct RenderResult {
    QByteArray svg;  // SVG от Graphviz (прозрачный фон)
    QImage image;    // растр; для пустой схемы — null
    QSizeF size;     // логический размер схемы в pt
    QString error;

    bool ok() const { return error.isEmpty(); }
};

// Diagram -> граф cgraph -> раскладка dot -> SVG -> QImage.
//
// Graphviz хранит глобальное состояние, поэтому render() сериализуется
// общим мьютексом: вызывать можно из любого потока, но одновременно идёт
// только один рендер. Нужен живой QGuiApplication: шрифты измеряет Qt.
class Renderer {
public:
    static constexpr int kMaxImageSide = 16'384;
    static constexpr qint64 kMaxImagePixels = 120'000'000;

    Renderer();
    ~Renderer();
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    RenderResult render(const Diagram& diagram, const RenderOptions& options = {});

    static QByteArray toPng(const QImage& image);
    static QString graphvizVersion();

    // Шрифт, который реально будет использован для темы (первый установленный).
    static QString resolveFontFamily(const Theme& theme);

private:
    struct Impl;
    std::unique_ptr<Impl> d;
};

}  // namespace birch
