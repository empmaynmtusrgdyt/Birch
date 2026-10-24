#include "ui/MainWindow.h"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QCloseEvent>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QListWidget>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QSaveFile>
#include <QSettings>
#include <QSplitter>
#include <QStandardPaths>
#include <QStatusBar>
#include <QStyleHints>
#include <QSvgRenderer>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

#include "app/RenderController.h"
#include "ui/CodeEditor.h"
#include "ui/PreviewView.h"
#include "ui/SyntaxHighlighter.h"

namespace birch {

namespace {

constexpr int kDebounceMs = 300;
constexpr qint64 kMaxFileBytes = 4 * 1024 * 1024;
constexpr qreal kExportScale = 2.0;

namespace keys {
const QString geometry = QStringLiteral("ui/geometry");
const QString windowState = QStringLiteral("ui/state");
const QString splitter = QStringLiteral("ui/splitter");
const QString theme = QStringLiteral("ui/theme");
const QString exportTheme = QStringLiteral("export/theme");
const QString transparent = QStringLiteral("export/transparent");
const QString lastDir = QStringLiteral("export/lastDir");
const QString horizontal = QStringLiteral("render/horizontal");
const QString monochrome = QStringLiteral("render/monochrome");
const QString autoRender = QStringLiteral("render/auto");
const QString sessionText = QStringLiteral("session/text");
const QString sessionFile = QStringLiteral("session/file");
}  // namespace keys

QString readResourceText(const QString& path) {
    QFile f(path);
    return f.open(QIODevice::ReadOnly) ? QString::fromUtf8(f.readAll()) : QString();
}

QIcon dotIcon(const QColor& color) {
    QPixmap pm(32, 32);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(color);
    p.drawEllipse(QRectF(9, 9, 14, 14));
    return QIcon(pm);
}

// Стили, которые сами перекрашивают виджеты при смене светлой/тёмной темы
// (QStyleHints::setColorScheme). Для остальных (Fusion, Breeze…) палитру задаём сами.
bool styleFollowsColorScheme() {
    const QString name = QApplication::style()->name().toLower();
    return name == QLatin1String("macos") || name == QLatin1String("windows11");
}

QPalette darkPalette() {
    const QColor window(0x2B2D31), base(0x1E1F22), text(0xE6E7EA), disabled(0x7A7F87);
    QPalette p;
    p.setColor(QPalette::Window, window);
    p.setColor(QPalette::WindowText, text);
    p.setColor(QPalette::Base, base);
    p.setColor(QPalette::AlternateBase, window);
    p.setColor(QPalette::ToolTipBase, window);
    p.setColor(QPalette::ToolTipText, text);
    p.setColor(QPalette::PlaceholderText, QColor(0x8A8F98));
    p.setColor(QPalette::Text, text);
    p.setColor(QPalette::Button, window);
    p.setColor(QPalette::ButtonText, text);
    p.setColor(QPalette::BrightText, QColor(0xFF6B6B));
    p.setColor(QPalette::Light, QColor(0x3A3D42));
    p.setColor(QPalette::Midlight, QColor(0x33363B));
    p.setColor(QPalette::Mid, QColor(0x26282C));
    p.setColor(QPalette::Dark, QColor(0x18191C));
    p.setColor(QPalette::Shadow, QColor(0x101113));
    p.setColor(QPalette::Highlight, QColor(0x3D6FD9));
    p.setColor(QPalette::HighlightedText, Qt::white);
    p.setColor(QPalette::Link, QColor(0x6CA0FF));
    for (const auto role : {QPalette::WindowText, QPalette::Text, QPalette::ButtonText})
        p.setColor(QPalette::Disabled, role, disabled);
    return p;
}

QString defaultDirectory() {
    const QString pics = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    return pics.isEmpty() ? QDir::homePath() : pics;
}

}  // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), render_(new RenderController(this)), debounce_(new QTimer(this)) {
    setWindowIcon(QIcon(QStringLiteral(":/icons/app.svg")));
    setMinimumSize(760, 480);
    resize(1280, 800);

    debounce_->setSingleShot(true);
    debounce_->setInterval(kDebounceMs);
    connect(debounce_, &QTimer::timeout, this, &MainWindow::renderNow);
    connect(render_, &RenderController::rendered, this, &MainWindow::onRendered);

    createActions();
    createCentralWidget();
    createMenus();
    createToolBar();

    statusInfo_ = new QLabel(this);
    statusBar()->addWidget(statusInfo_, 1);
    statusBar()->setSizeGripEnabled(false);

    connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged, this, [this] { applyTheme(); });

    restoreSession();  // включает тему -> applyTheme()
    renderNow();
}

MainWindow::~MainWindow() = default;

// =============================================================================
// Действия, меню, панель инструментов
// =============================================================================

