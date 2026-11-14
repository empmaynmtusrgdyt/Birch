<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="de" sourcelanguage="ru">
<context>
    <name>CommandLine</name>
    <message>
        <source>неизвестный ключ «%1»</source>
        <translation>unbekannte Option „%1“</translation>
    </message>
    <message>
        <source>ключу «%1» нужно значение</source>
        <translation>Option „%1“ erfordert ein Argument</translation>
    </message>
    <message>
        <source>лишний аргумент «%1»: можно указать только один файл</source>
        <translation>überflüssiger Operand „%1“: es kann nur eine Datei angegeben werden</translation>
    </message>
    <message>
        <source>ключ «%1» работает только вместе с -o</source>
        <translation>Option „%1“ funktioniert nur zusammen mit -o</translation>
    </message>
    <message>
        <source>чтение схемы со стандартного ввода («-») работает только вместе с -o</source>
        <translation>das Lesen des Diagramms von der Standardeingabe („-“) funktioniert nur zusammen mit -o</translation>
    </message>
    <message>
        <source>ключу «-o» нужно значение</source>
        <translation>Option „-o“ erfordert ein Argument</translation>
    </message>
    <message>
        <source>неизвестный формат файла «%1»: поддерживаются .png, .svg и .mmd</source>
        <translation>unbekanntes Dateiformat „%1“: unterstützt werden .png, .svg und .mmd</translation>
    </message>
    <message>
        <source>не указан файл схемы</source>
        <translation>keine Diagrammdatei angegeben</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--theme»: допустимы light и dark</source>
        <translation>ungültiger Wert „%1“ für Option „--theme“: gültig sind light und dark</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--scale»: нужно целое число от 1 до %2</source>
        <translation>ungültiger Wert „%1“ für Option „--scale“: erwartet wird eine ganze Zahl von 1 bis %2</translation>
    </message>
    <message>
        <source>Использование: %1 [ПАРАМЕТР]... [ФАЙЛ]
Открывает ФАЙЛ (.birch) в редакторе или, с ключом -o, рисует по нему
блок-схему в файл, не открывая окна.
Если ФАЙЛ — «-», текст схемы читается со стандартного ввода.

Аргументы, обязательные для длинных ключей, обязательны и для коротких.
  -o, --output=ФАЙЛ        сохранить схему в ФАЙЛ и выйти; формат по
                             расширению: .png, .svg или .mmd (Mermaid)
  -t, --theme=ТЕМА         тема: light (по умолчанию) или dark
  -m, --monochrome         чёрно-белая схема
  -H, --horizontal         располагать блоки слева направо
  -a, --transparent        прозрачный фон (PNG и SVG)
  -s, --scale=N            масштаб PNG от 1 до 10 (по умолчанию 2)
  -h, --help               показать эту справку и выйти
  -v, --version            показать информацию о версии и выйти

Примеры:
  %1 схема.birch                       открыть схему в редакторе
  %1 схема.birch -o схема.png          нарисовать схему в PNG
  %1 -t dark -s 3 схема.birch -o схема.png
  cat схема.birch | %1 - -o схема.svg

Коды выхода:
  0  успешно
  1  в тексте схемы есть ошибки или схема пуста
  2  ошибка чтения или записи файла
  3  неверные ключи командной строки

Домашняя страница: &lt;https://github.com/empmaynmtusrgdyt/Birch&gt;
Об ошибках сообщайте: &lt;https://github.com/empmaynmtusrgdyt/Birch/issues&gt;
</source>
        <translation>Aufruf: %1 [OPTION]... [DATEI]
Öffnet DATEI (.birch) im Editor oder zeichnet mit -o daraus ein
Flussdiagramm in eine Datei, ohne ein Fenster zu öffnen.
Ist DATEI „-“, wird der Diagrammtext von der Standardeingabe gelesen.

Erforderliche Argumente für lange Optionen gelten auch für kurze.
  -o, --output=DATEI       Diagramm in DATEI speichern und beenden; die
                             Endung bestimmt das Format: .png, .svg oder .mmd
  -t, --theme=THEMA        Thema: light (Standard) oder dark
  -m, --monochrome         Diagramm in Schwarz-Weiß
  -H, --horizontal         Blöcke von links nach rechts anordnen
  -a, --transparent        transparenter Hintergrund (PNG und SVG)
  -s, --scale=N            PNG-Maßstab von 1 bis 10 (Standard 2)
  -h, --help               diese Hilfe anzeigen und beenden
  -v, --version            Versionsinformation anzeigen und beenden

