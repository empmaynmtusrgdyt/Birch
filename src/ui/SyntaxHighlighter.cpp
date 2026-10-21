#include "ui/SyntaxHighlighter.h"

namespace birch {

namespace {

QTextCharFormat makeFormat(const QColor& color, bool bold = false, bool italic = false) {
    QTextCharFormat f;
    f.setForeground(color);
    if (bold) f.setFontWeight(QFont::DemiBold);
    f.setFontItalic(italic);
    return f;
}

// Позиция сразу за закрывающим символом (или конец строки), с учётом '\'-экранирования.
qsizetype skipUntil(const QString& text, qsizetype from, QChar close) {
    qsizetype j = from + 1;
    while (j < text.size()) {
        if (text[j] == u'\\' && j + 1 < text.size()) {
            j += 2;
            continue;
        }
        if (text[j++] == close) break;
    }
    return j;
}

}  // namespace

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* document) : QSyntaxHighlighter(document) {
    setDarkMode(false);
}

void SyntaxHighlighter::setDarkMode(bool dark) {
    if (dark) {
        process_ = makeFormat(QColor(0x7AA7FF));
        decision_ = makeFormat(QColor(0xF2C14E));
        terminal_ = makeFormat(QColor(0x4FD69C));
        string_ = makeFormat(QColor(0xD7A6FF));
        arrow_ = makeFormat(QColor(0xA3AEC2), true);
        punctuation_ = makeFormat(QColor(0x8792A6));
        comment_ = makeFormat(QColor(0x6B7385), false, true);
    } else {
        process_ = makeFormat(QColor(0x2459D6));
        decision_ = makeFormat(QColor(0xA75F00));
        terminal_ = makeFormat(QColor(0x0B7A52));
        string_ = makeFormat(QColor(0x8A3FD1));
        arrow_ = makeFormat(QColor(0x55627A), true);
        punctuation_ = makeFormat(QColor(0x8792A6));
        comment_ = makeFormat(QColor(0x9AA3B2), false, true);
    }
    rehighlight();
}

void SyntaxHighlighter::highlightBlock(const QString& text) {
    auto fmt = [this](qsizetype start, qsizetype end, const QTextCharFormat& f) {
        setFormat(static_cast<int>(start), static_cast<int>(end - start), f);
    };

    qsizetype i = 0;
    const qsizetype n = text.size();
    while (i < n) {
        const QChar c = text[i];
        const QChar next = i + 1 < n ? text[i + 1] : QChar();

        if (c == u'#' || (c == u'/' && next == u'/')) {
            fmt(i, n, comment_);
            return;
        }

        const QTextCharFormat* f = nullptr;
        QChar close;
        switch (c.unicode()) {
        case u'[': f = &process_; close = u']'; break;
        case u'{': f = &decision_; close = u'}'; break;
        case u'(': f = &terminal_; close = u')'; break;
        case u'"': f = &string_; close = u'"'; break;
        case u'«': f = &string_; close = u'»'; break;
        case u'“': f = &string_; close = u'”'; break;
        default: break;
        }
        if (f) {
            const qsizetype end = skipUntil(text, i, close);
            fmt(i, end, *f);
            i = end;
            continue;
        }

        if (c == u'-' && next == u'>') {
            fmt(i, i + 2, arrow_);
            i += 2;
            continue;
        }
        if (c == u'→') fmt(i, i + 1, arrow_);
        else if (c == u';') fmt(i, i + 1, punctuation_);
        ++i;
    }
}

}  // namespace birch
