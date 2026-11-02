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

// Frame — «=Название=» или «==»: заголовок рамки или указание рамки перед блоком.
enum class TokKind { Node, String, Frame, Arrow, Semicolon, Newline, End, Invalid };

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
               c == u'“' || c == u';' || c == u'#' || c == u'-' || c == u'=' || c == kUnicodeArrow;
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
            case u'=': return frameMark();
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

    // «=Название=» или «==» (пустое название — общая часть схемы, вне рамок).
    Token frameMark() {
        const int start = pos_;
        QString raw;
        if (!readUntil(u'=', raw))
            return invalid(start, QStringLiteral("Не закрыт заголовок рамки — ожидался «=» в конце, "
                                                 "например =Авторизация="));
        if (at(pos_) == u'=') {  // «===», «=А==»: непонятно, где кончается название
            while (at(pos_) == u'=') ++pos_;
            return invalid(start, QStringLiteral("Непонятная запись «%1». Рамка открывается так: "
                                                 "=Название=, а закрывается так: ==")
                                      .arg(src_.mid(start, pos_ - start).left(40)));
        }
        const QString name = normalizeText(raw);
        if (name.size() > Parser::kMaxLabelLength)
            return invalid(start, QStringLiteral("Слишком длинное название рамки (больше %1 символов)")
                                      .arg(Parser::kMaxLabelLength));
        Token t = make(TokKind::Frame, start, pos_ - start);
        t.text = name;
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
    case TokKind::Frame:
        return t.text.isEmpty() ? QStringLiteral("«==»") : QStringLiteral("рамка «=%1=»").arg(t.text.left(40));
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
        declareGroups();
        while (!stopped_ && cur().kind != TokKind::End) {
            if (isTerminator(cur().kind)) {
                ++i_;
                continue;
            }
            parseStatement();
        }
        if (!stopped_) hintMissingQualifiers();
    }

