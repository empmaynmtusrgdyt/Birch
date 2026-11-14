/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "ui/CodeEditor.h"

#include <QFontDatabase>
#include <QHelpEvent>
#include <QPainter>
#include <QTextBlock>
#include <QToolTip>
#include <QWheelEvent>

namespace birch {

namespace {

class LineNumberArea : public QWidget {
public:
    explicit LineNumberArea(CodeEditor* editor) : QWidget(editor), editor_(editor) {}
    QSize sizeHint() const override { return {editor_->lineNumberAreaWidth(), 0}; }

protected:
    void paintEvent(QPaintEvent* event) override { editor_->paintLineNumberArea(event); }

private:
    CodeEditor* editor_;
};

constexpr int kTabSpaces = 4;
constexpr int kGutterPadding = 10;

enum class FontZoom { None, In, Out, Reset };

// ⌘/Ctrl + «+» (или «=» без Shift), «−», «0» — масштаб шрифта редактора.
FontZoom fontZoomFor(const QKeyEvent* event) {
    const Qt::KeyboardModifiers mods = event->modifiers() & ~(Qt::ShiftModifier | Qt::KeypadModifier);
    if (mods != Qt::ControlModifier) return FontZoom::None;
    switch (event->key()) {
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        return FontZoom::In;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
        return FontZoom::Out;
    case Qt::Key_0:
        return FontZoom::Reset;
    default:
        return FontZoom::None;
    }
}

}  // namespace

CodeEditor::CodeEditor(QWidget* parent) : QPlainTextEdit(parent), lineNumberArea_(new LineNumberArea(this)) {
    setLineWrapMode(QPlainTextEdit::NoWrap);
    setFrameShape(QFrame::NoFrame);
    setPlaceholderText(tr("Опишите блок-схему, например:\n\n[Старт] -> [Шаг] -> [Готово];"));
    applyEditorFont();

    connect(this, &QPlainTextEdit::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &QPlainTextEdit::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &CodeEditor::updateExtraSelections);

    updateLineNumberAreaWidth();
    updateExtraSelections();
}

qreal CodeEditor::defaultFontPointSize() {
    return qMax(QFontDatabase::systemFont(QFontDatabase::FixedFont).pointSizeF(), 13.0);
}

void CodeEditor::applyEditorFont() {
    QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    f.setPointSizeF(customFontPointSize_ > 0 ? customFontPointSize_ : defaultFontPointSize());
    setFont(f);
    setTabStopDistance(QFontMetricsF(f).horizontalAdvance(QString(kTabSpaces, u' ')));
    updateLineNumberAreaWidth();  // номера строк — тем же шрифтом
    lineNumberArea_->update();
}

void CodeEditor::setFontPointSize(qreal size) {
    const qreal old = fontPointSize();
    customFontPointSize_ = size > 0 ? qBound(kMinFontPointSize, size, kMaxFontPointSize) : 0;
    if (qFuzzyCompare(customFontPointSize_, defaultFontPointSize())) customFontPointSize_ = 0;
    applyEditorFont();
    if (!qFuzzyCompare(old, fontPointSize())) emit fontPointSizeChanged(fontPointSize());
}

bool CodeEditor::event(QEvent* event) {
    // Сочетания масштаба есть и у окна (масштаб схемы). Когда фокус в редакторе,
    // забираем их себе: они придут в keyPressEvent, а не в действия окна.
    if (event->type() == QEvent::ShortcutOverride && fontZoomFor(static_cast<QKeyEvent*>(event)) != FontZoom::None) {
        event->accept();
        return true;
    }
    return QPlainTextEdit::event(event);
}

void CodeEditor::wheelEvent(QWheelEvent* event) {
    if (!(event->modifiers() & Qt::ControlModifier)) {
        QPlainTextEdit::wheelEvent(event);
        return;
    }
    // Колесо мыши — шаг 120; трекпад присылает много мелких шагов, копим их.
    wheelZoomDelta_ += event->angleDelta().y();
    while (wheelZoomDelta_ >= 120) {
        increaseFontSize();
        wheelZoomDelta_ -= 120;
    }
    while (wheelZoomDelta_ <= -120) {
        decreaseFontSize();
        wheelZoomDelta_ += 120;
    }
    event->accept();
}

bool CodeEditor::isDark() const { return palette().color(QPalette::Base).lightness() < 128; }
QColor CodeEditor::errorColor() const { return isDark() ? QColor(0xFF6B6B) : QColor(0xD92D20); }
QColor CodeEditor::warningColor() const { return isDark() ? QColor(0xF5B83D) : QColor(0xC77700); }

// --- номера строк -----------------------------------------------------------

int CodeEditor::lineNumberAreaWidth() const {
    const int digits = static_cast<int>(QString::number(qMax(1, blockCount())).size());
    return kGutterPadding * 2 + fontMetrics().horizontalAdvance(u'9') * qMax(2, digits);
}

void CodeEditor::updateLineNumberAreaWidth() { setViewportMargins(lineNumberAreaWidth(), 0, 0, 0); }

void CodeEditor::updateLineNumberArea(const QRect& rect, int dy) {
    if (dy) lineNumberArea_->scroll(0, dy);
    else lineNumberArea_->update(0, rect.y(), lineNumberArea_->width(), rect.height());
    if (rect.contains(viewport()->rect())) updateLineNumberAreaWidth();
}

void CodeEditor::resizeEvent(QResizeEvent* event) {
    QPlainTextEdit::resizeEvent(event);
    const QRect cr = contentsRect();
    lineNumberArea_->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::paintLineNumberArea(QPaintEvent* event) {
    QPainter p(lineNumberArea_);
    p.fillRect(event->rect(), palette().color(QPalette::Base));

    const QColor normal = palette().color(QPalette::PlaceholderText);
    const QColor current = palette().color(QPalette::Text);
    const int currentBlock = textCursor().blockNumber();
    const int width = lineNumberArea_->width();

    QTextBlock block = firstVisibleBlock();
    int number = block.blockNumber();
    qreal top = blockBoundingGeometry(block).translated(contentOffset()).top();
    qreal bottom = top + blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            const QRectF lineRect(0, top, width - kGutterPadding, fontMetrics().height());
            QColor color = number == currentBlock ? current : normal;

            if (errorLines_.contains(number) || warningLines_.contains(number)) {
                color = errorLines_.contains(number) ? errorColor() : warningColor();
                p.setPen(Qt::NoPen);
                p.setBrush(color);
                p.setRenderHint(QPainter::Antialiasing);
                const qreal r = 3.0;
                p.drawEllipse(QPointF(kGutterPadding / 2.0 + 1, lineRect.center().y()), r, r);
            }
            p.setPen(color);
            p.drawText(lineRect, Qt::AlignRight | Qt::AlignVCenter, QString::number(number + 1));
        }
        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
        ++number;
    }
}

