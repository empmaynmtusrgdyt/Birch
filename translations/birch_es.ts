<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="es" sourcelanguage="ru">
<context>
    <name>CommandLine</name>
    <message>
        <source>неизвестный ключ «%1»</source>
        <translation>opción no reconocida «%1»</translation>
    </message>
    <message>
        <source>ключу «%1» нужно значение</source>
        <translation>la opción «%1» requiere un argumento</translation>
    </message>
    <message>
        <source>лишний аргумент «%1»: можно указать только один файл</source>
        <translation>operando adicional «%1»: solo se puede indicar un archivo</translation>
    </message>
    <message>
        <source>ключ «%1» работает только вместе с -o</source>
        <translation>la opción «%1» solo funciona junto con -o</translation>
    </message>
    <message>
        <source>чтение схемы со стандартного ввода («-») работает только вместе с -o</source>
        <translation>leer el diagrama de la entrada estándar («-») solo funciona junto con -o</translation>
    </message>
    <message>
        <source>ключу «-o» нужно значение</source>
        <translation>la opción «-o» requiere un argumento</translation>
    </message>
    <message>
        <source>неизвестный формат файла «%1»: поддерживаются .png, .svg и .mmd</source>
        <translation>formato de archivo desconocido «%1»: se admiten .png, .svg y .mmd</translation>
    </message>
    <message>
        <source>не указан файл схемы</source>
        <translation>no se indicó ningún archivo de diagrama</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--theme»: допустимы light и dark</source>
        <translation>valor «%1» no válido para la opción «--theme»: los valores válidos son light y dark</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--scale»: нужно целое число от 1 до %2</source>
        <translation>valor «%1» no válido para la opción «--scale»: se espera un número entero de 1 a %2</translation>
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
        <translation>Uso: %1 [OPCIÓN]... [ARCHIVO]
Abre ARCHIVO (.birch) en el editor o, con -o, dibuja a partir de él un
diagrama de flujo en un archivo sin abrir ninguna ventana.
Si ARCHIVO es «-», el texto del diagrama se lee de la entrada estándar.

Los argumentos obligatorios en opciones largas también lo son en las cortas.
  -o, --output=ARCHIVO     guardar el diagrama en ARCHIVO y salir; la
                             extensión fija el formato: .png, .svg o .mmd
  -t, --theme=TEMA         tema: light (predeterminado) o dark
  -m, --monochrome         diagrama en blanco y negro
  -H, --horizontal         colocar los bloques de izquierda a derecha
  -a, --transparent        fondo transparente (PNG y SVG)
  -s, --scale=N            escala del PNG de 1 a 10 (predeterminada 2)
  -h, --help               mostrar esta ayuda y salir
  -v, --version            mostrar la información de versión y salir

Ejemplos:
  %1 diagrama.birch                  abrir el diagrama en el editor
  %1 diagrama.birch -o diagrama.png  dibujar el diagrama en PNG
  %1 -t dark -s 3 diagrama.birch -o diagrama.png
  cat diagrama.birch | %1 - -o diagrama.svg

Estado de salida:
  0  correcto
  1  el texto del diagrama tiene errores o el diagrama está vacío
  2  error al leer o escribir un archivo
  3  opciones de línea de órdenes no válidas

Página web: &lt;https://github.com/empmaynmtusrgdyt/Birch&gt;
Informe de errores: &lt;https://github.com/empmaynmtusrgdyt/Birch/issues&gt;
</translation>
    </message>
    <message>
        <source>Лицензия MPL 2.0: Mozilla Public License 2.0 &lt;https://mozilla.org/MPL/2.0/&gt;
Это свободное ПО: вы можете изменять и распространять его.
НИКАКИХ ГАРАНТИЙ не предоставляется в пределах, допускаемых законом.

Используются: Qt %1, Graphviz %2
</source>
        <translation>Licencia MPL 2.0: Mozilla Public License 2.0 &lt;https://mozilla.org/MPL/2.0/&gt;
