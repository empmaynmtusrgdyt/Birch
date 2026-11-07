<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN" sourcelanguage="ru">
<context>
    <name>CommandLine</name>
    <message>
        <source>неизвестный ключ «%1»</source>
        <translation>无法识别的选项“%1”</translation>
    </message>
    <message>
        <source>ключу «%1» нужно значение</source>
        <translation>选项“%1”需要一个参数</translation>
    </message>
    <message>
        <source>лишний аргумент «%1»: можно указать только один файл</source>
        <translation>多余的操作数“%1”：只能指定一个文件</translation>
    </message>
    <message>
        <source>ключ «%1» работает только вместе с -o</source>
        <translation>选项“%1”只能与 -o 一起使用</translation>
    </message>
    <message>
        <source>чтение схемы со стандартного ввода («-») работает только вместе с -o</source>
        <translation>从标准输入（“-”）读取流程图只能与 -o 一起使用</translation>
    </message>
    <message>
        <source>ключу «-o» нужно значение</source>
        <translation>选项“-o”需要一个参数</translation>
    </message>
    <message>
        <source>неизвестный формат файла «%1»: поддерживаются .png, .svg и .mmd</source>
        <translation>未知的文件格式“%1”：支持 .png、.svg 和 .mmd</translation>
    </message>
    <message>
        <source>не указан файл схемы</source>
        <translation>未指定流程图文件</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--theme»: допустимы light и dark</source>
        <translation>选项“--theme”的值“%1”无效：有效值为 light 和 dark</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--scale»: нужно целое число от 1 до %2</source>
        <translation>选项“--scale”的值“%1”无效：应为 1 到 %2 之间的整数</translation>
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
        <translation>用法：%1 [选项]... [文件]
在编辑器中打开文件（.birch），或使用 -o 不打开窗口，
直接根据它把流程图绘制到文件中。
如果文件为“-”，则从标准输入读取流程图文本。

长选项必须使用的参数对于短选项也是必需的。
  -o, --output=文件        把流程图保存到文件后退出；
                             格式由扩展名决定：.png、.svg 或 .mmd
  -t, --theme=主题         主题：light（默认）或 dark
  -m, --monochrome         黑白流程图
  -H, --horizontal         从左到右排列方框
  -a, --transparent        透明背景（PNG 和 SVG）
  -s, --scale=N            PNG 缩放倍数，1 到 10（默认 2）
  -h, --help               显示此帮助信息并退出
  -v, --version            显示版本信息并退出

示例：
  %1 diagram.birch                 在编辑器中打开流程图
  %1 diagram.birch -o diagram.png  把流程图绘制为 PNG
  %1 -t dark -s 3 diagram.birch -o diagram.png
  cat diagram.birch | %1 - -o diagram.svg

退出状态：
  0  成功
  1  流程图文本有错误，或流程图为空
  2  读取或写入文件时出错
  3  命令行选项无效

主页： &lt;https://github.com/empmaynmtusrgdyt/Birch&gt;
报告错误： &lt;https://github.com/empmaynmtusrgdyt/Birch/issues&gt;
</translation>
    </message>
    <message>
        <source>Лицензия MPL 2.0: Mozilla Public License 2.0 &lt;https://mozilla.org/MPL/2.0/&gt;
Это свободное ПО: вы можете изменять и распространять его.
НИКАКИХ ГАРАНТИЙ не предоставляется в пределах, допускаемых законом.

Используются: Qt %1, Graphviz %2
</source>
        <translation>许可证 MPL 2.0：Mozilla Public License 2.0 &lt;https://mozilla.org/MPL/2.0/&gt;
这是自由软件：您可以自由修改和重新发布它。
在法律允许的范围内，不提供任何担保。

使用：Qt %1，Graphviz %2
</translation>
    </message>
    <message>
        <source>%1: %2
Попробуйте «%1 --help» для получения дополнительной информации.
</source>
        <translation>%1：%2
请尝试执行“%1 --help”获取更多信息。
</translation>
    </message>
    <message>
        <source>%1: не удалось открыть «%2»: %3
</source>
        <translation>%1：无法打开“%2”：%3
</translation>
    </message>
    <message>
        <source>ошибка</source>
        <translation>错误</translation>
    </message>
    <message>
        <source>предупреждение</source>
        <translation>警告</translation>
    </message>
    <message>
        <source>%1: схема пуста — нечего рисовать