Beispiele:
  %1 diagramm.birch                  Diagramm im Editor öffnen
  %1 diagramm.birch -o diagramm.png  Diagramm als PNG zeichnen
  %1 -t dark -s 3 diagramm.birch -o diagramm.png
  cat diagramm.birch | %1 - -o diagramm.svg

Exit-Status:
  0  Erfolg
  1  der Diagrammtext enthält Fehler oder das Diagramm ist leer
  2  Fehler beim Lesen oder Schreiben einer Datei
  3  ungültige Befehlszeilenoptionen

Homepage: &lt;https://github.com/empmaynmtusrgdyt/Birch&gt;
Fehler melden: &lt;https://github.com/empmaynmtusrgdyt/Birch/issues&gt;
</translation>
    </message>
    <message>
        <source>Лицензия MPL 2.0: Mozilla Public License 2.0 &lt;https://mozilla.org/MPL/2.0/&gt;
Это свободное ПО: вы можете изменять и распространять его.
НИКАКИХ ГАРАНТИЙ не предоставляется в пределах, допускаемых законом.

Используются: Qt %1, Graphviz %2
</source>
        <translation>Lizenz MPL 2.0: Mozilla Public License 2.0 &lt;https://mozilla.org/MPL/2.0/&gt;
Dies ist freie Software: Sie dürfen sie ändern und weitergeben.
Es gibt KEINE GARANTIE, soweit gesetzlich zulässig.

Verwendet: Qt %1, Graphviz %2
</translation>
    </message>
    <message>
        <source>%1: %2
Попробуйте «%1 --help» для получения дополнительной информации.
</source>
        <translation>%1: %2
„%1 --help“ liefert weitere Informationen.
</translation>
    </message>
    <message>
        <source>%1: не удалось открыть «%2»: %3
</source>
        <translation>%1: „%2“ kann nicht geöffnet werden: %3
</translation>
    </message>
    <message>
        <source>ошибка</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <source>предупреждение</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>%1: схема пуста — нечего рисовать
</source>
        <translation>%1: das Diagramm ist leer — nichts zu zeichnen
</translation>
    </message>
    <message>
        <source>%1: не удалось записать «%2»: %3
</source>
        <translation>%1: „%2“ kann nicht geschrieben werden: %3
</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--screenshot-theme»</source>
        <translation>ungültiger Wert „%1“ für Option „--screenshot-theme“</translation>
    </message>
</context>
<context>
    <name>InstallCommand</name>
    <message>
        <source>Откройте новое окно терминала и попробуйте:

    birch --help
    birch схема.birch -o схема.png</source>
        <translation>Öffnen Sie ein neues Terminalfenster und versuchen Sie:

    birch --help
    birch diagramm.birch -o diagramm.png</translation>
    </message>
    <message>
        <source>Команда birch установлена: %1.</source>
        <translation>Der Befehl birch ist installiert: %1.</translation>
    </message>
    <message>
        <source>

Папки ~/.local/bin пока нет в PATH. Обычно она появляется там после повторного входа в систему. Если нет — добавьте в ~/.bashrc строку:

    export PATH=&quot;$HOME/.local/bin:$PATH&quot;</source>
        <translation>

~/.local/bin ist noch nicht in Ihrem PATH. Meist erscheint es dort nach einer erneuten Anmeldung. Falls nicht, fügen Sie diese Zeile in ~/.bashrc ein:

    export PATH=&quot;$HOME/.local/bin:$PATH&quot;</translation>
    </message>
    <message>
        <source>

Если переместите файл AppImage, выберите этот пункт ещё раз.</source>
        <translation>

Wenn Sie die AppImage-Datei verschieben, wählen Sie diesen Menüpunkt erneut.</translation>
    </message>
    <message>
        <source>Команда birch</source>
        <translation>Befehl birch</translation>
    </message>
    <message>
        <source>Birch установит команду birch в /usr/local/bin.</source>
        <translation>Birch installiert den Befehl birch in /usr/local/bin.</translation>
    </message>
    <message>
        <source>Не удалось установить команду birch.

%1</source>
        <translation>Der Befehl birch konnte nicht installiert werden.

%1</translation>
    </message>
    <message>
        <source>Birch запущен не из папки «Программы», поэтому команда на него перестанет работать.

Перетащите Birch в «Программы», откройте его оттуда и выберите этот пункт ещё раз. Если Birch уже там, выполните в Терминале:

    xattr -dr com.apple.quarantine /Applications/Birch.app</source>
        <translation>Birch wird nicht aus dem Ordner „Programme“ ausgeführt, daher würde ein Befehl, der darauf verweist, nicht mehr funktionieren.

