<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="en" sourcelanguage="ru">
<context>
    <name>InstallCommand</name>
    <message>
        <source>Откройте новое окно терминала и попробуйте:

    birch --help
    birch схема.birch -o схема.png</source>
        <translation>Open a new terminal window and try:

    birch --help
    birch diagram.birch -o diagram.png</translation>
    </message>
    <message>
        <source>Команда birch установлена: %1.</source>
        <translation>The birch command is installed: %1.</translation>
    </message>
    <message>
        <source>

Папки ~/.local/bin пока нет в PATH. Обычно она появляется там после повторного входа в систему. Если нет — добавьте в ~/.bashrc строку:

    export PATH=&quot;$HOME/.local/bin:$PATH&quot;</source>
        <translation>

~/.local/bin is not in your PATH yet. It usually appears there after you log out and back in. If it doesn&apos;t, add this line to ~/.bashrc:

    export PATH=&quot;$HOME/.local/bin:$PATH&quot;</translation>
    </message>
    <message>
        <source>

Если переместите файл AppImage, выберите этот пункт ещё раз.</source>
        <translation>

If you move the AppImage file, choose this menu item again.</translation>
    </message>
    <message>
        <source>Команда birch</source>
        <translation>birch Command</translation>
    </message>
    <message>
        <source>Birch установит команду birch в /usr/local/bin.</source>
        <translation>Birch will install the birch command in /usr/local/bin.</translation>
    </message>
    <message>
        <source>Не удалось установить команду birch.

%1</source>
        <translation>Could not install the birch command.

%1</translation>
    </message>
    <message>
        <source>Birch запущен не из папки «Программы», поэтому команда на него перестанет работать.

Перетащите Birch в «Программы», откройте его оттуда и выберите этот пункт ещё раз. Если Birch уже там, выполните в Терминале:

    xattr -dr com.apple.quarantine /Applications/Birch.app</source>
        <translation>Birch is not running from the Applications folder, so a command pointing to it would stop working.

Drag Birch to Applications, open it from there and choose this menu item again. If Birch is already there, run this in Terminal:

    xattr -dr com.apple.quarantine /Applications/Birch.app</translation>
    </message>
    <message>
        <source>Команда birch уже установлена: %1.</source>
        <translation>The birch command is already installed: %1.</translation>
    </message>
    <message>
        <source>По пути %1 уже есть другой файл. Заменить его командой Birch?</source>
        <translation>There is already a different file at %1. Replace it with the Birch command?</translation>
    </message>
    <message>
        <source>Не удалось установить команду birch: %1.</source>
        <translation>Could not install the birch command: %1.</translation>
    </message>
</context>
<context>
    <name>birch::CodeEditor</name>
    <message>
        <source>Опишите блок-схему, например:

[Старт] -&gt; [Шаг] -&gt; [Готово];</source>
        <translation>Describe a flowchart, for example:

[Start] -&gt; [Step] -&gt; [Done];</translation>
    </message>
    <message>
        <source>Ошибка</source>
        <translation>Error</translation>
    </message>
    <message>
        <source>Предупреждение</source>
        <translation>Warning</translation>
    </message>