</source>
        <translation>%1：流程图为空，没有可绘制的内容
</translation>
    </message>
    <message>
        <source>%1: не удалось записать «%2»: %3
</source>
        <translation>%1：无法写入“%2”：%3
</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--screenshot-theme»</source>
        <translation>选项“--screenshot-theme”的值“%1”无效</translation>
    </message>
</context>
<context>
    <name>InstallCommand</name>
    <message>
        <source>Откройте новое окно терминала и попробуйте:

    birch --help
    birch схема.birch -o схема.png</source>
        <translation>请打开一个新的终端窗口并尝试：

    birch --help
    birch diagram.birch -o diagram.png</translation>
    </message>
    <message>
        <source>Команда birch установлена: %1.</source>
        <translation>birch 命令已安装：%1。</translation>
    </message>
    <message>
        <source>

Папки ~/.local/bin пока нет в PATH. Обычно она появляется там после повторного входа в систему. Если нет — добавьте в ~/.bashrc строку:

    export PATH=&quot;$HOME/.local/bin:$PATH&quot;</source>
        <translation>

~/.local/bin 尚未在 PATH 中。通常重新登录后它会出现在那里。如果没有，请在 ~/.bashrc 中添加这一行：

    export PATH=&quot;$HOME/.local/bin:$PATH&quot;</translation>
    </message>
    <message>
        <source>

Если переместите файл AppImage, выберите этот пункт ещё раз.</source>
        <translation>

如果移动了 AppImage 文件，请再次选择此菜单项。</translation>
    </message>
    <message>
        <source>Команда birch</source>
        <translation>birch 命令</translation>
    </message>
    <message>
        <source>Birch установит команду birch в /usr/local/bin.</source>
        <translation>Birch 将把 birch 命令安装到 /usr/local/bin。</translation>
    </message>
    <message>
        <source>Не удалось установить команду birch.

%1</source>
        <translation>无法安装 birch 命令。

%1</translation>
    </message>
    <message>
        <source>Birch запущен не из папки «Программы», поэтому команда на него перестанет работать.

Перетащите Birch в «Программы», откройте его оттуда и выберите этот пункт ещё раз. Если Birch уже там, выполните в Терминале:

    xattr -dr com.apple.quarantine /Applications/Birch.app</source>
        <translation>Birch 不是从“应用程序”文件夹运行的，指向它的命令将无法使用。

请把 Birch 拖到“应用程序”中，从那里打开它，然后再次选择此菜单项。如果 Birch 已经在那里，请在“终端”中运行：

    xattr -dr com.apple.quarantine /Applications/Birch.app</translation>
    </message>
    <message>
        <source>Команда birch уже установлена: %1.</source>
        <translation>birch 命令已经安装：%1。</translation>
    </message>
    <message>
        <source>По пути %1 уже есть другой файл. Заменить его командой Birch?</source>
        <translation>%1 处已有另一个文件。要用 birch 命令替换它吗？</translation>
    </message>
    <message>
        <source>Не удалось установить команду birch: %1.</source>
        <translation>无法安装 birch 命令：%1。</translation>
    </message>