Ziehen Sie Birch in „Programme“, öffnen Sie es von dort und wählen Sie diesen Menüpunkt erneut. Falls Birch bereits dort ist, führen Sie im Terminal aus:

    xattr -dr com.apple.quarantine /Applications/Birch.app</translation>
    </message>
    <message>
        <source>Команда birch уже установлена: %1.</source>
        <translation>Der Befehl birch ist bereits installiert: %1.</translation>
    </message>
    <message>
        <source>По пути %1 уже есть другой файл. Заменить его командой Birch?</source>
        <translation>Unter %1 gibt es bereits eine andere Datei. Durch den Befehl birch ersetzen?</translation>
    </message>
    <message>
        <source>Не удалось установить команду birch: %1.</source>
        <translation>Der Befehl birch konnte nicht installiert werden: %1.</translation>
    </message>
</context>
<context>
    <name>Language</name>
    <message>
        <source>Русский</source>
        <extracomment>Название языка этого перевода на нём самом: Deutsch, Español, 日本語</extracomment>
        <translation>Deutsch</translation>
    </message>
</context>
<context>
    <name>Parser</name>
    <message>
        <source>прямоугольник [ ]</source>
        <translation>Rechteck [ ]</translation>
    </message>
    <message>
        <source>ромб { }</source>
        <translation>Raute { }</translation>
    </message>
    <message>
        <source>овал ( )</source>
        <translation>Oval ( )</translation>
    </message>
    <message>
        <source>Непонятный текст «%1». Узлы пишутся в скобках: [Шаг], {Условие?}, (Старт)</source>
        <translation>Unerwarteter Text „%1“. Knoten werden in Klammern geschrieben: [Schritt], {Bedingung?}, (Start)</translation>
    </message>
    <message>
        <source>Не закрыта скобка «%1» — ожидалась «%2» до конца строки</source>
        <translation>„%1“ nicht geschlossen — „%2“ vor dem Zeilenende erwartet</translation>
    </message>
    <message>
        <source>Пустой узел «%1%2» — добавьте текст</source>
        <translation>Leerer Knoten „%1%2“ — fügen Sie Text hinzu</translation>
    </message>
    <message>
        <source>Слишком длинный текст узла (больше %1 символов)</source>
        <translation>Knotentext ist zu lang (mehr als %1 Zeichen)</translation>
    </message>
    <message>
        <source>Не закрыт заголовок рамки — ожидался «=» в конце, например =Авторизация=</source>
        <translation>Rahmentitel nicht geschlossen — „=“ am Ende erwartet, zum Beispiel =Anmeldung=</translation>
    </message>
    <message>
        <source>Непонятная запись «%1». Рамка открывается так: =Название=, а закрывается так: ==</source>
        <translation>Unerwartetes „%1“. Ein Rahmen wird mit =Name= geöffnet und mit == geschlossen</translation>
    </message>
    <message>
        <source>Слишком длинное название рамки (больше %1 символов)</source>
        <translation>Rahmenname ist zu lang (mehr als %1 Zeichen)</translation>
    </message>
    <message>
        <source>Не закрыта кавычка — ожидалась «%1» до конца строки</source>
        <translation>Anführungszeichen nicht geschlossen — „%1“ vor dem Zeilenende erwartet</translation>
    </message>
    <message>
        <source>Слишком длинная подпись (больше %1 символов)</source>
        <translation>Beschriftung ist zu lang (mehr als %1 Zeichen)</translation>
    </message>
    <message>
        <source>узел «%1»</source>
        <translation>Knoten „%1“</translation>
    </message>
    <message>
        <source>подпись «%1»</source>
        <translation>Beschriftung „%1“</translation>
    </message>
    <message>
        <source>рамка «=%1=»</source>
        <translation>Rahmen „=%1=“</translation>
    </message>
    <message>
        <source>конец строки</source>
        <translation>Zeilenende</translation>
    </message>
    <message>
        <source>конец текста</source>
        <translation>Textende</translation>
    </message>
    <message>
        <source>ошибочный фрагмент</source>
        <translation>ungültiges Fragment</translation>
    </message>
    <message>
        <source>После «%1» в той же строке может идти только блок, например %1 [Шаг]. Чтобы открыть рамку, напишите %1 на отдельной строке</source>
        <translation>Nach „%1“ darf in derselben Zeile nur ein Block folgen, zum Beispiel %1 [Schritt]. Um einen Rahmen zu öffnen, schreiben Sie %1 in eine eigene Zeile</translation>
    </message>
    <message>
        <source>Рамка «%1» не найдена. Рамка открывается на отдельной строке: =%1=</source>
        <translation>Rahmen „%1“ nicht gefunden. Ein Rahmen wird in einer eigenen Zeile geöffnet: =%1=</translation>
    </message>
    <message>
        <source>%1, а найдено %2</source>
        <translation>%1, gefunden wurde aber %2</translation>
    </message>
    <message>
        <source>Заголовок рамки пишется на отдельной строке: %1</source>
        <translation>Ein Rahmentitel steht in einer eigenen Zeile: %1</translation>
    </message>
    <message>
        <source>Инструкция должна начинаться с узла, например [Шаг], а найдено %1</source>
        <translation>Eine Anweisung muss mit einem Knoten beginnen, zum Beispiel [Schritt], gefunden wurde aber %1</translation>
    </message>
    <message>
        <source>Ожидался узел, например [Шаг]</source>
        <translation>Knoten erwartet, zum Beispiel [Schritt]</translation>
    </message>
    <message>
        <source>Подпись должна идти после стрелки и узла: [A] -&gt; [B] &quot;Да&quot;</source>
        <translation>Eine Beschriftung steht nach dem Pfeil und dem Knoten: [A] -&gt; [B] &quot;Ja&quot;</translation>
    </message>
    <message>
        <source>После «-&gt;» ожидался узел</source>
        <translation>Knoten nach „-&gt;“ erwartet</translation>
    </message>
    <message>
        <source>Ожидались «-&gt;», «;» или новая строка, а найдено %1</source>
        <translation>„-&gt;“, „;“ oder neue Zeile erwartet, gefunden wurde aber %1</translation>
    </message>
    <message>
        <source>Слишком много узлов (максимум %1)</source>
        <translation>Zu viele Knoten (höchstens %1)</translation>
    </message>
    <message>
        <source>Узел «%1» уже объявлен как %2 — форма %3 проигнорирована</source>
        <translation>Knoten „%1“ ist bereits als %2 deklariert — Form %3 wird ignoriert</translation>
    </message>
    <message>
        <source>Слишком много связей (максимум %1)</source>
        <translation>Zu viele Kanten (höchstens %1)</translation>
    </message>
    <message>
        <source>Блок «%1» есть среди общих блоков. Чтобы провести стрелку к нему, напишите == %2</source>
        <translation>Block „%1“ ist ein gemeinsamer Block. Um einen Pfeil zu ihm zu zeichnen, schreiben Sie == %2</translation>
    </message>
    <message>
        <source>Текст слишком большой (больше %1 символов)</source>
        <translation>Der Text ist zu groß (mehr als %1 Zeichen)</translation>
    </message>
