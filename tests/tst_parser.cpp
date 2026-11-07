/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <QRandomGenerator>
#include <QtTest>

#include "core/Parser.h"

using namespace birch;

namespace {

ParseResult parse(const QString& s) { return Parser{}.parse(s); }

QStringList nodeLabels(const Diagram& d) {
    QStringList out;
    for (const auto& n : d.nodes) out << n.label;
    return out;
}

int errorCount(const ParseResult& r) {
    int n = 0;
    for (const auto& d : r.diagnostics) n += d.isError() ? 1 : 0;
    return n;
}

}  // namespace

class TestParser : public QObject {
    Q_OBJECT

private slots:
    // --- корректный ввод ---------------------------------------------------

    void exampleFromSpec() {
        const auto r = parse(QStringLiteral("[Старт] -> [Проверка условия];\n"
                                            "[Проверка условия] -> [Успех] \"Да\";\n"
                                            "[Проверка условия] -> [Ошибка] \"Нет\";\n"));
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(nodeLabels(r.diagram),
                 (QStringList{"Старт", "Проверка условия", "Успех", "Ошибка"}));
        QCOMPARE(r.diagram.edges.size(), 3);
        QCOMPARE(r.diagram.edges[0].label, QString());
        QCOMPARE(r.diagram.edges[1].from, QStringLiteral("Проверка условия"));
        QCOMPARE(r.diagram.edges[1].to, QStringLiteral("Успех"));
        QCOMPARE(r.diagram.edges[1].label, QStringLiteral("Да"));
        QCOMPARE(r.diagram.edges[2].label, QStringLiteral("Нет"));
    }

    void emptyAndCommentsOnly() {
        QVERIFY(parse("").diagram.isEmpty());
        const auto r = parse("// комментарий\n# ещё один\n\n  ;;\n");
        QVERIFY(r.diagnostics.isEmpty());
        QVERIFY(r.diagram.isEmpty());
    }

    void chainWithLabels() {
        const auto r = parse("[A] -> [B] \"x\" -> [C] -> [D] \"y\"");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(r.diagram.edges.size(), 3);
        QCOMPARE(r.diagram.edges[0].label, QStringLiteral("x"));
        QCOMPARE(r.diagram.edges[1].label, QString());
        QCOMPARE(r.diagram.edges[2].label, QStringLiteral("y"));
    }

    void standaloneNode() {
        const auto r = parse("[Одинокий]");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(r.diagram.nodes.size(), 1);
        QCOMPARE(r.diagram.edges.size(), 0);
    }

    void shapes() {
        const auto r = parse("(Старт) -> {Ок?} -> [Шаг]");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(r.diagram.nodes[0].shape, NodeShape::Terminal);
        QCOMPARE(r.diagram.nodes[1].shape, NodeShape::Decision);
        QCOMPARE(r.diagram.nodes[2].shape, NodeShape::Process);
    }

    void shapeUpgradeFromProcess() {
        const auto r = parse("[X] -> [Y]\n{X}");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(r.diagram.nodes.size(), 2);
        QCOMPARE(r.diagram.nodes[0].shape, NodeShape::Decision);
    }

    void shapeConflictWarns() {
        const auto r = parse("{X}\n(X)");
        QCOMPARE(r.diagnostics.size(), 1);
        QVERIFY(!r.diagnostics[0].isError());
        QVERIFY(!r.hasErrors());
        QCOMPARE(r.diagram.nodes[0].shape, NodeShape::Decision);
        QCOMPARE(r.diagnostics[0].line, 2);
    }

    void sameTextIsSameNode() {
        const auto r = parse("[Проверка условия] -> [A]\n[Проверка   условия] -> [B]");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(r.diagram.nodes.size(), 3);
    }

