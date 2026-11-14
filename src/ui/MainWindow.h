/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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
class FindBar;
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
    // false — не сохранять сессию при закрытии (смоук-тесты, --screenshot).
    void setSessionSaving(bool enabled) { sessionSaving_ = enabled; }
    // Тема на этот запуск: "system", "light" или "dark" (смоук-тесты, --theme).
    bool setThemeByName(const QString& name);
    // Чёрно-белая схема на этот запуск (снимки для README, --monochrome).
    void setMonochrome(bool on);
    // Открыть панель замены с этим текстом в поле поиска (снимки панели, --find).
    void openReplace(const QString& pattern);
    bool isFindBarOpen() const;
    // Ширины панели поиска хватает на все её поля и кнопки (смоук-тесты).
    bool findBarFits() const;
    // Область текста редактора в координатах окна (смоук-тесты проверяют её цвет).
    QRect editorViewportRect() const;

signals:
    // Схема отрисована (ok) или рендер завершился ошибкой (!ok).
    void previewRendered(bool ok, const QString& error);

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
    QString mermaidForExport();
    void exportMermaid();
    void copyMermaid();

    // Файлы
    void newDocument();
    void openDialog();
    bool save();
    bool saveAs();
    bool writeFile(const QString& path);
    bool maybeSave();
    void setCurrentFile(const QString& path);
    void loadExample(const QString& resource);

    // Оформление
    void setThemeMode(ThemeMode mode);
    bool isDark() const;
    Theme previewTheme() const;
    void applyTheme();
    void applyPalette();
    QIcon icon(const QString& name) const;
    void updateTitle();
    void updateZoomLabel(qreal zoom);
    void updateEditorFontActions();

    void showSyntaxHelp();
    void showAbout();
    void chooseLanguage(const QString& code);  // пустой код — «как в системе»

    // Виджеты
    QSplitter* splitter_ = nullptr;
    CodeEditor* editor_ = nullptr;
    SyntaxHighlighter* highlighter_ = nullptr;
    FindBar* findBar_ = nullptr;
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
    QAction* framesExampleAct_ = nullptr;
    QAction* quitAct_ = nullptr;
    QAction* undoAct_ = nullptr;
    QAction* redoAct_ = nullptr;
    QAction* cutAct_ = nullptr;
    QAction* copyAct_ = nullptr;
    QAction* pasteAct_ = nullptr;
    QAction* selectAllAct_ = nullptr;
    QAction* findAct_ = nullptr;
    QAction* replaceAct_ = nullptr;
    QAction* findNextAct_ = nullptr;
    QAction* findPreviousAct_ = nullptr;
    QAction* renderAct_ = nullptr;
    QAction* autoRenderAct_ = nullptr;
    QAction* exportPngAct_ = nullptr;
    QAction* exportSvgAct_ = nullptr;
    QAction* copyImageAct_ = nullptr;
    QAction* exportMermaidAct_ = nullptr;
    QAction* copyMermaidAct_ = nullptr;
    QAction* horizontalAct_ = nullptr;
    QAction* monochromeAct_ = nullptr;
    QAction* transparentAct_ = nullptr;
    QAction* zoomInAct_ = nullptr;
    QAction* zoomOutAct_ = nullptr;
    QAction* zoomResetAct_ = nullptr;
    QAction* zoomFitAct_ = nullptr;
    QAction* editorFontBiggerAct_ = nullptr;
    QAction* editorFontSmallerAct_ = nullptr;
    QAction* editorFontResetAct_ = nullptr;
    QActionGroup* themeGroup_ = nullptr;
    QActionGroup* exportThemeGroup_ = nullptr;
    QActionGroup* languageGroup_ = nullptr;  // «Как в системе» и все языки перевода
    QAction* syntaxAct_ = nullptr;
    QAction* aboutAct_ = nullptr;
    QAction* installCommandAct_ = nullptr;  // только macOS и AppImage

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
    bool sessionSaving_ = true;
};

}  // namespace birch