</context>
<context>
    <name>Parser</name>
    <message>
        <source>прямоугольник [ ]</source>
        <translation>矩形 [ ]</translation>
    </message>
    <message>
        <source>ромб { }</source>
        <translation>菱形 { }</translation>
    </message>
    <message>
        <source>овал ( )</source>
        <translation>椭圆 ( )</translation>
    </message>
    <message>
        <source>Непонятный текст «%1». Узлы пишутся в скобках: [Шаг], {Условие?}, (Старт)</source>
        <translation>意外的文本“%1”。节点要写在括号中：[步骤]、{条件?}、(开始)</translation>
    </message>
    <message>
        <source>Не закрыта скобка «%1» — ожидалась «%2» до конца строки</source>
        <translation>“%1”未闭合：行尾之前应有“%2”</translation>
    </message>
    <message>
        <source>Пустой узел «%1%2» — добавьте текст</source>
        <translation>空节点“%1%2”：请添加文本</translation>
    </message>
    <message>
        <source>Слишком длинный текст узла (больше %1 символов)</source>
        <translation>节点文本过长（超过 %1 个字符）</translation>
    </message>
    <message>
        <source>Не закрыт заголовок рамки — ожидался «=» в конце, например =Авторизация=</source>
        <translation>框标题未闭合：末尾应有“=”，例如 =登录=</translation>
    </message>
    <message>
        <source>Непонятная запись «%1». Рамка открывается так: =Название=, а закрывается так: ==</source>
        <translation>意外的“%1”。框用 =名称= 打开，用 == 关闭</translation>
    </message>
    <message>
        <source>Слишком длинное название рамки (больше %1 символов)</source>
        <translation>框名称过长（超过 %1 个字符）</translation>
    </message>
    <message>
        <source>Не закрыта кавычка — ожидалась «%1» до конца строки</source>
        <translation>引号未闭合：行尾之前应有“%1”</translation>
    </message>
    <message>
        <source>Слишком длинная подпись (больше %1 символов)</source>
        <translation>标签过长（超过 %1 个字符）</translation>
    </message>
    <message>
        <source>узел «%1»</source>
        <translation>节点“%1”</translation>
    </message>
    <message>
        <source>подпись «%1»</source>
        <translation>标签“%1”</translation>
    </message>
    <message>
        <source>рамка «=%1=»</source>
        <translation>框“=%1=”</translation>
    </message>
    <message>
        <source>конец строки</source>
        <translation>行尾</translation>
    </message>
    <message>
        <source>конец текста</source>
        <translation>文本末尾</translation>
    </message>
    <message>
        <source>ошибочный фрагмент</source>
        <translation>无效片段</translation>
    </message>
    <message>
        <source>После «%1» в той же строке может идти только блок, например %1 [Шаг]. Чтобы открыть рамку, напишите %1 на отдельной строке</source>
        <translation>“%1”之后的同一行只能跟一个方框，例如 %1 [步骤]。要打开一个框，请把 %1 单独写一行</translation>
    </message>
    <message>
        <source>Рамка «%1» не найдена. Рамка открывается на отдельной строке: =%1=</source>
        <translation>找不到框“%1”。框要单独一行打开：=%1=</translation>
    </message>
    <message>
        <source>%1, а найдено %2</source>
        <translation>%1，但找到的是%2</translation>
    </message>
    <message>
        <source>Заголовок рамки пишется на отдельной строке: %1</source>
        <translation>框标题要单独写一行：%1</translation>
    </message>
    <message>
        <source>Инструкция должна начинаться с узла, например [Шаг], а найдено %1</source>
        <translation>语句必须以节点开头，例如 [步骤]，但找到的是%1</translation>
    </message>
    <message>
        <source>Ожидался узел, например [Шаг]</source>
        <translation>此处应为节点，例如 [步骤]</translation>
    </message>
    <message>
        <source>Подпись должна идти после стрелки и узла: [A] -&gt; [B] &quot;Да&quot;</source>
        <translation>标签写在箭头和节点之后：[A] -&gt; [B] &quot;是&quot;</translation>
    </message>
    <message>
        <source>После «-&gt;» ожидался узел</source>
        <translation>“-&gt;”之后应为节点</translation>
    </message>
    <message>
        <source>Ожидались «-&gt;», «;» или новая строка, а найдено %1</source>
        <translation>此处应为“-&gt;”、“;”或换行，但找到的是%1</translation>
    </message>
    <message>
        <source>Слишком много узлов (максимум %1)</source>
        <translation>节点过多（最多 %1 个）</translation>
    </message>
    <message>
        <source>Узел «%1» уже объявлен как %2 — форма %3 проигнорирована</source>
        <translation>节点“%1”已声明为%2，形状%3被忽略</translation>
    </message>
    <message>
        <source>Слишком много связей (максимум %1)</source>
        <translation>箭头过多（最多 %1 个）</translation>
    </message>
    <message>
        <source>Блок «%1» есть среди общих блоков. Чтобы провести стрелку к нему, напишите == %2</source>
        <translation>方框“%1”是公共方框之一。要画一条指向它的箭头，请写 == %2</translation>
    </message>
    <message>
        <source>Текст слишком большой (больше %1 символов)</source>
        <translation>文本过大（超过 %1 个字符）</translation>
    </message>
