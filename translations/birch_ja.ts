<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ja" sourcelanguage="ru">
<context>
    <name>CommandLine</name>
    <message>
        <source>неизвестный ключ «%1»</source>
        <translation>認識できないオプション「%1」</translation>
    </message>
    <message>
        <source>ключу «%1» нужно значение</source>
        <translation>オプション「%1」には引数が必要です</translation>
    </message>
    <message>
        <source>лишний аргумент «%1»: можно указать только один файл</source>
        <translation>余分なオペランド「%1」: 指定できるファイルは 1 つだけです</translation>
    </message>
    <message>
        <source>ключ «%1» работает только вместе с -o</source>
        <translation>オプション「%1」は -o と一緒にのみ使えます</translation>
    </message>
    <message>
        <source>чтение схемы со стандартного ввода («-») работает только вместе с -o</source>
        <translation>標準入力 (「-」) からの図の読み込みは -o と一緒にのみ使えます</translation>
    </message>
    <message>
        <source>ключу «-o» нужно значение</source>
        <translation>オプション「-o」には引数が必要です</translation>
    </message>
    <message>
        <source>неизвестный формат файла «%1»: поддерживаются .png, .svg и .mmd</source>
        <translation>不明なファイル形式「%1」: .png、.svg、.mmd に対応しています</translation>
    </message>
    <message>
        <source>не указан файл схемы</source>
        <translation>図のファイルが指定されていません</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--theme»: допустимы light и dark</source>
        <translation>オプション「--theme」の値「%1」が無効です: 有効な値は light と dark です</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--scale»: нужно целое число от 1 до %2</source>
        <translation>オプション「--scale」の値「%1」が無効です: 1 から %2 までの整数を指定してください</translation>
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
        <translation>使い方: %1 [オプション]... [ファイル]
ファイル (.birch) をエディターで開きます。-o を指定すると、
ウィンドウを開かずにフローチャートをファイルに描画します。
ファイルが「-」の場合、図のテキストを標準入力から読み込みます。

長いオプションで必須の引数は短いオプションでも必須です。
  -o, --output=ファイル    図をファイルに保存して終了します。
                             形式は拡張子で決まります: .png、.svg、.mmd
  -t, --theme=テーマ       テーマ: light (既定) または dark
  -m, --monochrome         白黒の図
  -H, --horizontal         ブロックを左から右に並べる
  -a, --transparent        透明な背景 (PNG と SVG)
  -s, --scale=N            PNG の倍率、1 から 10 (既定 2)
  -h, --help               このヘルプを表示して終了する
  -v, --version            バージョン情報を表示して終了する

例:
  %1 diagram.birch                 図をエディターで開く
  %1 diagram.birch -o diagram.png  図を PNG に描画する
  %1 -t dark -s 3 diagram.birch -o diagram.png
  cat diagram.birch | %1 - -o diagram.svg

終了ステータス:
  0  成功
  1  図のテキストにエラーがあるか、図が空
  2  ファイルの読み込みまたは書き込みのエラー
  3  コマンドラインオプションが無効

ホームページ: &lt;https://github.com/empmaynmtusrgdyt/Birch&gt;
バグの報告先: &lt;https://github.com/empmaynmtusrgdyt/Birch/issues&gt;
</translation>
    </message>
    <message>
        <source>Лицензия MPL 2.0: Mozilla Public License 2.0 &lt;https://mozilla.org/MPL/2.0/&gt;
Это свободное ПО: вы можете изменять и распространять его.
НИКАКИХ ГАРАНТИЙ не предоставляется в пределах, допускаемых законом.

Используются: Qt %1, Graphviz %2
</source>
        <translation>ライセンス MPL 2.0: Mozilla Public License 2.0 &lt;https://mozilla.org/MPL/2.0/&gt;
これはフリーソフトウェアです: 自由に変更や再配布ができます。
法律で認められる範囲で、一切の保証はありません。

使用: Qt %1、Graphviz %2
</translation>
    </message>
    <message>
        <source>%1: %2
Попробуйте «%1 --help» для получения дополнительной информации.
</source>
        <translation>%1: %2