Esto es software libre: usted es libre de modificarlo y redistribuirlo.
NO hay GARANTÍA, en la medida en que lo permita la ley.

Usa: Qt %1, Graphviz %2
</translation>
    </message>
    <message>
        <source>%1: %2
Попробуйте «%1 --help» для получения дополнительной информации.
</source>
        <translation>%1: %2
Pruebe «%1 --help» para más información.
</translation>
    </message>
    <message>
        <source>%1: не удалось открыть «%2»: %3
</source>
        <translation>%1: no se puede abrir «%2»: %3
</translation>
    </message>
    <message>
        <source>ошибка</source>
        <translation>error</translation>
    </message>
    <message>
        <source>предупреждение</source>
        <translation>aviso</translation>
    </message>
    <message>
        <source>%1: схема пуста — нечего рисовать
</source>
        <translation>%1: el diagrama está vacío; no hay nada que dibujar
</translation>
    </message>
    <message>
        <source>%1: не удалось записать «%2»: %3
</source>
        <translation>%1: no se puede escribir «%2»: %3
</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--screenshot-theme»</source>
        <translation>valor «%1» no válido para la opción «--screenshot-theme»</translation>
    </message>
</context>
<context>
    <name>InstallCommand</name>
    <message>
        <source>Откройте новое окно терминала и попробуйте:

    birch --help
    birch схема.birch -o схема.png</source>
        <translation>Abra una ventana de terminal nueva y pruebe:

    birch --help
    birch diagrama.birch -o diagrama.png</translation>
    </message>
    <message>
        <source>Команда birch установлена: %1.</source>
        <translation>La orden birch está instalada: %1.</translation>
    </message>
    <message>
        <source>

Папки ~/.local/bin пока нет в PATH. Обычно она появляется там после повторного входа в систему. Если нет — добавьте в ~/.bashrc строку:

    export PATH=&quot;$HOME/.local/bin:$PATH&quot;</source>
        <translation>

~/.local/bin todavía no está en su PATH. Normalmente aparece después de volver a iniciar sesión. Si no, añada esta línea a ~/.bashrc:

    export PATH=&quot;$HOME/.local/bin:$PATH&quot;</translation>
    </message>
    <message>
        <source>

Если переместите файл AppImage, выберите этот пункт ещё раз.</source>
        <translation>

Si mueve el archivo AppImage, vuelva a elegir esta opción del menú.</translation>
    </message>
    <message>
        <source>Команда birch</source>
        <translation>Orden birch</translation>
    </message>
    <message>
        <source>Birch установит команду birch в /usr/local/bin.</source>
        <translation>Birch instalará la orden birch en /usr/local/bin.</translation>
    </message>
    <message>
        <source>Не удалось установить команду birch.

%1</source>
        <translation>No se pudo instalar la orden birch.

%1</translation>
    </message>
    <message>
        <source>Birch запущен не из папки «Программы», поэтому команда на него перестанет работать.

Перетащите Birch в «Программы», откройте его оттуда и выберите этот пункт ещё раз. Если Birch уже там, выполните в Терминале:

    xattr -dr com.apple.quarantine /Applications/Birch.app</source>
        <translation>Birch no se está ejecutando desde la carpeta Aplicaciones, así que una orden que apunte a él dejaría de funcionar.

Arrastre Birch a Aplicaciones, ábralo desde allí y vuelva a elegir esta opción del menú. Si Birch ya está allí, ejecute en Terminal:

    xattr -dr com.apple.quarantine /Applications/Birch.app</translation>
    </message>
    <message>
        <source>Команда birch уже установлена: %1.</source>
        <translation>La orden birch ya está instalada: %1.</translation>
    </message>
    <message>
        <source>По пути %1 уже есть другой файл. Заменить его командой Birch?</source>
        <translation>Ya hay otro archivo en %1. ¿Reemplazarlo por la orden birch?</translation>
    </message>
    <message>
        <source>Не удалось установить команду birch: %1.</source>
        <translation>No se pudo instalar la orden birch: %1.</translation>
    </message>
