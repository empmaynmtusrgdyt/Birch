#pragma once

#include <QIcon>
#include <QList>
#include <QRegularExpression>
#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QToolButton;

namespace birch {

class CodeEditor;

// Панель поиска и замены под редактором. Находит все совпадения в тексте,
// подсвечивает их в редакторе и переходит между ними; заменяет по одному или
// все сразу (одним действием отмены).
class FindBar : public QWidget {
    Q_OBJECT
public:
    explicit FindBar(CodeEditor* editor, QWidget* parent = nullptr);

    // Открыть панель: только поиск или поиск с заменой. Выделенный в редакторе
    // текст (в пределах строки) подставляется в поле поиска.
    void openFind();
    void openReplace();
    // Скрыть панель, убрать подсветку и вернуть фокус редактору.
    void close();
    bool isActive() const { return active_; }

    void setPattern(const QString& pattern);
    void setReplacement(const QString& replacement);
    void setCaseSensitive(bool on);
    void setRegularExpression(bool on);

    // Перейти к следующему/предыдущему совпадению (по кругу).
    void findNext();
    void findPrevious();
    // Заменить выделенное совпадение и перейти к следующему. Если выделено не
    // совпадение — только перейти к нему. true, если замена была.
    bool replaceCurrent();
    // Заменить все совпадения; возвращает их число.
    int replaceAll();

    int matchCount() const { return static_cast<int>(matches_.size()); }
    int currentMatch() const { return current_; }  // с 0; -1 — курсор не на совпадении
    bool hasPatternError() const { return !patternError_.isEmpty(); }
    QString statusText() const;

    // Иконки кнопок перекрашиваются вместе с темой окна.
    void setIcons(const QIcon& previous, const QIcon& next, const QIcon& close);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void changeEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    struct Match {
        int start = 0;
        int length = 0;
        QString replacement;  // заполняется только для «Заменить все»
    };

    void open(bool withReplace);
    void onSearchChanged();
    QList<Match> collectMatches(bool withReplacements);
    void updateMatches();
    void updateCurrentFromCursor();
    void selectMatch(int index);
    QString expandReplacement(const QRegularExpressionMatch& match) const;
    void showStatus();
    void applyFieldPalette();

    CodeEditor* editor_;
    QLineEdit* find_ = nullptr;
    QLineEdit* replace_ = nullptr;
    QLabel* status_ = nullptr;
    QToolButton* previous_ = nullptr;
    QToolButton* next_ = nullptr;
    QToolButton* caseSensitive_ = nullptr;
    QToolButton* regex_ = nullptr;
    QToolButton* close_ = nullptr;
    QWidget* replaceButtons_ = nullptr;
    QPushButton* replaceOne_ = nullptr;
    QPushButton* replaceAll_ = nullptr;

    QRegularExpression re_;
    QString patternError_;
    QList<std::pair<int, int>> matches_;  // начало и длина
    int current_ = -1;
    int origin_ = 0;          // откуда искать при наборе в поле поиска
    int replacedCount_ = -1;  // для строки состояния после «Заменить все»
    bool active_ = false;
    bool replacing_ = false;  // идёт «Заменить все»: не пересчитывать на каждом шаге
};

}  // namespace birch
