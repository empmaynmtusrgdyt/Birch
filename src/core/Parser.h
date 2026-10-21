#pragma once

#include "core/Diagram.h"

namespace birch {

// Сообщение парсера с точной позицией — UI подчёркивает этот фрагмент в редакторе.
struct Diagnostic {
    enum class Severity { Error, Warning };

    Severity severity = Severity::Error;
    int line = 0;    // с 1
    int column = 0;  // с 1, в UTF-16 code units (как QTextCursor)
    int offset = 0;  // абсолютная позиция в исходном тексте
    int length = 0;  // длина проблемного фрагмента (>= 1)
    QString message;

    bool isError() const { return severity == Severity::Error; }
};

struct ParseResult {
    // Содержит все корректные инструкции, даже если в других есть ошибки:
    // при живом рендеринге недописанная строка не «ломает» всю схему.
    Diagram diagram;
    QList<Diagnostic> diagnostics;

    bool hasErrors() const;
};

// Разбирает DSL Birch.
//
//   Узлы:      [Процесс]   {Условие?}   (Старт)
//   Связи:     [A] -> [B]            цепочки: [A] -> [B] -> [C]
//   Подписи:   [A] -> [B] "Да"       подпись относится к стрелке, ведущей в [B]
//   Конец:     ';' или перевод строки; строка, начинающаяся с '->', продолжает предыдущую
//   Текст:     '_' -> пробел, '\n' -> перенос строки, '\x' -> символ x как есть
//   Прочее:    комментарии // и #, стрелка →, кавычки «» и “”
//
// Одинаковый текст означает один и тот же узел. Форма из {} или () «сильнее» [],
// поэтому узел можно объявить как {Условие?}, а дальше писать [Условие?].
class Parser {
public:
    static constexpr int kMaxSourceLength = 1'000'000;
    static constexpr int kMaxLabelLength = 1'000;
    static constexpr int kMaxNodes = 5'000;
    static constexpr int kMaxEdges = 20'000;

    ParseResult parse(const QString& source) const;
};

}  // namespace birch