</context>
<context>
    <name>Language</name>
    <message>
        <source>Русский</source>
        <extracomment>Название языка этого перевода на нём самом: Deutsch, Español, 日本語</extracomment>
        <translation>Español</translation>
    </message>
</context>
<context>
    <name>Parser</name>
    <message>
        <source>прямоугольник [ ]</source>
        <translation>rectángulo [ ]</translation>
    </message>
    <message>
        <source>ромб { }</source>
        <translation>rombo { }</translation>
    </message>
    <message>
        <source>овал ( )</source>
        <translation>óvalo ( )</translation>
    </message>
    <message>
        <source>Непонятный текст «%1». Узлы пишутся в скобках: [Шаг], {Условие?}, (Старт)</source>
        <translation>Texto inesperado «%1». Los nodos se escriben entre corchetes: [Paso], {¿Condición?}, (Inicio)</translation>
    </message>
    <message>
        <source>Не закрыта скобка «%1» — ожидалась «%2» до конца строки</source>
        <translation>«%1» sin cerrar: se esperaba «%2» antes del final de la línea</translation>
    </message>
    <message>
        <source>Пустой узел «%1%2» — добавьте текст</source>
        <translation>Nodo vacío «%1%2»: añada texto</translation>
    </message>
    <message>
        <source>Слишком длинный текст узла (больше %1 символов)</source>
        <translation>El texto del nodo es demasiado largo (más de %1 caracteres)</translation>
    </message>
    <message>
        <source>Не закрыт заголовок рамки — ожидался «=» в конце, например =Авторизация=</source>
        <translation>Título de marco sin cerrar: se esperaba «=» al final, por ejemplo =Acceso=</translation>
    </message>
    <message>
        <source>Непонятная запись «%1». Рамка открывается так: =Название=, а закрывается так: ==</source>
        <translation>«%1» inesperado. Un marco se abre con =Nombre= y se cierra con ==</translation>
    </message>
    <message>
        <source>Слишком длинное название рамки (больше %1 символов)</source>
        <translation>El nombre del marco es demasiado largo (más de %1 caracteres)</translation>
    </message>
    <message>
        <source>Не закрыта кавычка — ожидалась «%1» до конца строки</source>
        <translation>Comillas sin cerrar: se esperaba «%1» antes del final de la línea</translation>
    </message>
    <message>
        <source>Слишком длинная подпись (больше %1 символов)</source>
        <translation>La etiqueta es demasiado larga (más de %1 caracteres)</translation>
    </message>
    <message>
        <source>узел «%1»</source>
        <translation>el nodo «%1»</translation>
    </message>
    <message>
        <source>подпись «%1»</source>
        <translation>la etiqueta «%1»</translation>
    </message>
    <message>
        <source>рамка «=%1=»</source>
        <translation>el marco «=%1=»</translation>
    </message>
    <message>
        <source>конец строки</source>
        <translation>el final de la línea</translation>
    </message>
    <message>
        <source>конец текста</source>
        <translation>el final del texto</translation>
    </message>
    <message>
        <source>ошибочный фрагмент</source>
        <translation>un fragmento no válido</translation>
    </message>
    <message>
        <source>После «%1» в той же строке может идти только блок, например %1 [Шаг]. Чтобы открыть рамку, напишите %1 на отдельной строке</source>
        <translation>Después de «%1» solo puede ir un bloque en la misma línea, por ejemplo %1 [Paso]. Para abrir un marco, escriba %1 en una línea aparte</translation>
    </message>
    <message>
        <source>Рамка «%1» не найдена. Рамка открывается на отдельной строке: =%1=</source>
        <translation>No se encontró el marco «%1». Un marco se abre en una línea aparte: =%1=</translation>
    </message>
    <message>
        <source>%1, а найдено %2</source>
        <translation>%1, pero se encontró %2</translation>
    </message>
    <message>
        <source>Заголовок рамки пишется на отдельной строке: %1</source>
        <translation>El título de un marco va en una línea aparte: %1</translation>
    </message>
    <message>
        <source>Инструкция должна начинаться с узла, например [Шаг], а найдено %1</source>
        <translation>Una instrucción debe empezar por un nodo, por ejemplo [Paso], pero se encontró %1</translation>
    </message>
    <message>
        <source>Ожидался узел, например [Шаг]</source>
        <translation>Se esperaba un nodo, por ejemplo [Paso]</translation>
    </message>
    <message>
        <source>Подпись должна идти после стрелки и узла: [A] -&gt; [B] &quot;Да&quot;</source>
        <translation>La etiqueta va después de la flecha y el nodo: [A] -&gt; [B] &quot;Sí&quot;</translation>
    </message>
    <message>
        <source>После «-&gt;» ожидался узел</source>
        <translation>Se esperaba un nodo después de «-&gt;»</translation>
    </message>
    <message>
        <source>Ожидались «-&gt;», «;» или новая строка, а найдено %1</source>
        <translation>Se esperaba «-&gt;», «;» o una línea nueva, pero se encontró %1</translation>
    </message>
    <message>
        <source>Слишком много узлов (максимум %1)</source>
        <translation>Demasiados nodos (como máximo %1)</translation>
    </message>
    <message>
        <source>Узел «%1» уже объявлен как %2 — форма %3 проигнорирована</source>
        <translation>El nodo «%1» ya está declarado como %2: se ignora la forma %3</translation>
    </message>
    <message>
        <source>Слишком много связей (максимум %1)</source>
        <translation>Demasiadas flechas (como máximo %1)</translation>
    </message>
    <message>
        <source>Блок «%1» есть среди общих блоков. Чтобы провести стрелку к нему, напишите == %2</source>
        <translation>El bloque «%1» es uno de los bloques comunes. Para trazar una flecha hacia él, escriba == %2</translation>
    </message>
    <message>
        <source>Текст слишком большой (больше %1 символов)</source>
        <translation>El texto es demasiado grande (más de %1 caracteres)</translation>
    </message>
