<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="pt_BR" sourcelanguage="ru">
<context>
    <name>CommandLine</name>
    <message>
        <source>неизвестный ключ «%1»</source>
        <translation>opção não reconhecida &quot;%1&quot;</translation>
    </message>
    <message>
        <source>ключу «%1» нужно значение</source>
        <translation>a opção &quot;%1&quot; requer um argumento</translation>
    </message>
    <message>
        <source>лишний аргумент «%1»: можно указать только один файл</source>
        <translation>operando extra &quot;%1&quot;: só é possível indicar um arquivo</translation>
    </message>
    <message>
        <source>ключ «%1» работает только вместе с -o</source>
        <translation>a opção &quot;%1&quot; só funciona junto com -o</translation>
    </message>
    <message>
        <source>чтение схемы со стандартного ввода («-») работает только вместе с -o</source>
        <translation>ler o diagrama da entrada padrão (&quot;-&quot;) só funciona junto com -o</translation>
    </message>
    <message>
        <source>ключу «-o» нужно значение</source>
        <translation>a opção &quot;-o&quot; requer um argumento</translation>
    </message>
    <message>
        <source>неизвестный формат файла «%1»: поддерживаются .png, .svg и .mmd</source>
        <translation>formato de arquivo desconhecido &quot;%1&quot;: são aceitos .png, .svg e .mmd</translation>
    </message>
    <message>
        <source>не указан файл схемы</source>
        <translation>nenhum arquivo de diagrama indicado</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--theme»: допустимы light и dark</source>
        <translation>valor &quot;%1&quot; inválido para a opção &quot;--theme&quot;: os valores válidos são light e dark</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--scale»: нужно целое число от 1 до %2</source>
        <translation>valor &quot;%1&quot; inválido para a opção &quot;--scale&quot;: esperado um número inteiro de 1 a %2</translation>
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
        <translation>Uso: %1 [OPÇÃO]... [ARQUIVO]
Abre ARQUIVO (.birch) no editor ou, com -o, desenha a partir dele um
fluxograma em um arquivo sem abrir nenhuma janela.
Se ARQUIVO for &quot;-&quot;, o texto do diagrama é lido da entrada padrão.

Argumentos obrigatórios para opções longas também o são para as curtas.
  -o, --output=ARQUIVO     salvar o diagrama em ARQUIVO e sair; a
                             extensão define o formato: .png, .svg ou .mmd
  -t, --theme=TEMA         tema: light (padrão) ou dark
  -m, --monochrome         diagrama em preto e branco
  -H, --horizontal         dispor os blocos da esquerda para a direita
  -a, --transparent        fundo transparente (PNG e SVG)
  -s, --scale=N            escala do PNG de 1 a 10 (padrão 2)
  -h, --help               mostrar esta ajuda e sair
  -v, --version            mostrar informações de versão e sair

Exemplos:
  %1 diagrama.birch                  abrir o diagrama no editor
  %1 diagrama.birch -o diagrama.png  desenhar o diagrama em PNG
  %1 -t dark -s 3 diagrama.birch -o diagrama.png
  cat diagrama.birch | %1 - -o diagrama.svg

Status de saída:
  0  sucesso
  1  o texto do diagrama tem erros ou o diagrama está vazio
  2  erro ao ler ou gravar um arquivo
  3  opções de linha de comando inválidas

Página inicial: &lt;https://github.com/empmaynmtusrgdyt/Birch&gt;
Relate erros em: &lt;https://github.com/empmaynmtusrgdyt/Birch/issues&gt;
</translation>
    </message>
    <message>
        <source>Лицензия MPL 2.0: Mozilla Public License 2.0 &lt;https://mozilla.org/MPL/2.0/&gt;
Это свободное ПО: вы можете изменять и распространять его.
НИКАКИХ ГАРАНТИЙ не предоставляется в пределах, допускаемых законом.

Используются: Qt %1, Graphviz %2
</source>
        <translation>Licença MPL 2.0: Mozilla Public License 2.0 &lt;https://mozilla.org/MPL/2.0/&gt;
Este é um software livre: você é livre para alterá-lo e redistribuí-lo.
NÃO HÁ GARANTIA, na máxima extensão permitida em lei.