</context>
<context>
    <name>Renderer</name>
    <message>
        <source>Не удалось инициализировать Graphviz</source>
        <translation>无法初始化 Graphviz</translation>
    </message>
    <message>
        <source>Graphviz: не удалось создать граф</source>
        <translation>Graphviz：无法创建图</translation>
    </message>
    <message>
        <source>Graphviz: ошибка раскладки. %1</source>
        <translation>Graphviz：布局错误。%1</translation>
    </message>
    <message>
        <source>Graphviz: ошибка рендеринга SVG. %1</source>
        <translation>Graphviz：生成 SVG 时出错。%1</translation>
    </message>
    <message>
        <source>Не удалось разобрать SVG от Graphviz</source>
        <translation>无法解析 Graphviz 生成的 SVG</translation>
    </message>
    <message>
        <source>Недостаточно памяти для картинки %1×%2</source>
        <translation>内存不足，无法创建 %1×%2 的图片</translation>
    </message>
</context>
<context>
    <name>ShellCommand</name>
    <message>
        <source>не удалось создать папку «%1»</source>
        <translation>无法创建文件夹“%1”</translation>
    </message>
    <message>
        <source>не удалось заменить «%1»: %2</source>
        <translation>无法替换“%1”：%2</translation>
    </message>
    <message>
        <source>не удалось создать «%1»: %2</source>
        <translation>无法创建“%1”：%2</translation>
    </message>
</context>
<context>
    <name>birch::CodeEditor</name>
    <message>
        <source>Опишите блок-схему, например:

[Старт] -&gt; [Шаг] -&gt; [Готово];</source>
        <translation>描述一个流程图，例如：

[开始] -&gt; [步骤] -&gt; [完成];</translation>
    </message>
    <message>
        <source>Ошибка</source>
        <translation>错误</translation>
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
            <numerusform>%n 个匹配项</numerusform>
        </translation>
    </message>
    <message>
        <source>Найти</source>
        <translation>查找</translation>
    </message>
    <message>
        <source>Заменить на</source>
        <translation>替换为</translation>
    </message>
    <message>
        <source>Предыдущее совпадение</source>
        <translation>上一个匹配项</translation>
    </message>
    <message>
        <source>Следующее совпадение</source>
        <translation>下一个匹配项</translation>
    </message>
    <message>
        <source>Учитывать регистр</source>
        <translation>区分大小写</translation>
    </message>
    <message>
        <source>Регулярное выражение. В замене $1, $2… — найденные группы</source>
        <translation>正则表达式。在替换文本中，$1、$2… 表示捕获的分组</translation>
    </message>
    <message>
        <source>Закрыть (Esc)</source>
        <translation>关闭（Esc）</translation>
    </message>
    <message>
        <source>Заменить</source>
        <translation>替换</translation>
    </message>
    <message>
        <source>Заменить все</source>
        <translation>全部替换</translation>
    </message>
    <message>
        <source>Заменено: %1</source>
        <translation>已替换：%1</translation>
    </message>
    <message>
        <source>Ошибка</source>
        <translation>错误</translation>
    </message>
    <message>
        <source>Не найдено</source>
        <translation>未找到</translation>
    </message>
    <message>
        <source>%1 из %2</source>
        <translation>%1 / %2</translation>
    </message>
    <message>
        <source>Ошибка в регулярном выражении: %1</source>
        <translation>正则表达式错误：%1</translation>
    </message>