</context>
<context>
    <name>Renderer</name>
    <message>
        <source>Не удалось инициализировать Graphviz</source>
        <translation>No se pudo inicializar Graphviz</translation>
    </message>
    <message>
        <source>Graphviz: не удалось создать граф</source>
        <translation>Graphviz: no se pudo crear el grafo</translation>
    </message>
    <message>
        <source>Graphviz: ошибка раскладки. %1</source>
        <translation>Graphviz: error de disposición. %1</translation>
    </message>
    <message>
        <source>Graphviz: ошибка рендеринга SVG. %1</source>
        <translation>Graphviz: error al generar el SVG. %1</translation>
    </message>
    <message>
        <source>Не удалось разобрать SVG от Graphviz</source>
        <translation>No se pudo leer el SVG generado por Graphviz</translation>
    </message>
    <message>
        <source>Недостаточно памяти для картинки %1×%2</source>
        <translation>No hay memoria suficiente para una imagen de %1×%2</translation>
    </message>
</context>
<context>
    <name>ShellCommand</name>
    <message>
        <source>не удалось создать папку «%1»</source>
        <translation>no se pudo crear la carpeta «%1»</translation>
    </message>
    <message>
        <source>не удалось заменить «%1»: %2</source>
        <translation>no se pudo reemplazar «%1»: %2</translation>
    </message>
    <message>
        <source>не удалось создать «%1»: %2</source>
        <translation>no se pudo crear «%1»: %2</translation>
    </message>
</context>
<context>
    <name>birch::CodeEditor</name>
    <message>
        <source>Опишите блок-схему, например:

[Старт] -&gt; [Шаг] -&gt; [Готово];</source>
        <translation>Describa un diagrama de flujo, por ejemplo:

