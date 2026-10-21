#pragma once

#include <QGraphicsView>

class QGraphicsSvgItem;
class QSvgRenderer;

namespace birch {

// Интерактивный холст: показывает SVG как вектор (чёткий при любом масштабе).
//  - ⌘ + колесо или щипок на трекпаде — масштаб относительно курсора;
//  - перетаскивание мышью — прокрутка; двойной щелчок — «по размеру окна».
// Пока пользователь сам не менял масштаб, схема вписывается в окно автоматически;
// после ручного зума масштаб сохраняется между перерисовками.
class PreviewView : public QGraphicsView {
    Q_OBJECT
public:
    static constexpr qreal kMinZoom = 0.1;
    static constexpr qreal kMaxZoom = 8.0;
    static constexpr qreal kMaxFitZoom = 1.5;  // мелкие схемы не раздуваем

    explicit PreviewView(QWidget* parent = nullptr);
    ~PreviewView() override;

    void setSvg(const QByteArray& svg);
    void clear();
    bool isEmpty() const { return item_ == nullptr; }

    void setCanvasColor(const QColor& color);
    void setPlaceholderText(const QString& text);
    qreal zoom() const { return zoom_; }

public slots:
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void fitToView();

signals:
    void zoomChanged(qreal zoom);

protected:
    void wheelEvent(QWheelEvent* event) override;
    bool viewportEvent(QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void drawForeground(QPainter* painter, const QRectF& rect) override;

private:
    void setZoom(qreal zoom, bool underMouse = false);
    void applyFit();

    QGraphicsScene scene_;
    QSvgRenderer* svg_ = nullptr;
    QGraphicsSvgItem* item_ = nullptr;
    QString placeholder_;
    qreal zoom_ = 1.0;
    bool fitMode_ = true;
};

}  // namespace birch