</context>
<context>
    <name>birch::FindBar</name>
    <message>
        <source>Найти</source>
        <translation>Find</translation>
    </message>
    <message>
        <source>Заменить на</source>
        <translation>Replace with</translation>
    </message>
    <message>
        <source>Предыдущее совпадение</source>
        <translation>Previous match</translation>
    </message>
    <message>
        <source>Следующее совпадение</source>
        <translation>Next match</translation>
    </message>
    <message>
        <source>Учитывать регистр</source>
        <translation>Match case</translation>
    </message>
    <message>
        <source>Регулярное выражение. В замене $1, $2… — найденные группы</source>
        <translation>Regular expression. In the replacement, $1, $2… are the captured groups</translation>
    </message>
    <message>
        <source>Закрыть (Esc)</source>
        <translation>Close (Esc)</translation>
    </message>
    <message>
        <source>Заменить</source>
        <translation>Replace</translation>
    </message>
    <message>
        <source>Заменить все</source>
        <translation>Replace All</translation>
    </message>
    <message>
        <source>Заменено: %1</source>
        <translation>Replaced: %1</translation>
    </message>
    <message>
        <source>Ошибка</source>
        <translation>Error</translation>
    </message>
    <message>
        <source>Не найдено</source>
        <translation>Not found</translation>
    </message>
    <message>
        <source>%1 из %2</source>
        <translation>%1 of %2</translation>
    </message>
    <message>
        <source>Ошибка в регулярном выражении: %1</source>
        <translation>Error in regular expression: %1</translation>
    </message>
    <message numerus="yes">
        <source>%n совпадений</source>
        <translation>
            <numerusform>%n match</numerusform>
            <numerusform>%n matches</numerusform>
        </translation>
    </message>