[Inicio] -&gt; [Paso] -&gt; [Listo];</translation>
    </message>
    <message>
        <source>Ошибка</source>
        <translation>Error</translation>
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
            <numerusform>%n coincidencia</numerusform>
            <numerusform>%n coincidencias</numerusform>
        </translation>
    </message>
    <message>
        <source>Найти</source>
        <translation>Buscar</translation>
    </message>
    <message>
        <source>Заменить на</source>
        <translation>Reemplazar por</translation>
    </message>
    <message>
        <source>Предыдущее совпадение</source>
        <translation>Coincidencia anterior</translation>
    </message>
    <message>
        <source>Следующее совпадение</source>
        <translation>Coincidencia siguiente</translation>
    </message>
    <message>
        <source>Учитывать регистр</source>
        <translation>Distinguir mayúsculas y minúsculas</translation>
    </message>
    <message>
        <source>Регулярное выражение. В замене $1, $2… — найденные группы</source>
        <translation>Expresión regular. En el reemplazo, $1, $2… son los grupos capturados</translation>
    </message>
    <message>
        <source>Закрыть (Esc)</source>
        <translation>Cerrar (Esc)</translation>
    </message>
    <message>
        <source>Заменить</source>
        <translation>Reemplazar</translation>
    </message>
    <message>
        <source>Заменить все</source>
        <translation>Reemplazar todo</translation>
    </message>
    <message>
        <source>Заменено: %1</source>
        <translation>Reemplazos: %1</translation>
    </message>
    <message>
        <source>Ошибка</source>
        <translation>Error</translation>
    </message>
    <message>
        <source>Не найдено</source>
        <translation>No encontrado</translation>
    </message>
    <message>
        <source>%1 из %2</source>
        <translation>%1 de %2</translation>
    </message>
    <message>
        <source>Ошибка в регулярном выражении: %1</source>
        <translation>Error en la expresión regular: %1</translation>
    </message>
