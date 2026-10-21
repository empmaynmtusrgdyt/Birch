#include "ui/PreviewView.h"

#include <QGraphicsSvgItem>
#include <QNativeGestureEvent>
#include <QSvgRenderer>
#include <QWheelEvent>
#include <QtMath>

namespace birch {

namespace {
constexpr qreal kZoomStep = 1.2;
}

PreviewView::PreviewView(QWidget* parent) : QGraphicsView(parent) {
    setScene(&scene_);
    setFrameShape(QFrame::NoFrame);
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setAlignment(Qt::AlignCenter);
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents, false);
}

PreviewView::~PreviewView() {
    clear();
}

void PreviewView::setSvg(const QByteArray& svg) {
    auto* renderer = new QSvgRenderer(svg, this);
    if (!renderer->isValid()) {
        delete renderer;
        return;
    }
    clear();
    svg_ = renderer;
    item_ = new QGraphicsSvgItem;
    item_->setSharedRenderer(svg_);
    item_->setCacheMode(QGraphicsItem::NoCache);  // перерисовываем вектор — чётко при любом зуме

    // 1 единица сцены = 1 pt схемы, как у PNG в логических пикселях.
    const QRectF bounds = item_->boundingRect();
    const QSizeF viewBox = svg_->viewBoxF().size();
    if (bounds.width() > 0 && !viewBox.isEmpty()) item_->setScale(viewBox.width() / bounds.width());

    scene_.addItem(item_);
    scene_.setSceneRect(item_->sceneBoundingRect());

    if (fitMode_) applyFit();
    viewport()->update();
}

void PreviewView::clear() {
    if (item_) {
        scene_.removeItem(item_);
        delete item_;
        item_ = nullptr;
    }
    delete svg_;
    svg_ = nullptr;
    scene_.setSceneRect(QRectF());
    viewport()->update();
}

void PreviewView::setCanvasColor(const QColor& color) {
    setBackgroundBrush(color);
    viewport()->update();
}

void PreviewView::setPlaceholderText(const QString& text) {
    placeholder_ = text;
    viewport()->update();
}

// --- масштаб ------------------------------------------------------------------

void PreviewView::setZoom(qreal zoom, bool underMouse) {
    zoom = qBound(kMinZoom, zoom, kMaxZoom);
    setTransformationAnchor(underMouse ? QGraphicsView::AnchorUnderMouse : QGraphicsView::AnchorViewCenter);
    setTransform(QTransform::fromScale(zoom, zoom));
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    if (!qFuzzyCompare(zoom, zoom_)) {
        zoom_ = zoom;
        emit zoomChanged(zoom_);
    }
}

void PreviewView::zoomIn() {
    fitMode_ = false;
    setZoom(zoom_ * kZoomStep);
}

void PreviewView::zoomOut() {
    fitMode_ = false;
    setZoom(zoom_ / kZoomStep);
}

void PreviewView::resetZoom() {
    fitMode_ = false;
    setZoom(1.0);
}

void PreviewView::fitToView() {
    fitMode_ = true;
    applyFit();
}

void PreviewView::applyFit() {
    if (!item_) return;
    const QRectF content = item_->sceneBoundingRect();
    const QSizeF avail = viewport()->size();
    if (content.isEmpty() || avail.isEmpty()) return;
    const qreal z = qMin(avail.width() / content.width(), avail.height() / content.height());
    setZoom(qMin(z, kMaxFitZoom));
    centerOn(content.center());
}

void PreviewView::wheelEvent(QWheelEvent* event) {
    // ⌘ на macOS (Qt отображает его в ControlModifier), Ctrl на Windows и Linux
    if (event->modifiers() & Qt::ControlModifier) {
        const qreal steps = event->angleDelta().y() / 120.0;
        if (!qFuzzyIsNull(steps)) {
            fitMode_ = false;
            setZoom(zoom_ * qPow(kZoomStep, steps), true);
        }
        event->accept();
        return;
    }
    QGraphicsView::wheelEvent(event);
}

bool PreviewView::viewportEvent(QEvent* event) {
    if (event->type() == QEvent::NativeGesture) {
        auto* g = static_cast<QNativeGestureEvent*>(event);
        if (g->gestureType() == Qt::ZoomNativeGesture) {
            fitMode_ = false;
            setZoom(zoom_ * (1.0 + g->value()), true);
            return true;
        }
        if (g->gestureType() == Qt::SmartZoomNativeGesture) {  // двойное касание двумя пальцами
            fitToView();
            return true;
        }
    }
    return QGraphicsView::viewportEvent(event);
}

void PreviewView::resizeEvent(QResizeEvent* event) {
    QGraphicsView::resizeEvent(event);
    if (fitMode_) applyFit();
}

void PreviewView::mouseDoubleClickEvent(QMouseEvent* event) {
    fitToView();
    event->accept();
}

void PreviewView::drawForeground(QPainter* painter, const QRectF& rect) {
    QGraphicsView::drawForeground(painter, rect);
    if (item_ || placeholder_.isEmpty()) return;

    painter->save();
    painter->resetTransform();
    painter->setPen(palette().color(QPalette::PlaceholderText));
    QFont f = font();
    f.setPointSizeF(f.pointSizeF() * 1.1);
    painter->setFont(f);
    painter->drawText(viewport()->rect().adjusted(24, 24, -24, -24), Qt::AlignCenter | Qt::TextWordWrap, placeholder_);
    painter->restore();
}

}  // namespace birch
