/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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
    QTextCharFormat frame_;  // =Рамка= и ==
};

}  // namespace birch
