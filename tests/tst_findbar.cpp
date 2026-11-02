// Тесты панели поиска и замены: счётчик совпадений, переход по кругу,
// замена по одной и всех сразу, отмена, регулярные выражения.

#include <QLineEdit>
#include <QTest>

#include "ui/CodeEditor.h"
#include "ui/FindBar.h"

using namespace birch;

namespace {

// Редактор с панелью поиска; панель уже открыта.
struct Fixture {
    CodeEditor editor;
    FindBar bar{&editor};

    explicit Fixture(const QString& text) {
        editor.setPlainText(text);
        editor.moveCursor(QTextCursor::Start);
        bar.openFind();
    }

    QString selected() const { return editor.textCursor().selectedText(); }
    int selectionStart() const { return editor.textCursor().selectionStart(); }
};

}  // namespace

class TestFindBar : public QObject {
    Q_OBJECT

private slots:
    // --- поиск ----------------------------------------------------------------

    void countsMatches() {
        Fixture f("[Шаг] -> [шаг] -> [ШАГ]");
        f.bar.setPattern("шаг");
        QCOMPARE(f.bar.matchCount(), 3);  // без учёта регистра, в том числе кириллица
        f.bar.setCaseSensitive(true);
        QCOMPARE(f.bar.matchCount(), 1);
    }

    void typingSelectsFirstMatch() {
        Fixture f("[A] -> [B]\n[B] -> [C]");
        f.bar.setPattern("[B]");  // обычный текст: скобки не спецсимволы
        QCOMPARE(f.bar.matchCount(), 2);
        QCOMPARE(f.bar.currentMatch(), 0);
        QCOMPARE(f.selected(), QStringLiteral("[B]"));
        QCOMPARE(f.bar.statusText(), QStringLiteral("1 из 2"));
    }

    void typingSearchesFromCursor() {
        Fixture f("x x x");
        f.bar.close();
        QTextCursor c = f.editor.textCursor();
        c.setPosition(2);  // перед вторым «x»
        f.editor.setTextCursor(c);
        f.bar.openFind();
        f.bar.setPattern("x");
        QCOMPARE(f.bar.currentMatch(), 1);  // ближайшее от курсора, а не первое в тексте
    }

    void nextWrapsAround() {
        Fixture f("a a a");
        f.bar.setPattern("a");
        QCOMPARE(f.bar.currentMatch(), 0);
        f.bar.findNext();
        QCOMPARE(f.bar.currentMatch(), 1);
        f.bar.findNext();
        QCOMPARE(f.bar.currentMatch(), 2);
        f.bar.findNext();
        QCOMPARE(f.bar.currentMatch(), 0);  // после последнего — снова первое
    }

    void previousWrapsAround() {
        Fixture f("a a a");
        f.bar.setPattern("a");
        QCOMPARE(f.bar.currentMatch(), 0);
        f.bar.findPrevious();
        QCOMPARE(f.bar.currentMatch(), 2);  // перед первым — последнее
        f.bar.findPrevious();
        QCOMPARE(f.bar.currentMatch(), 1);
    }

    void cursorElsewhereShowsTotal() {
        Fixture f("a b a b a");
        f.bar.setPattern("a");
        QTextCursor c = f.editor.textCursor();
        c.setPosition(2);  // курсор у «b», не на совпадении
        f.editor.setTextCursor(c);
        QCOMPARE(f.bar.currentMatch(), -1);
        QCOMPARE(f.bar.statusText(), QStringLiteral("3 совпадения"));
        f.bar.findNext();  // следующее — после курсора
        QCOMPARE(f.selectionStart(), 4);
    }

    void pluralForms() {
        Fixture f(QString("x ").repeated(21));
        f.bar.setPattern("x");
        QTextCursor c = f.editor.textCursor();
        c.setPosition(1);
        f.editor.setTextCursor(c);
        QCOMPARE(f.bar.statusText(), QStringLiteral("21 совпадение"));
        f.bar.setPattern(" ");
        f.editor.setTextCursor(QTextCursor(f.editor.document()));
        QCOMPARE(f.bar.statusText(), QStringLiteral("21 совпадение"));
        f.editor.setPlainText(QString("x").repeated(11));
        f.bar.setPattern("x");
        f.editor.setTextCursor(QTextCursor(f.editor.document()));
        QCOMPARE(f.bar.statusText(), QStringLiteral("11 совпадений"));
    }