    // Подчёркивание — обычный символ, а не замена пробела
    void underscoreIsLiteral() {
        const auto r = parse("[snake_case] -> {is_valid?} \"x_y\" -> [snake case]");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(nodeLabels(r.diagram), (QStringList{"snake_case", "is_valid?", "snake case"}));
        QCOMPARE(r.diagram.edges[0].label, QStringLiteral("x_y"));
    }

    void newlineTerminatesWithoutSemicolon() {
        const auto r = parse("[A] -> [B]\n[B] -> [C]\r\n[C] -> [A]");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(r.diagram.edges.size(), 3);
    }

    void multiLineChain() {
        const auto r = parse("[A]\n  -> [B] \"да\"\n  ->\n  [C];");
        QVERIFY2(r.diagnostics.isEmpty(), qPrintable(r.diagnostics.value(0).message));
        QCOMPARE(r.diagram.edges.size(), 2);
        QCOMPARE(r.diagram.edges[0].label, QStringLiteral("да"));
    }

    void escapes() {
        const auto r = parse("[a\\]b] -> [snake\\_case] -> [Две\\nстроки] \"кав\\\"ычки\"");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(nodeLabels(r.diagram),
                 (QStringList{"a]b", "snake_case", "Две\nстроки"}));
        QCOMPARE(r.diagram.edges[1].label, QStringLiteral("кав\"ычки"));
    }

    void unicodeArrowAndQuotes() {
        const auto r = parse("[A] → [B] «Да» → [C] “Нет”");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(r.diagram.edges.size(), 2);
        QCOMPARE(r.diagram.edges[0].label, QStringLiteral("Да"));
        QCOMPARE(r.diagram.edges[1].label, QStringLiteral("Нет"));
    }

    void bracketsInsideOtherBrackets() {
        const auto r = parse("{f(x) > 0?} -> [arr\\[i]");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(nodeLabels(r.diagram), (QStringList{"f(x) > 0?", "arr[i"}));
    }

    // Примеры на всех языках — без ошибок и одинаковые по устройству
    void bundledExamplesAreValid_data() {
        QTest::addColumn<QString>("file");
        QTest::addColumn<int>("nodes");
        QTest::addColumn<int>("edges");
        QTest::addColumn<int>("groups");
        QTest::newRow("welcome") << "welcome.birch" << 6 << 6 << 0;
        QTest::newRow("welcome.en") << "welcome.en.birch" << 6 << 6 << 0;
        QTest::newRow("frames") << "frames.birch" << 9 << 10 << 2;
        QTest::newRow("frames.en") << "frames.en.birch" << 9 << 10 << 2;
    }
    void bundledExamplesAreValid() {
        QFETCH(QString, file);
        QFETCH(int, nodes);
        QFETCH(int, edges);
        QFETCH(int, groups);
        QFile f(QStringLiteral(BIRCH_EXAMPLES_DIR "/") + file);
        QVERIFY(f.open(QIODevice::ReadOnly));
        const auto r = parse(QString::fromUtf8(f.readAll()));
        QVERIFY2(r.diagnostics.isEmpty(), qPrintable(r.diagnostics.value(0).message));
        QCOMPARE(r.diagram.nodes.size(), nodes);
        QCOMPARE(r.diagram.edges.size(), edges);
        QCOMPARE(r.diagram.groups.size(), groups);
    }

    // Стартовый текст программы — тот же пример, что в начале README на этом языке.
    void welcomeMatchesReadme_data() {
        QTest::addColumn<QString>("readmePath");
        QTest::addColumn<QString>("welcomeFile");
        QTest::newRow("ru") << QStringLiteral(BIRCH_README) << "welcome.birch";
        QTest::newRow("en") << QStringLiteral(BIRCH_README_EN) << "welcome.en.birch";
    }
    void welcomeMatchesReadme() {
        QFETCH(QString, readmePath);
        QFETCH(QString, welcomeFile);
        QFile readme(readmePath);
        QFile welcome(QStringLiteral(BIRCH_EXAMPLES_DIR "/") + welcomeFile);
        QVERIFY(readme.open(QIODevice::ReadOnly | QIODevice::Text));
        QVERIFY(welcome.open(QIODevice::ReadOnly | QIODevice::Text));
        const QString text = QString::fromUtf8(readme.readAll());
        const qsizetype start = text.indexOf(QStringLiteral("```text\n"));
        QVERIFY(start >= 0);
        const qsizetype from = start + 8;
        const QString example = text.mid(from, text.indexOf(QStringLiteral("```"), from) - from);
        QCOMPARE(QString::fromUtf8(welcome.readAll()), example);
    }

