#pragma once

#include <QString>
#include <QStringList>

namespace birch::cli {

// Разбор командной строки и рендер схемы в файл без окна:
//
//   birch схема.birch -o схема.png -t dark -s 3
//
// Справка и версия — в формате GNU. Разбор не требует QApplication, поэтому
// --help и --version работают мгновенно и без экрана.

enum class Mode {
    Gui,      // открыть окно (и, возможно, файл)
    Help,     // -h, --help
    Version,  // -v, --version
    Render,   // -o: нарисовать схему в файл и выйти
    Error,    // неверные ключи: сообщение в error, код выхода 3
};

// Коды выхода — их разбирают скрипты.
enum ExitCode {
    kExitOk = 0,
    kExitDiagramErrors = 1,  // ошибки в тексте схемы
    kExitIoError = 2,        // не удалось прочитать или записать файл
    kExitUsage = 3,          // неверные ключи
};

struct Arguments {
    Mode mode = Mode::Gui;
    QString error;  // для Mode::Error — без имени программы и подсказки про --help

    QString input;   // путь к схеме или «-» (стандартный ввод)
    QString output;  // -o: файл .png, .svg или .mmd

    // Вид картинки (только вместе с -o)
    bool dark = false;
    bool monochrome = false;
    bool horizontal = false;
    bool transparent = false;
    int scale = 2;

    // Служебные ключи для снимков окна в CI (в справке их нет)
    QString screenshot;
    QString screenshotTheme;
    bool screenshotMonochrome = false;
    QString find;
};

inline constexpr int kMaxScale = 10;

// Аргументы как их видит пользователь (на Windows — из командной строки в UTF-16).
QStringList rawArguments(int argc, char* argv[]);

// Без ключей, которые разбирает сам Qt (-style, -platform и т. п.): их поймёт
// QApplication, а наш разбор принял бы их за склеенные короткие ключи.
QStringList withoutQtOptions(const QStringList& args);

// args[0] — путь к программе, как в argv.
Arguments parse(const QStringList& args);

// Имя, под которым программу запустили: «birch» по ссылке, «Birch» в бандле macOS.
QString programName(const QString& argv0);

QString helpText(const QString& program);
QString versionText(const QString& version);
// Сообщение об ошибке ключей и подсказка про --help — две строки, как в GNU.
QString usageErrorText(const QString& program, const QString& error);

// Рисует схему в файл по разобранным аргументам (Mode::Render). Сообщения для
// stderr — в errors. Нужен живой QGuiApplication: шрифты измеряет Qt.
int render(const Arguments& args, const QString& program, QString& errors);

// На Windows Birch.exe — оконное приложение без консоли: подключаемся к консоли,
// из которой его запустили, чтобы вывод был виден. На других системах ничего не делает.
void attachParentConsole();

// Печать в stdout/stderr в UTF-8.
void printOut(const QString& text);
void printErr(const QString& text);

}  // namespace birch::cli