private:
    // Блок в инструкции: токен и рамка, которой он принадлежит (-1 — общий блок).
    struct PendingNode {
        const Token* token = nullptr;
        int group = -1;
    };
    struct PendingEdge {
        int from = 0;  // индексы в Pending::nodes
        int to = 0;
        QString label;
    };
    struct Pending {
        std::vector<PendingNode> nodes;
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

    // --- рамки ------------------------------------------------------------------

    // Заголовок — «=Название=» или «==», одни на своей строке (комментарий уже убран лексером).
    bool isHeader(size_t j) const {
        return toks_[j].kind == TokKind::Frame && (j == 0 || toks_[j - 1].kind == TokKind::Newline) &&
               isTerminator(at(j + 1).kind);
    }

    // Рамки объявляются заранее, по всем заголовкам файла: так стрелку в рамку
    // можно написать раньше самой рамки.
    void declareGroups() {
        for (size_t j = 0; j < toks_.size(); ++j) {
            if (!isHeader(j) || toks_[j].text.isEmpty() || groupIndex_.contains(toks_[j].text)) continue;
            groupIndex_.insert(toks_[j].text, static_cast<int>(result_.diagram.groups.size()));
            result_.diagram.groups.append({toks_[j].text});
        }
    }

    static QString frameMark(const QString& name) {
        return name.isEmpty() ? QStringLiteral("==") : QStringLiteral("=%1=").arg(name);
    }

    static QString bracketed(const QString& text, NodeShape shape) {
        switch (shape) {
        case NodeShape::Decision: return QStringLiteral("{%1}").arg(text);
        case NodeShape::Terminal: return QStringLiteral("(%1)").arg(text);
        case NodeShape::Process: break;
        }
        return QStringLiteral("[%1]").arg(text);
    }

    // Блок с необязательным указанием места перед ним: «=Рамка= [Блок]», «== [Блок]».
    // what — что ожидалось, для сообщения об ошибке.
    bool takeNode(Pending& st, const QString& what) {
        int group = currentGroup_;
        if (cur().kind == TokKind::Frame) {
            const Token& mark = cur();
            ++i_;
            if (cur().kind != TokKind::Node) {
                error(mark, QStringLiteral("После «%1» в той же строке может идти только блок, например %1 [Шаг]. "
                                           "Чтобы открыть рамку, напишите %1 на отдельной строке")
                                .arg(frameMark(mark.text)));
                return false;
            }
            if (mark.text.isEmpty()) {
                group = -1;
            } else if (auto it = groupIndex_.constFind(mark.text); it != groupIndex_.constEnd()) {
                group = *it;
            } else {
                error(mark, QStringLiteral("Рамка «%1» не найдена. Рамка открывается на отдельной строке: =%1=")
                                .arg(mark.text.left(40)));
                return false;
            }
        }
        if (cur().kind != TokKind::Node) {
            error(cur(), QStringLiteral("%1, а найдено %2").arg(what, describe(cur())));
            return false;
        }
        st.nodes.push_back({&cur(), group});
        ++i_;
        return true;
    }

    // --- инструкции ---------------------------------------------------------------

    void parseStatement() {
        if (cur().kind == TokKind::Frame && isTerminator(at(i_ + 1).kind)) {
            if (isHeader(i_)) {
                currentGroup_ = cur().text.isEmpty() ? -1 : groupIndex_.value(cur().text, -1);
            } else {
                error(cur(), QStringLiteral("Заголовок рамки пишется на отдельной строке: %1")
                                 .arg(frameMark(cur().text)));
            }
            ++i_;
            return;
        }

        Pending st;
        if (cur().kind != TokKind::Node && cur().kind != TokKind::Frame) {
            error(cur(), QStringLiteral("Инструкция должна начинаться с узла, например [Шаг], а найдено %1")
                             .arg(describe(cur())));
            ++i_;
            return synchronize();
        }
        if (!takeNode(st, QStringLiteral("Ожидался узел, например [Шаг]"))) return synchronize();

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
            if (!takeNode(st, QStringLiteral("После «->» ожидался узел"))) return synchronize();

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

    // Блок определяется рамкой и текстом: у общего блока ключ — сам текст (как
    // было до рамок), у блока рамки — номер рамки и текст.
    static QString nodeKey(int group, const QString& text) {
        return group < 0 ? text : QString::number(group) + QChar(0x1F) + text;
    }

    // Инструкция применяется целиком или не применяется вовсе.
    void commit(const Pending& st) {
        Diagram& dia = result_.diagram;
        std::vector<int> indices;
        indices.reserve(st.nodes.size());

        for (const PendingNode& pn : st.nodes) {
            const Token* t = pn.token;
            const QString key = nodeKey(pn.group, t->text);
            auto it = index_.constFind(key);
            if (it == index_.constEnd()) {
                if (dia.nodes.size() >= Parser::kMaxNodes) {
                    error(*t, QStringLiteral("Слишком много узлов (максимум %1)").arg(Parser::kMaxNodes));
                    stopped_ = true;
                    return;
                }
                const int idx = static_cast<int>(dia.nodes.size());
                dia.nodes.append({key, t->text, t->shape, pn.group});
                index_.insert(key, idx);
                mentions_.append({t, 1});
                indices.push_back(idx);
                continue;
            }

            Node& n = dia.nodes[*it];
            ++mentions_[*it].count;
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
                error(*st.nodes[static_cast<size_t>(pe.to)].token,
                      QStringLiteral("Слишком много связей (максимум %1)").arg(Parser::kMaxEdges));
                stopped_ = true;
                return;
            }
            dia.edges.append({dia.nodes[indices[static_cast<size_t>(pe.from)]].id,
                              dia.nodes[indices[static_cast<size_t>(pe.to)]].id, pe.label});
        }
    }

    // Блок рамки, упомянутый один раз, с текстом общего блока — скорее всего,
    // забыто «==» перед ним, и вместо стрелки к общему блоку вышел новый блок.
    void hintMissingQualifiers() {
        const Diagram& dia = result_.diagram;
        for (int i = 0; i < dia.nodes.size(); ++i) {
            const Node& n = dia.nodes[i];
            if (n.group < 0 || mentions_[i].count != 1) continue;
            const auto global = index_.constFind(nodeKey(-1, n.label));
            if (global == index_.constEnd()) continue;
            const QString target = bracketed(n.label, dia.nodes[*global].shape);
            warning(*mentions_[i].first,
                    QStringLiteral("Блок «%1» есть среди общих блоков. Чтобы провести стрелку к нему, напишите == %2")
                        .arg(n.label.left(40), target.left(60)));
        }
    }

    struct Mentions {
        const Token* first = nullptr;
        int count = 0;
    };

    const std::vector<Token> toks_;
    ParseResult& result_;
    QHash<QString, int> index_;       // ключ блока -> индекс в diagram.nodes
    QList<Mentions> mentions_;        // по индексам diagram.nodes
    QHash<QString, int> groupIndex_;  // название рамки -> индекс в diagram.groups
    int currentGroup_ = -1;
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