// --- выделения: текущая строка + ошибки ---------------------------------------

void CodeEditor::setDiagnostics(const QList<Diagnostic>& diagnostics) {
    diagnostics_ = diagnostics;
    errorLines_.clear();
    warningLines_.clear();
    for (const auto& d : diagnostics_)
        (d.isError() ? errorLines_ : warningLines_).insert(d.line - 1);
    updateExtraSelections();
    lineNumberArea_->update();
}

void CodeEditor::setSearchMatches(const QList<std::pair<int, int>>& matches, int current) {
    searchMatches_ = matches;
    currentSearchMatch_ = current;
    if (searchPaletteSet_ != !matches.isEmpty()) applySearchPalette();
    updateExtraSelections();
}

QColor CodeEditor::searchMatchColor() const { return isDark() ? QColor(0x6B5A1E) : QColor(0xFFE58F); }
QColor CodeEditor::currentSearchMatchColor() const { return isDark() ? QColor(0xB0791C) : QColor(0xFFB648); }

void CodeEditor::applySearchPalette() {
    // Текущее совпадение выделено курсором. Пока фокус в поле поиска, выделение
    // неактивного редактора серое и перекрывает подсветку — красим его в цвет
    // текущего совпадения. Без поиска — снова обычная палитра окна.
    applyingPalette_ = true;
    searchPaletteSet_ = !searchMatches_.isEmpty();
    if (searchPaletteSet_) {
        QPalette p = palette();
        p.setColor(QPalette::Inactive, QPalette::Highlight, currentSearchMatchColor());
        p.setColor(QPalette::Inactive, QPalette::HighlightedText,
                   isDark() ? QColor(0xFFFFFF) : p.color(QPalette::Active, QPalette::Text));
        setPalette(p);
    } else {
        setPalette(QPalette());
    }
    applyingPalette_ = false;
}

QTextEdit::ExtraSelection CodeEditor::rangeSelection(int start, int end) const {
    QTextEdit::ExtraSelection sel;
    sel.cursor = QTextCursor(document());
    sel.cursor.setPosition(start);
    sel.cursor.setPosition(end, QTextCursor::KeepAnchor);
    return sel;
}

