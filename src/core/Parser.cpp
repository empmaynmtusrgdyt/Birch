#include "core/Parser.h"

#include <QHash>
#include <QStringList>

#include <algorithm>
#include <vector>

namespace birch {

bool ParseResult::hasErrors() const {
    for (const auto& d : diagnostics)
        if (d.isError()) return true;
    return false;
}

namespace {

// ---------------------------------------------------------------------------
// Лексер
// ---------------------------------------------------------------------------

enum class TokKind { Node, String, Arrow, Semicolon, Newline, End, Invalid };

struct Token {
    TokKind kind = TokKind::End;
    QString text;
    NodeShape shape = NodeShape::Process;
    int offset = 0;
    int length = 0;
    int line = 1;
    int column = 1;
};

constexpr char16_t kUnicodeArrow = u'→';  // →

QString shapeName(NodeShape s) {
    switch (s) {
    case NodeShape::Process: return QStringLiteral("прямоугольник [ ]");
    case NodeShape::Decision: return QStringLiteral("ромб { }");
    case NodeShape::Terminal: return QStringLiteral("овал ( )");
    }
    return {};
}

// Схлопывает пробелы в каждой строке, убирает пустые края.
QString normalizeText(const QString& raw) {
    QStringList lines = raw.split(u'\n');
    for (auto& l : lines) l = l.simplified();
    while (!lines.isEmpty() && lines.front().isEmpty()) lines.removeFirst();
    while (!lines.isEmpty() && lines.back().isEmpty()) lines.removeLast();
    return lines.join(u'\n');
}

class Lexer {
public:
    Lexer(const QString& src, QList<Diagnostic>& diags)
        : src_(src), size_(static_cast<int>(src.size())), diags_(diags) {}

    std::vector<Token> tokenize() {
        std::vector<Token> out;
        for (;;) {
            out.push_back(next());
            if (out.back().kind == TokKind::End) break;
        }
        return out;
    }

private:
    bool atEnd() const { return pos_ >= size_; }
    QChar at(int p) const { return p < size_ ? src_[p] : QChar(); }
    int column(int offset) const { return offset - lineStart_ + 1; }

    Token make(TokKind kind, int start, int length) const {
        Token t;
        t.kind = kind;
        t.offset = start;
        t.length = length;
        t.line = line_;
        t.column = column(start);
        return t;
    }

    Token invalid(int start, const QString& message) {
        Token t = make(TokKind::Invalid, start, qMax(1, pos_ - start));
        diags_.append({Diagnostic::Severity::Error, t.line, t.column, t.offset, t.length, message});
        return t;
    }

    static bool isSpecial(QChar c) {
        return c.isSpace() || c == u'[' || c == u'{' || c == u'(' || c == u'"' || c == u'«' ||
               c == u'“' || c == u';' || c == u'#' || c == u'-' || c == kUnicodeArrow;
    }

    Token next() {
        for (;;) {
            if (atEnd()) return make(TokKind::End, size_, 0);

            const QChar c = src_[pos_];
            if (c == u'\n') {
                Token t = make(TokKind::Newline, pos_, 1);
                ++pos_;
                ++line_;
                lineStart_ = pos_;
                return t;
            }
            if (c.isSpace()) {  // пробелы, табы, \r, неразрывный пробел
                ++pos_;
                continue;
            }
            if (c == u'#' || (c == u'/' && at(pos_ + 1) == u'/')) {
                while (!atEnd() && src_[pos_] != u'\n') ++pos_;
                continue;
            }

            const int start = pos_;
            switch (c.unicode()) {
            case u'[': return bracketed(u']', NodeShape::Process);
            case u'{': return bracketed(u'}', NodeShape::Decision);
            case u'(': return bracketed(u')', NodeShape::Terminal);
            case u'"': return quoted(u'"');
            case u'«': return quoted(u'»');
            case u'“': return quoted(u'”');
            case u';': ++pos_; return make(TokKind::Semicolon, start, 1);
            case kUnicodeArrow: ++pos_; return make(TokKind::Arrow, start, 1);
            default: break;
            }
            if (c == u'-' && at(pos_ + 1) == u'>') {
                pos_ += 2;
                return make(TokKind::Arrow, start, 2);
            }

            // Неизвестный текст: съедаем «слово» целиком, чтобы выдать одну ошибку, а не десять.
            ++pos_;
            while (!atEnd() && !isSpecial(src_[pos_])) ++pos_;
            const QString word = src_.mid(start, pos_ - start);
            return invalid(start, QStringLiteral("Непонятный текст «%1». Узлы пишутся в скобках: "
                                                 "[Шаг], {Условие?}, (Старт)")
                                      .arg(word.left(40)));
        }
    }