</context>
<context>
    <name>birch::MainWindow</name>
    <message>
        <source>Новый</source>
        <translation>新建</translation>
    </message>
    <message>
        <source>Открыть…</source>
        <translation>打开…</translation>
    </message>
    <message>
        <source>Сохранить</source>
        <translation>保存</translation>
    </message>
    <message>
        <source>Сохранить как…</source>
        <translation>另存为…</translation>
    </message>
    <message>
        <source>Открыть пример</source>
        <translation>打开示例</translation>
    </message>
    <message>
        <source>Открыть пример с рамками</source>
        <translation>打开带框的示例</translation>
    </message>
    <message>
        <source>Выход</source>
        <translation>退出</translation>
    </message>
    <message>
        <source>Отменить</source>
        <translation>撤销</translation>
    </message>
    <message>
        <source>Повторить</source>
        <translation>重做</translation>
    </message>
    <message>
        <source>Вырезать</source>
        <translation>剪切</translation>
    </message>
    <message>
        <source>Копировать</source>
        <translation>复制</translation>
    </message>
    <message>
        <source>Вставить</source>
        <translation>粘贴</translation>
    </message>
    <message>
        <source>Выбрать все</source>
        <translation>全选</translation>
    </message>
    <message>
        <source>Найти…</source>
        <translation>查找…</translation>
    </message>
    <message>
        <source>Заменить…</source>
        <translation>替换…</translation>
    </message>
    <message>
        <source>Найти далее</source>
        <translation>查找下一个</translation>
    </message>
    <message>
        <source>Найти назад</source>
        <translation>查找上一个</translation>
    </message>
    <message>
        <source>Рендерить</source>
        <translation>绘制</translation>
    </message>
    <message>
        <source>Перерисовать схему (%1)</source>
        <translation>重新绘制流程图（%1）</translation>
    </message>
    <message>
        <source>Автоматический рендеринг</source>
        <translation>自动绘制</translation>
    </message>
    <message>
        <source>Скачать PNG…</source>
        <translation>保存 PNG…</translation>
    </message>
    <message>
        <source>Сохранить картинку в файл .png (%1)</source>
        <translation>把图片保存为 .png 文件（%1）</translation>
    </message>
    <message>
        <source>Экспорт SVG…</source>
        <translation>导出 SVG…</translation>
    </message>
    <message>
        <source>Экспорт Mermaid…</source>
        <translation>导出 Mermaid…</translation>
    </message>
    <message>
        <source>Копировать как Mermaid</source>
        <translation>复制为 Mermaid</translation>
    </message>
    <message>
        <source>Текст схемы для README на GitHub, Notion, Obsidian — там он отображается как схема</source>
        <translation>用于 GitHub、Notion 或 Obsidian 中 README 的流程图文本，它们会将其显示为图表</translation>
    </message>
    <message>
        <source>Копировать в буфер</source>
        <translation>复制到剪贴板</translation>
    </message>
    <message>
        <source>Скопировать PNG — можно сразу вставить в Telegram, Slack, Word (%1)</source>
        <translation>复制为 PNG，可直接粘贴到 Telegram、Slack 或 Word（%1）</translation>
    </message>
    <message>
        <source>Горизонтальная схема</source>
        <translation>水平布局</translation>
    </message>
    <message>
        <source>Располагать блоки слева направо</source>
        <translation>从左到右排列方框</translation>
    </message>
    <message>
        <source>Чёрно-белая схема</source>
        <translation>黑白</translation>
    </message>
    <message>
        <source>Ч/Б</source>
        <translation>黑白</translation>
    </message>
    <message>
        <source>Чёрно-белая схема — для печати и документов (%1)</source>
        <translation>黑白流程图，适合打印和文档（%1）</translation>
    </message>
    <message>
        <source>Прозрачный фон</source>
        <translation>透明背景</translation>
    </message>
    <message>
        <source>Увеличить</source>
        <translation>放大</translation>
    </message>
    <message>
        <source>Уменьшить</source>
        <translation>缩小</translation>
    </message>
    <message>
        <source>Реальный размер</source>
        <translation>实际大小</translation>
    </message>
    <message>
        <source>По размеру окна</source>
        <translation>适合窗口</translation>
    </message>
    <message>
        <source>Как в системе</source>
        <translation>跟随系统</translation>
    </message>
    <message>
        <source>Светлая</source>
        <translation>浅色</translation>
    </message>
    <message>
        <source>Тёмная</source>
        <translation>深色</translation>
    </message>
    <message>
        <source>Как в превью</source>
        <translation>与预览相同</translation>
    </message>
    <message>
        <source>Всегда светлая</source>
        <translation>始终浅色</translation>
    </message>
    <message>
        <source>Всегда тёмная</source>
        <translation>始终深色</translation>
    </message>
    <message>
        <source>Синтаксис…</source>
        <translation>语法…</translation>
    </message>
    <message>
        <source>О программе Birch</source>
        <translation>关于 Birch</translation>
    </message>
    <message>
        <source>Установить команду birch…</source>
        <translation>安装 birch 命令…</translation>
    </message>
    <message>
        <source>Файл</source>
        <translation>文件</translation>
    </message>
    <message>
        <source>Правка</source>
        <translation>编辑</translation>
    </message>
    <message>
        <source>Вид</source>
        <translation>视图</translation>
    </message>
    <message>
        <source>Тема</source>
        <translation>主题</translation>
    </message>
    <message>
        <source>Язык</source>
        <translation>语言</translation>
    </message>
    <message>
        <source>Экспорт картинки</source>
        <translation>图片导出</translation>
    </message>
    <message>
        <source>Тема картинки</source>
        <translation>图片主题</translation>
    </message>
    <message>
        <source>Справка</source>
        <translation>帮助</translation>
    </message>
    <message>
        <source>Панель инструментов</source>
        <translation>工具栏</translation>
    </message>
    <message>
        <source>Схема появится здесь.
