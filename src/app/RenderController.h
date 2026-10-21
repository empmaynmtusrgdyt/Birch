#pragma once

#include <QObject>
#include <QThread>

#include <atomic>
#include <memory>

#include "core/Renderer.h"

namespace birch {

// Рендерит в фоновом потоке, чтобы ввод текста не подвисал.
// Побеждает последний запрос: устаревшие пропускаются ещё до начала работы,
// а их результаты, если всё же успели посчитаться, отбрасываются.
class RenderController : public QObject {
    Q_OBJECT
public:
    explicit RenderController(QObject* parent = nullptr);
    ~RenderController() override;

    void submit(const Diagram& diagram, const RenderOptions& options);
    void cancel() { ++latest_; }  // результат уже запущенного рендера будет отброшен

signals:
    // Всегда в потоке GUI. elapsedMs — время работы Graphviz + растеризации.
    void rendered(const birch::RenderResult& result, qint64 elapsedMs);

private:
    QThread thread_;
    QObject* worker_ = nullptr;           // контекст для вызовов в фоновом потоке
    std::unique_ptr<Renderer> renderer_;  // используется только из thread_
    std::atomic<quint64> latest_{0};
};

}  // namespace birch