</context>
<context>
    <name>Renderer</name>
    <message>
        <source>Не удалось инициализировать Graphviz</source>
        <translation>Graphviz konnte nicht initialisiert werden</translation>
    </message>
    <message>
        <source>Graphviz: не удалось создать граф</source>
        <translation>Graphviz: Graph konnte nicht erstellt werden</translation>
    </message>
    <message>
        <source>Graphviz: ошибка раскладки. %1</source>
        <translation>Graphviz: Layoutfehler. %1</translation>
    </message>
    <message>
        <source>Graphviz: ошибка рендеринга SVG. %1</source>
        <translation>Graphviz: Fehler beim Erzeugen des SVG. %1</translation>
    </message>
    <message>
        <source>Не удалось разобрать SVG от Graphviz</source>
        <translation>Das SVG von Graphviz konnte nicht gelesen werden</translation>
    </message>
    <message>
        <source>Недостаточно памяти для картинки %1×%2</source>
        <translation>Nicht genug Speicher für ein Bild von %1×%2</translation>
    </message>
</context>
<context>
    <name>ShellCommand</name>
    <message>
        <source>не удалось создать папку «%1»</source>
        <translation>Ordner „%1“ konnte nicht erstellt werden</translation>
    </message>
    <message>
        <source>не удалось заменить «%1»: %2</source>
        <translation>„%1“ konnte nicht ersetzt werden: %2</translation>
    </message>
    <message>
        <source>не удалось создать «%1»: %2</source>
        <translation>„%1“ konnte nicht erstellt werden: %2</translation>
    </message>
</context>
<context>
    <name>birch::CodeEditor</name>
    <message>
        <source>Опишите блок-схему, например:

[Старт] -&gt; [Шаг] -&gt; [Готово];</source>
        <translation>Beschreiben Sie ein Flussdiagramm, zum Beispiel:

[Start] -&gt; [Schritt] -&gt; [Fertig];</translation>
    </message>
    <message>
        <source>Ошибка</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <source>Предупреждение</source>
        <translation>Warnung</translation>
    </message>
</context>
<context>
    <name>birch::FindBar</name>
    <message numerus="yes">
        <source>%n совпадений</source>
        <translation>
            <numerusform>%n Treffer</numerusform>
            <numerusform>%n Treffer</numerusform>
        </translation>
    </message>
    <message>
        <source>Найти</source>
        <translation>Suchen</translation>
    </message>
    <message>
        <source>Заменить на</source>
        <translation>Ersetzen durch</translation>
    </message>
    <message>
        <source>Предыдущее совпадение</source>
        <translation>Vorheriger Treffer</translation>
    </message>
    <message>
        <source>Следующее совпадение</source>
        <translation>Nächster Treffer</translation>
    </message>
    <message>
        <source>Учитывать регистр</source>
        <translation>Groß-/Kleinschreibung beachten</translation>
    </message>
    <message>
        <source>Регулярное выражение. В замене $1, $2… — найденные группы</source>
        <translation>Regulärer Ausdruck. In der Ersetzung stehen $1, $2… für die gefundenen Gruppen</translation>
    </message>
    <message>
        <source>Закрыть (Esc)</source>
        <translation>Schließen (Esc)</translation>
    </message>
    <message>
        <source>Заменить</source>
        <translation>Ersetzen</translation>
    </message>
    <message>
        <source>Заменить все</source>
        <translation>Alle ersetzen</translation>
    </message>
    <message>
        <source>Заменено: %1</source>
        <translation>Ersetzt: %1</translation>
    </message>
    <message>
        <source>Ошибка</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <source>Не найдено</source>
        <translation>Nicht gefunden</translation>
    </message>
    <message>
        <source>%1 из %2</source>
        <translation>%1 von %2</translation>
    </message>
    <message>
        <source>Ошибка в регулярном выражении: %1</source>
        <translation>Fehler im regulären Ausdruck: %1</translation>
    </message>