Опишите её слева, например:
[Старт] -&gt; [Готово]</source>
        <translation>流程图将显示在这里。
请在左侧描述它，例如：
[开始] -&gt; [完成]</translation>
    </message>
    <message>
        <source>Узлов: %1 · связей: %2 · %3 мс</source>
        <translation>节点：%1 · 箭头：%2 · %3 毫秒</translation>
    </message>
    <message>
        <source>Строка %1: %2</source>
        <translation>第 %1 行：%2</translation>
    </message>
    <message>
        <source>Нечего экспортировать — схема пуста</source>
        <translation>没有可导出的内容：流程图为空</translation>
    </message>
    <message>
        <source>Ошибка рендеринга</source>
        <translation>绘制错误</translation>
    </message>
    <message>
        <source>В тексте есть ошибки — в картинку попали только корректные строки</source>
        <translation>文本有错误：图片中只包含正确的行</translation>
    </message>
    <message>
        <source>Скачать PNG</source>
        <translation>保存 PNG</translation>
    </message>
    <message>
        <source>Изображение PNG (*.png)</source>
        <translation>PNG 图片 (*.png)</translation>
    </message>
    <message>
        <source>Не удалось сохранить</source>
        <translation>无法保存</translation>
    </message>
    <message>
        <source>Не удалось записать файл:
%1

%2</source>
        <translation>无法写入文件：
%1

%2</translation>
    </message>
    <message>
        <source>Сохранено: %1 (%2×%3)</source>
        <translation>已保存：%1（%2×%3）</translation>
    </message>
    <message>
        <source>Экспорт SVG</source>
        <translation>导出 SVG</translation>
    </message>
    <message>
        <source>Векторное изображение SVG (*.svg)</source>
        <translation>SVG 矢量图 (*.svg)</translation>
    </message>
    <message>
        <source>Сохранено: %1</source>
        <translation>已保存：%1</translation>
    </message>
    <message>
        <source>Картинка скопирована — вставьте её в Telegram, Slack, Word…</source>
        <translation>图片已复制，可粘贴到 Telegram、Slack、Word…</translation>
    </message>
    <message>
        <source>В тексте есть ошибки — в Mermaid попали только корректные строки</source>
        <translation>文本有错误：Mermaid 图中只包含正确的行</translation>
    </message>
    <message>
        <source>Экспорт Mermaid</source>
        <translation>导出 Mermaid</translation>
    </message>
    <message>
        <source>Схема Mermaid (*.mmd)</source>
        <translation>Mermaid 图 (*.mmd)</translation>
    </message>
    <message>
        <source>Mermaid скопирован — вставьте его в README на GitHub, в Notion или Obsidian</source>
        <translation>Mermaid 已复制，可粘贴到 GitHub 的 README、Notion 或 Obsidian 中</translation>
    </message>
    <message>
        <source>Открыть схему</source>
        <translation>打开流程图</translation>
    </message>
    <message>
        <source>Схемы Birch (*.birch *.txt);;Все файлы (*)</source>
        <translation>Birch 流程图 (*.birch *.txt);;所有文件 (*)</translation>
    </message>
    <message>
        <source>Файл слишком большой</source>
        <translation>文件过大</translation>
    </message>
    <message>
        <source>Файл «%1» больше %2 МБ.</source>
        <translation>文件“%1”超过 %2 MB。</translation>
    </message>
    <message>
        <source>Не удалось открыть</source>
        <translation>无法打开</translation>
    </message>
    <message>
        <source>%1

%2</source>
        <translation>%1

