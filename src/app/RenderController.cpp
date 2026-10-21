#include "app/RenderController.h"

#include <QElapsedTimer>

namespace birch {

RenderController::RenderController(QObject* parent)
    : QObject(parent), worker_(new QObject), renderer_(std::make_unique<Renderer>()) {
    thread_.setObjectName(QStringLiteral("BirchRender"));
    worker_->moveToThread(&thread_);
    thread_.start();
}

RenderController::~RenderController() {
    latest_ = ~quint64{0};  // все стоящие в очереди задачи станут устаревшими
    thread_.quit();
    thread_.wait();
    delete worker_;
}

void RenderController::submit(const Diagram& diagram, const RenderOptions& options) {
    const quint64 id = ++latest_;

    QMetaObject::invokeMethod(
        worker_,
        [this, id, diagram, options] {
            if (id != latest_.load()) return;  // пока ждали, пришёл запрос новее

            QElapsedTimer timer;
            timer.start();
            RenderResult result = renderer_->render(diagram, options);
            const qint64 ms = timer.elapsed();

            if (id != latest_.load()) return;
            // Если контроллер уже удалён, Qt просто отбросит этот вызов.
            QMetaObject::invokeMethod(
                this,
                [this, id, result, ms] {
                    if (id == latest_.load()) emit rendered(result, ms);
                },
                Qt::QueuedConnection);
        },
        Qt::QueuedConnection);
}

}  // namespace birch