void MainWindow::createActions() {
    auto make = [this](const QString& text, auto slot, const QList<QKeySequence>& keys = {}) {
        auto* a = new QAction(text, this);
        a->setShortcuts(keys);
        connect(a, &QAction::triggered, this, slot);
        return a;
    };

    newAct_ = make(tr("Новый"), &MainWindow::newDocument, {QKeySequence::New});
    openAct_ = make(tr("Открыть…"), &MainWindow::openDialog, {QKeySequence::Open});
    saveAct_ = make(tr("Сохранить"), &MainWindow::save, {QKeySequence::Save});
    saveAsAct_ = make(tr("Сохранить как…"), &MainWindow::saveAs, {QKeySequence::SaveAs});
    exampleAct_ = make(tr("Открыть пример"), &MainWindow::loadExample);
    quitAct_ = make(tr("Выход"), &QWidget::close, {QKeySequence::Quit});
    quitAct_->setMenuRole(QAction::QuitRole);

    renderAct_ = make(tr("Рендерить"), &MainWindow::renderNow,
                      {QKeySequence(Qt::CTRL | Qt::Key_Return), QKeySequence(Qt::CTRL | Qt::Key_R)});
    renderAct_->setToolTip(tr("Перерисовать схему (%1)").arg(renderAct_->shortcut().toString(QKeySequence::NativeText)));

    autoRenderAct_ = new QAction(tr("Автоматический рендеринг"), this);
    autoRenderAct_->setCheckable(true);
    autoRenderAct_->setChecked(true);
    connect(autoRenderAct_, &QAction::toggled, this, [this](bool on) {
        if (on) renderNow();
    });

    exportPngAct_ = make(tr("Скачать PNG…"), &MainWindow::exportPng, {QKeySequence(Qt::CTRL | Qt::Key_E)});
    exportPngAct_->setToolTip(tr("Сохранить картинку в файл .png (%1)")
                                  .arg(exportPngAct_->shortcut().toString(QKeySequence::NativeText)));
    exportSvgAct_ = make(tr("Экспорт SVG…"), &MainWindow::exportSvg);
    copyImageAct_ = make(tr("Копировать в буфер"), &MainWindow::copyImage,
                         {QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_C)});
    copyImageAct_->setToolTip(tr("Скопировать PNG — можно сразу вставить в Telegram, Slack, Word (%1)")
                                  .arg(copyImageAct_->shortcut().toString(QKeySequence::NativeText)));

    horizontalAct_ = new QAction(tr("Горизонтальная схема"), this);
    horizontalAct_->setCheckable(true);
    horizontalAct_->setToolTip(tr("Располагать блоки слева направо"));
    horizontalAct_->setPriority(QAction::LowPriority);
    connect(horizontalAct_, &QAction::toggled, this, [this] {
        preview_->fitToView();
        renderNow();
    });

    monochromeAct_ = new QAction(tr("Чёрно-белая схема"), this);
    monochromeAct_->setIconText(tr("Ч/Б"));  // короткая подпись на панели инструментов
    monochromeAct_->setCheckable(true);
    monochromeAct_->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_B));
    monochromeAct_->setToolTip(tr("Чёрно-белая схема — для печати и документов (%1)")
                                   .arg(monochromeAct_->shortcut().toString(QKeySequence::NativeText)));
    connect(monochromeAct_, &QAction::toggled, this, [this] {
        preview_->setCanvasColor(previewTheme().background);
        renderNow();
    });

    transparentAct_ = new QAction(tr("Прозрачный фон"), this);
    transparentAct_->setCheckable(true);

    zoomInAct_ = new QAction(tr("Увеличить"), this);
    zoomInAct_->setShortcuts(QKeySequence::ZoomIn);
    zoomOutAct_ = new QAction(tr("Уменьшить"), this);
    zoomOutAct_->setShortcuts(QKeySequence::ZoomOut);
    zoomResetAct_ = new QAction(tr("Реальный размер"), this);
    zoomResetAct_->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0));
    zoomFitAct_ = new QAction(tr("По размеру окна"), this);
    zoomFitAct_->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_9));
    for (QAction* a : {zoomInAct_, zoomOutAct_, zoomResetAct_, zoomFitAct_}) {
        a->setPriority(QAction::LowPriority);  // в панели — только иконка
        a->setToolTip(QStringLiteral("%1 (%2)").arg(a->text(), a->shortcut().toString(QKeySequence::NativeText)));
    }

    themeGroup_ = new QActionGroup(this);
    const std::pair<QString, ThemeMode> themes[] = {
        {tr("Как в системе"), ThemeMode::System},
        {tr("Светлая"), ThemeMode::Light},
        {tr("Тёмная"), ThemeMode::Dark},
    };
    for (const auto& [text, mode] : themes) {
        QAction* a = themeGroup_->addAction(text);
        a->setCheckable(true);
        a->setData(static_cast<int>(mode));
        connect(a, &QAction::triggered, this, [this, mode] { setThemeMode(mode); });
    }

    exportThemeGroup_ = new QActionGroup(this);
    const std::pair<QString, ExportTheme> exportThemes[] = {
        {tr("Как в превью"), ExportTheme::AsPreview},
        {tr("Всегда светлая"), ExportTheme::Light},
        {tr("Всегда тёмная"), ExportTheme::Dark},
    };
    for (const auto& [text, theme] : exportThemes) {
        QAction* a = exportThemeGroup_->addAction(text);
        a->setCheckable(true);
        a->setData(static_cast<int>(theme));
        connect(a, &QAction::triggered, this, [this, theme] { exportTheme_ = theme; });
    }

    syntaxAct_ = make(tr("Синтаксис…"), &MainWindow::showSyntaxHelp, {QKeySequence::HelpContents});
    aboutAct_ = make(tr("О программе Birch"), &MainWindow::showAbout);
    aboutAct_->setMenuRole(QAction::AboutRole);
}

