/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// Тесты размера шрифта в редакторе: сочетания клавиш, колесо, границы и
// независимость от масштаба схемы (сочетания не уходят окну).

#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QSignalSpy>
#include <QTest>
#include <QWheelEvent>

#include "ui/CodeEditor.h"

using birch::CodeEditor;

class TestEditor : public QObject {
    Q_OBJECT

private slots:
    void defaultSize() {
        CodeEditor editor;
        QCOMPARE(editor.fontPointSize(), CodeEditor::defaultFontPointSize());
        QVERIFY(editor.hasDefaultFontSize());
    }

    void keysChangeSize() {
        CodeEditor editor;
        const qreal base = editor.fontPointSize();
        QSignalSpy spy(&editor, &CodeEditor::fontPointSizeChanged);
        QTest::keyClick(&editor, Qt::Key_Equal, Qt::ControlModifier);
        QCOMPARE(editor.fontPointSize(), base + 1);
        QTest::keyClick(&editor, Qt::Key_Plus, Qt::ControlModifier | Qt::ShiftModifier);
        QCOMPARE(editor.fontPointSize(), base + 2);
        QTest::keyClick(&editor, Qt::Key_Minus, Qt::ControlModifier);
        QCOMPARE(editor.fontPointSize(), base + 1);
        QVERIFY(!editor.hasDefaultFontSize());
        QTest::keyClick(&editor, Qt::Key_0, Qt::ControlModifier);
        QCOMPARE(editor.fontPointSize(), base);
        QVERIFY(editor.hasDefaultFontSize());
        QCOMPARE(spy.count(), 4);
        QVERIFY(editor.toPlainText().isEmpty());  // в текст ничего не попало
    }

    void limits() {
        CodeEditor editor;
        editor.setFontPointSize(100);
        QCOMPARE(editor.fontPointSize(), CodeEditor::kMaxFontPointSize);
        editor.increaseFontSize();
        QCOMPARE(editor.fontPointSize(), CodeEditor::kMaxFontPointSize);
        editor.setFontPointSize(1);
        QCOMPARE(editor.fontPointSize(), CodeEditor::kMinFontPointSize);
        editor.decreaseFontSize();
        QCOMPARE(editor.fontPointSize(), CodeEditor::kMinFontPointSize);
        editor.setFontPointSize(0);
        QVERIFY(editor.hasDefaultFontSize());
    }

    // Номера строк — тем же шрифтом: шире при крупном шрифте.
    void lineNumbersFollowFont() {
        CodeEditor editor;
        const int before = editor.lineNumberAreaWidth();
        editor.setFontPointSize(30);
        QVERIFY(editor.lineNumberAreaWidth() > before);
    }

    // Колесо с ⌘/Ctrl: шаг 120 — один пункт, мелкие шаги трекпада копятся.
    void wheel() {
        CodeEditor editor;
        const qreal base = editor.fontPointSize();
        const auto send = [&editor](int delta) {
            QWheelEvent e(QPointF(10, 10), QPointF(10, 10), QPoint(), QPoint(0, delta), Qt::NoButton,
                          Qt::ControlModifier, Qt::NoScrollPhase, false);
            QCoreApplication::sendEvent(editor.viewport(), &e);
        };
        send(120);
        QCOMPARE(editor.fontPointSize(), base + 1);
        send(40);
        send(40);
        QCOMPARE(editor.fontPointSize(), base + 1);
        send(40);
        QCOMPARE(editor.fontPointSize(), base + 2);
        send(-240);
        QCOMPARE(editor.fontPointSize(), base);
    }

    // Комментирование строк: текущая строка, выделение, снятие, отступы, отмена.
    static void select(CodeEditor& editor, int from, int to) {
        QTextCursor c = editor.textCursor();
        c.setPosition(from);
        c.setPosition(to, QTextCursor::KeepAnchor);
        editor.setTextCursor(c);
    }

    void commentCurrentLine() {
        CodeEditor editor;
        editor.setPlainText("[A] -> [B]\n[B] -> [C]");
        QTextCursor c = editor.textCursor();
        c.setPosition(12);  // во второй строке
        editor.setTextCursor(c);
        editor.toggleComment();
        QCOMPARE(editor.toPlainText(), QStringLiteral("[A] -> [B]\n// [B] -> [C]"));
        editor.toggleComment();
        QCOMPARE(editor.toPlainText(), QStringLiteral("[A] -> [B]\n[B] -> [C]"));
    }

    void commentSelectionKeepsIndentAndSkipsEmpty() {
        CodeEditor editor;
        const QString text = "=Рамка=\n    [A] -> [B]\n\n  [B] -> [C]\n[C] -> [D]";
        editor.setPlainText(text);
        // От середины второй строки до начала последней: последняя не захвачена
        select(editor, 10, static_cast<int>(text.lastIndexOf("[C] -> [D]")));
        editor.toggleComment();
        QCOMPARE(editor.toPlainText(), QStringLiteral("=Рамка=\n  //   [A] -> [B]\n\n  // [B] -> [C]\n[C] -> [D]"));
        editor.document()->undo();  // один шаг отмены
        QCOMPARE(editor.toPlainText(), text);
    }

    void uncommentBothStyles() {
        CodeEditor editor;
        editor.setPlainText("// [A] -> [B]\n#[B] -> [C]\n  //[C] -> [D]");
        editor.selectAll();
        editor.toggleComment();
        QCOMPARE(editor.toPlainText(), QStringLiteral("[A] -> [B]\n[B] -> [C]\n  [C] -> [D]"));
    }

    void mixedSelectionGetsCommented() {
        CodeEditor editor;
        editor.setPlainText("// [A] -> [B]\n[B] -> [C]");
        editor.selectAll();
        editor.toggleComment();
        QCOMPARE(editor.toPlainText(), QStringLiteral("// // [A] -> [B]\n// [B] -> [C]"));
    }

    void emptyTextUntouched() {
        CodeEditor editor;
        editor.setPlainText("\n\n");
        editor.selectAll();
        editor.toggleComment();
        QCOMPARE(editor.toPlainText(), QStringLiteral("\n\n"));
    }

    // Когда фокус в редакторе, сочетание масштаба не срабатывает в окне
    // (там оно масштабирует схему), а когда фокус не в редакторе — срабатывает.
    void shortcutsGoToEditorOnlyWithFocus() {
        QMainWindow window;
        auto* editor = new CodeEditor;
        window.setCentralWidget(editor);
        int windowZooms = 0;
        auto* zoomIn = new QAction(&window);
        zoomIn->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Equal));
        window.addAction(zoomIn);
        connect(zoomIn, &QAction::triggered, this, [&windowZooms] { ++windowZooms; });
        window.show();
        QVERIFY(QTest::qWaitForWindowActive(&window));
        editor->setFocus();
        QTRY_VERIFY(editor->hasFocus());

        // Нажатие получает виджет с фокусом, как при настоящем вводе.
        const qreal base = editor->fontPointSize();
        QTest::keyClick(QApplication::focusWidget(), Qt::Key_Equal, Qt::ControlModifier);
        QCOMPARE(editor->fontPointSize(), base + 1);
        QCOMPARE(windowZooms, 0);

        editor->clearFocus();
        QTest::keyClick(&window, Qt::Key_Equal, Qt::ControlModifier);
        QCOMPARE(windowZooms, 1);
        QCOMPARE(editor->fontPointSize(), base + 1);
    }
};

QTEST_MAIN(TestEditor)
#include "tst_editor.moc"