Usa: Qt %1, Graphviz %2
</translation>
    </message>
    <message>
        <source>%1: %2
Попробуйте «%1 --help» для получения дополнительной информации.
</source>
        <translation>%1: %2
Tente &quot;%1 --help&quot; para mais informações.
</translation>
    </message>
    <message>
        <source>%1: не удалось открыть «%2»: %3
</source>
        <translation>%1: não foi possível abrir &quot;%2&quot;: %3
</translation>
    </message>
    <message>
        <source>ошибка</source>
        <translation>erro</translation>
    </message>
    <message>
        <source>предупреждение</source>
        <translation>aviso</translation>
    </message>
    <message>
        <source>%1: схема пуста — нечего рисовать
</source>
        <translation>%1: o diagrama está vazio — nada para desenhar
</translation>
    </message>
    <message>
        <source>%1: не удалось записать «%2»: %3
</source>
        <translation>%1: não foi possível gravar &quot;%2&quot;: %3
</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--screenshot-theme»</source>
        <translation>valor &quot;%1&quot; inválido para a opção &quot;--screenshot-theme&quot;</translation>
    </message>
</context>
<context>
    <name>InstallCommand</name>
    <message>
        <source>Откройте новое окно терминала и попробуйте:

    birch --help
    birch схема.birch -o схема.png</source>
        <translation>Abra uma nova janela do terminal e experimente:

    birch --help
    birch diagrama.birch -o diagrama.png</translation>
    </message>
    <message>
        <source>Команда birch установлена: %1.</source>
        <translation>O comando birch está instalado: %1.</translation>
    </message>
    <message>
        <source>

Папки ~/.local/bin пока нет в PATH. Обычно она появляется там после повторного входа в систему. Если нет — добавьте в ~/.bashrc строку:

    export PATH=&quot;$HOME/.local/bin:$PATH&quot;</source>
        <translation>

~/.local/bin ainda não está no seu PATH. Normalmente ele aparece lá depois que você sai e entra de novo na sessão. Se não aparecer, adicione esta linha ao ~/.bashrc:

    export PATH=&quot;$HOME/.local/bin:$PATH&quot;</translation>
    </message>
    <message>
        <source>

Если переместите файл AppImage, выберите этот пункт ещё раз.</source>
        <translation>

Se você mover o arquivo AppImage, escolha este item do menu de novo.</translation>
    </message>
    <message>
        <source>Команда birch</source>
        <translation>Comando birch</translation>
    </message>
    <message>
        <source>Birch установит команду birch в /usr/local/bin.</source>
        <translation>O Birch vai instalar o comando birch em /usr/local/bin.</translation>
    </message>
    <message>
        <source>Не удалось установить команду birch.

%1</source>
        <translation>Não foi possível instalar o comando birch.

%1</translation>
    </message>
    <message>
        <source>Birch запущен не из папки «Программы», поэтому команда на него перестанет работать.

Перетащите Birch в «Программы», откройте его оттуда и выберите этот пункт ещё раз. Если Birch уже там, выполните в Терминале:

    xattr -dr com.apple.quarantine /Applications/Birch.app</source>
        <translation>O Birch não está sendo executado da pasta Aplicativos, então um comando que aponte para ele deixaria de funcionar.

Arraste o Birch para Aplicativos, abra-o de lá e escolha este item do menu de novo. Se o Birch já estiver lá, execute no Terminal:

    xattr -dr com.apple.quarantine /Applications/Birch.app</translation>
    </message>
    <message>
        <source>Команда birch уже установлена: %1.</source>
        <translation>O comando birch já está instalado: %1.</translation>
    </message>
    <message>
        <source>По пути %1 уже есть другой файл. Заменить его командой Birch?</source>
        <translation>Já existe outro arquivo em %1. Substituí-lo pelo comando birch?</translation>
    </message>
    <message>
        <source>Не удалось установить команду birch: %1.</source>
        <translation>Não foi possível instalar o comando birch: %1.</translation>
    </message>
</context>
<context>
    <name>Language</name>
    <message>
        <source>Русский</source>
        <extracomment>Название языка этого перевода на нём самом: Deutsch, Español, 日本語</extracomment>
        <translation>Português (Brasil)</translation>
    </message>