void MainWindow::createMenus() {
    QMenu* file = menuBar()->addMenu(tr("Файл"));
    file->addActions({newAct_, openAct_, saveAct_, saveAsAct_});
    file->addSeparator();
    file->addAction(exampleAct_);
    file->addSeparator();
    file->addActions({exportPngAct_, exportSvgAct_, copyImageAct_});
    file->addSeparator();
    file->addAction(quitAct_);

    QMenu* view = menuBar()->addMenu(tr("Вид"));
    view->addActions({renderAct_, autoRenderAct_});
    view->addSeparator();
    view->addActions({horizontalAct_, monochromeAct_});
    view->addSeparator();
    view->addActions({zoomInAct_, zoomOutAct_, zoomResetAct_, zoomFitAct_});
    view->addSeparator();
    QMenu* theme = view->addMenu(tr("Тема"));
    theme->addActions(themeGroup_->actions());

    QMenu* exportMenu = view->addMenu(tr("Экспорт картинки"));
    exportMenu->addAction(transparentAct_);
    exportMenu->addSeparator();
    exportMenu->addSection(tr("Тема картинки"));
    exportMenu->addActions(exportThemeGroup_->actions());

    QMenu* help = menuBar()->addMenu(tr("Справка"));
    help->addActions({syntaxAct_, aboutAct_});
}

void MainWindow::createToolBar() {
    QToolBar* tb = addToolBar(tr("Панель инструментов"));
    tb->setObjectName(QStringLiteral("mainToolBar"));
    tb->setMovable(false);
    tb->setFloatable(false);
    tb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tb->setIconSize(QSize(18, 18));

    tb->addAction(renderAct_);
    tb->addSeparator();
    tb->addAction(exportPngAct_);
    tb->addAction(copyImageAct_);
    tb->addSeparator();
    tb->addAction(horizontalAct_);
    tb->addAction(monochromeAct_);

    auto* spacer = new QWidget(tb);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    tb->addWidget(spacer);

    tb->addAction(zoomOutAct_);
    zoomButton_ = new QToolButton(tb);
    zoomButton_->setToolButtonStyle(Qt::ToolButtonTextOnly);
    zoomButton_->setToolTip(tr("Реальный размер"));
    zoomButton_->setMinimumWidth(zoomButton_->fontMetrics().horizontalAdvance(QStringLiteral("0000%")) + 12);
    connect(zoomButton_, &QToolButton::clicked, zoomResetAct_, &QAction::trigger);
    tb->addWidget(zoomButton_);
    tb->addAction(zoomInAct_);
    tb->addAction(zoomFitAct_);
}