</context>
<context>
    <name>birch::MainWindow</name>
    <message>
        <source>Новый</source>
        <translation>New</translation>
    </message>
    <message>
        <source>Открыть…</source>
        <translation>Open…</translation>
    </message>
    <message>
        <source>Сохранить</source>
        <translation>Save</translation>
    </message>
    <message>
        <source>Сохранить как…</source>
        <translation>Save As…</translation>
    </message>
    <message>
        <source>Открыть пример</source>
        <translation>Open Example</translation>
    </message>
    <message>
        <source>Открыть пример с рамками</source>
        <translation>Open Frames Example</translation>
    </message>
    <message>
        <source>Выход</source>
        <translation>Quit</translation>
    </message>
    <message>
        <source>Отменить</source>
        <translation>Undo</translation>
    </message>
    <message>
        <source>Повторить</source>
        <translation>Redo</translation>
    </message>
    <message>
        <source>Вырезать</source>
        <translation>Cut</translation>
    </message>
    <message>
        <source>Копировать</source>
        <translation>Copy</translation>
    </message>
    <message>
        <source>Вставить</source>
        <translation>Paste</translation>
    </message>
    <message>
        <source>Выбрать все</source>
        <translation>Select All</translation>
    </message>
    <message>
        <source>Найти…</source>
        <translation>Find…</translation>
    </message>
    <message>
        <source>Заменить…</source>
        <translation>Replace…</translation>
    </message>
    <message>
        <source>Найти далее</source>
        <translation>Find Next</translation>
    </message>
    <message>
        <source>Найти назад</source>
        <translation>Find Previous</translation>
    </message>
    <message>
        <source>Рендерить</source>
        <translation>Render</translation>
    </message>
    <message>
        <source>Перерисовать схему (%1)</source>
        <translation>Redraw the diagram (%1)</translation>
    </message>
    <message>
        <source>Автоматический рендеринг</source>
        <translation>Render Automatically</translation>
    </message>
    <message>
        <source>Скачать PNG…</source>
        <translation>Save PNG…</translation>
    </message>
    <message>
        <source>Сохранить картинку в файл .png (%1)</source>
        <translation>Save the picture as a .png file (%1)</translation>
    </message>
    <message>
        <source>Экспорт SVG…</source>
        <translation>Export SVG…</translation>
    </message>
    <message>
        <source>Экспорт Mermaid…</source>
        <translation>Export Mermaid…</translation>
    </message>
    <message>
        <source>Копировать как Mermaid</source>
        <translation>Copy as Mermaid</translation>
    </message>
    <message>
        <source>Текст схемы для README на GitHub, Notion, Obsidian — там он отображается как схема</source>
        <translation>Diagram text for a README on GitHub, Notion or Obsidian — they display it as a diagram</translation>
    </message>
    <message>
        <source>Копировать в буфер</source>
        <translation>Copy to Clipboard</translation>
    </message>
    <message>
        <source>Скопировать PNG — можно сразу вставить в Telegram, Slack, Word (%1)</source>
        <translation>Copy as PNG — paste it straight into Telegram, Slack or Word (%1)</translation>
    </message>
    <message>
        <source>Горизонтальная схема</source>
        <translation>Horizontal Layout</translation>
    </message>
    <message>
        <source>Располагать блоки слева направо</source>
        <translation>Lay out blocks from left to right</translation>
    </message>
    <message>
        <source>Чёрно-белая схема</source>
        <translation>Black and White</translation>
    </message>
    <message>
        <source>Ч/Б</source>
        <translation>B/W</translation>
    </message>
    <message>
        <source>Чёрно-белая схема — для печати и документов (%1)</source>
        <translation>Black and white diagram — for printing and documents (%1)</translation>
    </message>
    <message>
        <source>Прозрачный фон</source>
        <translation>Transparent Background</translation>
    </message>
    <message>
        <source>Увеличить</source>
        <translation>Zoom In</translation>
    </message>
    <message>
        <source>Уменьшить</source>
        <translation>Zoom Out</translation>
    </message>
    <message>
        <source>Реальный размер</source>
        <translation>Actual Size</translation>
    </message>
    <message>
        <source>По размеру окна</source>
        <translation>Fit to Window</translation>
    </message>
    <message>
        <source>Как в системе</source>
        <translation>System</translation>
    </message>
    <message>
        <source>Светлая</source>
        <translation>Light</translation>
    </message>
    <message>
        <source>Тёмная</source>
        <translation>Dark</translation>
    </message>
    <message>
        <source>Как в превью</source>
        <translation>Same as Preview</translation>
    </message>
    <message>
        <source>Всегда светлая</source>
        <translation>Always Light</translation>
    </message>
    <message>
        <source>Всегда тёмная</source>
        <translation>Always Dark</translation>
    </message>
    <message>
        <source>Синтаксис…</source>
        <translation>Syntax…</translation>
    </message>
    <message>
        <source>О программе Birch</source>
        <translation>About Birch</translation>
    </message>
    <message>
        <source>Установить команду birch…</source>
        <translation>Install birch Command…</translation>
    </message>
    <message>
        <source>Файл</source>
        <translation>File</translation>
    </message>
    <message>
        <source>Правка</source>
        <translation>Edit</translation>
    </message>
    <message>
        <source>Вид</source>
        <translation>View</translation>
    </message>
    <message>
        <source>Тема</source>
        <translation>Theme</translation>
    </message>
    <message>
        <source>Экспорт картинки</source>
        <translation>Picture Export</translation>
    </message>
    <message>
        <source>Тема картинки</source>
        <translation>Picture Theme</translation>
    </message>
    <message>
        <source>Справка</source>
        <translation>Help</translation>
    </message>
    <message>
        <source>Панель инструментов</source>
        <translation>Toolbar</translation>
    </message>
    <message>
        <source>Схема появится здесь.
Опишите её слева, например:
[Старт] -&gt; [Готово]</source>
        <translation>The diagram will appear here.
Describe it on the left, for example:
[Start] -&gt; [Done]</translation>
    </message>
    <message>
        <source>Узлов: %1 · связей: %2 · %3 мс</source>
        <translation>Nodes: %1 · edges: %2 · %3 ms</translation>
    </message>
    <message>
        <source>Строка %1: %2</source>
        <translation>Line %1: %2</translation>
    </message>
    <message>
        <source>Нечего экспортировать — схема пуста</source>
        <translation>Nothing to export — the diagram is empty</translation>
    </message>
    <message>
        <source>Ошибка рендеринга</source>
        <translation>Rendering error</translation>
    </message>
    <message>
        <source>В тексте есть ошибки — в картинку попали только корректные строки</source>
        <translation>The text has errors — only the correct lines are in the picture</translation>
    </message>
    <message>
        <source>Скачать PNG</source>
        <translation>Save PNG</translation>
    </message>
    <message>
        <source>Изображение PNG (*.png)</source>
        <translation>PNG image (*.png)</translation>
    </message>
    <message>
        <source>Не удалось сохранить</source>
        <translation>Could Not Save</translation>
    </message>
    <message>
        <source>Не удалось записать файл:
%1

%2</source>
        <translation>Could not write the file:
%1

%2</translation>
    </message>
    <message>
        <source>Сохранено: %1 (%2×%3)</source>
        <translation>Saved: %1 (%2×%3)</translation>
    </message>
    <message>
        <source>Экспорт SVG</source>
        <translation>Export SVG</translation>
    </message>
    <message>
        <source>Векторное изображение SVG (*.svg)</source>
        <translation>SVG vector image (*.svg)</translation>
    </message>
    <message>
        <source>Сохранено: %1</source>
        <translation>Saved: %1</translation>
    </message>
    <message>
        <source>Картинка скопирована — вставьте её в Telegram, Slack, Word…</source>
        <translation>Picture copied — paste it into Telegram, Slack, Word…</translation>
    </message>
    <message>
        <source>В тексте есть ошибки — в Mermaid попали только корректные строки</source>
        <translation>The text has errors — only the correct lines are in the Mermaid diagram</translation>
    </message>
    <message>
        <source>Экспорт Mermaid</source>
        <translation>Export Mermaid</translation>
    </message>
    <message>
        <source>Схема Mermaid (*.mmd)</source>
        <translation>Mermaid diagram (*.mmd)</translation>
    </message>
    <message>
        <source>Mermaid скопирован — вставьте его в README на GitHub, в Notion или Obsidian</source>
        <translation>Mermaid copied — paste it into a README on GitHub, into Notion or Obsidian</translation>
    </message>
    <message>
        <source>Открыть схему</source>
        <translation>Open Diagram</translation>
    </message>
    <message>
        <source>Схемы Birch (*.birch *.txt);;Все файлы (*)</source>
        <translation>Birch diagrams (*.birch *.txt);;All files (*)</translation>
    </message>
    <message>
        <source>Файл слишком большой</source>
        <translation>File Too Large</translation>
    </message>
    <message>
        <source>Файл «%1» больше %2 МБ.</source>
        <translation>The file “%1” is larger than %2 MB.</translation>
    </message>
    <message>
        <source>Не удалось открыть</source>
        <translation>Could Not Open</translation>
    </message>
    <message>
        <source>%1

%2</source>
        <translation>%1