詳しくは「%1 --help」を実行してください。
</translation>
    </message>
    <message>
        <source>%1: не удалось открыть «%2»: %3
</source>
        <translation>%1: 「%2」を開けません: %3
</translation>
    </message>
    <message>
        <source>ошибка</source>
        <translation>エラー</translation>
    </message>
    <message>
        <source>предупреждение</source>
        <translation>警告</translation>
    </message>
    <message>
        <source>%1: схема пуста — нечего рисовать
</source>
        <translation>%1: 図が空です。描画するものがありません
</translation>
    </message>
    <message>
        <source>%1: не удалось записать «%2»: %3
</source>
        <translation>%1: 「%2」に書き込めません: %3
</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--screenshot-theme»</source>
        <translation>オプション「--screenshot-theme」の値「%1」が無効です</translation>
    </message>
</context>
<context>
    <name>InstallCommand</name>
    <message>
        <source>Откройте новое окно терминала и попробуйте:

    birch --help
    birch схема.birch -o схема.png</source>
        <translation>新しいターミナルウィンドウを開いて試してください:

    birch --help
    birch diagram.birch -o diagram.png</translation>
    </message>
    <message>
        <source>Команда birch установлена: %1.</source>
        <translation>birch コマンドをインストールしました: %1。</translation>
    </message>
    <message>
        <source>

Папки ~/.local/bin пока нет в PATH. Обычно она появляется там после повторного входа в систему. Если нет — добавьте в ~/.bashrc строку:

    export PATH=&quot;$HOME/.local/bin:$PATH&quot;</source>
        <translation>

~/.local/bin はまだ PATH に含まれていません。通常はログインし直すと追加されます。追加されない場合は、~/.bashrc に次の行を加えてください:

    export PATH=&quot;$HOME/.local/bin:$PATH&quot;</translation>
    </message>
    <message>
        <source>

Если переместите файл AppImage, выберите этот пункт ещё раз.</source>
        <translation>

AppImage ファイルを移動した場合は、このメニュー項目をもう一度選んでください。</translation>
    </message>
    <message>
        <source>Команда birch</source>
        <translation>birch コマンド</translation>
    </message>
    <message>
        <source>Birch установит команду birch в /usr/local/bin.</source>
        <translation>Birch は birch コマンドを /usr/local/bin にインストールします。</translation>
    </message>
    <message>
        <source>Не удалось установить команду birch.

%1</source>
        <translation>birch コマンドをインストールできませんでした。

%1</translation>
    </message>
    <message>
        <source>Birch запущен не из папки «Программы», поэтому команда на него перестанет работать.

Перетащите Birch в «Программы», откройте его оттуда и выберите этот пункт ещё раз. Если Birch уже там, выполните в Терминале:

    xattr -dr com.apple.quarantine /Applications/Birch.app</source>
        <translation>Birch が「アプリケーション」フォルダーから実行されていないため、Birch を指すコマンドは動作しなくなります。

Birch を「アプリケーション」にドラッグし、そこから開いて、このメニュー項目をもう一度選んでください。Birch がすでにそこにある場合は、「ターミナル」で次を実行してください:

    xattr -dr com.apple.quarantine /Applications/Birch.app</translation>
    </message>
    <message>
        <source>Команда birch уже установлена: %1.</source>
        <translation>birch コマンドはすでにインストールされています: %1。</translation>
    </message>
    <message>
        <source>По пути %1 уже есть другой файл. Заменить его командой Birch?</source>
        <translation>%1 には別のファイルがすでにあります。birch コマンドで置き換えますか?</translation>
    </message>
    <message>
        <source>Не удалось установить команду birch: %1.</source>
        <translation>birch コマンドをインストールできませんでした: %1。</translation>
    </message>
</context>
<context>
    <name>Language</name>
    <message>
        <source>Русский</source>
        <extracomment>Название языка этого перевода на нём самом: Deutsch, Español, 日本語</extracomment>
        <translation>日本語</translation>
    </message>