</context>
<context>
    <name>birch::MainWindow</name>
    <message>
        <source>Новый</source>
        <translation>Nuevo</translation>
    </message>
    <message>
        <source>Открыть…</source>
        <translation>Abrir…</translation>
    </message>
    <message>
        <source>Сохранить</source>
        <translation>Guardar</translation>
    </message>
    <message>
        <source>Сохранить как…</source>
        <translation>Guardar como…</translation>
    </message>
    <message>
        <source>Открыть пример</source>
        <translation>Abrir ejemplo</translation>
    </message>
    <message>
        <source>Открыть пример с рамками</source>
        <translation>Abrir ejemplo con marcos</translation>
    </message>
    <message>
        <source>Выход</source>
        <translation>Salir</translation>
    </message>
    <message>
        <source>Отменить</source>
        <translation>Deshacer</translation>
    </message>
    <message>
        <source>Повторить</source>
        <translation>Rehacer</translation>
    </message>
    <message>
        <source>Вырезать</source>
        <translation>Cortar</translation>
    </message>
    <message>
        <source>Копировать</source>
        <translation>Copiar</translation>
    </message>
    <message>
        <source>Вставить</source>
        <translation>Pegar</translation>
    </message>
    <message>
        <source>Выбрать все</source>
        <translation>Seleccionar todo</translation>
    </message>
    <message>
        <source>Найти…</source>
        <translation>Buscar…</translation>
    </message>
    <message>
        <source>Заменить…</source>
        <translation>Reemplazar…</translation>
    </message>
    <message>
        <source>Найти далее</source>
        <translation>Buscar siguiente</translation>
    </message>
    <message>
        <source>Найти назад</source>
        <translation>Buscar anterior</translation>
    </message>
    <message>
        <source>Рендерить</source>
        <translation>Dibujar</translation>
    </message>
    <message>
        <source>Перерисовать схему (%1)</source>
        <translation>Volver a dibujar el diagrama (%1)</translation>
    </message>
    <message>
        <source>Автоматический рендеринг</source>
        <translation>Dibujar automáticamente</translation>
    </message>
    <message>
        <source>Скачать PNG…</source>
        <translation>Guardar PNG…</translation>
    </message>
    <message>
        <source>Сохранить картинку в файл .png (%1)</source>
        <translation>Guardar la imagen como archivo .png (%1)</translation>
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
        <translation>Texto del diagrama para un README en GitHub, Notion u Obsidian: lo muestran como diagrama</translation>
    </message>
    <message>
        <source>Копировать в буфер</source>
        <translation>Copiar al portapapeles</translation>
    </message>
    <message>
        <source>Скопировать PNG — можно сразу вставить в Telegram, Slack, Word (%1)</source>
        <translation>Copiar como PNG para pegarlo directamente en Telegram, Slack o Word (%1)</translation>
    </message>
    <message>
        <source>Горизонтальная схема</source>
        <translation>Disposición horizontal</translation>
    </message>
    <message>
        <source>Располагать блоки слева направо</source>
        <translation>Colocar los bloques de izquierda a derecha</translation>
    </message>
    <message>
        <source>Чёрно-белая схема</source>
        <translation>Blanco y negro</translation>
    </message>
    <message>
        <source>Ч/Б</source>
        <translation>B/N</translation>
    </message>
    <message>
        <source>Чёрно-белая схема — для печати и документов (%1)</source>
        <translation>Diagrama en blanco y negro, para imprimir y para documentos (%1)</translation>
    </message>
    <message>
        <source>Прозрачный фон</source>
        <translation>Fondo transparente</translation>
    </message>
    <message>
        <source>Увеличить</source>
        <translation>Acercar</translation>
    </message>
    <message>
        <source>Уменьшить</source>
        <translation>Alejar</translation>
    </message>
    <message>
        <source>Реальный размер</source>
        <translation>Tamaño real</translation>
    </message>
    <message>
        <source>По размеру окна</source>
        <translation>Ajustar a la ventana</translation>
    </message>
    <message>
        <source>Как в системе</source>
        <translation>Como el sistema</translation>
    </message>
    <message>
        <source>Светлая</source>
        <translation>Claro</translation>
    </message>
    <message>
        <source>Тёмная</source>
        <translation>Oscuro</translation>
    </message>
    <message>
        <source>Как в превью</source>
        <translation>Como la vista previa</translation>
    </message>
    <message>
        <source>Всегда светлая</source>
        <translation>Siempre claro</translation>
    </message>
    <message>
        <source>Всегда тёмная</source>
        <translation>Siempre oscuro</translation>
    </message>
    <message>
        <source>Синтаксис…</source>
        <translation>Sintaxis…</translation>
    </message>
    <message>
        <source>О программе Birch</source>
        <translation>Acerca de Birch</translation>
    </message>
    <message>
        <source>Установить команду birch…</source>
        <translation>Instalar la orden birch…</translation>
    </message>
    <message>
        <source>Файл</source>
        <translation>Archivo</translation>
    </message>
    <message>
        <source>Правка</source>
        <translation>Edición</translation>
    </message>
    <message>
        <source>Вид</source>
        <translation>Ver</translation>
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
        <translation>Exportación de imagen</translation>
    </message>
    <message>
        <source>Тема картинки</source>
        <translation>Tema de la imagen</translation>
    </message>
    <message>
        <source>Справка</source>
        <translation>Ayuda</translation>
    </message>
    <message>
        <source>Панель инструментов</source>
        <translation>Barra de herramientas</translation>
    </message>
    <message>
        <source>Схема появится здесь.
Опишите её слева, например:
[Старт] -&gt; [Готово]</source>
        <translation>Aquí aparecerá el diagrama.
