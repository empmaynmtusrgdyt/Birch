#include "ui/CodeEditor.h"

#include <QFontDatabase>
#include <QHelpEvent>
#include <QPainter>
#include <QTextBlock>
#include <QToolTip>

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

void CodeEditor::applyEditorFont() {
    QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    f.setPointSizeF(qMax(f.pointSizeF(), 13.0));
    setFont(f);
    setTabStopDistance(QFontMetricsF(f).horizontalAdvance(QString(kTabSpaces, u' ')));
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
    for (const auto& d : diagnostics_) {
        // Ошибка «в конце текста» указывает за последний символ — подчёркиваем последний.
        int start = qMin(d.offset, docEnd);
        int end = qMin(d.offset + d.length, docEnd);
        if (start == end && start > 0) --start;
        if (start == end) continue;

        QTextEdit::ExtraSelection sel;
        sel.format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
        sel.format.setUnderlineColor(d.isError() ? errorColor() : warningColor());
        sel.cursor = QTextCursor(document());
        sel.cursor.setPosition(start);
        sel.cursor.setPosition(end, QTextCursor::KeepAnchor);
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
        updateExtraSelections();
    } else if (event->type() == QEvent::ApplicationFontChange) {
        applyEditorFont();
    }
}

}  // namespace birch