</context>
<context>
    <name>Parser</name>
    <message>
        <source>прямоугольник [ ]</source>
        <translation>長方形 [ ]</translation>
    </message>
    <message>
        <source>ромб { }</source>
        <translation>ひし形 { }</translation>
    </message>
    <message>
        <source>овал ( )</source>
        <translation>楕円 ( )</translation>
    </message>
    <message>
        <source>Непонятный текст «%1». Узлы пишутся в скобках: [Шаг], {Условие?}, (Старт)</source>
        <translation>予期しないテキスト「%1」。ノードは括弧で囲んで書きます: [手順]、{条件?}、(開始)</translation>
    </message>
    <message>
        <source>Не закрыта скобка «%1» — ожидалась «%2» до конца строки</source>
        <translation>「%1」が閉じていません。行末までに「%2」が必要です</translation>
    </message>
    <message>
        <source>Пустой узел «%1%2» — добавьте текст</source>
        <translation>空のノード「%1%2」。テキストを入力してください</translation>
    </message>
    <message>
        <source>Слишком длинный текст узла (больше %1 символов)</source>
        <translation>ノードのテキストが長すぎます (%1 文字を超えています)</translation>
    </message>
    <message>
        <source>Не закрыт заголовок рамки — ожидался «=» в конце, например =Авторизация=</source>
        <translation>枠のタイトルが閉じていません。末尾に「=」が必要です。例: =ログイン=</translation>
    </message>
    <message>
        <source>Непонятная запись «%1». Рамка открывается так: =Название=, а закрывается так: ==</source>
        <translation>予期しない「%1」。枠は =名前= で開き、== で閉じます</translation>
    </message>
    <message>
        <source>Слишком длинное название рамки (больше %1 символов)</source>
        <translation>枠の名前が長すぎます (%1 文字を超えています)</translation>
    </message>
    <message>
        <source>Не закрыта кавычка — ожидалась «%1» до конца строки</source>
        <translation>引用符が閉じていません。行末までに「%1」が必要です</translation>
    </message>
    <message>
        <source>Слишком длинная подпись (больше %1 символов)</source>
        <translation>ラベルが長すぎます (%1 文字を超えています)</translation>
    </message>
    <message>
        <source>узел «%1»</source>
        <translation>ノード「%1」</translation>
    </message>
    <message>
        <source>подпись «%1»</source>
        <translation>ラベル「%1」</translation>
    </message>
    <message>
        <source>рамка «=%1=»</source>
        <translation>枠「=%1=」</translation>
    </message>
    <message>
        <source>конец строки</source>
        <translation>行末</translation>
    </message>
    <message>
        <source>конец текста</source>
        <translation>テキストの終わり</translation>
    </message>
    <message>
        <source>ошибочный фрагмент</source>
        <translation>無効な部分</translation>
    </message>
    <message>
        <source>После «%1» в той же строке может идти только блок, например %1 [Шаг]. Чтобы открыть рамку, напишите %1 на отдельной строке</source>
        <translation>「%1」の後の同じ行には、ブロックしか書けません (例: %1 [手順])。枠を開くには、%1 を単独の行に書いてください</translation>
    </message>
    <message>
        <source>Рамка «%1» не найдена. Рамка открывается на отдельной строке: =%1=</source>
        <translation>枠「%1」が見つかりません。枠は単独の行で開きます: =%1=</translation>
    </message>
    <message>
        <source>%1, а найдено %2</source>
        <translation>%1。しかし %2 がありました</translation>
    </message>
    <message>
        <source>Заголовок рамки пишется на отдельной строке: %1</source>
        <translation>枠のタイトルは単独の行に書きます: %1</translation>
    </message>
    <message>
        <source>Инструкция должна начинаться с узла, например [Шаг], а найдено %1</source>
        <translation>文はノードで始める必要があります (例: [手順])。しかし %1 がありました</translation>
    </message>
    <message>
        <source>Ожидался узел, например [Шаг]</source>
        <translation>ノードが必要です (例: [手順])</translation>
    </message>
    <message>
        <source>Подпись должна идти после стрелки и узла: [A] -&gt; [B] &quot;Да&quot;</source>
        <translation>ラベルは矢印とノードの後に書きます: [A] -&gt; [B] &quot;はい&quot;</translation>
    </message>
    <message>
        <source>После «-&gt;» ожидался узел</source>
        <translation>「-&gt;」の後にノードが必要です</translation>
    </message>
    <message>
        <source>Ожидались «-&gt;», «;» или новая строка, а найдено %1</source>
        <translation>「-&gt;」、「;」または改行が必要です。しかし %1 がありました</translation>
    </message>
    <message>
        <source>Слишком много узлов (максимум %1)</source>
        <translation>ノードが多すぎます (最大 %1 個)</translation>
    </message>
    <message>
        <source>Узел «%1» уже объявлен как %2 — форма %3 проигнорирована</source>
        <translation>ノード「%1」はすでに %2 として宣言されています。形状 %3 は無視されます</translation>
    </message>
    <message>
        <source>Слишком много связей (максимум %1)</source>
        <translation>矢印が多すぎます (最大 %1 本)</translation>
    </message>
    <message>
        <source>Блок «%1» есть среди общих блоков. Чтобы провести стрелку к нему, напишите == %2</source>
        <translation>ブロック「%1」は共通ブロックの 1 つです。このブロックへ矢印を引くには == %2 と書いてください</translation>
    </message>
    <message>
        <source>Текст слишком большой (больше %1 символов)</source>
        <translation>テキストが大きすぎます (%1 文字を超えています)</translation>
    </message>