Descríbalo a la izquierda, por ejemplo:
[Inicio] -&gt; [Listo]</translation>
    </message>
    <message>
        <source>Узлов: %1 · связей: %2 · %3 мс</source>
        <translation>Nodos: %1 · flechas: %2 · %3 ms</translation>
    </message>
    <message>
        <source>Строка %1: %2</source>
        <translation>Línea %1: %2</translation>
    </message>
    <message>
        <source>Нечего экспортировать — схема пуста</source>
        <translation>Nada que exportar: el diagrama está vacío</translation>
    </message>
    <message>
        <source>Ошибка рендеринга</source>
        <translation>Error al dibujar</translation>
    </message>
    <message>
        <source>В тексте есть ошибки — в картинку попали только корректные строки</source>
        <translation>El texto tiene errores: en la imagen solo están las líneas correctas</translation>
    </message>
    <message>
        <source>Скачать PNG</source>
        <translation>Guardar PNG</translation>
    </message>
    <message>
        <source>Изображение PNG (*.png)</source>
        <translation>Imagen PNG (*.png)</translation>
    </message>
    <message>
        <source>Не удалось сохранить</source>
        <translation>No se pudo guardar</translation>
    </message>
    <message>
        <source>Не удалось записать файл:
%1

%2</source>
        <translation>No se pudo escribir el archivo:
%1

%2</translation>
    </message>
    <message>
        <source>Сохранено: %1 (%2×%3)</source>
        <translation>Guardado: %1 (%2×%3)</translation>
    </message>
    <message>
        <source>Экспорт SVG</source>
        <translation>Exportar SVG</translation>
    </message>
    <message>
        <source>Векторное изображение SVG (*.svg)</source>
        <translation>Imagen vectorial SVG (*.svg)</translation>
    </message>
    <message>
        <source>Сохранено: %1</source>
        <translation>Guardado: %1</translation>
    </message>
    <message>
        <source>Картинка скопирована — вставьте её в Telegram, Slack, Word…</source>
        <translation>Imagen copiada: péguela en Telegram, Slack, Word…</translation>
    </message>
    <message>
        <source>В тексте есть ошибки — в Mermaid попали только корректные строки</source>
        <translation>El texto tiene errores: en el diagrama Mermaid solo están las líneas correctas</translation>
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
        <translation>Mermaid copiado: péguelo en un README de GitHub, en Notion u Obsidian</translation>
    </message>
    <message>
        <source>Открыть схему</source>
        <translation>Abrir diagrama</translation>
    </message>
    <message>
        <source>Схемы Birch (*.birch *.txt);;Все файлы (*)</source>
        <translation>Diagramas de Birch (*.birch *.txt);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Файл слишком большой</source>
        <translation>Archivo demasiado grande</translation>
    </message>
    <message>
        <source>Файл «%1» больше %2 МБ.</source>
        <translation>El archivo «%1» ocupa más de %2 MB.</translation>
    </message>
    <message>
        <source>Не удалось открыть</source>
        <translation>No se pudo abrir</translation>
    </message>
    <message>
        <source>%1

%2</source>
        <translation>%1