    // --- рамки -----------------------------------------------------------------

    void framesAreOptional() {
        const auto r = parse("[A] -> [B]\n[B] -> [A]");
        QVERIFY(r.diagnostics.isEmpty());
        QVERIFY(r.diagram.groups.isEmpty());
        for (const auto& n : r.diagram.nodes) QCOMPARE(n.group, -1);
    }

    void sameTextInTwoFramesIsTwoNodes() {
        const auto r = parse("=Авторизация=\n[Ввод] -> {Проверка}\n"
                             "=Загрузка=\n[Профиль] -> {Проверка}");
        QVERIFY2(r.diagnostics.isEmpty(), qPrintable(r.diagnostics.value(0).message));
        QCOMPARE(r.diagram.groups.size(), 2);
        QCOMPARE(r.diagram.groups[0].name, QStringLiteral("Авторизация"));
        QCOMPARE(r.diagram.groups[1].name, QStringLiteral("Загрузка"));
        QCOMPARE(nodeLabels(r.diagram), (QStringList{"Ввод", "Проверка", "Профиль", "Проверка"}));
        QCOMPARE(r.diagram.nodes[1].group, 0);
        QCOMPARE(r.diagram.nodes[3].group, 1);
        QVERIFY(r.diagram.nodes[1].id != r.diagram.nodes[3].id);
        QCOMPARE(r.diagram.edges[1].to, r.diagram.nodes[3].id);
    }

    void closingReturnsToGlobal() {
        const auto r = parse("=Рамка=\n[A]\n==\n[B]\n= =\n[C]");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(r.diagram.nodes[0].group, 0);
        QCOMPARE(r.diagram.nodes[1].group, -1);
        QCOMPARE(r.diagram.nodes[2].group, -1);
        QCOMPARE(r.diagram.nodes[1].id, QStringLiteral("B"));  // у общего блока ключ — текст, как раньше
    }

    void globalQualifierFromTwoFrames() {
        const auto r = parse("=А=\n{Ошибка?} -> == (Выход)\n=Б=\n{Ошибка?} -> ==(Выход)");
        QVERIFY2(r.diagnostics.isEmpty(), qPrintable(r.diagnostics.value(0).message));
        QCOMPARE(nodeLabels(r.diagram), (QStringList{"Ошибка?", "Выход", "Ошибка?"}));
        QCOMPARE(r.diagram.nodes[1].group, -1);  // создан ссылкой, вне рамок его нет
        QCOMPARE(r.diagram.nodes[1].shape, NodeShape::Terminal);
        QCOMPARE(r.diagram.edges[0].to, r.diagram.edges[1].to);
    }

    void globalNodeDoesNotMergeFrameNodes() {
        // Дописанный общий блок с тем же текстом не трогает блоки рамок.
        const auto r = parse("=А=\n[X] -> {Проверка}\n=Б=\n[Y] -> {Проверка}\n==\n[Z] -> {Проверка}");
        QVERIFY(errorCount(r) == 0);
        QCOMPARE(r.diagram.nodes.size(), 6);
    }