</context>
<context>
    <name>Parser</name>
    <message>
        <source>прямоугольник [ ]</source>
        <translation>retângulo [ ]</translation>
    </message>
    <message>
        <source>ромб { }</source>
        <translation>losango { }</translation>
    </message>
    <message>
        <source>овал ( )</source>
        <translation>oval ( )</translation>
    </message>
    <message>
        <source>Непонятный текст «%1». Узлы пишутся в скобках: [Шаг], {Условие?}, (Старт)</source>
        <translation>Texto inesperado “%1”. Os nós são escritos entre colchetes: [Passo], {Condição?}, (Início)</translation>
    </message>
    <message>
        <source>Не закрыта скобка «%1» — ожидалась «%2» до конца строки</source>
        <translation>“%1” não fechado — esperado “%2” antes do fim da linha</translation>
    </message>
    <message>
        <source>Пустой узел «%1%2» — добавьте текст</source>
        <translation>Nó vazio “%1%2” — adicione algum texto</translation>
    </message>
    <message>
        <source>Слишком длинный текст узла (больше %1 символов)</source>
        <translation>O texto do nó é longo demais (mais de %1 caracteres)</translation>
    </message>
    <message>
        <source>Не закрыт заголовок рамки — ожидался «=» в конце, например =Авторизация=</source>
        <translation>Título de moldura não fechado — esperado “=” no fim, por exemplo =Login=</translation>
    </message>
    <message>
        <source>Непонятная запись «%1». Рамка открывается так: =Название=, а закрывается так: ==</source>
        <translation>“%1” inesperado. Uma moldura é aberta com =Nome= e fechada com ==</translation>
    </message>
    <message>
        <source>Слишком длинное название рамки (больше %1 символов)</source>
        <translation>O nome da moldura é longo demais (mais de %1 caracteres)</translation>
    </message>
    <message>
        <source>Не закрыта кавычка — ожидалась «%1» до конца строки</source>
        <translation>Aspas não fechadas — esperado “%1” antes do fim da linha</translation>
    </message>
    <message>
        <source>Слишком длинная подпись (больше %1 символов)</source>
        <translation>O rótulo é longo demais (mais de %1 caracteres)</translation>
    </message>
    <message>
        <source>узел «%1»</source>
        <translation>o nó “%1”</translation>
    </message>
    <message>
        <source>подпись «%1»</source>
        <translation>o rótulo “%1”</translation>
    </message>
    <message>
        <source>рамка «=%1=»</source>
        <translation>a moldura “=%1=”</translation>
    </message>
    <message>
        <source>конец строки</source>
        <translation>o fim da linha</translation>
    </message>
    <message>
        <source>конец текста</source>
        <translation>o fim do texto</translation>
    </message>
    <message>
        <source>ошибочный фрагмент</source>
        <translation>um trecho inválido</translation>
    </message>
    <message>
        <source>После «%1» в той же строке может идти только блок, например %1 [Шаг]. Чтобы открыть рамку, напишите %1 на отдельной строке</source>
        <translation>Depois de “%1” só pode vir um bloco na mesma linha, por exemplo %1 [Passo]. Para abrir uma moldura, escreva %1 em uma linha separada</translation>
    </message>
    <message>
        <source>Рамка «%1» не найдена. Рамка открывается на отдельной строке: =%1=</source>
        <translation>Moldura “%1” não encontrada. Uma moldura é aberta em uma linha separada: =%1=</translation>
    </message>
    <message>
        <source>%1, а найдено %2</source>
        <translation>%1, mas foi encontrado %2</translation>
    </message>
    <message>
        <source>Заголовок рамки пишется на отдельной строке: %1</source>
        <translation>O título de uma moldura fica em uma linha separada: %1</translation>
    </message>
    <message>
        <source>Инструкция должна начинаться с узла, например [Шаг], а найдено %1</source>
        <translation>Uma instrução deve começar com um nó, por exemplo [Passo], mas foi encontrado %1</translation>
    </message>
    <message>
        <source>Ожидался узел, например [Шаг]</source>
        <translation>Esperado um nó, por exemplo [Passo]</translation>
    </message>
    <message>
        <source>Подпись должна идти после стрелки и узла: [A] -&gt; [B] &quot;Да&quot;</source>
        <translation>O rótulo vem depois da seta e do nó: [A] -&gt; [B] &quot;Sim&quot;</translation>
    </message>
    <message>
        <source>После «-&gt;» ожидался узел</source>
        <translation>Esperado um nó depois de “-&gt;”</translation>
    </message>
    <message>
        <source>Ожидались «-&gt;», «;» или новая строка, а найдено %1</source>
        <translation>Esperado “-&gt;”, “;” ou uma nova linha, mas foi encontrado %1</translation>
    </message>
    <message>
        <source>Слишком много узлов (максимум %1)</source>
        <translation>Nós demais (no máximo %1)</translation>
    </message>
    <message>
        <source>Узел «%1» уже объявлен как %2 — форма %3 проигнорирована</source>
        <translation>O nó “%1” já está declarado como %2 — forma %3 ignorada</translation>
    </message>
    <message>
        <source>Слишком много связей (максимум %1)</source>
        <translation>Setas demais (no máximo %1)</translation>
    </message>
    <message>
        <source>Блок «%1» есть среди общих блоков. Чтобы провести стрелку к нему, напишите == %2</source>
        <translation>O bloco “%1” é um dos blocos comuns. Para desenhar uma seta até ele, escreva == %2</translation>
    </message>
    <message>
        <source>Текст слишком большой (больше %1 символов)</source>
        <translation>O texto é grande demais (mais de %1 caracteres)</translation>
    </message>