</context>
<context>
    <name>Renderer</name>
    <message>
        <source>Не удалось инициализировать Graphviz</source>
        <translation>Graphviz を初期化できませんでした</translation>
    </message>
    <message>
        <source>Graphviz: не удалось создать граф</source>
        <translation>Graphviz: グラフを作成できませんでした</translation>
    </message>
    <message>
        <source>Graphviz: ошибка раскладки. %1</source>
        <translation>Graphviz: レイアウトエラー。%1</translation>
    </message>
    <message>
        <source>Graphviz: ошибка рендеринга SVG. %1</source>
        <translation>Graphviz: SVG の生成エラー。%1</translation>
    </message>
    <message>
        <source>Не удалось разобрать SVG от Graphviz</source>
        <translation>Graphviz が生成した SVG を読み込めませんでした</translation>
    </message>
    <message>
        <source>Недостаточно памяти для картинки %1×%2</source>
        <translation>%1×%2 の画像を作成するためのメモリが足りません</translation>
    </message>
</context>
<context>
    <name>ShellCommand</name>
    <message>
        <source>не удалось создать папку «%1»</source>
        <translation>フォルダー「%1」を作成できませんでした</translation>
    </message>
    <message>
        <source>не удалось заменить «%1»: %2</source>
        <translation>「%1」を置き換えられませんでした: %2</translation>
    </message>
    <message>
        <source>не удалось создать «%1»: %2</source>
        <translation>「%1」を作成できませんでした: %2</translation>
    </message>
</context>
<context>
    <name>birch::CodeEditor</name>
    <message>
        <source>Опишите блок-схему, например:

[Старт] -&gt; [Шаг] -&gt; [Готово];</source>
        <translation>フローチャートを記述してください。例:

[開始] -&gt; [手順] -&gt; [完了];</translation>
    </message>
    <message>
        <source>Ошибка</source>
        <translation>エラー</translation>
    </message>
    <message>
        <source>Предупреждение</source>
        <translation>警告</translation>
    </message>