    void qualifiedNodeFromAnotherFrame() {
        const auto r = parse("=А=\n[Ввод] -> =Б= [Профиль] \"Вход\"\n=Б=\n[Профиль] -> [Готово]\n"
                             "==\n[Старт] -> =А= [Ввод]");
        QVERIFY2(r.diagnostics.isEmpty(), qPrintable(r.diagnostics.value(0).message));
        QCOMPARE(nodeLabels(r.diagram), (QStringList{"Ввод", "Профиль", "Готово", "Старт"}));
        QCOMPARE(r.diagram.nodes[1].group, 1);  // стрелка в рамку написана раньше самой рамки
        QCOMPARE(r.diagram.edges[0].label, QStringLiteral("Вход"));
        QCOMPARE(r.diagram.edges[2].to, r.diagram.nodes[0].id);
    }

    void identicalFramesConnected() {
        const auto r = parse("=Попытка 1=\n[Ввод] -> {Проверка}\n{Проверка} -> =Попытка 2= [Ввод] \"Ещё раз\"\n"
                             "=Попытка 2=\n[Ввод] -> {Проверка}");
        QVERIFY2(r.diagnostics.isEmpty(), qPrintable(r.diagnostics.value(0).message));
        QCOMPARE(r.diagram.nodes.size(), 4);
        QCOMPARE(r.diagram.edges.size(), 3);
        const Edge& again = r.diagram.edges[1];
        int toIndex = -1;
        for (int i = 0; i < r.diagram.nodes.size(); ++i)
            if (r.diagram.nodes[i].id == again.to) toIndex = i;
        QCOMPARE(r.diagram.nodes[toIndex].group, 1);
        QCOMPARE(r.diagram.nodes[toIndex].label, QStringLiteral("Ввод"));
    }

    void qualifierAppliesToNextNodeOnly() {
        const auto r = parse("=А=\n[X]\n=Б=\n[Y] -> =А= [X] -> [Z]");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(nodeLabels(r.diagram), (QStringList{"X", "Y", "Z"}));
        QCOMPARE(r.diagram.nodes[2].group, 1);  // Z — блок рамки, где написана строка
    }

    void ownFrameQualifierChangesNothing() {
        const auto r = parse("=А=\n[X] -> =А= [X]");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(r.diagram.nodes.size(), 1);
    }

    void unknownFrameQualifier() {
        // Попытка открыть рамку и сразу написать стрелку в одну строку.
        const auto r = parse("=Попытка 1= [Ввод] -> {Проверка}\n[A]");
        QCOMPARE(errorCount(r), 1);
        QVERIFY(r.diagnostics[0].message.contains(QStringLiteral("не найдена")));
        QVERIFY(r.diagnostics[0].message.contains(QStringLiteral("на отдельной строке")));
        QCOMPARE(r.diagnostics[0].line, 1);
        QCOMPARE(nodeLabels(r.diagram), (QStringList{"A"}));  // сломанная строка не применилась
    }

    void hintForgottenGlobalQualifier() {
        const auto r = parse("==\n(Выход)\n=А=\n[Ошибка] -> (Выход)");
        QCOMPARE(errorCount(r), 0);
        QCOMPARE(r.diagnostics.size(), 1);
        QCOMPARE(r.diagnostics[0].severity, Diagnostic::Severity::Warning);
        QVERIFY(r.diagnostics[0].message.contains(QStringLiteral("== (Выход)")));
        QCOMPARE(r.diagnostics[0].line, 4);
        // Если блок рамки упомянут не раз — он нужен сам по себе, подсказки нет.
        QVERIFY(parse("(Выход)\n=А=\n[Ошибка] -> (Выход)\n(Выход) -> [Ошибка]").diagnostics.isEmpty());
    }

    void shapeStaysInsideFrame() {
        const auto r = parse("=А=\n{Проверка}\n=Б=\n[Проверка] -> [X]");
        QCOMPARE(errorCount(r), 0);
        QCOMPARE(r.diagram.nodes[0].shape, NodeShape::Decision);
        QCOMPARE(r.diagram.nodes[1].shape, NodeShape::Process);
    }

