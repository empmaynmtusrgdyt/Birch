#pragma once

#include <QMainWindow>

#include <optional>

#include "core/Parser.h"
#include "core/Renderer.h"

class QAction;
class QActionGroup;
class QLabel;
class QListWidget;
class QSplitter;
class QTimer;
class QToolButton;

namespace birch {

class CodeEditor;
class PreviewView;
class RenderController;
class SyntaxHighlighter;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    bool openFile(const QString& path);
    // Открытие по запросу системы (двойной щелчок в Finder и т.п.): спрашивает про несохранённое.
    void openFromSystem(const QString& path);

protected:
    void closeEvent(QCloseEvent* event) override;
    void changeEvent(QEvent* event) override;

private:
    enum class ThemeMode { System, Light, Dark };
    enum class ExportTheme { AsPreview, Light, Dark };

    // Построение интерфейса
    void createActions();
    void createMenus();
    void createToolBar();
    void createCentralWidget();
    void restoreSession();
    void saveSession();

    // Рендеринг
    void onTextChanged();
    void renderNow();
    void onRendered(const RenderResult& result, qint64 elapsedMs);
    void showProblems(const QList<Diagnostic>& diagnostics);
    RenderOptions previewOptions() const;
    RenderOptions exportOptions() const;
    std::optional<RenderResult> renderForExport(bool rasterize);

    // Экспорт
    void exportPng();
    void exportSvg();
    void copyImage();

    // Файлы
    void newDocument();
    void openDialog();
    bool save();
    bool saveAs();
    bool writeFile(const QString& path);
    bool maybeSave();
    void setCurrentFile(const QString& path);
    void loadExample();

    // Оформление
    void setThemeMode(ThemeMode mode);
    bool isDark() const;
    Theme previewTheme() const;
    void applyTheme();
    QIcon icon(const QString& name) const;
    void updateTitle();
    void updateZoomLabel(qreal zoom);

    void showSyntaxHelp();
    void showAbout();

    // Виджеты
    QSplitter* splitter_ = nullptr;
    CodeEditor* editor_ = nullptr;
    SyntaxHighlighter* highlighter_ = nullptr;
    QListWidget* problems_ = nullptr;
    PreviewView* preview_ = nullptr;
    QLabel* renderError_ = nullptr;
    QLabel* statusInfo_ = nullptr;
    QToolButton* zoomButton_ = nullptr;

    // Действия
    QAction* newAct_ = nullptr;
    QAction* openAct_ = nullptr;
    QAction* saveAct_ = nullptr;
    QAction* saveAsAct_ = nullptr;
    QAction* exampleAct_ = nullptr;
    QAction* quitAct_ = nullptr;
    QAction* renderAct_ = nullptr;
    QAction* autoRenderAct_ = nullptr;
    QAction* exportPngAct_ = nullptr;
    QAction* exportSvgAct_ = nullptr;
    QAction* copyImageAct_ = nullptr;
    QAction* horizontalAct_ = nullptr;
    QAction* monochromeAct_ = nullptr;
    QAction* transparentAct_ = nullptr;
    QAction* zoomInAct_ = nullptr;
    QAction* zoomOutAct_ = nullptr;
    QAction* zoomResetAct_ = nullptr;
    QAction* zoomFitAct_ = nullptr;
    QActionGroup* themeGroup_ = nullptr;
    QActionGroup* exportThemeGroup_ = nullptr;
    QAction* syntaxAct_ = nullptr;
    QAction* aboutAct_ = nullptr;

    // Состояние
    Parser parser_;
    Renderer exportRenderer_;  // синхронный рендер для экспорта (в потоке GUI)
    RenderController* render_ = nullptr;
    QTimer* debounce_ = nullptr;
    QList<Diagnostic> diagnostics_;
    qsizetype pendingNodes_ = 0;  // для строки состояния после рендера
    qsizetype pendingEdges_ = 0;
    QString filePath_;
    ThemeMode themeMode_ = ThemeMode::System;
    ExportTheme exportTheme_ = ExportTheme::AsPreview;
    bool applyingTheme_ = false;
};

}  // namespace birch