void MainWindow::createCentralWidget() {
    // Слева: редактор + список проблем
    editor_ = new CodeEditor;
    highlighter_ = new SyntaxHighlighter(editor_->document());
    connect(editor_, &QPlainTextEdit::textChanged, this, &MainWindow::onTextChanged);
    connect(editor_->document(), &QTextDocument::modificationChanged, this, [this] { updateTitle(); });

    problems_ = new QListWidget;
    problems_->setFrameShape(QFrame::NoFrame);
    problems_->setUniformItemSizes(true);
    problems_->setTextElideMode(Qt::ElideRight);
    problems_->hide();
    connect(problems_, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        const int i = item->data(Qt::UserRole).toInt();
        if (i >= 0 && i < diagnostics_.size()) editor_->jumpTo(diagnostics_[i]);
    });

    auto* left = new QWidget;
    auto* leftLayout = new QVBoxLayout(left);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);
    leftLayout->addWidget(editor_, 1);
    leftLayout->addWidget(problems_);

    // Справа: баннер ошибки рендеринга + холст
    renderError_ = new QLabel;
    renderError_->setWordWrap(true);
    renderError_->setContentsMargins(12, 8, 12, 8);
    renderError_->setTextInteractionFlags(Qt::TextSelectableByMouse);
    renderError_->hide();

    preview_ = new PreviewView;
    preview_->setPlaceholderText(tr("Схема появится здесь.\nОпишите её слева, например:\n[Старт] -> [Готово]"));
    connect(preview_, &PreviewView::zoomChanged, this, &MainWindow::updateZoomLabel);
    connect(zoomInAct_, &QAction::triggered, preview_, &PreviewView::zoomIn);
    connect(zoomOutAct_, &QAction::triggered, preview_, &PreviewView::zoomOut);
    connect(zoomResetAct_, &QAction::triggered, preview_, &PreviewView::resetZoom);
    connect(zoomFitAct_, &QAction::triggered, preview_, &PreviewView::fitToView);

    auto* right = new QWidget;
    auto* rightLayout = new QVBoxLayout(right);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);
    rightLayout->addWidget(renderError_);
    rightLayout->addWidget(preview_, 1);

    splitter_ = new QSplitter(Qt::Horizontal);
    splitter_->setChildrenCollapsible(false);
    splitter_->setHandleWidth(1);
    splitter_->addWidget(left);
    splitter_->addWidget(right);
    splitter_->setStretchFactor(0, 4);
    splitter_->setStretchFactor(1, 6);
    left->setMinimumWidth(260);
    right->setMinimumWidth(300);

    setCentralWidget(splitter_);
    updateZoomLabel(preview_->zoom());
}

// =============================================================================
// Рендеринг
// =============================================================================

void MainWindow::onTextChanged() {
    if (autoRenderAct_->isChecked()) {
        debounce_->start();
    } else {
        // Позиции ошибок устарели — не показываем их на неправильных местах.
        editor_->clearDiagnostics();
    }
}

void MainWindow::renderNow() {
    debounce_->stop();
    const ParseResult parsed = parser_.parse(editor_->toPlainText());
    editor_->setDiagnostics(parsed.diagnostics);
    showProblems(parsed.diagnostics);

    const bool empty = parsed.diagram.isEmpty();
    for (QAction* a : {exportPngAct_, exportSvgAct_, copyImageAct_}) a->setEnabled(!empty);

    if (empty) {
        render_->cancel();
        preview_->clear();
        renderError_->hide();
        statusInfo_->clear();
        return;
    }
    pendingNodes_ = parsed.diagram.nodes.size();
    pendingEdges_ = parsed.diagram.edges.size();
    render_->submit(parsed.diagram, previewOptions());
}

void MainWindow::onRendered(const RenderResult& result, qint64 elapsedMs) {
    if (!result.ok()) {
        renderError_->setText(result.error);
        renderError_->show();
        emit previewRendered(false, result.error);
        return;  // старая картинка остаётся на месте
    }
    renderError_->hide();
    preview_->setSvg(result.svg);

    statusInfo_->setText(tr("Узлов: %1 · связей: %2 · %3 мс").arg(pendingNodes_).arg(pendingEdges_).arg(elapsedMs));
    emit previewRendered(true, {});
}

void MainWindow::showProblems(const QList<Diagnostic>& diagnostics) {
    diagnostics_ = diagnostics;
    problems_->clear();
    if (diagnostics.isEmpty()) {
        problems_->hide();
        return;
    }

    const bool dark = isDark();
    const QIcon errorIcon = dotIcon(dark ? QColor(0xFF6B6B) : QColor(0xD92D20));
    const QIcon warningIcon = dotIcon(dark ? QColor(0xF5B83D) : QColor(0xC77700));
    for (qsizetype i = 0; i < diagnostics.size(); ++i) {
        const Diagnostic& d = diagnostics[i];
        auto* item = new QListWidgetItem(d.isError() ? errorIcon : warningIcon,
                                         tr("Строка %1: %2").arg(d.line).arg(d.message), problems_);
        item->setToolTip(d.message);
        item->setData(Qt::UserRole, static_cast<int>(i));
    }
    const int rows = static_cast<int>(qMin<qsizetype>(diagnostics.size(), 4));
    problems_->setFixedHeight(rows * problems_->sizeHintForRow(0) + 2 * problems_->frameWidth() + 6);
    problems_->show();
}

RenderOptions MainWindow::previewOptions() const {
    RenderOptions opt;
    opt.theme = previewTheme();
    opt.direction = horizontalAct_->isChecked() ? LayoutDirection::LeftToRight : LayoutDirection::TopToBottom;
    opt.rasterize = false;  // превью рисует вектор
    return opt;
}

RenderOptions MainWindow::exportOptions() const {
    RenderOptions opt = previewOptions();
    const bool mono = monochromeAct_->isChecked();
    if (exportTheme_ == ExportTheme::Light) opt.theme = Theme::select(false, mono);
    if (exportTheme_ == ExportTheme::Dark) opt.theme = Theme::select(true, mono);
    opt.transparentBackground = transparentAct_->isChecked();
    opt.scale = kExportScale;
    opt.rasterize = true;
    return opt;
}