</context>
<context>
    <name>birch::FindBar</name>
    <message numerus="yes">
        <source>%n совпадений</source>
        <translation>
            <numerusform>%n 件の一致</numerusform>
        </translation>
    </message>
    <message>
        <source>Найти</source>
        <translation>検索</translation>
    </message>
    <message>
        <source>Заменить на</source>
        <translation>置換後の文字列</translation>
    </message>
    <message>
        <source>Предыдущее совпадение</source>
        <translation>前の一致</translation>
    </message>
    <message>
        <source>Следующее совпадение</source>
        <translation>次の一致</translation>
    </message>
    <message>
        <source>Учитывать регистр</source>
        <translation>大文字と小文字を区別</translation>
    </message>
    <message>
        <source>Регулярное выражение. В замене $1, $2… — найденные группы</source>
        <translation>正規表現。置換文字列では $1、$2… がキャプチャしたグループを表します</translation>
    </message>
    <message>
        <source>Закрыть (Esc)</source>
        <translation>閉じる (Esc)</translation>
    </message>
    <message>
        <source>Заменить</source>
        <translation>置換</translation>
    </message>
    <message>
        <source>Заменить все</source>
        <translation>すべて置換</translation>
    </message>
    <message>
        <source>Заменено: %1</source>
        <translation>置換しました: %1</translation>
    </message>
    <message>
        <source>Ошибка</source>
        <translation>エラー</translation>
    </message>
    <message>
        <source>Не найдено</source>
        <translation>見つかりません</translation>
    </message>
    <message>
        <source>%1 из %2</source>
        <translation>%1 / %2</translation>
    </message>
    <message>
        <source>Ошибка в регулярном выражении: %1</source>
        <translation>正規表現のエラー: %1</translation>
    </message>