</context>
<context>
    <name>Renderer</name>
    <message>
        <source>Не удалось инициализировать Graphviz</source>
        <translation>Não foi possível inicializar o Graphviz</translation>
    </message>
    <message>
        <source>Graphviz: не удалось создать граф</source>
        <translation>Graphviz: não foi possível criar o grafo</translation>
    </message>
    <message>
        <source>Graphviz: ошибка раскладки. %1</source>
        <translation>Graphviz: erro de layout. %1</translation>
    </message>
    <message>
        <source>Graphviz: ошибка рендеринга SVG. %1</source>
        <translation>Graphviz: erro ao gerar o SVG. %1</translation>
    </message>
    <message>
        <source>Не удалось разобрать SVG от Graphviz</source>
        <translation>Não foi possível ler o SVG do Graphviz</translation>
    </message>
    <message>
        <source>Недостаточно памяти для картинки %1×%2</source>
        <translation>Memória insuficiente para uma imagem de %1×%2</translation>
    </message>
</context>
<context>
    <name>ShellCommand</name>
    <message>
        <source>не удалось создать папку «%1»</source>
        <translation>não foi possível criar a pasta “%1”</translation>
    </message>
    <message>
        <source>не удалось заменить «%1»: %2</source>
        <translation>não foi possível substituir “%1”: %2</translation>
    </message>
    <message>
        <source>не удалось создать «%1»: %2</source>
        <translation>não foi possível criar “%1”: %2</translation>
    </message>
</context>
<context>
    <name>birch::CodeEditor</name>
    <message>
        <source>Опишите блок-схему, например:

[Старт] -&gt; [Шаг] -&gt; [Готово];</source>
        <translation>Descreva um fluxograma, por exemplo:

[Início] -&gt; [Passo] -&gt; [Pronto];</translation>
    </message>
    <message>
        <source>Ошибка</source>
        <translation>Erro</translation>
    </message>
    <message>
        <source>Предупреждение</source>
        <translation>Aviso</translation>
    </message>