// Экспорт всегда перерисовывает текущий текст: картинка не отстаёт от ввода.
std::optional<RenderResult> MainWindow::renderForExport(bool rasterize) {
    const ParseResult parsed = parser_.parse(editor_->toPlainText());
    if (parsed.diagram.isEmpty()) {
        statusBar()->showMessage(tr("Нечего экспортировать — схема пуста"), 4000);
        return std::nullopt;
    }

    RenderOptions opt = exportOptions();
    opt.rasterize = rasterize;
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    RenderResult r = exportRenderer_.render(parsed.diagram, opt);
    QGuiApplication::restoreOverrideCursor();

    if (!r.ok()) {
        QMessageBox::warning(this, tr("Ошибка рендеринга"), r.error);
        return std::nullopt;
    }
    if (parsed.hasErrors())
        statusBar()->showMessage(tr("В тексте есть ошибки — в картинку попали только корректные строки"), 6000);
    return r;
}

// =============================================================================
// Экспорт
// =============================================================================

void MainWindow::exportPng() {
    const auto r = renderForExport(true);
    if (!r) return;

    QSettings settings;
    const QString dir = settings.value(keys::lastDir, defaultDirectory()).toString();
    const QString base = filePath_.isEmpty() ? QStringLiteral("flowchart") : QFileInfo(filePath_).completeBaseName();
    QString path = QFileDialog::getSaveFileName(this, tr("Скачать PNG"), QDir(dir).filePath(base + ".png"),
                                                tr("Изображение PNG (*.png)"));
    if (path.isEmpty()) return;
    if (!path.endsWith(QStringLiteral(".png"), Qt::CaseInsensitive)) path += QStringLiteral(".png");

    QSaveFile f(path);
    const QByteArray png = Renderer::toPng(r->image);
    if (png.isEmpty() || !f.open(QIODevice::WriteOnly) || f.write(png) != png.size() || !f.commit()) {
        QMessageBox::warning(this, tr("Не удалось сохранить"),
                             tr("Не удалось записать файл:\n%1\n\n%2").arg(path, f.errorString()));
        return;
    }
    settings.setValue(keys::lastDir, QFileInfo(path).absolutePath());
    statusBar()->showMessage(tr("Сохранено: %1 (%2×%3)")
                                 .arg(QFileInfo(path).fileName())
                                 .arg(r->image.width())
                                 .arg(r->image.height()),
                             5000);
}

void MainWindow::exportSvg() {
    const auto r = renderForExport(false);
    if (!r) return;

    QSettings settings;
    const QString dir = settings.value(keys::lastDir, defaultDirectory()).toString();
    const QString base = filePath_.isEmpty() ? QStringLiteral("flowchart") : QFileInfo(filePath_).completeBaseName();
    QString path = QFileDialog::getSaveFileName(this, tr("Экспорт SVG"), QDir(dir).filePath(base + ".svg"),
                                                tr("Векторное изображение SVG (*.svg)"));
    if (path.isEmpty()) return;
    if (!path.endsWith(QStringLiteral(".svg"), Qt::CaseInsensitive)) path += QStringLiteral(".svg");

    QSaveFile f(path);
    if (!f.open(QIODevice::WriteOnly) || f.write(r->svg) != r->svg.size() || !f.commit()) {
        QMessageBox::warning(this, tr("Не удалось сохранить"),
                             tr("Не удалось записать файл:\n%1\n\n%2").arg(path, f.errorString()));
        return;
    }
    settings.setValue(keys::lastDir, QFileInfo(path).absolutePath());
    statusBar()->showMessage(tr("Сохранено: %1").arg(QFileInfo(path).fileName()), 5000);
}

void MainWindow::copyImage() {
    const auto r = renderForExport(true);
    if (!r) return;

    // Два формата сразу: системный растровый (вставляется везде) и PNG
    // (сохраняет прозрачность в Office, Slack и браузерах).
    auto* mime = new QMimeData;
    mime->setImageData(r->image);
    mime->setData(QStringLiteral("image/png"), Renderer::toPng(r->image));
    QGuiApplication::clipboard()->setMimeData(mime);

    statusBar()->showMessage(tr("Картинка скопирована — вставьте её в Telegram, Slack, Word…"), 5000);
}

// =============================================================================
// Файлы и сессия
// =============================================================================

void MainWindow::newDocument() {
    if (!maybeSave()) return;
    editor_->clear();
    setCurrentFile({});
    editor_->setFocus();
}

void MainWindow::openDialog() {
    if (!maybeSave()) return;
    const QString dir = filePath_.isEmpty() ? QDir::homePath() : QFileInfo(filePath_).absolutePath();
    const QString path = QFileDialog::getOpenFileName(this, tr("Открыть схему"), dir,
                                                      tr("Схемы Birch (*.birch *.txt);;Все файлы (*)"));
    if (!path.isEmpty()) openFile(path);
}