</context>
<context>
    <name>birch::MainWindow</name>
    <message>
        <source>Новый</source>
        <translation>新規</translation>
    </message>
    <message>
        <source>Открыть…</source>
        <translation>開く…</translation>
    </message>
    <message>
        <source>Сохранить</source>
        <translation>保存</translation>
    </message>
    <message>
        <source>Сохранить как…</source>
        <translation>名前を付けて保存…</translation>
    </message>
    <message>
        <source>Открыть пример</source>
        <translation>例を開く</translation>
    </message>
    <message>
        <source>Открыть пример с рамками</source>
        <translation>枠の例を開く</translation>
    </message>
    <message>
        <source>Выход</source>
        <translation>終了</translation>
    </message>
    <message>
        <source>Отменить</source>
        <translation>取り消す</translation>
    </message>
    <message>
        <source>Повторить</source>
        <translation>やり直す</translation>
    </message>
    <message>
        <source>Вырезать</source>
        <translation>カット</translation>
    </message>
    <message>
        <source>Копировать</source>
        <translation>コピー</translation>
    </message>
    <message>
        <source>Вставить</source>
        <translation>ペースト</translation>
    </message>
    <message>
        <source>Выбрать все</source>
        <translation>すべてを選択</translation>
    </message>
    <message>
        <source>Найти…</source>
        <translation>検索…</translation>
    </message>
    <message>
        <source>Заменить…</source>
        <translation>置換…</translation>
    </message>
    <message>
        <source>Найти далее</source>
        <translation>次を検索</translation>
    </message>
    <message>
        <source>Найти назад</source>
        <translation>前を検索</translation>
    </message>
    <message>
        <source>Рендерить</source>
        <translation>描画</translation>
    </message>
    <message>
        <source>Перерисовать схему (%1)</source>
        <translation>図を再描画 (%1)</translation>
    </message>
    <message>
        <source>Автоматический рендеринг</source>
        <translation>自動で描画</translation>
    </message>
    <message>
        <source>Скачать PNG…</source>
        <translation>PNG を保存…</translation>
    </message>
    <message>
        <source>Сохранить картинку в файл .png (%1)</source>
        <translation>画像を .png ファイルとして保存 (%1)</translation>
    </message>
    <message>
        <source>Экспорт SVG…</source>
        <translation>SVG を書き出す…</translation>
    </message>
    <message>
        <source>Экспорт Mermaid…</source>
        <translation>Mermaid を書き出す…</translation>
    </message>
    <message>
        <source>Копировать как Mermaid</source>
        <translation>Mermaid としてコピー</translation>
    </message>
    <message>
        <source>Текст схемы для README на GitHub, Notion, Obsidian — там он отображается как схема</source>
        <translation>GitHub、Notion、Obsidian の README 用の図のテキスト。そこでは図として表示されます</translation>
    </message>
    <message>
        <source>Копировать в буфер</source>
        <translation>クリップボードにコピー</translation>
    </message>
    <message>
        <source>Скопировать PNG — можно сразу вставить в Telegram, Slack, Word (%1)</source>
        <translation>PNG としてコピー。Telegram、Slack、Word にそのまま貼り付けられます (%1)</translation>
    </message>
    <message>
        <source>Горизонтальная схема</source>
        <translation>横方向のレイアウト</translation>
    </message>
    <message>
        <source>Располагать блоки слева направо</source>
        <translation>ブロックを左から右に並べる</translation>
    </message>
    <message>
        <source>Чёрно-белая схема</source>
        <translation>白黒</translation>
    </message>
    <message>
        <source>Ч/Б</source>
        <translation>白黒</translation>
    </message>
    <message>
        <source>Чёрно-белая схема — для печати и документов (%1)</source>
        <translation>白黒の図。印刷や文書向け (%1)</translation>
    </message>
    <message>
        <source>Прозрачный фон</source>
        <translation>透明な背景</translation>
    </message>
    <message>
        <source>Увеличить</source>
        <translation>拡大</translation>
    </message>
    <message>
        <source>Уменьшить</source>
        <translation>縮小</translation>
    </message>
    <message>
        <source>Реальный размер</source>
        <translation>実際のサイズ</translation>
    </message>
    <message>
        <source>По размеру окна</source>
        <translation>ウィンドウに合わせる</translation>
    </message>
    <message>
        <source>Как в системе</source>
        <translation>システムに合わせる</translation>
    </message>
    <message>
        <source>Светлая</source>
        <translation>ライト</translation>
    </message>
    <message>
        <source>Тёмная</source>
        <translation>ダーク</translation>
    </message>
    <message>
        <source>Как в превью</source>
        <translation>プレビューと同じ</translation>
    </message>
    <message>
        <source>Всегда светлая</source>
        <translation>常にライト</translation>
    </message>
    <message>
        <source>Всегда тёмная</source>
        <translation>常にダーク</translation>
    </message>
    <message>
        <source>Синтаксис…</source>
        <translation>構文…</translation>
    </message>
    <message>
        <source>О программе Birch</source>
        <translation>Birch について</translation>
    </message>
    <message>
        <source>Установить команду birch…</source>
        <translation>birch コマンドをインストール…</translation>
    </message>
    <message>
        <source>Файл</source>
        <translation>ファイル</translation>
    </message>
    <message>
        <source>Правка</source>
        <translation>編集</translation>
    </message>
    <message>
        <source>Вид</source>
        <translation>表示</translation>
    </message>
    <message>
        <source>Тема</source>
        <translation>テーマ</translation>
    </message>
    <message>
        <source>Язык</source>
        <translation>言語</translation>
    </message>
    <message>
        <source>Экспорт картинки</source>
        <translation>画像の書き出し</translation>
    </message>
    <message>
        <source>Тема картинки</source>
        <translation>画像のテーマ</translation>
    </message>
    <message>
        <source>Справка</source>
        <translation>ヘルプ</translation>
    </message>
    <message>
        <source>Панель инструментов</source>
        <translation>ツールバー</translation>
    </message>
    <message>
        <source>Схема появится здесь.
Опишите её слева, например:
[Старт] -&gt; [Готово]</source>
        <translation>ここに図が表示されます。
左側に記述してください。例:
[開始] -&gt; [完了]</translation>
    </message>
    <message>
        <source>Узлов: %1 · связей: %2 · %3 мс</source>
        <translation>ノード: %1 · 矢印: %2 · %3 ミリ秒</translation>
    </message>
    <message>
        <source>Строка %1: %2</source>
        <translation>%1 行目: %2</translation>
    </message>
    <message>
        <source>Нечего экспортировать — схема пуста</source>
        <translation>書き出すものがありません。図が空です</translation>
    </message>
    <message>
        <source>Ошибка рендеринга</source>
        <translation>描画エラー</translation>
    </message>
    <message>
        <source>В тексте есть ошибки — в картинку попали только корректные строки</source>
        <translation>テキストにエラーがあります。画像には正しい行だけが含まれます</translation>
    </message>
    <message>
        <source>Скачать PNG</source>
        <translation>PNG を保存</translation>
    </message>
    <message>
        <source>Изображение PNG (*.png)</source>
        <translation>PNG 画像 (*.png)</translation>
    </message>
    <message>
        <source>Не удалось сохранить</source>
        <translation>保存できませんでした</translation>
    </message>
    <message>
        <source>Не удалось записать файл:
%1

%2</source>
        <translation>ファイルに書き込めませんでした:
%1

%2</translation>
    </message>
    <message>
        <source>Сохранено: %1 (%2×%3)</source>
        <translation>保存しました: %1 (%2×%3)</translation>
    </message>
    <message>
        <source>Экспорт SVG</source>
        <translation>SVG を書き出す</translation>
    </message>
    <message>
        <source>Векторное изображение SVG (*.svg)</source>
        <translation>SVG ベクター画像 (*.svg)</translation>
    </message>
    <message>
        <source>Сохранено: %1</source>
        <translation>保存しました: %1</translation>
    </message>
    <message>
        <source>Картинка скопирована — вставьте её в Telegram, Slack, Word…</source>
        <translation>画像をコピーしました。Telegram、Slack、Word などに貼り付けられます</translation>
    </message>
    <message>
        <source>В тексте есть ошибки — в Mermaid попали только корректные строки</source>
        <translation>テキストにエラーがあります。Mermaid の図には正しい行だけが含まれます</translation>
    </message>
    <message>
        <source>Экспорт Mermaid</source>
        <translation>Mermaid を書き出す</translation>
    </message>
    <message>
        <source>Схема Mermaid (*.mmd)</source>
        <translation>Mermaid の図 (*.mmd)</translation>
    </message>
    <message>
        <source>Mermaid скопирован — вставьте его в README на GitHub, в Notion или Obsidian</source>
        <translation>Mermaid をコピーしました。GitHub の README、Notion、Obsidian に貼り付けられます</translation>
    </message>
    <message>
        <source>Открыть схему</source>
        <translation>図を開く</translation>
    </message>
    <message>
        <source>Схемы Birch (*.birch *.txt);;Все файлы (*)</source>
        <translation>Birch の図 (*.birch *.txt);;すべてのファイル (*)</translation>
    </message>
    <message>
        <source>Файл слишком большой</source>
        <translation>ファイルが大きすぎます</translation>
    </message>
    <message>
        <source>Файл «%1» больше %2 МБ.</source>
        <translation>ファイル「%1」は %2 MB を超えています。</translation>
    </message>
    <message>
        <source>Не удалось открыть</source>
        <translation>開けませんでした</translation>
    </message>
    <message>
        <source>%1

%2</source>
        <translation>%1

