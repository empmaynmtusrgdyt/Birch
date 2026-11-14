/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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

    // Подсветка совпадений поиска: все совпадения и текущее (current, -1 — нет).
    // Каждое совпадение — пара «начало, длина» в позициях документа.
    void setSearchMatches(const QList<std::pair<int, int>>& matches, int current);

    // Размер шрифта в пунктах. Меняется сочетаниями ⌘/Ctrl + «+», «−», «0» и
    // ⌘/Ctrl + колесо, когда фокус в редакторе, — не трогая масштаб схемы.
    static constexpr qreal kMinFontPointSize = 8;
    static constexpr qreal kMaxFontPointSize = 40;
    static qreal defaultFontPointSize();
    qreal fontPointSize() const { return font().pointSizeF(); }
    // 0 — размер по умолчанию; иначе ограничивается от kMin до kMax.
    void setFontPointSize(qreal size);
    void increaseFontSize() { setFontPointSize(fontPointSize() + 1); }
    void decreaseFontSize() { setFontPointSize(fontPointSize() - 1); }
    void resetFontSize() { setFontPointSize(0); }
    bool hasDefaultFontSize() const { return customFontPointSize_ <= 0; }

    // Для LineNumberArea
    int lineNumberAreaWidth() const;
    void paintLineNumberArea(QPaintEvent* event);

signals:
    void fontPointSizeChanged(qreal size);

protected:
    bool event(QEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    bool viewportEvent(QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void changeEvent(QEvent* event) override;

private:
    bool isDark() const;
    QColor errorColor() const;
    QColor warningColor() const;
    QColor searchMatchColor() const;
    QColor currentSearchMatchColor() const;
    void applySearchPalette();
    QTextEdit::ExtraSelection rangeSelection(int start, int end) const;
    void applyEditorFont();
    void updateLineNumberAreaWidth();
    void updateLineNumberArea(const QRect& rect, int dy);
    void updateExtraSelections();
    const Diagnostic* diagnosticAt(int position) const;

    QWidget* lineNumberArea_ = nullptr;
    QList<Diagnostic> diagnostics_;
    QSet<int> errorLines_;    // номера блоков (с 0)
    QSet<int> warningLines_;
    QList<std::pair<int, int>> searchMatches_;
    int currentSearchMatch_ = -1;
    bool searchPaletteSet_ = false;
    bool applyingPalette_ = false;
    qreal customFontPointSize_ = 0;  // 0 — по умолчанию
    int wheelZoomDelta_ = 0;         // накопленная прокрутка трекпада
};

}  // namespace birch