</context>
<context>
    <name>birch::MainWindow</name>
    <message>
        <source>Новый</source>
        <translation>Neu</translation>
    </message>
    <message>
        <source>Открыть…</source>
        <translation>Öffnen…</translation>
    </message>
    <message>
        <source>Сохранить</source>
        <translation>Speichern</translation>
    </message>
    <message>
        <source>Сохранить как…</source>
        <translation>Speichern unter…</translation>
    </message>
    <message>
        <source>Открыть пример</source>
        <translation>Beispiel öffnen</translation>
    </message>
    <message>
        <source>Открыть пример с рамками</source>
        <translation>Beispiel mit Rahmen öffnen</translation>
    </message>
    <message>
        <source>Выход</source>
        <translation>Beenden</translation>
    </message>
    <message>
        <source>Отменить</source>
        <translation>Rückgängig</translation>
    </message>
    <message>
        <source>Повторить</source>
        <translation>Wiederholen</translation>
    </message>
    <message>
        <source>Вырезать</source>
        <translation>Ausschneiden</translation>
    </message>
    <message>
        <source>Копировать</source>
        <translation>Kopieren</translation>
    </message>
    <message>
        <source>Вставить</source>
        <translation>Einfügen</translation>
    </message>
    <message>
        <source>Выбрать все</source>
        <translation>Alles auswählen</translation>
    </message>
    <message>
        <source>Найти…</source>
        <translation>Suchen…</translation>
    </message>
    <message>
        <source>Заменить…</source>
        <translation>Ersetzen…</translation>
    </message>
    <message>
        <source>Найти далее</source>
        <translation>Weitersuchen</translation>
    </message>
    <message>
        <source>Найти назад</source>
        <translation>Rückwärts suchen</translation>
    </message>
    <message>
        <source>Рендерить</source>
        <translation>Zeichnen</translation>
    </message>
    <message>
        <source>Перерисовать схему (%1)</source>
        <translation>Diagramm neu zeichnen (%1)</translation>
    </message>
    <message>
        <source>Автоматический рендеринг</source>
        <translation>Automatisch zeichnen</translation>
    </message>
    <message>
        <source>Скачать PNG…</source>
        <translation>PNG speichern…</translation>
    </message>
    <message>
        <source>Сохранить картинку в файл .png (%1)</source>
        <translation>Bild als .png-Datei speichern (%1)</translation>
    </message>
    <message>
        <source>Экспорт SVG…</source>
        <translation>SVG exportieren…</translation>
    </message>
    <message>
        <source>Экспорт Mermaid…</source>
        <translation>Mermaid exportieren…</translation>
    </message>
    <message>
        <source>Копировать как Mermaid</source>
        <translation>Als Mermaid kopieren</translation>
    </message>
    <message>
        <source>Текст схемы для README на GitHub, Notion, Obsidian — там он отображается как схема</source>
        <translation>Diagrammtext für ein README auf GitHub, Notion oder Obsidian — dort wird er als Diagramm angezeigt</translation>
    </message>
    <message>
        <source>Копировать в буфер</source>
        <translation>In die Zwischenablage</translation>
    </message>
    <message>
        <source>Скопировать PNG — можно сразу вставить в Telegram, Slack, Word (%1)</source>
        <translation>Als PNG kopieren — direkt in Telegram, Slack oder Word einfügen (%1)</translation>
    </message>
    <message>
        <source>Горизонтальная схема</source>
        <translation>Horizontales Layout</translation>
    </message>
    <message>
        <source>Располагать блоки слева направо</source>
        <translation>Blöcke von links nach rechts anordnen</translation>
    </message>
    <message>
        <source>Чёрно-белая схема</source>
        <translation>Schwarz-Weiß</translation>
    </message>
    <message>
        <source>Ч/Б</source>
        <translation>S/W</translation>
    </message>
    <message>
        <source>Чёрно-белая схема — для печати и документов (%1)</source>
        <translation>Diagramm in Schwarz-Weiß — für Druck und Dokumente (%1)</translation>
    </message>
    <message>
        <source>Прозрачный фон</source>
        <translation>Transparenter Hintergrund</translation>
    </message>
    <message>
        <source>Увеличить</source>
        <translation>Vergrößern</translation>
    </message>
    <message>
        <source>Уменьшить</source>
        <translation>Verkleinern</translation>
    </message>
    <message>
        <source>Реальный размер</source>
        <translation>Originalgröße</translation>
    </message>
    <message>
        <source>По размеру окна</source>
        <translation>An Fenster anpassen</translation>
    </message>
    <message>
        <source>Как в системе</source>
        <translation>Systemeinstellung</translation>
    </message>
    <message>
        <source>Светлая</source>
        <translation>Hell</translation>
    </message>
    <message>
        <source>Тёмная</source>
        <translation>Dunkel</translation>
    </message>
    <message>
        <source>Как в превью</source>
        <translation>Wie die Vorschau</translation>
    </message>
    <message>
        <source>Всегда светлая</source>
        <translation>Immer hell</translation>
    </message>
    <message>
        <source>Всегда тёмная</source>
        <translation>Immer dunkel</translation>
    </message>
    <message>
        <source>Синтаксис…</source>
        <translation>Syntax…</translation>
    </message>
    <message>
        <source>О программе Birch</source>
        <translation>Über Birch</translation>
    </message>
    <message>
        <source>Установить команду birch…</source>
        <translation>Befehl birch installieren…</translation>
    </message>
    <message>
        <source>Файл</source>
        <translation>Datei</translation>
    </message>
    <message>
        <source>Правка</source>
        <translation>Bearbeiten</translation>
    </message>
    <message>
        <source>Вид</source>
        <translation>Ansicht</translation>
    </message>
    <message>
        <source>Тема</source>
        <translation>Thema</translation>
    </message>
    <message>
        <source>Язык</source>
        <translation>Sprache</translation>
    </message>
    <message>
        <source>Экспорт картинки</source>
        <translation>Bildexport</translation>
    </message>
    <message>
        <source>Тема картинки</source>
        <translation>Bildthema</translation>
    </message>
    <message>
        <source>Справка</source>
        <translation>Hilfe</translation>
    </message>
    <message>
        <source>Панель инструментов</source>
        <translation>Werkzeugleiste</translation>
    </message>
    <message>
        <source>Схема появится здесь.
Опишите её слева, например:
[Старт] -&gt; [Готово]</source>
        <translation>Hier erscheint das Diagramm.