bool MainWindow::openFile(const QString& path) {
    QFile f(path);
    if (f.size() > kMaxFileBytes) {
        QMessageBox::warning(this, tr("Файл слишком большой"),
                             tr("Файл «%1» больше %2 МБ.").arg(QFileInfo(path).fileName()).arg(kMaxFileBytes >> 20));
        return false;
    }
    if (!f.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Не удалось открыть"), tr("%1\n\n%2").arg(path, f.errorString()));
        return false;
    }
    editor_->setPlainText(QString::fromUtf8(f.readAll()));
    setCurrentFile(path);
    preview_->fitToView();
    renderNow();
    return true;
}

void MainWindow::openFromSystem(const QString& path) {
    if (QFileInfo(path) == QFileInfo(filePath_)) return;  // уже открыт
    if (maybeSave()) openFile(path);
    raise();
    activateWindow();
}

bool MainWindow::save() { return filePath_.isEmpty() ? saveAs() : writeFile(filePath_); }

bool MainWindow::saveAs() {
    const QString dir = filePath_.isEmpty() ? QDir::homePath() : QFileInfo(filePath_).absolutePath();
    const QString name = filePath_.isEmpty() ? QStringLiteral("flowchart.birch") : QFileInfo(filePath_).fileName();
    QString path = QFileDialog::getSaveFileName(this, tr("Сохранить схему"), QDir(dir).filePath(name),
                                                tr("Схемы Birch (*.birch)"));
    if (path.isEmpty()) return false;
    if (QFileInfo(path).suffix().isEmpty()) path += QStringLiteral(".birch");
    return writeFile(path);
}

bool MainWindow::writeFile(const QString& path) {
    QSaveFile f(path);
    const QByteArray bytes = editor_->toPlainText().toUtf8();
    if (!f.open(QIODevice::WriteOnly) || f.write(bytes) != bytes.size() || !f.commit()) {
        QMessageBox::warning(this, tr("Не удалось сохранить"), tr("%1\n\n%2").arg(path, f.errorString()));
        return false;
    }
    setCurrentFile(path);
    statusBar()->showMessage(tr("Сохранено: %1").arg(QFileInfo(path).fileName()), 3000);
    return true;
}