%2</translation>
    </message>
    <message>
        <source>Сохранить схему</source>
        <translation>Guardar diagrama</translation>
    </message>
    <message>
        <source>Схемы Birch (*.birch)</source>
        <translation>Diagramas de Birch (*.birch)</translation>
    </message>
    <message>
        <source>Сохранить изменения?</source>
        <translation>¿Guardar los cambios?</translation>
    </message>
    <message>
        <source>В файле «%1» есть несохранённые изменения.</source>
        <translation>«%1» tiene cambios sin guardar.</translation>
    </message>
    <message>
        <source>Без названия</source>
        <translation>Sin título</translation>
    </message>
    <message>
        <source>Синтаксис Birch</source>
        <translation>Sintaxis de Birch</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Синтаксис Birch&lt;/h3&gt;&lt;table cellspacing=&apos;6&apos;&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[Текст]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;шаг (прямоугольник)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;{Условие?}&lt;/code&gt;&lt;/td&gt;&lt;td&gt;решение (ромб)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;(Старт)&lt;/code&gt;&lt;/td&gt;&lt;td&gt;начало / конец&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;стрелка; можно цепочкой: &lt;code&gt;[A] -&amp;gt; [B] -&amp;gt; [C]&lt;/code&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B] &quot;Да&quot;&lt;/code&gt;&lt;/td&gt;&lt;td&gt;подпись стрелки&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;;&lt;/code&gt; или Enter&lt;/td&gt;&lt;td&gt;конец инструкции&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;// …&lt;/code&gt; или &lt;code&gt;# …&lt;/code&gt;&lt;/td&gt;&lt;td&gt;комментарий&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;\n&lt;/code&gt;&lt;/td&gt;&lt;td&gt;перенос строки в тексте&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Рамка=&lt;/code&gt;&lt;/td&gt;&lt;td&gt;на отдельной строке: открыть рамку; &lt;code&gt;==&lt;/code&gt; — закрыть&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Рамка= [A]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;блок из рамки «Рамка»; &lt;code&gt;== [A]&lt;/code&gt; — общий блок&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;&lt;p&gt;Одинаковый текст — один и тот же узел, но в каждой рамке свои блоки: «[Шаг]» в двух рамках — два разных блока. Пример — «Файл → Открыть пример с рамками».&lt;/p&gt;&lt;p&gt;Если узел хоть раз записан как &lt;code&gt;{…}&lt;/code&gt; или &lt;code&gt;(…)&lt;/code&gt;, он останется этой фигурой.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Sintaxis de Birch&lt;/h3&gt;&lt;table cellspacing=&apos;6&apos;&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[Texto]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;paso (rectángulo)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;{¿Condición?}&lt;/code&gt;&lt;/td&gt;&lt;td&gt;decisión (rombo)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;(Inicio)&lt;/code&gt;&lt;/td&gt;&lt;td&gt;inicio / fin&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;flecha; también en cadena: &lt;code&gt;[A] -&amp;gt; [B] -&amp;gt; [C]&lt;/code&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B] &quot;Sí&quot;&lt;/code&gt;&lt;/td&gt;&lt;td&gt;etiqueta de la flecha&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;;&lt;/code&gt; o Intro&lt;/td&gt;&lt;td&gt;fin de la instrucción&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;// …&lt;/code&gt; o &lt;code&gt;# …&lt;/code&gt;&lt;/td&gt;&lt;td&gt;comentario&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;\n&lt;/code&gt;&lt;/td&gt;&lt;td&gt;salto de línea en el texto&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Marco=&lt;/code&gt;&lt;/td&gt;&lt;td&gt;en una línea aparte: abrir un marco; &lt;code&gt;==&lt;/code&gt; lo cierra&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Marco= [A]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;bloque del marco «Marco»; &lt;code&gt;== [A]&lt;/code&gt; es un bloque común&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;&lt;p&gt;El mismo texto es el mismo nodo, pero cada marco tiene sus propios bloques: «[Paso]» en dos marcos son dos bloques distintos. Vea Archivo → Abrir ejemplo con marcos.&lt;/p&gt;&lt;p&gt;Si un nodo se escribe al menos una vez como &lt;code&gt;{…}&lt;/code&gt; o &lt;code&gt;(…)&lt;/code&gt;, conserva esa forma.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Язык интерфейса изменится после перезапуска Birch.</source>
        <translation>El idioma de la interfaz cambiará al reiniciar Birch.</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Birch %1&lt;/h3&gt;&lt;p&gt;Блок-схемы из текста — без ИИ, полностью офлайн.&lt;/p&gt;&lt;p&gt;Graphviz %2 · Qt %3&lt;/p&gt;&lt;p&gt;Распространяется по лицензии &lt;a href=&quot;https://mozilla.org/MPL/2.0/&quot;&gt;Mozilla Public License 2.0&lt;/a&gt;.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Birch %1&lt;/h3&gt;&lt;p&gt;Diagramas de flujo a partir de texto, sin IA y totalmente sin conexión.&lt;/p&gt;&lt;p&gt;Graphviz %2 · Qt %3&lt;/p&gt;&lt;p&gt;Distribuido bajo la &lt;a href=&quot;https://mozilla.org/MPL/2.0/&quot;&gt;Mozilla Public License 2.0&lt;/a&gt;.&lt;/p&gt;</translation>
    </message>
</context>
</TS>
