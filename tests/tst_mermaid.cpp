// Тесты экспорта в Mermaid: фигуры, подписи, экранирование, направление, рамки.
//
// С переменной BIRCH_MERMAID_DIR тест сохраняет туда экспорт примеров (*.mmd),
// а CI проверяет их официальным mermaid-cli.

#include <QDir>
#include <QFile>
#include <QtTest>

#include "core/MermaidExporter.h"
#include "core/Parser.h"

using namespace birch;

namespace {

Diagram parseOk(const QString& src) {
    const ParseResult r = Parser{}.parse(src);
    if (!r.diagnostics.isEmpty()) qWarning() << r.diagnostics.first().message;
    return r.diagram;
}

QString readExample(const QString& name) {
    QFile f(QStringLiteral(BIRCH_EXAMPLES_DIR "/") + name);
    return f.open(QIODevice::ReadOnly) ? QString::fromUtf8(f.readAll()) : QString();
}

void dump(const QString& name, const QString& text) {
    const QString dir = qEnvironmentVariable("BIRCH_MERMAID_DIR");
    if (dir.isEmpty()) return;
    QDir().mkpath(dir);
    QFile f(dir + u'/' + name + QStringLiteral(".mmd"));
    if (f.open(QIODevice::WriteOnly)) f.write(text.toUtf8());
}

}  // namespace

class TestMermaid : public QObject {
    Q_OBJECT

private slots:
    void shapesAndEdges() {
        const QString m = toMermaid(parseOk("(Старт) -> [Шаг] -> {Ок?}\n{Ок?} -> (Конец) \"Да\""));
        QCOMPARE(m, QStringLiteral("flowchart TD\n"
                                   "    n1([\"Старт\"])\n"
                                   "    n2(\"Шаг\")\n"
                                   "    n3{\"Ок?\"}\n"
                                   "    n4([\"Конец\"])\n"
                                   "    n1 --> n2\n"
                                   "    n2 --> n3\n"
                                   "    n3 -->|\"Да\"| n4\n"));
    }

    void leftToRight() {
        MermaidOptions opt;
        opt.leftToRight = true;
        QVERIFY(toMermaid(parseOk("[A] -> [B]"), opt).startsWith(QStringLiteral("flowchart LR\n")));
    }

    void escaping() {
        const QString m = toMermaid(parseOk("[Сказал \\\"привет\\\"] -> [a<b & c>d] \"#1; x\"\n[Две\\nстроки]"));
        QVERIFY(m.contains(QStringLiteral("n1(\"Сказал #quot;привет#quot;\")")));
        QVERIFY(m.contains(QStringLiteral("n2(\"a#lt;b #amp; c#gt;d\")")));
        QVERIFY(m.contains(QStringLiteral("-->|\"#35;1; x\"|")));
        QVERIFY(m.contains(QStringLiteral("n3(\"Две<br/>строки\")")));
    }

    void bracketsAndKeywordsInText() {
        // Скобки и слова Mermaid внутри кавычек — просто текст.
        const QString m = toMermaid(parseOk("[end] -> {subgraph (x) [y]?}"));
        QVERIFY(m.contains(QStringLiteral("n1(\"end\")")));
        QVERIFY(m.contains(QStringLiteral("n2{\"subgraph (x) [y]?\"}")));
    }

    void framesBecomeSubgraphs() {
        const QString m = toMermaid(parseOk("(Старт) -> =А= [Ввод]\n"
                                            "=А=\n[Ввод] -> {Проверка} -> == (Выход)\n"
                                            "=Б=\n[Профиль] -> {Проверка}\n"
                                            "=Пустая=\n"));
        QCOMPARE(m, QStringLiteral("flowchart TD\n"
                                   "    n1([\"Старт\"])\n"
                                   "    n4([\"Выход\"])\n"
                                   "    subgraph g1[\"А\"]\n"
                                   "        n2(\"Ввод\")\n"
                                   "        n3{\"Проверка\"}\n"
                                   "    end\n"
                                   "    subgraph g2[\"Б\"]\n"
                                   "        n5(\"Профиль\")\n"
                                   "        n6{\"Проверка\"}\n"
                                   "    end\n"
                                   "    n1 --> n2\n"
                                   "    n2 --> n3\n"
                                   "    n3 --> n4\n"
                                   "    n5 --> n6\n"));
    }

    void markdownBlock() {
        const QString m = toMermaidMarkdown(parseOk("[A]"));
        QVERIFY(m.startsWith(QStringLiteral("```mermaid\nflowchart TD\n")));
        QVERIFY(m.endsWith(QStringLiteral("\n```\n")));
    }

    void emptyDiagram() { QCOMPARE(toMermaid(Diagram{}), QStringLiteral("flowchart TD\n")); }

    // Не проверка, а файлы для mermaid-cli в CI (см. BIRCH_MERMAID_DIR).
    void dumpExamples() {
        for (const QString& name : {QStringLiteral("welcome"), QStringLiteral("frames")}) {
            const QString src = readExample(name + QStringLiteral(".birch"));
            QVERIFY(!src.isEmpty());
            dump(name, toMermaid(parseOk(src)));
        }
        MermaidOptions lr;
        lr.leftToRight = true;
        dump(QStringLiteral("frames-lr"), toMermaid(parseOk(readExample(QStringLiteral("frames.birch"))), lr));
        dump(QStringLiteral("special"),
             toMermaid(parseOk("[Сказал \\\"привет\\\"] -> [a<b & c>d] \"#1; x\"\n"
                               "[Две\\nстроки] -> {end?} -> (subgraph)\n=Рамка \\\"1\\\" <b>=\n[Шаг]")));
    }
};

QTEST_GUILESS_MAIN(TestMermaid)
#include "tst_mermaid.moc"