%2</translation>
    </message>
    <message>
        <source>Сохранить схему</source>
        <translation>図を保存</translation>
    </message>
    <message>
        <source>Схемы Birch (*.birch)</source>
        <translation>Birch の図 (*.birch)</translation>
    </message>
    <message>
        <source>Сохранить изменения?</source>
        <translation>変更を保存しますか?</translation>
    </message>
    <message>
        <source>В файле «%1» есть несохранённые изменения.</source>
        <translation>「%1」には保存されていない変更があります。</translation>
    </message>
    <message>
        <source>Без названия</source>
        <translation>名称未設定</translation>
    </message>
    <message>
        <source>Синтаксис Birch</source>
        <translation>Birch の構文</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Синтаксис Birch&lt;/h3&gt;&lt;table cellspacing=&apos;6&apos;&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[Текст]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;шаг (прямоугольник)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;{Условие?}&lt;/code&gt;&lt;/td&gt;&lt;td&gt;решение (ромб)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;(Старт)&lt;/code&gt;&lt;/td&gt;&lt;td&gt;начало / конец&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;стрелка; можно цепочкой: &lt;code&gt;[A] -&amp;gt; [B] -&amp;gt; [C]&lt;/code&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B] &quot;Да&quot;&lt;/code&gt;&lt;/td&gt;&lt;td&gt;подпись стрелки&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;;&lt;/code&gt; или Enter&lt;/td&gt;&lt;td&gt;конец инструкции&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;// …&lt;/code&gt; или &lt;code&gt;# …&lt;/code&gt;&lt;/td&gt;&lt;td&gt;комментарий&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;\n&lt;/code&gt;&lt;/td&gt;&lt;td&gt;перенос строки в тексте&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Рамка=&lt;/code&gt;&lt;/td&gt;&lt;td&gt;на отдельной строке: открыть рамку; &lt;code&gt;==&lt;/code&gt; — закрыть&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Рамка= [A]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;блок из рамки «Рамка»; &lt;code&gt;== [A]&lt;/code&gt; — общий блок&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;&lt;p&gt;Одинаковый текст — один и тот же узел, но в каждой рамке свои блоки: «[Шаг]» в двух рамках — два разных блока. Пример — «Файл → Открыть пример с рамками».&lt;/p&gt;&lt;p&gt;Если узел хоть раз записан как &lt;code&gt;{…}&lt;/code&gt; или &lt;code&gt;(…)&lt;/code&gt;, он останется этой фигурой.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Birch の構文&lt;/h3&gt;&lt;table cellspacing=&apos;6&apos;&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[テキスト]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;手順 (長方形)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;{条件?}&lt;/code&gt;&lt;/td&gt;&lt;td&gt;分岐 (ひし形)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;(開始)&lt;/code&gt;&lt;/td&gt;&lt;td&gt;開始 / 終了&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;矢印。続けて書けます: &lt;code&gt;[A] -&amp;gt; [B] -&amp;gt; [C]&lt;/code&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B] &quot;はい&quot;&lt;/code&gt;&lt;/td&gt;&lt;td&gt;矢印のラベル&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;;&lt;/code&gt; または Enter&lt;/td&gt;&lt;td&gt;文の終わり&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;// …&lt;/code&gt; または &lt;code&gt;# …&lt;/code&gt;&lt;/td&gt;&lt;td&gt;コメント&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;\n&lt;/code&gt;&lt;/td&gt;&lt;td&gt;テキスト内の改行&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=枠=&lt;/code&gt;&lt;/td&gt;&lt;td&gt;単独の行で枠を開く。&lt;code&gt;==&lt;/code&gt; で閉じる&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=枠= [A]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;枠「枠」のブロック。&lt;code&gt;== [A]&lt;/code&gt; は共通ブロック&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;&lt;p&gt;同じテキストは同じノードですが、枠ごとに独自のブロックがあります。2 つの枠にある「[手順]」は別々のブロックです。「ファイル → 枠の例を開く」を参照してください。&lt;/p&gt;&lt;p&gt;ノードを一度でも &lt;code&gt;{…}&lt;/code&gt; または &lt;code&gt;(…)&lt;/code&gt; と書くと、その形状が保たれます。&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Язык интерфейса изменится после перезапуска Birch.</source>
        <translation>インターフェイスの言語は Birch の再起動後に変わります。</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Birch %1&lt;/h3&gt;&lt;p&gt;Блок-схемы из текста — без ИИ, полностью офлайн.&lt;/p&gt;&lt;p&gt;Graphviz %2 · Qt %3&lt;/p&gt;&lt;p&gt;Распространяется по лицензии &lt;a href=&quot;https://mozilla.org/MPL/2.0/&quot;&gt;Mozilla Public License 2.0&lt;/a&gt;.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Birch %1&lt;/h3&gt;&lt;p&gt;テキストからフローチャートを作成。AI 不要、完全オフライン。&lt;/p&gt;&lt;p&gt;Graphviz %2 · Qt %3&lt;/p&gt;&lt;p&gt;&lt;a href=&quot;https://mozilla.org/MPL/2.0/&quot;&gt;Mozilla Public License 2.0&lt;/a&gt; のもとで配布されています。&lt;/p&gt;</translation>
    </message>
</context>
</TS>