</context>
<context>
    <name>birch::FindBar</name>
    <message numerus="yes">
        <source>%n совпадений</source>
        <translation>
            <numerusform>%n ocorrência</numerusform>
            <numerusform>%n ocorrências</numerusform>
        </translation>
    </message>
    <message>
        <source>Найти</source>
        <translation>Localizar</translation>
    </message>
    <message>
        <source>Заменить на</source>
        <translation>Substituir por</translation>
    </message>
    <message>
        <source>Предыдущее совпадение</source>
        <translation>Ocorrência anterior</translation>
    </message>
    <message>
        <source>Следующее совпадение</source>
        <translation>Próxima ocorrência</translation>
    </message>
    <message>
        <source>Учитывать регистр</source>
        <translation>Diferenciar maiúsculas e minúsculas</translation>
    </message>
    <message>
        <source>Регулярное выражение. В замене $1, $2… — найденные группы</source>
        <translation>Expressão regular. Na substituição, $1, $2… são os grupos capturados</translation>
    </message>
    <message>
        <source>Закрыть (Esc)</source>
        <translation>Fechar (Esc)</translation>
    </message>
    <message>
        <source>Заменить</source>
        <translation>Substituir</translation>
    </message>
    <message>
        <source>Заменить все</source>
        <translation>Substituir tudo</translation>
    </message>
    <message>
        <source>Заменено: %1</source>
        <translation>Substituições: %1</translation>
    </message>
    <message>
        <source>Ошибка</source>
        <translation>Erro</translation>
    </message>
    <message>
        <source>Не найдено</source>
        <translation>Não encontrado</translation>
    </message>
    <message>
        <source>%1 из %2</source>
        <translation>%1 de %2</translation>
    </message>
    <message>
        <source>Ошибка в регулярном выражении: %1</source>
        <translation>Erro na expressão regular: %1</translation>
    </message>
