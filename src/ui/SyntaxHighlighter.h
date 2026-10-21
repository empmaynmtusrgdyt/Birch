#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

namespace birch {

// Подсветка DSL Birch. Узлы окрашены в цвета своих фигур на схеме.
class SyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    explicit SyntaxHighlighter(QTextDocument* document);

    void setDarkMode(bool dark);

protected:
    void highlightBlock(const QString& text) override;

private:
    QTextCharFormat process_;
    QTextCharFormat decision_;
    QTextCharFormat terminal_;
    QTextCharFormat string_;
    QTextCharFormat arrow_;
    QTextCharFormat punctuation_;
    QTextCharFormat comment_;
};

}  // namespace birch