%2</translation>
    </message>
    <message>
        <source>Сохранить схему</source>
        <translation>Save Diagram</translation>
    </message>
    <message>
        <source>Схемы Birch (*.birch)</source>
        <translation>Birch diagrams (*.birch)</translation>
    </message>
    <message>
        <source>Сохранить изменения?</source>
        <translation>Save changes?</translation>
    </message>
    <message>
        <source>В файле «%1» есть несохранённые изменения.</source>
        <translation>“%1” has unsaved changes.</translation>
    </message>
    <message>
        <source>Без названия</source>
        <translation>Untitled</translation>
    </message>
    <message>
        <source>Синтаксис Birch</source>
        <translation>Birch Syntax</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Синтаксис Birch&lt;/h3&gt;&lt;table cellspacing=&apos;6&apos;&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[Текст]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;шаг (прямоугольник)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;{Условие?}&lt;/code&gt;&lt;/td&gt;&lt;td&gt;решение (ромб)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;(Старт)&lt;/code&gt;&lt;/td&gt;&lt;td&gt;начало / конец&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;стрелка; можно цепочкой: &lt;code&gt;[A] -&amp;gt; [B] -&amp;gt; [C]&lt;/code&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B] &quot;Да&quot;&lt;/code&gt;&lt;/td&gt;&lt;td&gt;подпись стрелки&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;;&lt;/code&gt; или Enter&lt;/td&gt;&lt;td&gt;конец инструкции&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;// …&lt;/code&gt; или &lt;code&gt;# …&lt;/code&gt;&lt;/td&gt;&lt;td&gt;комментарий&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;\n&lt;/code&gt;&lt;/td&gt;&lt;td&gt;перенос строки в тексте&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Рамка=&lt;/code&gt;&lt;/td&gt;&lt;td&gt;на отдельной строке: открыть рамку; &lt;code&gt;==&lt;/code&gt; — закрыть&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Рамка= [A]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;блок из рамки «Рамка»; &lt;code&gt;== [A]&lt;/code&gt; — общий блок&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;&lt;p&gt;Одинаковый текст — один и тот же узел, но в каждой рамке свои блоки: «[Шаг]» в двух рамках — два разных блока. Пример — «Файл → Открыть пример с рамками».&lt;/p&gt;&lt;p&gt;Если узел хоть раз записан как &lt;code&gt;{…}&lt;/code&gt; или &lt;code&gt;(…)&lt;/code&gt;, он останется этой фигурой.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Birch Syntax&lt;/h3&gt;&lt;table cellspacing=&apos;6&apos;&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[Text]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;step (rectangle)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;{Condition?}&lt;/code&gt;&lt;/td&gt;&lt;td&gt;decision (diamond)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;(Start)&lt;/code&gt;&lt;/td&gt;&lt;td&gt;start / end&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;arrow; chains work too: &lt;code&gt;[A] -&amp;gt; [B] -&amp;gt; [C]&lt;/code&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B] &quot;Yes&quot;&lt;/code&gt;&lt;/td&gt;&lt;td&gt;arrow label&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;;&lt;/code&gt; or Enter&lt;/td&gt;&lt;td&gt;end of statement&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;// …&lt;/code&gt; or &lt;code&gt;# …&lt;/code&gt;&lt;/td&gt;&lt;td&gt;comment&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;\n&lt;/code&gt;&lt;/td&gt;&lt;td&gt;line break inside text&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Frame=&lt;/code&gt;&lt;/td&gt;&lt;td&gt;on its own line: open a frame; &lt;code&gt;==&lt;/code&gt; closes it&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Frame= [A]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;block from the frame “Frame”; &lt;code&gt;== [A]&lt;/code&gt; is a shared block&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;&lt;p&gt;The same text is the same node, but each frame has its own blocks: “[Step]” in two frames is two different blocks. See File → Open Frames Example.&lt;/p&gt;&lt;p&gt;If a node is written as &lt;code&gt;{…}&lt;/code&gt; or &lt;code&gt;(…)&lt;/code&gt; at least once, it keeps that shape.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Birch %1&lt;/h3&gt;&lt;p&gt;Блок-схемы из текста — без ИИ, полностью офлайн.&lt;/p&gt;&lt;p&gt;Graphviz %2 · Qt %3&lt;/p&gt;&lt;p&gt;Распространяется по лицензии &lt;a href=&quot;https://mozilla.org/MPL/2.0/&quot;&gt;Mozilla Public License 2.0&lt;/a&gt;.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Birch %1&lt;/h3&gt;&lt;p&gt;Flowcharts from text — no AI, fully offline.&lt;/p&gt;&lt;p&gt;Graphviz %2 · Qt %3&lt;/p&gt;&lt;p&gt;Distributed under the &lt;a href=&quot;https://mozilla.org/MPL/2.0/&quot;&gt;Mozilla Public License 2.0&lt;/a&gt;.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Язык</source>
        <translation>Language</translation>
    </message>
    <message>
        <source>Язык интерфейса изменится после перезапуска Birch.</source>
        <translation>The interface language will change after you restart Birch.</translation>
    </message>
</context>
</TS>