%2</translation>
    </message>
    <message>
        <source>Сохранить схему</source>
        <translation>保存流程图</translation>
    </message>
    <message>
        <source>Схемы Birch (*.birch)</source>
        <translation>Birch 流程图 (*.birch)</translation>
    </message>
    <message>
        <source>Сохранить изменения?</source>
        <translation>保存更改吗？</translation>
    </message>
    <message>
        <source>В файле «%1» есть несохранённые изменения.</source>
        <translation>“%1”有未保存的更改。</translation>
    </message>
    <message>
        <source>Без названия</source>
        <translation>未命名</translation>
    </message>
    <message>
        <source>Синтаксис Birch</source>
        <translation>Birch 语法</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Синтаксис Birch&lt;/h3&gt;&lt;table cellspacing=&apos;6&apos;&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[Текст]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;шаг (прямоугольник)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;{Условие?}&lt;/code&gt;&lt;/td&gt;&lt;td&gt;решение (ромб)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;(Старт)&lt;/code&gt;&lt;/td&gt;&lt;td&gt;начало / конец&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;стрелка; можно цепочкой: &lt;code&gt;[A] -&amp;gt; [B] -&amp;gt; [C]&lt;/code&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B] &quot;Да&quot;&lt;/code&gt;&lt;/td&gt;&lt;td&gt;подпись стрелки&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;;&lt;/code&gt; или Enter&lt;/td&gt;&lt;td&gt;конец инструкции&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;// …&lt;/code&gt; или &lt;code&gt;# …&lt;/code&gt;&lt;/td&gt;&lt;td&gt;комментарий&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;\n&lt;/code&gt;&lt;/td&gt;&lt;td&gt;перенос строки в тексте&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Рамка=&lt;/code&gt;&lt;/td&gt;&lt;td&gt;на отдельной строке: открыть рамку; &lt;code&gt;==&lt;/code&gt; — закрыть&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Рамка= [A]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;блок из рамки «Рамка»; &lt;code&gt;== [A]&lt;/code&gt; — общий блок&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;&lt;p&gt;Одинаковый текст — один и тот же узел, но в каждой рамке свои блоки: «[Шаг]» в двух рамках — два разных блока. Пример — «Файл → Открыть пример с рамками».&lt;/p&gt;&lt;p&gt;Если узел хоть раз записан как &lt;code&gt;{…}&lt;/code&gt; или &lt;code&gt;(…)&lt;/code&gt;, он останется этой фигурой.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Birch 语法&lt;/h3&gt;&lt;table cellspacing=&apos;6&apos;&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[文本]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;步骤（矩形）&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;{条件?}&lt;/code&gt;&lt;/td&gt;&lt;td&gt;判断（菱形）&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;(开始)&lt;/code&gt;&lt;/td&gt;&lt;td&gt;开始 / 结束&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;箭头；也可以连写：&lt;code&gt;[A] -&amp;gt; [B] -&amp;gt; [C]&lt;/code&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B] &quot;是&quot;&lt;/code&gt;&lt;/td&gt;&lt;td&gt;箭头标签&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;;&lt;/code&gt; 或回车&lt;/td&gt;&lt;td&gt;语句结束&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;// …&lt;/code&gt; 或 &lt;code&gt;# …&lt;/code&gt;&lt;/td&gt;&lt;td&gt;注释&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;\n&lt;/code&gt;&lt;/td&gt;&lt;td&gt;文本内换行&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=框=&lt;/code&gt;&lt;/td&gt;&lt;td&gt;单独一行：打开一个框；&lt;code&gt;==&lt;/code&gt; 关闭它&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=框= [A]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;来自“框”的方框；&lt;code&gt;== [A]&lt;/code&gt; 是公共方框&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;&lt;p&gt;相同的文本表示同一个节点，但每个框都有自己的方框：两个框中的“[步骤]”是两个不同的方框。参见 文件 → 打开带框的示例。&lt;/p&gt;&lt;p&gt;如果节点至少有一次写成 &lt;code&gt;{…}&lt;/code&gt; 或 &lt;code&gt;(…)&lt;/code&gt;，它就会保持这种形状。&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Язык интерфейса изменится после перезапуска Birch.</source>
        <translation>界面语言将在重新启动 Birch 后更改。</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Birch %1&lt;/h3&gt;&lt;p&gt;Блок-схемы из текста — без ИИ, полностью офлайн.&lt;/p&gt;&lt;p&gt;Graphviz %2 · Qt %3&lt;/p&gt;&lt;p&gt;Распространяется по лицензии &lt;a href=&quot;https://mozilla.org/MPL/2.0/&quot;&gt;Mozilla Public License 2.0&lt;/a&gt;.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Birch %1&lt;/h3&gt;&lt;p&gt;用文本生成流程图：无需 AI，完全离线。&lt;/p&gt;&lt;p&gt;Graphviz %2 · Qt %3&lt;/p&gt;&lt;p&gt;依据 &lt;a href=&quot;https://mozilla.org/MPL/2.0/&quot;&gt;Mozilla Public License 2.0&lt;/a&gt; 发布。&lt;/p&gt;</translation>
    </message>
</context>
</TS>