    void frameNameSpacing() {
        const auto r = parse("=Обработка  ошибок=\n[A]\n= Обработка ошибок =\n[B]\n=обработка ошибок=\n[C]");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(r.diagram.groups.size(), 2);  // двойной пробел схлопнут; регистр важен
        QCOMPARE(r.diagram.groups[0].name, QStringLiteral("Обработка ошибок"));
        QCOMPARE(r.diagram.nodes[0].group, r.diagram.nodes[1].group);  // повторный заголовок продолжает рамку
        QVERIFY(r.diagram.nodes[2].group != r.diagram.nodes[0].group);
    }

    void frameNameEscapes() {
        const auto r = parse("=a\\=b\\nc=\n[X]");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(r.diagram.groups[0].name, QStringLiteral("a=b\nc"));
    }

    void headerWithComment() {
        const auto r = parse("=А= // начало\n[X]");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(r.diagram.nodes[0].group, 0);
    }

    void headerFollowedByArrowIsError() {
        const auto r = parse("=А= -> [X]\n== \"Да\"\n[Y]");
        QCOMPARE(errorCount(r), 2);
        QVERIFY(r.diagnostics[0].message.contains(QStringLiteral("только блок")));
        QCOMPARE(nodeLabels(r.diagram), (QStringList{"Y"}));
    }

    void headerNotOnOwnLine() {
        const auto r = parse("[A]; =Б=\n[B]");
        QCOMPARE(errorCount(r), 1);
        QVERIFY(r.diagnostics[0].message.contains(QStringLiteral("отдельной строке")));
        QCOMPARE(r.diagram.nodes[1].group, -1);  // рамка не открылась
    }

    void tripleEqualsIsError() {
        const auto r = parse("===\n[A]\n=А==\n[B]");
        QCOMPARE(errorCount(r), 2);
        QVERIFY(r.diagnostics[0].message.contains(QStringLiteral("===")));
        QCOMPARE(r.diagram.nodes.size(), 2);
    }

    void unclosedHeader() {
        const auto r = parse("=Авторизация\n[A]");
        QCOMPARE(errorCount(r), 1);
        QVERIFY(r.diagnostics[0].message.contains(QStringLiteral("Не закрыт заголовок рамки")));
        QCOMPARE(r.diagnostics[0].line, 1);
        QCOMPARE(r.diagram.nodes[0].group, -1);
    }

    void emptyFrameIsKeptButEmpty() {
        const auto r = parse("=Пустая=\n=Полная=\n[A]");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(r.diagram.groups.size(), 2);
        QCOMPARE(r.diagram.nodes[0].group, 1);
    }

    void equalsInsideTextIsPlain() {
        const auto r = parse("[a = b] -> [x == y] \"=\"");
        QVERIFY(r.diagnostics.isEmpty());
        QCOMPARE(nodeLabels(r.diagram), (QStringList{"a = b", "x == y"}));
        QCOMPARE(r.diagram.edges[0].label, QStringLiteral("="));
    }

    // --- ошибки -------------------------------------------------------------

    void errorPositions_data() {
        QTest::addColumn<QString>("source");
        QTest::addColumn<int>("line");
        QTest::addColumn<int>("column");
        QTest::addColumn<int>("length");

        QTest::newRow("unclosed bracket")   << "[A] -> [B]\n[Привет -> [C]" << 2 << 1 << 11;
        QTest::newRow("empty node")         << "[A] -> [  ]"               << 1 << 8 << 4;
        QTest::newRow("bare word")          << "[A] -> Б"                  << 1 << 8 << 1;
        QTest::newRow("arrow without node") << "[A] -> ;"                  << 1 << 8 << 1;
        QTest::newRow("dangling arrow")     << "[A] ->"                    << 1 << 7 << 1;
        QTest::newRow("label w/o arrow")    << "[A] \"Да\""                << 1 << 5 << 4;
        QTest::newRow("missing separator")  << "[A] -> [B] [C]"            << 1 << 12 << 3;
        QTest::newRow("starts with arrow")  << "-> [B]"                    << 1 << 1 << 2;
        QTest::newRow("unclosed quote")     << "[A] -> [B] \"Да"           << 1 << 12 << 3;
    }

