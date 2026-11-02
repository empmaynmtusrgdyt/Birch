#include "ui/FindBar.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QTextBlock>
#include <QToolButton>

#include "ui/CodeEditor.h"

namespace birch {

namespace {

// Больше совпадений не ищем: на огромном тексте с «.» поиск иначе подвиснет.
constexpr int kMaxMatches = 10000;

// «1 совпадение», «2 совпадения», «5 совпадений»
QString matchesText(int n) {
    const int mod10 = n % 10;
    const int mod100 = n % 100;
    if (mod10 == 1 && mod100 != 11) return FindBar::tr("%1 совпадение").arg(n);
    if (mod10 >= 2 && mod10 <= 4 && (mod100 < 12 || mod100 > 14)) return FindBar::tr("%1 совпадения").arg(n);
    return FindBar::tr("%1 совпадений").arg(n);
}

QToolButton* toolButton(const QString& text, const QString& toolTip, QWidget* parent) {
    auto* b = new QToolButton(parent);
    b->setText(text);
    b->setToolTip(toolTip);
    b->setAutoRaise(true);
    b->setFocusPolicy(Qt::TabFocus);
    return b;
}

}  // namespace

FindBar::FindBar(CodeEditor* editor, QWidget* parent) : QWidget(parent), editor_(editor) {
    find_ = new QLineEdit(this);
    // Подписи «Найти:» и «Заменить:» — внутри полей: панель должна помещаться
    // и в узкую левую часть окна.
    find_->setPlaceholderText(tr("Найти"));
    find_->setAccessibleName(tr("Найти"));
    find_->setClearButtonEnabled(true);
    find_->setMinimumWidth(90);
    find_->installEventFilter(this);

    replace_ = new QLineEdit(this);
    replace_->setPlaceholderText(tr("Заменить на"));
    replace_->setAccessibleName(tr("Заменить на"));
    replace_->setClearButtonEnabled(true);
    replace_->setMinimumWidth(90);
    replace_->installEventFilter(this);

    status_ = new QLabel(this);
    // Ширина под «99 из 99», чтобы поле не прыгало при наборе; длинный текст раздвинет.
    status_->setMinimumWidth(status_->fontMetrics().horizontalAdvance(QStringLiteral("99 из 99")) + 6);

    previous_ = toolButton(QStringLiteral("↑"), tr("Предыдущее совпадение"), this);
    next_ = toolButton(QStringLiteral("↓"), tr("Следующее совпадение"), this);
    caseSensitive_ = toolButton(QStringLiteral("Aa"), tr("Учитывать регистр"), this);
    caseSensitive_->setCheckable(true);
    regex_ = toolButton(QStringLiteral(".*"), tr("Регулярное выражение. В замене $1, $2… — найденные группы"), this);
    regex_->setCheckable(true);
    close_ = toolButton(QStringLiteral("✕"), tr("Закрыть (Esc)"), this);

    replaceOne_ = new QPushButton(tr("Заменить"), this);
    replaceAll_ = new QPushButton(tr("Заменить все"), this);
    for (QPushButton* b : {replaceOne_, replaceAll_}) {
        b->setAutoDefault(false);
        b->setFocusPolicy(Qt::TabFocus);
    }
    replaceButtons_ = new QWidget(this);
    auto* buttons = new QHBoxLayout(replaceButtons_);
    buttons->setContentsMargins(0, 0, 0, 0);
    buttons->addWidget(replaceOne_);
    buttons->addWidget(replaceAll_);
    buttons->addStretch(1);

    auto* grid = new QGridLayout(this);
    grid->setContentsMargins(6, 6, 4, 6);
    grid->setHorizontalSpacing(6);
    grid->setVerticalSpacing(4);
    grid->addWidget(find_, 0, 0);
    auto* tools = new QHBoxLayout;
    tools->setSpacing(0);
    tools->addWidget(status_);
    for (QToolButton* b : {previous_, next_, caseSensitive_, regex_, close_}) tools->addWidget(b);
    grid->addLayout(tools, 0, 1);
    grid->addWidget(replace_, 1, 0);
    grid->addWidget(replaceButtons_, 1, 1);
    grid->setColumnStretch(0, 1);

    connect(find_, &QLineEdit::textChanged, this, &FindBar::onSearchChanged);
    connect(caseSensitive_, &QToolButton::toggled, this, &FindBar::onSearchChanged);
    connect(regex_, &QToolButton::toggled, this, &FindBar::onSearchChanged);
    connect(previous_, &QToolButton::clicked, this, &FindBar::findPrevious);
    connect(next_, &QToolButton::clicked, this, &FindBar::findNext);
    connect(close_, &QToolButton::clicked, this, &FindBar::close);
    connect(replaceOne_, &QPushButton::clicked, this, &FindBar::replaceCurrent);
    connect(replaceAll_, &QPushButton::clicked, this, &FindBar::replaceAll);

    // Текст изменился (набор, отмена, замена) — совпадения пересчитываются.
    // Изменения только оформления (подсветка синтаксиса) пропускаем.
    connect(editor_->document(), &QTextDocument::contentsChange, this, [this](int, int removed, int added) {
        if (active_ && !replacing_ && (removed || added)) updateMatches();
    });
    // Текущее совпадение — выделенное в редакторе. Выделение может сняться и без
    // сдвига курсора (щелчок в конце совпадения), поэтому следим за обоими.
    const auto syncCurrent = [this] {
        if (active_ && !replacing_) updateCurrentFromCursor();
    };
    connect(editor_, &QPlainTextEdit::cursorPositionChanged, this, syncCurrent);
    connect(editor_, &QPlainTextEdit::selectionChanged, this, syncCurrent);

    hide();
}

// --- открытие и закрытие --------------------------------------------------------

void FindBar::openFind() { open(false); }
void FindBar::openReplace() { open(true); }

void FindBar::open(bool withReplace) {
    const QTextCursor cursor = editor_->textCursor();
    origin_ = cursor.selectionStart();
    // Выделение в пределах одной строки — готовый текст для поиска.
    const QString selected = cursor.selectedText();
    const bool prefill = !selected.isEmpty() && !selected.contains(QChar::ParagraphSeparator);

    active_ = true;
    replace_->setVisible(withReplace);
    replaceButtons_->setVisible(withReplace);
    show();

    if (prefill) {
        const QSignalBlocker block(find_);
        find_->setText(regex_->isChecked() ? QRegularExpression::escape(selected) : selected);
    }
    updateMatches();
    // В режиме замены с уже заполненным поиском удобнее сразу вводить замену.
    QLineEdit* field = withReplace && !find_->text().isEmpty() && prefill ? replace_ : find_;
    field->setFocus();
    field->selectAll();
}

void FindBar::close() {
    active_ = false;
    matches_.clear();
    current_ = -1;
    editor_->setSearchMatches({}, -1);
    hide();
    editor_->setFocus();
}

// --- параметры поиска -------------------------------------------------------------

void FindBar::setPattern(const QString& pattern) { find_->setText(pattern); }
void FindBar::setReplacement(const QString& replacement) { replace_->setText(replacement); }
void FindBar::setCaseSensitive(bool on) { caseSensitive_->setChecked(on); }
void FindBar::setRegularExpression(bool on) { regex_->setChecked(on); }

void FindBar::onSearchChanged() {
    if (!active_) return;
    updateMatches();
    if (matches_.isEmpty()) return;
    // Поиск при наборе — от места, где он начался, а не от найденного раньше.
    int index = 0;
    for (int i = 0; i < matches_.size(); ++i) {
        if (matches_[i].first >= origin_) {
            index = i;
            break;
        }
    }
    selectMatch(index);
}

// --- совпадения -------------------------------------------------------------------

QList<FindBar::Match> FindBar::collectMatches(bool withReplacements) {
    QList<Match> result;
    patternError_.clear();
    const QString pattern = find_->text();
    if (pattern.isEmpty()) return result;

    // MultilineOption: «^» и «$» — начало и конец строки, а не всего текста.
    QRegularExpression::PatternOptions options =
        QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::MultilineOption;
    if (!caseSensitive_->isChecked()) options |= QRegularExpression::CaseInsensitiveOption;
    re_ = QRegularExpression(regex_->isChecked() ? pattern : QRegularExpression::escape(pattern), options);
    if (!re_.isValid()) {
        patternError_ = re_.errorString();
        return result;
    }

    // Позиции в toPlainText() совпадают с позициями документа: конец строки —
    // один символ «\n», как и разделитель блоков.
    const QString text = editor_->toPlainText();
    QRegularExpressionMatchIterator it = re_.globalMatch(text);
    while (it.hasNext() && result.size() < kMaxMatches) {
        const QRegularExpressionMatch m = it.next();
        if (m.capturedLength() == 0) continue;  // «^», «a*» и т. п. — пустое совпадение не выделить
        Match match{static_cast<int>(m.capturedStart()), static_cast<int>(m.capturedLength()), {}};
        if (withReplacements) match.replacement = expandReplacement(m);
        result.append(match);
    }
    return result;
}

void FindBar::updateMatches() {
    replacedCount_ = -1;
    matches_.clear();
    if (active_) {
        for (const Match& m : collectMatches(false)) matches_.append({m.start, m.length});
    }
    updateCurrentFromCursor();
}

void FindBar::updateCurrentFromCursor() {
    // Текущее совпадение — то, что выделено в редакторе (после перехода к нему).
    const QTextCursor cursor = editor_->textCursor();
    current_ = -1;
    for (int i = 0; i < matches_.size(); ++i) {
        const auto [start, length] = matches_[i];
        if (start == cursor.selectionStart() && start + length == cursor.selectionEnd()) {
            current_ = i;
            break;
        }
    }
    editor_->setSearchMatches(matches_, current_);
    showStatus();
}

void FindBar::selectMatch(int index) {
    const auto [start, length] = matches_[index];
    QTextCursor cursor(editor_->document());
    cursor.setPosition(start);
    cursor.setPosition(start + length, QTextCursor::KeepAnchor);
    editor_->setTextCursor(cursor);  // прокручивает к совпадению; текущее обновится по сигналу курсора
    editor_->ensureCursorVisible();
    updateCurrentFromCursor();
}

// --- переходы ---------------------------------------------------------------------

void FindBar::findNext() {
    if (!active_) open(false);
    replacedCount_ = -1;
    if (matches_.isEmpty()) {
        showStatus();
        return;
    }
    // Следующее после курсора: так переход работает и после щелчка в тексте.
    const int from = editor_->textCursor().selectionEnd();
    int index = 0;
    for (int i = 0; i < matches_.size(); ++i) {
        if (matches_[i].first >= from) {
            index = i;
            break;
        }
    }
    selectMatch(index);
    origin_ = matches_[index].first;
}

void FindBar::findPrevious() {
    if (!active_) open(false);
    replacedCount_ = -1;
    if (matches_.isEmpty()) {
        showStatus();
        return;
    }
    const int from = editor_->textCursor().selectionStart();
    int index = static_cast<int>(matches_.size()) - 1;
    for (int i = index; i >= 0; --i) {
        if (matches_[i].first < from) {
            index = i;
            break;
        }
    }
    selectMatch(index);
    origin_ = matches_[index].first;
}

// --- замена -----------------------------------------------------------------------

QString FindBar::expandReplacement(const QRegularExpressionMatch& match) const {
    const QString pattern = replace_->text();
    if (!regex_->isChecked()) return pattern;

    // $1…$99 — группы, $0 — всё совпадение, $$ — сам «$»; \n и \t — перенос и табуляция.
    QString out;
    out.reserve(pattern.size());
    for (qsizetype i = 0; i < pattern.size(); ++i) {
        const QChar c = pattern[i];
        const QChar next = i + 1 < pattern.size() ? pattern[i + 1] : QChar();
        if (c == u'$' && next == u'$') {
            out += u'$';
            ++i;
        } else if (c == u'$' && next.isDigit()) {
            int group = next.digitValue();
            ++i;
            // Двузначный номер — только если такая группа есть: «$10» при одной группе — это «$1» и «0».
            if (i + 1 < pattern.size() && pattern[i + 1].isDigit()) {
                const int twoDigits = group * 10 + pattern[i + 1].digitValue();
                if (twoDigits <= match.lastCapturedIndex()) {
                    group = twoDigits;
                    ++i;
                }
            }
            out += match.captured(group);
        } else if (c == u'\\' && next == u'n') {
            out += u'\n';
            ++i;
        } else if (c == u'\\' && next == u't') {
            out += u'\t';
            ++i;
        } else if (c == u'\\' && next == u'\\') {
            out += u'\\';
            ++i;
        } else {
            out += c;
        }
    }
    return out;
}

bool FindBar::replaceCurrent() {
    if (!active_) return false;
    replacedCount_ = -1;
    if (current_ < 0) {
        findNext();
        return false;
    }
    // Замену считаем заново от самого совпадения: для групп $1… нужен его разбор.
    const auto [start, length] = matches_[current_];
    const QRegularExpressionMatch m = re_.match(editor_->toPlainText(), start);
    if (!m.hasMatch() || m.capturedStart() != start || m.capturedLength() != length) {
        findNext();
        return false;
    }
    QTextCursor cursor = editor_->textCursor();
    cursor.insertText(expandReplacement(m));  // одно действие отмены
    editor_->setTextCursor(cursor);
    findNext();
    return true;
}

int FindBar::replaceAll() {
    if (!active_) return 0;
    const QList<Match> all = collectMatches(true);
    if (all.isEmpty()) {
        updateMatches();
        return 0;
    }

    replacing_ = true;
    QTextCursor cursor(editor_->document());
    cursor.beginEditBlock();  // всё — одно действие отмены
    // С конца, чтобы позиции ещё не заменённых совпадений не сдвигались.
    for (auto it = all.crbegin(); it != all.crend(); ++it) {
        cursor.setPosition(it->start);
        cursor.setPosition(it->start + it->length, QTextCursor::KeepAnchor);
        cursor.insertText(it->replacement);
    }
    cursor.endEditBlock();
    replacing_ = false;

    updateMatches();
    replacedCount_ = static_cast<int>(all.size());
    showStatus();
    return replacedCount_;
}

// --- вид ----------------------------------------------------------------------------

QString FindBar::statusText() const {
    if (replacedCount_ >= 0) return tr("Заменено: %1").arg(replacedCount_);
    if (find_->text().isEmpty()) return {};
    if (hasPatternError()) return tr("Ошибка");  // подробности — в подсказке
    if (matches_.isEmpty()) return tr("Не найдено");
    if (current_ >= 0) return tr("%1 из %2").arg(current_ + 1).arg(matches_.size());
    return matchesText(static_cast<int>(matches_.size()));
}

void FindBar::showStatus() {
    status_->setText(statusText());
    const QString error = hasPatternError() ? tr("Ошибка в регулярном выражении: %1").arg(patternError_) : QString();
    status_->setToolTip(error);
    find_->setToolTip(error);
    const bool canReplace = !matches_.isEmpty();
    replaceOne_->setEnabled(canReplace);
    replaceAll_->setEnabled(canReplace);
    previous_->setEnabled(canReplace);
    next_->setEnabled(canReplace);
    applyFieldPalette();
}

void FindBar::applyFieldPalette() {
    // Ничего не найдено или ошибка в выражении — поле поиска слегка краснеет.
    const bool bad = !find_->text().isEmpty() && replacedCount_ < 0 && (hasPatternError() || matches_.isEmpty());
    if (!bad) {
        find_->setPalette(QPalette());  // снова как у окна
        return;
    }
    QPalette p = palette();
    const QColor base = p.color(QPalette::Base);
    const bool dark = base.lightness() < 128;
    const QColor red = dark ? QColor(0xFF6B6B) : QColor(0xD92D20);
    const qreal k = dark ? 0.25 : 0.12;
    p.setColor(QPalette::Base, QColor::fromRgbF(static_cast<float>(base.redF() * (1 - k) + red.redF() * k),
                                                static_cast<float>(base.greenF() * (1 - k) + red.greenF() * k),
                                                static_cast<float>(base.blueF() * (1 - k) + red.blueF() * k)));
    find_->setPalette(p);
}

void FindBar::setIcons(const QIcon& previous, const QIcon& next, const QIcon& close) {
    const std::pair<QToolButton*, const QIcon*> buttons[] = {{previous_, &previous}, {next_, &next}, {close_, &close}};
    for (const auto& [button, icon] : buttons) {
        button->setIcon(*icon);
        button->setToolButtonStyle(icon->isNull() ? Qt::ToolButtonTextOnly : Qt::ToolButtonIconOnly);
    }
}

// --- события ----------------------------------------------------------------------

bool FindBar::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        const auto* key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter) {
            if (watched == replace_) {
                replaceCurrent();
            } else if (key->modifiers() & Qt::ShiftModifier) {
                findPrevious();
            } else {
                findNext();
            }
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void FindBar::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        close();
        return;
    }
    QWidget::keyPressEvent(event);
}

void FindBar::changeEvent(QEvent* event) {
    QWidget::changeEvent(event);
    if (event->type() == QEvent::PaletteChange) applyFieldPalette();
}

void FindBar::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    // Тонкая линия сверху отделяет панель от текста редактора.
    QPainter p(this);
    p.setPen(palette().color(QPalette::Mid));
    p.drawLine(0, 0, width(), 0);
}

}  // namespace birch