Beschreiben Sie es links, zum Beispiel:
[Start] -&gt; [Fertig]</translation>
    </message>
    <message>
        <source>Узлов: %1 · связей: %2 · %3 мс</source>
        <translation>Knoten: %1 · Kanten: %2 · %3 ms</translation>
    </message>
    <message>
        <source>Строка %1: %2</source>
        <translation>Zeile %1: %2</translation>
    </message>
    <message>
        <source>Нечего экспортировать — схема пуста</source>
        <translation>Nichts zu exportieren — das Diagramm ist leer</translation>
    </message>
    <message>
        <source>Ошибка рендеринга</source>
        <translation>Fehler beim Zeichnen</translation>
    </message>
    <message>
        <source>В тексте есть ошибки — в картинку попали только корректные строки</source>
        <translation>Der Text enthält Fehler — im Bild sind nur die korrekten Zeilen</translation>
    </message>
    <message>
        <source>Скачать PNG</source>
        <translation>PNG speichern</translation>
    </message>
    <message>
        <source>Изображение PNG (*.png)</source>
        <translation>PNG-Bild (*.png)</translation>
    </message>
    <message>
        <source>Не удалось сохранить</source>
        <translation>Speichern fehlgeschlagen</translation>
    </message>
    <message>
        <source>Не удалось записать файл:
%1

%2</source>
        <translation>Die Datei konnte nicht geschrieben werden:
%1

%2</translation>
    </message>
    <message>
        <source>Сохранено: %1 (%2×%3)</source>
        <translation>Gespeichert: %1 (%2×%3)</translation>
    </message>
    <message>
        <source>Экспорт SVG</source>
        <translation>SVG exportieren</translation>
    </message>
    <message>
        <source>Векторное изображение SVG (*.svg)</source>
        <translation>SVG-Vektorgrafik (*.svg)</translation>
    </message>
    <message>
        <source>Сохранено: %1</source>
        <translation>Gespeichert: %1</translation>
    </message>
    <message>
        <source>Картинка скопирована — вставьте её в Telegram, Slack, Word…</source>
        <translation>Bild kopiert — in Telegram, Slack, Word … einfügen</translation>
    </message>
    <message>
        <source>В тексте есть ошибки — в Mermaid попали только корректные строки</source>
        <translation>Der Text enthält Fehler — im Mermaid-Diagramm sind nur die korrekten Zeilen</translation>
    </message>
    <message>
        <source>Экспорт Mermaid</source>
        <translation>Mermaid exportieren</translation>
    </message>
    <message>
        <source>Схема Mermaid (*.mmd)</source>
        <translation>Mermaid-Diagramm (*.mmd)</translation>
    </message>
    <message>
        <source>Mermaid скопирован — вставьте его в README на GitHub, в Notion или Obsidian</source>
        <translation>Mermaid kopiert — in ein README auf GitHub, in Notion oder Obsidian einfügen</translation>
    </message>
    <message>
        <source>Открыть схему</source>
        <translation>Diagramm öffnen</translation>
    </message>
    <message>
        <source>Схемы Birch (*.birch *.txt);;Все файлы (*)</source>
        <translation>Birch-Diagramme (*.birch *.txt);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Файл слишком большой</source>
        <translation>Datei zu groß</translation>
    </message>
    <message>
        <source>Файл «%1» больше %2 МБ.</source>
        <translation>Die Datei „%1“ ist größer als %2 MB.</translation>
    </message>
    <message>
        <source>Не удалось открыть</source>
        <translation>Öffnen fehlgeschlagen</translation>
    </message>
    <message>
        <source>%1

%2</source>
        <translation>%1