    void notFound() {
        Fixture f("[A] -> [B]");
        f.bar.setPattern("Z");
        QCOMPARE(f.bar.matchCount(), 0);
        QCOMPARE(f.bar.statusText(), QStringLiteral("Не найдено"));
        f.bar.findNext();  // ничего не происходит, но и не падает
        QCOMPARE(f.bar.currentMatch(), -1);

        // Поле поиска краснеет, а когда совпадения есть — снова обычное.
        auto* field = f.bar.findChild<QLineEdit*>();
        QVERIFY(field);
        const QColor normal = f.bar.palette().color(QPalette::Base);
        QVERIFY(field->palette().color(QPalette::Base) != normal);
        f.bar.setPattern("A");
        QCOMPARE(field->palette().color(QPalette::Base), normal);
    }

    void recountsWhenTextChanges() {
        Fixture f("a a");
        f.bar.setPattern("a");
        QCOMPARE(f.bar.matchCount(), 2);
        QTextCursor c(f.editor.document());
        c.movePosition(QTextCursor::End);
        c.insertText(" a");
        QCOMPARE(f.bar.matchCount(), 3);
    }

    void prefillsFromSelection() {
        CodeEditor editor;
        FindBar bar(&editor);
        editor.setPlainText("[Ввод данных] -> [Готово]");
        QTextCursor c(editor.document());
        c.setPosition(1);
        c.setPosition(12, QTextCursor::KeepAnchor);
        editor.setTextCursor(c);
        bar.openFind();
        QCOMPARE(bar.matchCount(), 1);
        QCOMPARE(bar.currentMatch(), 0);  // выделенное — уже текущее совпадение
    }

    void closeClearsMatches() {
        Fixture f("a a");
        f.bar.setPattern("a");
        f.bar.close();
        QVERIFY(!f.bar.isActive());
        QCOMPARE(f.bar.matchCount(), 0);
    }

    // --- регулярные выражения ---------------------------------------------------

    void regex() {
        Fixture f("[Шаг 1] -> [Шаг 22] -> [Конец]");
        f.bar.setRegularExpression(true);
        f.bar.setPattern("Шаг \\d+");
        QCOMPARE(f.bar.matchCount(), 2);
        f.bar.setPattern("^\\[Шаг");  // «^» — начало строки
        QCOMPARE(f.bar.matchCount(), 1);
    }

    void invalidRegexIsReportedNotCrashing() {
        Fixture f("[A] -> [B]");
        f.bar.setRegularExpression(true);
        f.bar.setPattern("([A");
        QVERIFY(f.bar.hasPatternError());
        QCOMPARE(f.bar.matchCount(), 0);
        QCOMPARE(f.bar.statusText(), QStringLiteral("Ошибка"));
        f.bar.findNext();
        QCOMPARE(f.bar.replaceAll(), 0);
        QCOMPARE(f.editor.toPlainText(), QStringLiteral("[A] -> [B]"));
    }

    void emptyMatchesAreSkipped() {
        Fixture f("baa b");
        f.bar.setRegularExpression(true);
        f.bar.setPattern("a*");  // находит и пустые строки — их не считаем
        QCOMPARE(f.bar.matchCount(), 1);
        f.bar.setPattern("^");
        QCOMPARE(f.bar.matchCount(), 0);
    }

    // --- замена -------------------------------------------------------------------

    void replaceCurrentAndMoveOn() {
        Fixture f("[A] -> [B] -> [A]");
        f.bar.openReplace();
        f.bar.setPattern("[A]");
        f.bar.setReplacement("[Старт]");
        QVERIFY(f.bar.replaceCurrent());
        QCOMPARE(f.editor.toPlainText(), QStringLiteral("[Старт] -> [B] -> [A]"));
        QCOMPARE(f.bar.matchCount(), 1);
        QCOMPARE(f.selected(), QStringLiteral("[A]"));  // уже выбрано следующее
        QVERIFY(f.bar.replaceCurrent());
        QCOMPARE(f.editor.toPlainText(), QStringLiteral("[Старт] -> [B] -> [Старт]"));
        QCOMPARE(f.bar.matchCount(), 0);
    }