</context>
<context>
    <name>birch::MainWindow</name>
    <message>
        <source>Новый</source>
        <translation>Novo</translation>
    </message>
    <message>
        <source>Открыть…</source>
        <translation>Abrir…</translation>
    </message>
    <message>
        <source>Сохранить</source>
        <translation>Salvar</translation>
    </message>
    <message>
        <source>Сохранить как…</source>
        <translation>Salvar como…</translation>
    </message>
    <message>
        <source>Открыть пример</source>
        <translation>Abrir exemplo</translation>
    </message>
    <message>
        <source>Открыть пример с рамками</source>
        <translation>Abrir exemplo com molduras</translation>
    </message>
    <message>
        <source>Выход</source>
        <translation>Sair</translation>
    </message>
    <message>
        <source>Отменить</source>
        <translation>Desfazer</translation>
    </message>
    <message>
        <source>Повторить</source>
        <translation>Refazer</translation>
    </message>
    <message>
        <source>Вырезать</source>
        <translation>Recortar</translation>
    </message>
    <message>
        <source>Копировать</source>
        <translation>Copiar</translation>
    </message>
    <message>
        <source>Вставить</source>
        <translation>Colar</translation>
    </message>
    <message>
        <source>Выбрать все</source>
        <translation>Selecionar tudo</translation>
    </message>
    <message>
        <source>Найти…</source>
        <translation>Localizar…</translation>
    </message>
    <message>
        <source>Заменить…</source>
        <translation>Substituir…</translation>
    </message>
    <message>
        <source>Найти далее</source>
        <translation>Localizar próxima</translation>
    </message>
    <message>
        <source>Найти назад</source>
        <translation>Localizar anterior</translation>
    </message>
    <message>
        <source>Рендерить</source>
        <translation>Desenhar</translation>
    </message>
    <message>
        <source>Перерисовать схему (%1)</source>
        <translation>Redesenhar o diagrama (%1)</translation>
    </message>
    <message>
        <source>Автоматический рендеринг</source>
        <translation>Desenhar automaticamente</translation>
    </message>
    <message>
        <source>Скачать PNG…</source>
        <translation>Salvar PNG…</translation>
    </message>
    <message>
        <source>Сохранить картинку в файл .png (%1)</source>
        <translation>Salvar a imagem como arquivo .png (%1)</translation>
    </message>
    <message>
        <source>Экспорт SVG…</source>
        <translation>Exportar SVG…</translation>
    </message>
    <message>
        <source>Экспорт Mermaid…</source>
        <translation>Exportar Mermaid…</translation>
    </message>
    <message>
        <source>Копировать как Mermaid</source>
        <translation>Copiar como Mermaid</translation>
    </message>
    <message>
        <source>Текст схемы для README на GitHub, Notion, Obsidian — там он отображается как схема</source>
        <translation>Texto do diagrama para um README no GitHub, Notion ou Obsidian — eles o exibem como diagrama</translation>
    </message>
    <message>
        <source>Копировать в буфер</source>
        <translation>Copiar para a área de transferência</translation>
    </message>
    <message>
        <source>Скопировать PNG — можно сразу вставить в Telegram, Slack, Word (%1)</source>
        <translation>Copiar como PNG — cole direto no Telegram, Slack ou Word (%1)</translation>
    </message>
    <message>
        <source>Горизонтальная схема</source>
        <translation>Layout horizontal</translation>
    </message>
    <message>
        <source>Располагать блоки слева направо</source>
        <translation>Dispor os blocos da esquerda para a direita</translation>
    </message>
    <message>
        <source>Чёрно-белая схема</source>
        <translation>Preto e branco</translation>
    </message>
    <message>
        <source>Ч/Б</source>
        <translation>P/B</translation>
    </message>
    <message>
        <source>Чёрно-белая схема — для печати и документов (%1)</source>
        <translation>Diagrama em preto e branco — para impressão e documentos (%1)</translation>
    </message>
    <message>
        <source>Прозрачный фон</source>
        <translation>Fundo transparente</translation>
    </message>
    <message>
        <source>Увеличить</source>
        <translation>Ampliar</translation>
    </message>
    <message>
        <source>Уменьшить</source>
        <translation>Reduzir</translation>
    </message>
    <message>
        <source>Реальный размер</source>
        <translation>Tamanho real</translation>
    </message>
    <message>
        <source>По размеру окна</source>
        <translation>Ajustar à janela</translation>
    </message>
    <message>
        <source>Как в системе</source>
        <translation>Igual ao sistema</translation>
    </message>
    <message>
        <source>Светлая</source>
        <translation>Claro</translation>
    </message>
    <message>
        <source>Тёмная</source>
        <translation>Escuro</translation>
    </message>
    <message>
        <source>Как в превью</source>
        <translation>Igual à pré-visualização</translation>
    </message>
    <message>
        <source>Всегда светлая</source>
        <translation>Sempre claro</translation>
    </message>
    <message>
        <source>Всегда тёмная</source>
        <translation>Sempre escuro</translation>
    </message>
    <message>
        <source>Синтаксис…</source>
        <translation>Sintaxe…</translation>
    </message>
    <message>
        <source>О программе Birch</source>
        <translation>Sobre o Birch</translation>
    </message>
    <message>
        <source>Установить команду birch…</source>
        <translation>Instalar o comando birch…</translation>
    </message>
    <message>
        <source>Файл</source>
        <translation>Arquivo</translation>
    </message>
    <message>
        <source>Правка</source>
        <translation>Editar</translation>
    </message>
    <message>
        <source>Вид</source>
        <translation>Exibir</translation>
    </message>
    <message>
        <source>Тема</source>
        <translation>Tema</translation>
    </message>
    <message>
        <source>Язык</source>
        <translation>Idioma</translation>
    </message>
    <message>
        <source>Экспорт картинки</source>
        <translation>Exportação de imagem</translation>
    </message>
    <message>
        <source>Тема картинки</source>
        <translation>Tema da imagem</translation>
    </message>
    <message>
        <source>Справка</source>
        <translation>Ajuda</translation>
    </message>
    <message>
        <source>Панель инструментов</source>
        <translation>Barra de ferramentas</translation>
    </message>
    <message>
        <source>Схема появится здесь.
Опишите её слева, например:
[Старт] -&gt; [Готово]</source>
        <translation>O diagrama aparecerá aqui.
Descreva-o à esquerda, por exemplo:
[Início] -&gt; [Pronto]</translation>
    </message>
    <message>
        <source>Узлов: %1 · связей: %2 · %3 мс</source>
        <translation>Nós: %1 · setas: %2 · %3 ms</translation>
    </message>
    <message>
        <source>Строка %1: %2</source>
        <translation>Linha %1: %2</translation>
    </message>
    <message>
        <source>Нечего экспортировать — схема пуста</source>
        <translation>Nada para exportar — o diagrama está vazio</translation>
    </message>
    <message>
        <source>Ошибка рендеринга</source>
        <translation>Erro ao desenhar</translation>
    </message>
    <message>
        <source>В тексте есть ошибки — в картинку попали только корректные строки</source>
        <translation>O texto tem erros — a imagem contém só as linhas corretas</translation>
    </message>
    <message>
        <source>Скачать PNG</source>
        <translation>Salvar PNG</translation>
    </message>
    <message>
        <source>Изображение PNG (*.png)</source>
        <translation>Imagem PNG (*.png)</translation>
    </message>
    <message>
        <source>Не удалось сохранить</source>
        <translation>Não foi possível salvar</translation>
    </message>
    <message>
        <source>Не удалось записать файл:
%1

%2</source>
        <translation>Não foi possível gravar o arquivo:
%1

%2</translation>
    </message>
    <message>
        <source>Сохранено: %1 (%2×%3)</source>
        <translation>Salvo: %1 (%2×%3)</translation>
    </message>
    <message>
        <source>Экспорт SVG</source>
        <translation>Exportar SVG</translation>
    </message>
    <message>
        <source>Векторное изображение SVG (*.svg)</source>
        <translation>Imagem vetorial SVG (*.svg)</translation>
    </message>
    <message>
        <source>Сохранено: %1</source>
        <translation>Salvo: %1</translation>
    </message>
    <message>
        <source>Картинка скопирована — вставьте её в Telegram, Slack, Word…</source>
        <translation>Imagem copiada — cole no Telegram, Slack, Word…</translation>
    </message>
    <message>
        <source>В тексте есть ошибки — в Mermaid попали только корректные строки</source>
        <translation>O texto tem erros — o diagrama Mermaid contém só as linhas corretas</translation>
    </message>
    <message>
        <source>Экспорт Mermaid</source>
        <translation>Exportar Mermaid</translation>
    </message>
    <message>
        <source>Схема Mermaid (*.mmd)</source>
        <translation>Diagrama Mermaid (*.mmd)</translation>
    </message>
    <message>
        <source>Mermaid скопирован — вставьте его в README на GitHub, в Notion или Obsidian</source>
        <translation>Mermaid copiado — cole em um README no GitHub, no Notion ou no Obsidian</translation>
    </message>
    <message>
        <source>Открыть схему</source>
        <translation>Abrir diagrama</translation>
    </message>
    <message>
        <source>Схемы Birch (*.birch *.txt);;Все файлы (*)</source>
        <translation>Diagramas do Birch (*.birch *.txt);;Todos os arquivos (*)</translation>
    </message>
    <message>
        <source>Файл слишком большой</source>
        <translation>Arquivo grande demais</translation>
    </message>
    <message>
        <source>Файл «%1» больше %2 МБ.</source>
        <translation>O arquivo “%1” tem mais de %2 MB.</translation>
    </message>
    <message>
        <source>Не удалось открыть</source>
        <translation>Não foi possível abrir</translation>
    </message>
    <message>
        <source>%1

%2</source>
        <translation>%1