// Несохранённый текст без файла не теряется — он хранится в сессии,
// поэтому спрашиваем только про изменённые файлы.
bool MainWindow::maybeSave() {
    if (filePath_.isEmpty() || !editor_->document()->isModified()) return true;
    const auto answer = QMessageBox::question(
        this, tr("Сохранить изменения?"),
        tr("В файле «%1» есть несохранённые изменения.").arg(QFileInfo(filePath_).fileName()),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
    if (answer == QMessageBox::Save) return save();
    return answer == QMessageBox::Discard;
}

void MainWindow::setCurrentFile(const QString& path) {
    filePath_ = path;
    editor_->document()->setModified(false);
    updateTitle();
}

void MainWindow::loadExample() {
    if (!maybeSave()) return;
    editor_->setPlainText(readResourceText(QStringLiteral(":/examples/welcome.birch")));
    setCurrentFile({});
    preview_->fitToView();
    renderNow();
}

void MainWindow::updateTitle() {
    const QString name = filePath_.isEmpty() ? tr("Без названия") : QFileInfo(filePath_).fileName();
    setWindowTitle(QStringLiteral("%1[*] — Birch").arg(name));
    setWindowFilePath(filePath_);  // macOS: иконка файла в заголовке
    setWindowModified(!filePath_.isEmpty() && editor_->document()->isModified());
}

namespace {
// До версии 0.3.3 включительно «_» в тексте узла означал пробел, и стартовый пример был
// записан через подчёркивания. Нетронутый старый пример из прошлого сеанса
// заменяем новым, иначе в нём появятся «Ввод_данных» и «Показать_ошибку».
bool isOldWelcomeExample(const QString& text) {
    static const QString old = QStringLiteral(
        "// Добро пожаловать в Birch!\n"
        "// [Процесс]   {Условие?}   (Старт/Конец)   подпись стрелки — в кавычках после узла\n"
        "\n"
        "(Старт) -> [Ввод_данных] -> {Данные корректны?};\n"
        "{Данные корректны?} -> [Сохранить] \"Да\" -> (Конец);\n"
        "{Данные корректны?} -> [Показать_ошибку] \"Нет\"\n"
        "    -> [Ввод_данных];");
    return text.trimmed() == old;
}
}  // namespace

void MainWindow::restoreSession() {
    QSettings s;
    restoreGeometry(s.value(keys::geometry).toByteArray());
    restoreState(s.value(keys::windowState).toByteArray());
    if (!splitter_->restoreState(s.value(keys::splitter).toByteArray())) splitter_->setSizes({480, 800});

    {
        const QSignalBlocker b1(horizontalAct_);
        horizontalAct_->setChecked(s.value(keys::horizontal, false).toBool());
        const QSignalBlocker b2(monochromeAct_);
        monochromeAct_->setChecked(s.value(keys::monochrome, false).toBool());
    }
    autoRenderAct_->setChecked(s.value(keys::autoRender, true).toBool());
    transparentAct_->setChecked(s.value(keys::transparent, false).toBool());

    exportTheme_ = static_cast<ExportTheme>(s.value(keys::exportTheme, 0).toInt());
    for (QAction* a : exportThemeGroup_->actions()) a->setChecked(a->data().toInt() == static_cast<int>(exportTheme_));
    setThemeMode(static_cast<ThemeMode>(s.value(keys::theme, 0).toInt()));

    const QSignalBlocker block(editor_);  // не запускаем рендер раньше времени
    const QString file = s.value(keys::sessionFile).toString();
    if (!file.isEmpty() && QFileInfo::exists(file)) {
        QFile f(file);
        if (f.size() <= kMaxFileBytes && f.open(QIODevice::ReadOnly)) {
            editor_->setPlainText(QString::fromUtf8(f.readAll()));
            setCurrentFile(file);
            return;
        }
    }
    if (s.contains(keys::sessionText) && !isOldWelcomeExample(s.value(keys::sessionText).toString())) {
        editor_->setPlainText(s.value(keys::sessionText).toString());
    } else {
        editor_->setPlainText(readResourceText(QStringLiteral(":/examples/welcome.birch")));
    }
    setCurrentFile({});
}

void MainWindow::saveSession() {
    QSettings s;
    s.setValue(keys::geometry, saveGeometry());
    s.setValue(keys::windowState, saveState());
    s.setValue(keys::splitter, splitter_->saveState());
    s.setValue(keys::horizontal, horizontalAct_->isChecked());
    s.setValue(keys::monochrome, monochromeAct_->isChecked());
    s.setValue(keys::autoRender, autoRenderAct_->isChecked());
    s.setValue(keys::transparent, transparentAct_->isChecked());
    s.setValue(keys::theme, static_cast<int>(themeMode_));
    s.setValue(keys::exportTheme, static_cast<int>(exportTheme_));

    if (filePath_.isEmpty()) {
        s.setValue(keys::sessionText, editor_->toPlainText());
        s.remove(keys::sessionFile);
    } else {
        s.setValue(keys::sessionFile, filePath_);
        s.remove(keys::sessionText);
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (!maybeSave()) {
        event->ignore();
        return;
    }
    if (sessionSaving_) saveSession();
    event->accept();
}

// =============================================================================
// Тема и иконки
// =============================================================================

void MainWindow::setThemeMode(ThemeMode mode) {
    themeMode_ = mode;
    for (QAction* a : themeGroup_->actions()) a->setChecked(a->data().toInt() == static_cast<int>(mode));

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    // Переключаем и сам интерфейс (палитру Qt), а не только схему.
    Qt::ColorScheme scheme = Qt::ColorScheme::Unknown;
    if (mode == ThemeMode::Light) scheme = Qt::ColorScheme::Light;
    if (mode == ThemeMode::Dark) scheme = Qt::ColorScheme::Dark;
    QGuiApplication::styleHints()->setColorScheme(scheme);
#endif
    applyTheme();
}

bool MainWindow::isDark() const {
    switch (themeMode_) {
    case ThemeMode::Light: return false;
    case ThemeMode::Dark: return true;
    case ThemeMode::System: break;
    }
    const Qt::ColorScheme scheme = QGuiApplication::styleHints()->colorScheme();
    if (scheme != Qt::ColorScheme::Unknown) return scheme == Qt::ColorScheme::Dark;
    return palette().color(QPalette::Window).lightness() < 128;
}

bool MainWindow::setThemeByName(const QString& name) {
    if (name == QLatin1String("system")) setThemeMode(ThemeMode::System);
    else if (name == QLatin1String("light")) setThemeMode(ThemeMode::Light);
    else if (name == QLatin1String("dark")) setThemeMode(ThemeMode::Dark);
    else return false;
    return true;
}

QRect MainWindow::editorViewportRect() const {
    const QWidget* vp = editor_->viewport();
    return QRect(vp->mapTo(this, QPoint(0, 0)), vp->size());
}

Theme MainWindow::previewTheme() const { return Theme::select(isDark(), monochromeAct_->isChecked()); }

QIcon MainWindow::icon(const QString& name) const {
    QFile f(QStringLiteral(":/icons/%1.svg").arg(name));
    if (!f.open(QIODevice::ReadOnly)) return {};
    QByteArray svg = f.readAll();
    svg.replace("currentColor", palette().color(QPalette::ButtonText).name().toUtf8());

    QSvgRenderer renderer(svg);
    QIcon result;
    for (const int size : {16, 18, 24, 32, 36, 48}) {
        QPixmap pm(size, size);
        pm.fill(Qt::transparent);
        QPainter p(&pm);
        renderer.render(&p);
        p.end();
        result.addPixmap(pm);
    }
    return result;
}

void MainWindow::applyPalette() {
    if (styleFollowsColorScheme()) return;  // стиль перекрасится сам (macOS, Windows 11)

    const Qt::ColorScheme system = QGuiApplication::styleHints()->colorScheme();
    if (themeMode_ == ThemeMode::System && system == Qt::ColorScheme::Unknown) {
        QApplication::setPalette(QPalette());  // тема системы неизвестна — палитра платформы
        return;
    }
    QApplication::setPalette(isDark() ? darkPalette() : QApplication::style()->standardPalette());
}

void MainWindow::applyTheme() {
    if (applyingTheme_ || !editor_) return;
    applyingTheme_ = true;

    applyPalette();
    const bool dark = isDark();
    highlighter_->setDarkMode(dark);
    preview_->setCanvasColor(previewTheme().background);

    renderAct_->setIcon(icon(QStringLiteral("render")));
    exportPngAct_->setIcon(icon(QStringLiteral("download")));
    copyImageAct_->setIcon(icon(QStringLiteral("copy")));
    horizontalAct_->setIcon(icon(QStringLiteral("direction")));
    monochromeAct_->setIcon(icon(QStringLiteral("monochrome")));
    zoomInAct_->setIcon(icon(QStringLiteral("zoom-in")));
    zoomOutAct_->setIcon(icon(QStringLiteral("zoom-out")));
    zoomFitAct_->setIcon(icon(QStringLiteral("fit")));

    const QColor bg = dark ? QColor(0x4A1F1F) : QColor(0xFDECEC);
    const QColor fg = dark ? QColor(0xFFD7D7) : QColor(0x8A1C1C);
    renderError_->setStyleSheet(QStringLiteral("background:%1; color:%2;").arg(bg.name(), fg.name()));
    // Тонкая линия-разделитель в цвет темы (стандартная ручка в тёмной теме слишком яркая).
    const QColor line = dark ? QColor(0x34363C) : QColor(0xDADDE3);
    splitter_->setStyleSheet(QStringLiteral("QSplitter::handle { background: %1; }").arg(line.name()));

    showProblems(diagnostics_);  // перекрасить значки
    renderNow();                 // схема в новых цветах

    applyingTheme_ = false;
}

void MainWindow::changeEvent(QEvent* event) {
    QMainWindow::changeEvent(event);
    if (event->type() == QEvent::PaletteChange) applyTheme();
}

void MainWindow::updateZoomLabel(qreal zoom) {
    if (zoomButton_) zoomButton_->setText(QStringLiteral("%1%").arg(qRound(zoom * 100)));
}

// =============================================================================
// Справка
// =============================================================================

void MainWindow::showSyntaxHelp() {
    QMessageBox box(this);
    box.setWindowTitle(tr("Синтаксис Birch"));
    box.setTextFormat(Qt::RichText);
    box.setText(tr("<h3>Синтаксис Birch</h3>"
                   "<table cellspacing='6'>"
                   "<tr><td><code>[Текст]</code></td><td>шаг (прямоугольник)</td></tr>"
                   "<tr><td><code>{Условие?}</code></td><td>решение (ромб)</td></tr>"
                   "<tr><td><code>(Старт)</code></td><td>начало / конец</td></tr>"
                   "<tr><td><code>[A] -&gt; [B]</code></td><td>стрелка; можно цепочкой: <code>[A] -&gt; [B] -&gt; [C]</code></td></tr>"
                   "<tr><td><code>[A] -&gt; [B] \"Да\"</code></td><td>подпись стрелки</td></tr>"
                   "<tr><td><code>;</code> или Enter</td><td>конец инструкции</td></tr>"
                   "<tr><td><code>// …</code> или <code># …</code></td><td>комментарий</td></tr>"
                   "<tr><td><code>\\n</code></td><td>перенос строки в тексте</td></tr>"
                   "</table>"
                   "<p>Одинаковый текст — один и тот же узел. Если узел хоть раз записан как "
                   "<code>{…}</code> или <code>(…)</code>, он останется этой фигурой.</p>"));
    box.exec();
}

void MainWindow::showAbout() {
    QMessageBox::about(this, tr("О программе Birch"),
                       tr("<h3>Birch %1</h3>"
                          "<p>Блок-схемы из текста — без ИИ, полностью офлайн.</p>"
                          "<p>Graphviz %2 · Qt %3</p>"
                          "<p>Распространяется по лицензии "
                          "<a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">GNU GPL v3</a> "
                          "с дополнительным разрешением на связывание с Graphviz.</p>")
                           .arg(QApplication::applicationVersion(), Renderer::graphvizVersion(),
                                QString::fromLatin1(qVersion())));
}

}  // namespace birch