    // Читает содержимое до закрывающего символа, обрабатывая '\'-экранирование.
    // Возвращает false, если строка кончилась раньше или встретилась ещё одна
    // открывающая скобка того же вида ([A -> [B] — почти наверняка опечатка).
    bool readUntil(QChar close, QString& out) {
        const QChar open = src_[pos_];
        ++pos_;
        while (!atEnd()) {
            const QChar c = src_[pos_];
            if (c == u'\n' || (c == open && open != close)) return false;
            if (c == u'\\' && pos_ + 1 < size_ && src_[pos_ + 1] != u'\n') {
                const QChar e = src_[pos_ + 1];
                out += (e == u'n') ? QChar(u'\n') : e;
                pos_ += 2;
                continue;
            }
            ++pos_;
            if (c == close) return true;
            out += c;
        }
        return false;
    }

    Token bracketed(QChar close, NodeShape shape) {
        const int start = pos_;
        const QChar open = src_[start];
        QString raw;
        if (!readUntil(close, raw))
            return invalid(start, QStringLiteral("Не закрыта скобка «%1» — ожидалась «%2» до конца строки")
                                      .arg(open)
                                      .arg(close));

        const QString text = normalizeText(raw);
        if (text.isEmpty())
            return invalid(start, QStringLiteral("Пустой узел «%1%2» — добавьте текст").arg(open).arg(close));
        if (text.size() > Parser::kMaxLabelLength)
            return invalid(start, QStringLiteral("Слишком длинный текст узла (больше %1 символов)")
                                      .arg(Parser::kMaxLabelLength));

        Token t = make(TokKind::Node, start, pos_ - start);
        t.text = text;
        t.shape = shape;
        return t;
    }

    Token quoted(QChar close) {
        const int start = pos_;
        QString raw;
        if (!readUntil(close, raw))
            return invalid(start, QStringLiteral("Не закрыта кавычка — ожидалась «%1» до конца строки").arg(close));
        if (raw.size() > Parser::kMaxLabelLength)
            return invalid(start, QStringLiteral("Слишком длинная подпись (больше %1 символов)")
                                      .arg(Parser::kMaxLabelLength));

        Token t = make(TokKind::String, start, pos_ - start);
        t.text = normalizeText(raw);
        return t;
    }

    const QString& src_;
    const int size_;
    QList<Diagnostic>& diags_;
    int pos_ = 0;
    int line_ = 1;
    int lineStart_ = 0;
};

// ---------------------------------------------------------------------------
// Парсер инструкций
// ---------------------------------------------------------------------------

QString describe(const Token& t) {
    switch (t.kind) {
    case TokKind::Node: return QStringLiteral("узел «%1»").arg(t.text.left(40));
    case TokKind::String: return QStringLiteral("подпись «%1»").arg(t.text.left(40));
    case TokKind::Arrow: return QStringLiteral("«->»");
    case TokKind::Semicolon: return QStringLiteral("«;»");
    case TokKind::Newline: return QStringLiteral("конец строки");
    case TokKind::End: return QStringLiteral("конец текста");
    case TokKind::Invalid: return QStringLiteral("ошибочный фрагмент");
    }
    return {};
}

bool isTerminator(TokKind k) {
    return k == TokKind::Semicolon || k == TokKind::Newline || k == TokKind::End;
}

class StatementParser {
public:
    StatementParser(std::vector<Token> tokens, ParseResult& result)
        : toks_(std::move(tokens)), result_(result) {}

    void run() {
        while (!stopped_ && cur().kind != TokKind::End) {
            if (isTerminator(cur().kind)) {
                ++i_;
                continue;
            }
            parseStatement();
        }
    }

private:
    struct PendingEdge {
        int from = 0;  // индексы в Pending::nodes
        int to = 0;
        QString label;
    };
    struct Pending {
        std::vector<const Token*> nodes;
        std::vector<PendingEdge> edges;
    };

    const Token& cur() const { return toks_[i_]; }
    const Token& at(size_t j) const { return toks_[qMin(j, toks_.size() - 1)]; }

    void error(const Token& t, const QString& msg) {
        // Об Invalid-токенах уже сообщил лексер — не дублируем.
        if (t.kind == TokKind::Invalid) return;
        result_.diagnostics.append(
            {Diagnostic::Severity::Error, t.line, t.column, t.offset, qMax(1, t.length), msg});
    }

    void warning(const Token& t, const QString& msg) {
        result_.diagnostics.append(
            {Diagnostic::Severity::Warning, t.line, t.column, t.offset, qMax(1, t.length), msg});
    }