%2</translation>
    </message>
    <message>
        <source>Сохранить схему</source>
        <translation>Diagramm speichern</translation>
    </message>
    <message>
        <source>Схемы Birch (*.birch)</source>
        <translation>Birch-Diagramme (*.birch)</translation>
    </message>
    <message>
        <source>Сохранить изменения?</source>
        <translation>Änderungen speichern?</translation>
    </message>
    <message>
        <source>В файле «%1» есть несохранённые изменения.</source>
        <translation>„%1“ enthält ungespeicherte Änderungen.</translation>
    </message>
    <message>
        <source>Без названия</source>
        <translation>Unbenannt</translation>
    </message>
    <message>
        <source>Синтаксис Birch</source>
        <translation>Birch-Syntax</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Синтаксис Birch&lt;/h3&gt;&lt;table cellspacing=&apos;6&apos;&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[Текст]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;шаг (прямоугольник)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;{Условие?}&lt;/code&gt;&lt;/td&gt;&lt;td&gt;решение (ромб)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;(Старт)&lt;/code&gt;&lt;/td&gt;&lt;td&gt;начало / конец&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;стрелка; можно цепочкой: &lt;code&gt;[A] -&amp;gt; [B] -&amp;gt; [C]&lt;/code&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B] &quot;Да&quot;&lt;/code&gt;&lt;/td&gt;&lt;td&gt;подпись стрелки&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;;&lt;/code&gt; или Enter&lt;/td&gt;&lt;td&gt;конец инструкции&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;// …&lt;/code&gt; или &lt;code&gt;# …&lt;/code&gt;&lt;/td&gt;&lt;td&gt;комментарий&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;\n&lt;/code&gt;&lt;/td&gt;&lt;td&gt;перенос строки в тексте&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Рамка=&lt;/code&gt;&lt;/td&gt;&lt;td&gt;на отдельной строке: открыть рамку; &lt;code&gt;==&lt;/code&gt; — закрыть&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Рамка= [A]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;блок из рамки «Рамка»; &lt;code&gt;== [A]&lt;/code&gt; — общий блок&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;&lt;p&gt;Одинаковый текст — один и тот же узел, но в каждой рамке свои блоки: «[Шаг]» в двух рамках — два разных блока. Пример — «Файл → Открыть пример с рамками».&lt;/p&gt;&lt;p&gt;Если узел хоть раз записан как &lt;code&gt;{…}&lt;/code&gt; или &lt;code&gt;(…)&lt;/code&gt;, он останется этой фигурой.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Birch-Syntax&lt;/h3&gt;&lt;table cellspacing=&apos;6&apos;&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[Text]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;Schritt (Rechteck)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;{Bedingung?}&lt;/code&gt;&lt;/td&gt;&lt;td&gt;Entscheidung (Raute)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;(Start)&lt;/code&gt;&lt;/td&gt;&lt;td&gt;Start / Ende&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;Pfeil; auch als Kette: &lt;code&gt;[A] -&amp;gt; [B] -&amp;gt; [C]&lt;/code&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B] &quot;Ja&quot;&lt;/code&gt;&lt;/td&gt;&lt;td&gt;Pfeilbeschriftung&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;;&lt;/code&gt; oder Enter&lt;/td&gt;&lt;td&gt;Ende der Anweisung&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;// …&lt;/code&gt; oder &lt;code&gt;# …&lt;/code&gt;&lt;/td&gt;&lt;td&gt;Kommentar&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;\n&lt;/code&gt;&lt;/td&gt;&lt;td&gt;Zeilenumbruch im Text&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Rahmen=&lt;/code&gt;&lt;/td&gt;&lt;td&gt;in eigener Zeile: Rahmen öffnen; &lt;code&gt;==&lt;/code&gt; schließt ihn&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Rahmen= [A]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;Block aus dem Rahmen „Rahmen“; &lt;code&gt;== [A]&lt;/code&gt; ist ein gemeinsamer Block&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;&lt;p&gt;Gleicher Text ist derselbe Knoten, aber jeder Rahmen hat eigene Blöcke: „[Schritt]“ in zwei Rahmen sind zwei verschiedene Blöcke. Siehe Datei → Beispiel mit Rahmen öffnen.&lt;/p&gt;&lt;p&gt;Wird ein Knoten mindestens einmal als &lt;code&gt;{…}&lt;/code&gt; oder &lt;code&gt;(…)&lt;/code&gt; geschrieben, behält er diese Form.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Язык интерфейса изменится после перезапуска Birch.</source>
        <translation>Die Sprache der Oberfläche ändert sich nach einem Neustart von Birch.</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Birch %1&lt;/h3&gt;&lt;p&gt;Блок-схемы из текста — без ИИ, полностью офлайн.&lt;/p&gt;&lt;p&gt;Graphviz %2 · Qt %3&lt;/p&gt;&lt;p&gt;Распространяется по лицензии &lt;a href=&quot;https://mozilla.org/MPL/2.0/&quot;&gt;Mozilla Public License 2.0&lt;/a&gt;.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Birch %1&lt;/h3&gt;&lt;p&gt;Flussdiagramme aus Text — ohne KI, komplett offline.&lt;/p&gt;&lt;p&gt;Graphviz %2 · Qt %3&lt;/p&gt;&lt;p&gt;Veröffentlicht unter der &lt;a href=&quot;https://mozilla.org/MPL/2.0/&quot;&gt;Mozilla Public License 2.0&lt;/a&gt;.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Крупнее</source>
        <translation>Größer</translation>
    </message>
    <message>
        <source>Мельче</source>
        <translation>Kleiner</translation>
    </message>
    <message>
        <source>Обычный размер</source>
        <translation>Standardgröße</translation>
    </message>
    <message>
        <source>Шрифт редактора</source>
        <translation>Editor-Schrift</translation>
    </message>
    <message>
        <source>Шрифт редактора: %1 пт</source>
        <translation>Editor-Schrift: %1 pt</translation>
    </message>
</context>
</TS>
