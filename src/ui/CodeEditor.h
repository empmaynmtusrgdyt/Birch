#pragma once

#include <QPlainTextEdit>
#include <QSet>

#include "core/Parser.h"

namespace birch {

// Текстовый редактор DSL: номера строк, подсветка текущей строки,
// волнистое подчёркивание ошибок/предупреждений с подсказкой при наведении.
class CodeEditor : public QPlainTextEdit {
    Q_OBJECT
public:
    explicit CodeEditor(QWidget* parent = nullptr);

    void setDiagnostics(const QList<Diagnostic>& diagnostics);
    void clearDiagnostics() { setDiagnostics({}); }
    void jumpTo(const Diagnostic& diagnostic);

    // Для LineNumberArea
    int lineNumberAreaWidth() const;
    void paintLineNumberArea(QPaintEvent* event);

protected:
    void resizeEvent(QResizeEvent* event) override;
    bool viewportEvent(QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void changeEvent(QEvent* event) override;

private:
    bool isDark() const;
    QColor errorColor() const;
    QColor warningColor() const;
    void applyEditorFont();
    void updateLineNumberAreaWidth();
    void updateLineNumberArea(const QRect& rect, int dy);
    void updateExtraSelections();
    const Diagnostic* diagnosticAt(int position) const;

    QWidget* lineNumberArea_ = nullptr;
    QList<Diagnostic> diagnostics_;
    QSet<int> errorLines_;    // номера блоков (с 0)
    QSet<int> warningLines_;
};

}  // namespace birch