    // Пропускаем остаток сломанной инструкции.
    void synchronize() {
        while (!isTerminator(cur().kind)) ++i_;
    }

    void parseStatement() {
        if (cur().kind != TokKind::Node) {
            error(cur(), QStringLiteral("Инструкция должна начинаться с узла, например [Шаг], а найдено %1")
                             .arg(describe(cur())));
            ++i_;
            synchronize();
            return;
        }

        Pending st;
        st.nodes.push_back(&cur());
        ++i_;

        for (;;) {
            // Перенос строки перед '->' продолжает инструкцию.
            size_t j = i_;
            while (at(j).kind == TokKind::Newline) ++j;

            if (at(j).kind == TokKind::Arrow) {
                i_ = j + 1;
            } else if (cur().kind == TokKind::String) {
                error(cur(), QStringLiteral("Подпись должна идти после стрелки и узла: [A] -> [B] \"Да\""));
                return synchronize();
            } else {
                break;
            }

            while (cur().kind == TokKind::Newline) ++i_;
            if (cur().kind != TokKind::Node) {
                error(cur(), QStringLiteral("После «->» ожидался узел, а найдено %1").arg(describe(cur())));
                return synchronize();
            }
            st.nodes.push_back(&cur());
            ++i_;

            PendingEdge e;
            e.from = static_cast<int>(st.nodes.size()) - 2;
            e.to = static_cast<int>(st.nodes.size()) - 1;
            if (cur().kind == TokKind::String) {
                e.label = cur().text;
                ++i_;
            }
            st.edges.push_back(std::move(e));
        }

        if (!isTerminator(cur().kind)) {
            error(cur(), QStringLiteral("Ожидались «->», «;» или новая строка, а найдено %1").arg(describe(cur())));
            return synchronize();
        }
        commit(st);
    }

    // Инструкция применяется целиком или не применяется вовсе.
    void commit(const Pending& st) {
        Diagram& dia = result_.diagram;
        std::vector<int> indices;
        indices.reserve(st.nodes.size());

        for (const Token* t : st.nodes) {
            auto it = index_.constFind(t->text);
            if (it == index_.constEnd()) {
                if (dia.nodes.size() >= Parser::kMaxNodes) {
                    error(*t, QStringLiteral("Слишком много узлов (максимум %1)").arg(Parser::kMaxNodes));
                    stopped_ = true;
                    return;
                }
                const int idx = static_cast<int>(dia.nodes.size());
                dia.nodes.append({t->text, t->text, t->shape});
                index_.insert(t->text, idx);
                indices.push_back(idx);
                continue;
            }

            Node& n = dia.nodes[*it];
            if (t->shape != NodeShape::Process && t->shape != n.shape) {
                if (n.shape == NodeShape::Process)
                    n.shape = t->shape;  // [] уточняется до {} или ()
                else
                    warning(*t, QStringLiteral("Узел «%1» уже объявлен как %2 — форма %3 проигнорирована")
                                    .arg(n.label.left(40), shapeName(n.shape), shapeName(t->shape)));
            }
            indices.push_back(*it);
        }

        for (const PendingEdge& pe : st.edges) {
            if (dia.edges.size() >= Parser::kMaxEdges) {
                error(*st.nodes[static_cast<size_t>(pe.to)],
                      QStringLiteral("Слишком много связей (максимум %1)").arg(Parser::kMaxEdges));
                stopped_ = true;
                return;
            }
            dia.edges.append({dia.nodes[indices[static_cast<size_t>(pe.from)]].id,
                              dia.nodes[indices[static_cast<size_t>(pe.to)]].id, pe.label});
        }
    }

    const std::vector<Token> toks_;
    ParseResult& result_;
    QHash<QString, int> index_;
    size_t i_ = 0;
    bool stopped_ = false;
};

}  // namespace

ParseResult Parser::parse(const QString& source) const {
    ParseResult result;

    if (source.size() > kMaxSourceLength) {
        result.diagnostics.append({Diagnostic::Severity::Error, 1, 1, 0, 1,
                                   QStringLiteral("Текст слишком большой (больше %1 символов)")
                                       .arg(kMaxSourceLength)});
        return result;
    }

    Lexer lexer(source, result.diagnostics);
    StatementParser parser(lexer.tokenize(), result);
    parser.run();

    std::stable_sort(result.diagnostics.begin(), result.diagnostics.end(),
                     [](const Diagnostic& a, const Diagnostic& b) { return a.offset < b.offset; });
    return result;
}

}  // namespace birch
