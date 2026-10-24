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

    void bundledExampleIsValid() {
        QFile f(QStringLiteral(BIRCH_EXAMPLES_DIR "/welcome.birch"));
        QVERIFY(f.open(QIODevice::ReadOnly));
        const auto r = parse(QString::fromUtf8(f.readAll()));
        QVERIFY2(r.diagnostics.isEmpty(), qPrintable(r.diagnostics.value(0).message));
        QCOMPARE(r.diagram.nodes.size(), 6);
        QCOMPARE(r.diagram.edges.size(), 6);
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