    void replaceWhenNotOnMatchOnlySelects() {
        Fixture f("x [A]");
        f.bar.openReplace();
        f.bar.setPattern("[A]");
        f.editor.moveCursor(QTextCursor::Start);  // ушли с совпадения
        f.bar.setReplacement("[B]");
        QVERIFY(!f.bar.replaceCurrent());
        QCOMPARE(f.editor.toPlainText(), QStringLiteral("x [A]"));
        QCOMPARE(f.selected(), QStringLiteral("[A]"));
    }

    void replaceAllIsOneUndoStep() {
        Fixture f("[Ввод данных] -> {Ок?}\n{Ок?} -> [Ввод данных]");
        f.bar.openReplace();
        f.bar.setPattern("Ввод данных");
        f.bar.setReplacement("Ввод информации");
        QCOMPARE(f.bar.replaceAll(), 2);
        QCOMPARE(f.editor.toPlainText(), QStringLiteral("[Ввод информации] -> {Ок?}\n{Ок?} -> [Ввод информации]"));
        QCOMPARE(f.bar.statusText(), QStringLiteral("Заменено: 2"));
        f.editor.undo();  // одно нажатие — всё как было
        QCOMPARE(f.editor.toPlainText(), QStringLiteral("[Ввод данных] -> {Ок?}\n{Ок?} -> [Ввод данных]"));
        QCOMPARE(f.bar.matchCount(), 2);
    }

    void replacementContainingPattern() {
        Fixture f("a a");
        f.bar.openReplace();
        f.bar.setPattern("a");
        f.bar.setReplacement("aa");
        QCOMPARE(f.bar.replaceAll(), 2);  // без зацикливания
        QCOMPARE(f.editor.toPlainText(), QStringLiteral("aa aa"));
    }

    void plainReplacementIsLiteral() {
        Fixture f("a");
        f.bar.openReplace();
        f.bar.setPattern("a");
        f.bar.setReplacement("$1 \\n");
        f.bar.replaceAll();
        QCOMPARE(f.editor.toPlainText(), QStringLiteral("$1 \\n"));
    }

    void regexReplacementGroups() {
        Fixture f("[Иван Петров] -> [Анна Смирнова]");
        f.bar.openReplace();
        f.bar.setRegularExpression(true);
        f.bar.setPattern("(\\w+) (\\w+)");
        f.bar.setReplacement("$2 $1 ($$)");
        QCOMPARE(f.bar.replaceAll(), 2);
        QCOMPARE(f.editor.toPlainText(), QStringLiteral("[Петров Иван ($)] -> [Смирнова Анна ($)]"));
    }

    void regexReplacementCurrent() {
        Fixture f("[Шаг 1] -> [Шаг 2]");
        f.bar.openReplace();
        f.bar.setRegularExpression(true);
        f.bar.setPattern("Шаг (\\d)");
        f.bar.setReplacement("Этап $1\\nготов");
        QVERIFY(f.bar.replaceCurrent());
        QCOMPARE(f.editor.toPlainText(), QStringLiteral("[Этап 1\nготов] -> [Шаг 2]"));
    }

    void searchHighlightsKeepDiagnostics() {
        // Подсветка поиска и подчёркивание ошибок живут вместе.
        Fixture f("[A] -> [B");
        f.editor.setDiagnostics({{Diagnostic::Severity::Error, 1, 8, 7, 2, "Не закрыта скобка"}});
        f.bar.setPattern("A");
        const auto selections = f.editor.extraSelections();
        bool hasUnderline = false;
        bool hasMatch = false;
        for (const auto& s : selections) {
            if (s.format.underlineStyle() == QTextCharFormat::WaveUnderline) hasUnderline = true;
            if (s.cursor.selectedText() == QStringLiteral("A") && s.format.hasProperty(QTextFormat::BackgroundBrush))
                hasMatch = true;
        }
        QVERIFY(hasUnderline);
        QVERIFY(hasMatch);
    }
};

QTEST_MAIN(TestFindBar)
#include "tst_findbar.moc"