void CodeEditor::updateExtraSelections() {
    QList<QTextEdit::ExtraSelection> selections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection line;
        const QColor base = palette().color(QPalette::Base);
        line.format.setBackground(isDark() ? base.lighter(125) : base.darker(104));
        line.format.setProperty(QTextFormat::FullWidthSelection, true);
        line.cursor = textCursor();
        line.cursor.clearSelection();
        selections.append(line);
    }

    const int docEnd = qMax(0, document()->characterCount() - 1);

    // Совпадения поиска — под подчёркиванием ошибок, чтобы оно оставалось видно.
    // Текущее совпадение обычно ещё и выделено курсором; яркий фон отличает его,
    // когда фокус в поле поиска и выделение редактора не видно.
    const bool dark = isDark();
    const QColor matchColor = searchMatchColor();
    const QColor currentColor = currentSearchMatchColor();
    for (int i = 0; i < searchMatches_.size(); ++i) {
        const auto [start, length] = searchMatches_[i];
        if (start + length > docEnd) continue;  // устарело: текст уже изменился
        QTextEdit::ExtraSelection sel = rangeSelection(start, start + length);
        sel.format.setBackground(i == currentSearchMatch_ ? currentColor : matchColor);
        if (dark) sel.format.setForeground(QColor(0xFFFFFF));
        selections.append(sel);
    }

    for (const auto& d : diagnostics_) {
        // Ошибка «в конце текста» указывает за последний символ — подчёркиваем последний.
        int start = qMin(d.offset, docEnd);
        int end = qMin(d.offset + d.length, docEnd);
        if (start == end && start > 0) --start;
        if (start == end) continue;

        QTextEdit::ExtraSelection sel = rangeSelection(start, end);
        sel.format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
        sel.format.setUnderlineColor(d.isError() ? errorColor() : warningColor());
        selections.append(sel);
    }
    setExtraSelections(selections);
    lineNumberArea_->update();
}

const Diagnostic* CodeEditor::diagnosticAt(int position) const {
    for (const auto& d : diagnostics_) {
        // +1: курсор над последним символом подчёркивания тоже считается попаданием
        if (position >= d.offset && position <= d.offset + d.length) return &d;
    }
    return nullptr;
}

void CodeEditor::jumpTo(const Diagnostic& d) {
    QTextCursor c(document());
    const int docEnd = qMax(0, document()->characterCount() - 1);
    c.setPosition(qMin(d.offset, docEnd));
    c.setPosition(qMin(d.offset + d.length, docEnd), QTextCursor::KeepAnchor);
    setTextCursor(c);
    centerCursor();
    setFocus();
}

bool CodeEditor::viewportEvent(QEvent* event) {
    if (event->type() == QEvent::ToolTip) {
        auto* help = static_cast<QHelpEvent*>(event);
        const QTextCursor c = cursorForPosition(help->pos());
        if (const Diagnostic* d = diagnosticAt(c.position())) {
            const QString kind = d->isError() ? tr("Ошибка") : tr("Предупреждение");
            QToolTip::showText(help->globalPos(), QStringLiteral("<b>%1:</b> %2").arg(kind, d->message.toHtmlEscaped()),
                               viewport());
        } else {
            QToolTip::hideText();
        }
        return true;
    }
    return QPlainTextEdit::viewportEvent(event);
}

// --- ввод ---------------------------------------------------------------------

void CodeEditor::keyPressEvent(QKeyEvent* event) {
    switch (fontZoomFor(event)) {
    case FontZoom::In:
        increaseFontSize();
        return;
    case FontZoom::Out:
        decreaseFontSize();
        return;
    case FontZoom::Reset:
        resetFontSize();
        return;
    case FontZoom::None:
        break;
    }
    // Tab -> пробелы
    if (event->key() == Qt::Key_Tab && event->modifiers() == Qt::NoModifier) {
        QTextCursor c = textCursor();
        const int column = c.positionInBlock();
        c.insertText(QString(kTabSpaces - column % kTabSpaces, u' '));
        return;
    }
    // Enter сохраняет отступ текущей строки
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) &&
        !(event->modifiers() & (Qt::ControlModifier | Qt::MetaModifier | Qt::AltModifier))) {
        const QString line = textCursor().block().text();
        qsizetype indent = 0;
        while (indent < line.size() && (line[indent] == u' ' || line[indent] == u'\t')) ++indent;
        QPlainTextEdit::keyPressEvent(event);
        textCursor().insertText(line.left(indent));
        return;
    }
    QPlainTextEdit::keyPressEvent(event);
}

void CodeEditor::changeEvent(QEvent* event) {
    QPlainTextEdit::changeEvent(event);
    if (event->type() == QEvent::PaletteChange) {
        if (applyingPalette_) return;
        if (searchPaletteSet_) applySearchPalette();  // цвета под новую тему
        updateExtraSelections();
    } else if (event->type() == QEvent::ApplicationFontChange) {
        applyEditorFont();
    }
}

}  // namespace birch