%2</translation>
    </message>
    <message>
        <source>Сохранить схему</source>
        <translation>Salvar diagrama</translation>
    </message>
    <message>
        <source>Схемы Birch (*.birch)</source>
        <translation>Diagramas do Birch (*.birch)</translation>
    </message>
    <message>
        <source>Сохранить изменения?</source>
        <translation>Salvar as alterações?</translation>
    </message>
    <message>
        <source>В файле «%1» есть несохранённые изменения.</source>
        <translation>“%1” tem alterações não salvas.</translation>
    </message>
    <message>
        <source>Без названия</source>
        <translation>Sem título</translation>
    </message>
    <message>
        <source>Синтаксис Birch</source>
        <translation>Sintaxe do Birch</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Синтаксис Birch&lt;/h3&gt;&lt;table cellspacing=&apos;6&apos;&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[Текст]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;шаг (прямоугольник)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;{Условие?}&lt;/code&gt;&lt;/td&gt;&lt;td&gt;решение (ромб)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;(Старт)&lt;/code&gt;&lt;/td&gt;&lt;td&gt;начало / конец&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;стрелка; можно цепочкой: &lt;code&gt;[A] -&amp;gt; [B] -&amp;gt; [C]&lt;/code&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B] &quot;Да&quot;&lt;/code&gt;&lt;/td&gt;&lt;td&gt;подпись стрелки&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;;&lt;/code&gt; или Enter&lt;/td&gt;&lt;td&gt;конец инструкции&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;// …&lt;/code&gt; или &lt;code&gt;# …&lt;/code&gt;&lt;/td&gt;&lt;td&gt;комментарий&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;\n&lt;/code&gt;&lt;/td&gt;&lt;td&gt;перенос строки в тексте&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Рамка=&lt;/code&gt;&lt;/td&gt;&lt;td&gt;на отдельной строке: открыть рамку; &lt;code&gt;==&lt;/code&gt; — закрыть&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Рамка= [A]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;блок из рамки «Рамка»; &lt;code&gt;== [A]&lt;/code&gt; — общий блок&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;&lt;p&gt;Одинаковый текст — один и тот же узел, но в каждой рамке свои блоки: «[Шаг]» в двух рамках — два разных блока. Пример — «Файл → Открыть пример с рамками».&lt;/p&gt;&lt;p&gt;Если узел хоть раз записан как &lt;code&gt;{…}&lt;/code&gt; или &lt;code&gt;(…)&lt;/code&gt;, он останется этой фигурой.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Sintaxe do Birch&lt;/h3&gt;&lt;table cellspacing=&apos;6&apos;&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[Texto]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;passo (retângulo)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;{Condição?}&lt;/code&gt;&lt;/td&gt;&lt;td&gt;decisão (losango)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;(Início)&lt;/code&gt;&lt;/td&gt;&lt;td&gt;início / fim&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;seta; também em cadeia: &lt;code&gt;[A] -&amp;gt; [B] -&amp;gt; [C]&lt;/code&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B] &quot;Sim&quot;&lt;/code&gt;&lt;/td&gt;&lt;td&gt;rótulo da seta&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;;&lt;/code&gt; ou Enter&lt;/td&gt;&lt;td&gt;fim da instrução&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;// …&lt;/code&gt; ou &lt;code&gt;# …&lt;/code&gt;&lt;/td&gt;&lt;td&gt;comentário&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;\n&lt;/code&gt;&lt;/td&gt;&lt;td&gt;quebra de linha no texto&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Moldura=&lt;/code&gt;&lt;/td&gt;&lt;td&gt;em uma linha separada: abrir uma moldura; &lt;code&gt;==&lt;/code&gt; a fecha&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Moldura= [A]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;bloco da moldura “Moldura”; &lt;code&gt;== [A]&lt;/code&gt; é um bloco comum&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;&lt;p&gt;O mesmo texto é o mesmo nó, mas cada moldura tem seus próprios blocos: “[Passo]” em duas molduras são dois blocos diferentes. Veja Arquivo → Abrir exemplo com molduras.&lt;/p&gt;&lt;p&gt;Se um nó for escrito pelo menos uma vez como &lt;code&gt;{…}&lt;/code&gt; ou &lt;code&gt;(…)&lt;/code&gt;, ele mantém essa forma.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Язык интерфейса изменится после перезапуска Birch.</source>
        <translation>O idioma da interface mudará depois que você reiniciar o Birch.</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Birch %1&lt;/h3&gt;&lt;p&gt;Блок-схемы из текста — без ИИ, полностью офлайн.&lt;/p&gt;&lt;p&gt;Graphviz %2 · Qt %3&lt;/p&gt;&lt;p&gt;Распространяется по лицензии &lt;a href=&quot;https://mozilla.org/MPL/2.0/&quot;&gt;Mozilla Public License 2.0&lt;/a&gt;.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Birch %1&lt;/h3&gt;&lt;p&gt;Fluxogramas a partir de texto — sem IA, totalmente offline.&lt;/p&gt;&lt;p&gt;Graphviz %2 · Qt %3&lt;/p&gt;&lt;p&gt;Distribuído sob a &lt;a href=&quot;https://mozilla.org/MPL/2.0/&quot;&gt;Mozilla Public License 2.0&lt;/a&gt;.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Крупнее</source>
        <translation>Maior</translation>
    </message>
    <message>
        <source>Мельче</source>
        <translation>Menor</translation>
    </message>
    <message>
        <source>Обычный размер</source>
        <translation>Tamanho padrão</translation>
    </message>
    <message>
        <source>Шрифт редактора</source>
        <translation>Fonte do editor</translation>
    </message>
    <message>
        <source>Шрифт редактора: %1 пт</source>
        <translation>Fonte do editor: %1 pt</translation>
    </message>
</context>
</TS>