    void errorPositions() {
        QFETCH(QString, source);
        QFETCH(int, line);
        QFETCH(int, column);
        QFETCH(int, length);

        const auto r = parse(source);
        QCOMPARE(errorCount(r), 1);
        const auto& d = r.diagnostics.front();
        QCOMPARE(d.line, line);
        QCOMPARE(d.column, column);
        QCOMPARE(d.length, length);
        QVERIFY(!d.message.isEmpty());
    }

    void brokenStatementIsSkippedOthersKept() {
        const auto r = parse("[A] -> [B]\n[B] -> ???\n[B] -> [C]");
        QCOMPARE(errorCount(r), 1);
        QCOMPARE(r.diagnostics[0].line, 2);
        QCOMPARE(nodeLabels(r.diagram), (QStringList{"A", "B", "C"}));
        QCOMPARE(r.diagram.edges.size(), 2);
    }

    void unfinishedLastLineDoesNotLeakNodes() {
        // Типичная ситуация при живом вводе.
        const auto r = parse("[A] -> [B];\n[B] -> [Нов");
        QCOMPARE(errorCount(r), 1);
        QCOMPARE(nodeLabels(r.diagram), (QStringList{"A", "B"}));
        QCOMPARE(r.diagram.edges.size(), 1);
    }

    void multipleErrorsAreAllReported() {
        const auto r = parse("[A] -> ;\nfoo\n[B] \"x\"\n[C] -> [D]");
        QCOMPARE(errorCount(r), 3);
        QCOMPARE(r.diagnostics[0].line, 1);
        QCOMPARE(r.diagnostics[1].line, 2);
        QCOMPARE(r.diagnostics[2].line, 3);
        QCOMPARE(r.diagram.edges.size(), 1);
    }

    void tooLongLabel() {
        const QString big(Parser::kMaxLabelLength + 1, u'x');
        QCOMPARE(errorCount(parse("[" + big + "]")), 1);
    }

    void tooLargeSource() {
        const QString huge(Parser::kMaxSourceLength + 1, u' ');
        const auto r = parse(huge);
        QCOMPARE(errorCount(r), 1);
        QVERIFY(r.diagram.isEmpty());
    }

    void nodeLimit() {
        QString src;
        for (int i = 0; i <= Parser::kMaxNodes; ++i) src += QStringLiteral("[n%1]\n").arg(i);
        const auto r = parse(src);
        QCOMPARE(errorCount(r), 1);
        QCOMPARE(r.diagram.nodes.size(), Parser::kMaxNodes);
    }

    // --- устойчивость -------------------------------------------------------

    void fuzzDoesNotCrash() {
        const QString alphabet = QStringLiteral("[]{}()\"«»“”;#/\\-> \n\r\t_aЯ→ ");
        auto* rng = QRandomGenerator::global();
        for (int iter = 0; iter < 3000; ++iter) {
            QString s;
            const int len = rng->bounded(80);
            for (int i = 0; i < len; ++i) s += alphabet[rng->bounded(static_cast<int>(alphabet.size()))];

            const auto r = parse(s);
            for (const auto& d : r.diagnostics) {
                QVERIFY(d.offset >= 0 && d.offset <= s.size());
                QVERIFY(d.length >= 1);
                QVERIFY(d.line >= 1 && d.column >= 1);
            }
            for (const auto& e : r.diagram.edges) {
                bool fromOk = false, toOk = false;
                for (const auto& n : r.diagram.nodes) {
                    fromOk |= n.id == e.from;
                    toOk |= n.id == e.to;
                }
                QVERIFY(fromOk && toOk);
            }
        }
    }
};

QTEST_GUILESS_MAIN(TestParser)
#include "tst_parser.moc"
