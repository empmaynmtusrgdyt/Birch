<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="fr" sourcelanguage="ru">
<context>
    <name>CommandLine</name>
    <message>
        <source>неизвестный ключ «%1»</source>
        <translation>option non reconnue «&#xa0;%1&#xa0;»</translation>
    </message>
    <message>
        <source>ключу «%1» нужно значение</source>
        <translation>l&apos;option «&#xa0;%1&#xa0;» nécessite un argument</translation>
    </message>
    <message>
        <source>лишний аргумент «%1»: можно указать только один файл</source>
        <translation>opérande en trop «&#xa0;%1&#xa0;»&#xa0;: un seul fichier peut être indiqué</translation>
    </message>
    <message>
        <source>ключ «%1» работает только вместе с -o</source>
        <translation>l&apos;option «&#xa0;%1&#xa0;» ne fonctionne qu&apos;avec -o</translation>
    </message>
    <message>
        <source>чтение схемы со стандартного ввода («-») работает только вместе с -o</source>
        <translation>la lecture du diagramme sur l&apos;entrée standard («&#xa0;-&#xa0;») ne fonctionne qu&apos;avec -o</translation>
    </message>
    <message>
        <source>ключу «-o» нужно значение</source>
        <translation>l&apos;option «&#xa0;-o&#xa0;» nécessite un argument</translation>
    </message>
    <message>
        <source>неизвестный формат файла «%1»: поддерживаются .png, .svg и .mmd</source>
        <translation>format de fichier inconnu «&#xa0;%1&#xa0;»&#xa0;: .png, .svg et .mmd sont pris en charge</translation>
    </message>
    <message>
        <source>не указан файл схемы</source>
        <translation>aucun fichier de diagramme indiqué</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--theme»: допустимы light и dark</source>
        <translation>valeur «&#xa0;%1&#xa0;» invalide pour l&apos;option «&#xa0;--theme&#xa0;»&#xa0;: valeurs possibles light et dark</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--scale»: нужно целое число от 1 до %2</source>
        <translation>valeur «&#xa0;%1&#xa0;» invalide pour l&apos;option «&#xa0;--scale&#xa0;»&#xa0;: nombre entier de 1 à %2 attendu</translation>
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
        <translation>Utilisation&#xa0;: %1 [OPTION]... [FICHIER]
Ouvre FICHIER (.birch) dans l&apos;éditeur ou, avec -o, en dessine un
organigramme dans un fichier sans ouvrir de fenêtre.
Si FICHIER est «&#xa0;-&#xa0;», le texte du diagramme est lu sur l&apos;entrée standard.

Les arguments obligatoires des options longues le sont aussi pour les courtes.
  -o, --output=FICHIER     enregistrer le diagramme dans FICHIER et quitter&#xa0;;
                             l&apos;extension fixe le format&#xa0;: .png, .svg ou .mmd
  -t, --theme=THÈME        thème&#xa0;: light (par défaut) ou dark
  -m, --monochrome         diagramme en noir et blanc
  -H, --horizontal         disposer les blocs de gauche à droite
  -a, --transparent        fond transparent (PNG et SVG)
  -s, --scale=N            échelle PNG de 1 à 10 (par défaut 2)
  -h, --help               afficher cette aide et quitter
  -v, --version            afficher les informations de version et quitter

Exemples&#xa0;:
  %1 diagramme.birch                   ouvrir le diagramme dans l&apos;éditeur
  %1 diagramme.birch -o diagramme.png  dessiner le diagramme en PNG
  %1 -t dark -s 3 diagramme.birch -o diagramme.png
  cat diagramme.birch | %1 - -o diagramme.svg

Code de sortie&#xa0;:
  0  succès
  1  le texte du diagramme contient des erreurs ou le diagramme est vide
  2  erreur de lecture ou d&apos;écriture d&apos;un fichier
  3  options de ligne de commande invalides

Page d&apos;accueil&#xa0;: &lt;https://github.com/empmaynmtusrgdyt/Birch&gt;
Signaler les bogues&#xa0;: &lt;https://github.com/empmaynmtusrgdyt/Birch/issues&gt;
</translation>
    </message>
    <message>
        <source>Лицензия MPL 2.0: Mozilla Public License 2.0 &lt;https://mozilla.org/MPL/2.0/&gt;
Это свободное ПО: вы можете изменять и распространять его.
НИКАКИХ ГАРАНТИЙ не предоставляется в пределах, допускаемых законом.

Используются: Qt %1, Graphviz %2
</source>
        <translation>Licence MPL 2.0&#xa0;: Mozilla Public License 2.0 &lt;https://mozilla.org/MPL/2.0/&gt;
Ceci est un logiciel libre&#xa0;: vous êtes libre de le modifier et de le redistribuer.
Il n&apos;y a AUCUNE GARANTIE, dans la mesure permise par la loi.

Utilise&#xa0;: Qt %1, Graphviz %2
</translation>
    </message>
    <message>
        <source>%1: %2
Попробуйте «%1 --help» для получения дополнительной информации.
</source>
        <translation>%1&#xa0;: %2
Saisissez «&#xa0;%1 --help&#xa0;» pour plus d&apos;informations.
</translation>
    </message>
    <message>
        <source>%1: не удалось открыть «%2»: %3
</source>
        <translation>%1&#xa0;: impossible d&apos;ouvrir «&#xa0;%2&#xa0;»&#xa0;: %3
</translation>
    </message>
    <message>
        <source>ошибка</source>
        <translation>erreur</translation>
    </message>
    <message>
        <source>предупреждение</source>
        <translation>avertissement</translation>
    </message>
    <message>
        <source>%1: схема пуста — нечего рисовать
</source>
        <translation>%1&#xa0;: le diagramme est vide — rien à dessiner
</translation>
    </message>
    <message>
        <source>%1: не удалось записать «%2»: %3
</source>
        <translation>%1&#xa0;: impossible d&apos;écrire «&#xa0;%2&#xa0;»&#xa0;: %3
</translation>
    </message>
    <message>
        <source>неверное значение «%1» для ключа «--screenshot-theme»</source>
        <translation>valeur «&#xa0;%1&#xa0;» invalide pour l&apos;option «&#xa0;--screenshot-theme&#xa0;»</translation>
    </message>
</context>
<context>
    <name>InstallCommand</name>
    <message>
        <source>Откройте новое окно терминала и попробуйте:

    birch --help
    birch схема.birch -o схема.png</source>
        <translation>Ouvrez une nouvelle fenêtre de terminal et essayez&#xa0;:

    birch --help
    birch diagramme.birch -o diagramme.png</translation>
    </message>
    <message>
        <source>Команда birch установлена: %1.</source>
        <translation>La commande birch est installée&#xa0;: %1.</translation>
    </message>
    <message>
        <source>

Папки ~/.local/bin пока нет в PATH. Обычно она появляется там после повторного входа в систему. Если нет — добавьте в ~/.bashrc строку:

    export PATH=&quot;$HOME/.local/bin:$PATH&quot;</source>
        <translation>

~/.local/bin n&apos;est pas encore dans votre PATH. Il y apparaît généralement après une nouvelle connexion. Sinon, ajoutez cette ligne à ~/.bashrc&#xa0;:

    export PATH=&quot;$HOME/.local/bin:$PATH&quot;</translation>
    </message>
    <message>
        <source>

Если переместите файл AppImage, выберите этот пункт ещё раз.</source>
        <translation>

Si vous déplacez le fichier AppImage, choisissez à nouveau cet élément de menu.</translation>
    </message>
    <message>
        <source>Команда birch</source>
        <translation>Commande birch</translation>
    </message>
    <message>
        <source>Birch установит команду birch в /usr/local/bin.</source>
        <translation>Birch va installer la commande birch dans /usr/local/bin.</translation>
    </message>
    <message>
        <source>Не удалось установить команду birch.

%1</source>
        <translation>Impossible d&apos;installer la commande birch.

%1</translation>
    </message>
    <message>
        <source>Birch запущен не из папки «Программы», поэтому команда на него перестанет работать.

Перетащите Birch в «Программы», откройте его оттуда и выберите этот пункт ещё раз. Если Birch уже там, выполните в Терминале:

    xattr -dr com.apple.quarantine /Applications/Birch.app</source>
        <translation>Birch n&apos;est pas lancé depuis le dossier Applications&#xa0;: une commande qui pointe vers lui cesserait de fonctionner.

Faites glisser Birch dans Applications, ouvrez-le depuis ce dossier et choisissez à nouveau cet élément de menu. Si Birch s&apos;y trouve déjà, exécutez dans le Terminal&#xa0;:

    xattr -dr com.apple.quarantine /Applications/Birch.app</translation>
    </message>
    <message>
        <source>Команда birch уже установлена: %1.</source>
        <translation>La commande birch est déjà installée&#xa0;: %1.</translation>
    </message>
    <message>
        <source>По пути %1 уже есть другой файл. Заменить его командой Birch?</source>
        <translation>Un autre fichier existe déjà à l&apos;emplacement %1. Le remplacer par la commande birch&#xa0;?</translation>
    </message>
    <message>
        <source>Не удалось установить команду birch: %1.</source>
        <translation>Impossible d&apos;installer la commande birch&#xa0;: %1.</translation>
    </message>
</context>
<context>
    <name>Language</name>
    <message>
        <source>Русский</source>
        <extracomment>Название языка этого перевода на нём самом: Deutsch, Español, 日本語</extracomment>
        <translation>Français</translation>
    </message>
</context>
<context>
    <name>Parser</name>
    <message>
        <source>прямоугольник [ ]</source>
        <translation>rectangle [ ]</translation>
    </message>
    <message>
        <source>ромб { }</source>
        <translation>losange { }</translation>
    </message>
    <message>
        <source>овал ( )</source>
        <translation>ovale ( )</translation>
    </message>
    <message>
        <source>Непонятный текст «%1». Узлы пишутся в скобках: [Шаг], {Условие?}, (Старт)</source>
        <translation>Texte inattendu «&#xa0;%1&#xa0;». Les nœuds s&apos;écrivent entre crochets&#xa0;: [Étape], {Condition?}, (Début)</translation>
    </message>
    <message>
        <source>Не закрыта скобка «%1» — ожидалась «%2» до конца строки</source>
        <translation>«&#xa0;%1&#xa0;» non fermé — «&#xa0;%2&#xa0;» attendu avant la fin de la ligne</translation>
    </message>
    <message>
        <source>Пустой узел «%1%2» — добавьте текст</source>
        <translation>Nœud vide «&#xa0;%1%2&#xa0;» — ajoutez du texte</translation>
    </message>
    <message>
        <source>Слишком длинный текст узла (больше %1 символов)</source>
        <translation>Texte du nœud trop long (plus de %1 caractères)</translation>
    </message>
    <message>
        <source>Не закрыт заголовок рамки — ожидался «=» в конце, например =Авторизация=</source>
        <translation>Titre de cadre non fermé — «&#xa0;=&#xa0;» attendu à la fin, par exemple =Connexion=</translation>
    </message>
    <message>
        <source>Непонятная запись «%1». Рамка открывается так: =Название=, а закрывается так: ==</source>
        <translation>«&#xa0;%1&#xa0;» inattendu. Un cadre s&apos;ouvre avec =Nom= et se ferme avec ==</translation>
    </message>
    <message>
        <source>Слишком длинное название рамки (больше %1 символов)</source>
        <translation>Nom du cadre trop long (plus de %1 caractères)</translation>
    </message>
    <message>
        <source>Не закрыта кавычка — ожидалась «%1» до конца строки</source>
        <translation>Guillemet non fermé — «&#xa0;%1&#xa0;» attendu avant la fin de la ligne</translation>
    </message>
    <message>
        <source>Слишком длинная подпись (больше %1 символов)</source>
        <translation>Étiquette trop longue (plus de %1 caractères)</translation>
    </message>
    <message>
        <source>узел «%1»</source>
        <translation>le nœud «&#xa0;%1&#xa0;»</translation>
    </message>
    <message>
        <source>подпись «%1»</source>
        <translation>l&apos;étiquette «&#xa0;%1&#xa0;»</translation>
    </message>
    <message>
        <source>рамка «=%1=»</source>
        <translation>le cadre «&#xa0;=%1=&#xa0;»</translation>
    </message>
    <message>
        <source>конец строки</source>
        <translation>la fin de ligne</translation>
    </message>
    <message>
        <source>конец текста</source>
        <translation>la fin du texte</translation>
    </message>
    <message>
        <source>ошибочный фрагмент</source>
        <translation>un fragment invalide</translation>
    </message>
    <message>
        <source>После «%1» в той же строке может идти только блок, например %1 [Шаг]. Чтобы открыть рамку, напишите %1 на отдельной строке</source>
        <translation>Après «&#xa0;%1&#xa0;», seul un bloc peut suivre sur la même ligne, par exemple %1 [Étape]. Pour ouvrir un cadre, écrivez %1 sur une ligne à part</translation>
    </message>
    <message>
        <source>Рамка «%1» не найдена. Рамка открывается на отдельной строке: =%1=</source>
        <translation>Cadre «&#xa0;%1&#xa0;» introuvable. Un cadre s&apos;ouvre sur une ligne à part&#xa0;: =%1=</translation>
    </message>
    <message>
        <source>%1, а найдено %2</source>
        <translation>%1, mais trouvé %2</translation>
    </message>
    <message>
        <source>Заголовок рамки пишется на отдельной строке: %1</source>
        <translation>Un titre de cadre s&apos;écrit sur une ligne à part&#xa0;: %1</translation>
    </message>
    <message>
        <source>Инструкция должна начинаться с узла, например [Шаг], а найдено %1</source>
        <translation>Une instruction doit commencer par un nœud, par exemple [Étape], mais trouvé %1</translation>
    </message>
    <message>
        <source>Ожидался узел, например [Шаг]</source>
        <translation>Nœud attendu, par exemple [Étape]</translation>
    </message>
    <message>
        <source>Подпись должна идти после стрелки и узла: [A] -&gt; [B] &quot;Да&quot;</source>
        <translation>Une étiquette se place après la flèche et le nœud&#xa0;: [A] -&gt; [B] &quot;Oui&quot;</translation>
    </message>
    <message>
        <source>После «-&gt;» ожидался узел</source>
        <translation>Nœud attendu après «&#xa0;-&gt;&#xa0;»</translation>
    </message>
    <message>
        <source>Ожидались «-&gt;», «;» или новая строка, а найдено %1</source>
        <translation>«&#xa0;-&gt;&#xa0;», «&#xa0;;&#xa0;» ou nouvelle ligne attendu, mais trouvé %1</translation>
    </message>
    <message>
        <source>Слишком много узлов (максимум %1)</source>
        <translation>Trop de nœuds (%1 au maximum)</translation>
    </message>
    <message>
        <source>Узел «%1» уже объявлен как %2 — форма %3 проигнорирована</source>
        <translation>Le nœud «&#xa0;%1&#xa0;» est déjà déclaré comme %2 — forme %3 ignorée</translation>
    </message>
    <message>
        <source>Слишком много связей (максимум %1)</source>
        <translation>Trop de flèches (%1 au maximum)</translation>
    </message>
    <message>
        <source>Блок «%1» есть среди общих блоков. Чтобы провести стрелку к нему, напишите == %2</source>
        <translation>Le bloc «&#xa0;%1&#xa0;» fait partie des blocs communs. Pour tracer une flèche vers lui, écrivez == %2</translation>
    </message>
    <message>
        <source>Текст слишком большой (больше %1 символов)</source>
        <translation>Le texte est trop volumineux (plus de %1 caractères)</translation>
    </message>
</context>
<context>
    <name>Renderer</name>
    <message>
        <source>Не удалось инициализировать Graphviz</source>
        <translation>Impossible d&apos;initialiser Graphviz</translation>
    </message>
    <message>
        <source>Graphviz: не удалось создать граф</source>
        <translation>Graphviz&#xa0;: impossible de créer le graphe</translation>
    </message>
    <message>
        <source>Graphviz: ошибка раскладки. %1</source>
        <translation>Graphviz&#xa0;: erreur de disposition. %1</translation>
    </message>
    <message>
        <source>Graphviz: ошибка рендеринга SVG. %1</source>
        <translation>Graphviz&#xa0;: erreur de rendu SVG. %1</translation>
    </message>
    <message>
        <source>Не удалось разобрать SVG от Graphviz</source>
        <translation>Impossible de lire le SVG produit par Graphviz</translation>
    </message>
    <message>
        <source>Недостаточно памяти для картинки %1×%2</source>
        <translation>Mémoire insuffisante pour une image de %1×%2</translation>
    </message>
</context>
<context>
    <name>ShellCommand</name>
    <message>
        <source>не удалось создать папку «%1»</source>
        <translation>impossible de créer le dossier «&#xa0;%1&#xa0;»</translation>
    </message>
    <message>
        <source>не удалось заменить «%1»: %2</source>
        <translation>impossible de remplacer «&#xa0;%1&#xa0;»&#xa0;: %2</translation>
    </message>
    <message>
        <source>не удалось создать «%1»: %2</source>
        <translation>impossible de créer «&#xa0;%1&#xa0;»&#xa0;: %2</translation>
    </message>
</context>
<context>
    <name>birch::CodeEditor</name>
    <message>
        <source>Опишите блок-схему, например:

[Старт] -&gt; [Шаг] -&gt; [Готово];</source>
        <translation>Décrivez un organigramme, par exemple&#xa0;:

[Début] -&gt; [Étape] -&gt; [Terminé];</translation>
    </message>
    <message>
        <source>Ошибка</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <source>Предупреждение</source>
        <translation>Avertissement</translation>
    </message>
</context>
<context>
    <name>birch::FindBar</name>
    <message numerus="yes">
        <source>%n совпадений</source>
        <translation>
            <numerusform>%n occurrence</numerusform>
            <numerusform>%n occurrences</numerusform>
        </translation>
    </message>
    <message>
        <source>Найти</source>
        <translation>Rechercher</translation>
    </message>
    <message>
        <source>Заменить на</source>
        <translation>Remplacer par</translation>
    </message>
    <message>
        <source>Предыдущее совпадение</source>
        <translation>Occurrence précédente</translation>
    </message>
    <message>
        <source>Следующее совпадение</source>
        <translation>Occurrence suivante</translation>
    </message>
    <message>
        <source>Учитывать регистр</source>
        <translation>Respecter la casse</translation>
    </message>
    <message>
        <source>Регулярное выражение. В замене $1, $2… — найденные группы</source>
        <translation>Expression régulière. Dans le remplacement, $1, $2… désignent les groupes capturés</translation>
    </message>
    <message>
        <source>Закрыть (Esc)</source>
        <translation>Fermer (Échap)</translation>
    </message>
    <message>
        <source>Заменить</source>
        <translation>Remplacer</translation>
    </message>
    <message>
        <source>Заменить все</source>
        <translation>Tout remplacer</translation>
    </message>
    <message>
        <source>Заменено: %1</source>
        <translation>Remplacements&#xa0;: %1</translation>
    </message>
    <message>
        <source>Ошибка</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <source>Не найдено</source>
        <translation>Introuvable</translation>
    </message>
    <message>
        <source>%1 из %2</source>
        <translation>%1 sur %2</translation>
    </message>
    <message>
        <source>Ошибка в регулярном выражении: %1</source>
        <translation>Erreur dans l&apos;expression régulière&#xa0;: %1</translation>
    </message>
</context>
<context>
    <name>birch::MainWindow</name>
    <message>
        <source>Новый</source>
        <translation>Nouveau</translation>
    </message>
    <message>
        <source>Открыть…</source>
        <translation>Ouvrir…</translation>
    </message>
    <message>
        <source>Сохранить</source>
        <translation>Enregistrer</translation>
    </message>
    <message>
        <source>Сохранить как…</source>
        <translation>Enregistrer sous…</translation>
    </message>
    <message>
        <source>Открыть пример</source>
        <translation>Ouvrir l&apos;exemple</translation>
    </message>
    <message>
        <source>Открыть пример с рамками</source>
        <translation>Ouvrir l&apos;exemple avec cadres</translation>
    </message>
    <message>
        <source>Выход</source>
        <translation>Quitter</translation>
    </message>
    <message>
        <source>Отменить</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <source>Повторить</source>
        <translation>Rétablir</translation>
    </message>
    <message>
        <source>Вырезать</source>
        <translation>Couper</translation>
    </message>
    <message>
        <source>Копировать</source>
        <translation>Copier</translation>
    </message>
    <message>
        <source>Вставить</source>
        <translation>Coller</translation>
    </message>
    <message>
        <source>Выбрать все</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <source>Найти…</source>
        <translation>Rechercher…</translation>
    </message>
    <message>
        <source>Заменить…</source>
        <translation>Remplacer…</translation>
    </message>
    <message>
        <source>Найти далее</source>
        <translation>Rechercher le suivant</translation>
    </message>
    <message>
        <source>Найти назад</source>
        <translation>Rechercher le précédent</translation>
    </message>
    <message>
        <source>Рендерить</source>
        <translation>Dessiner</translation>
    </message>
    <message>
        <source>Перерисовать схему (%1)</source>
        <translation>Redessiner le diagramme (%1)</translation>
    </message>
    <message>
        <source>Автоматический рендеринг</source>
        <translation>Dessin automatique</translation>
    </message>
    <message>
        <source>Скачать PNG…</source>
        <translation>Enregistrer en PNG…</translation>
    </message>
    <message>
        <source>Сохранить картинку в файл .png (%1)</source>
        <translation>Enregistrer l&apos;image dans un fichier .png (%1)</translation>
    </message>
    <message>
        <source>Экспорт SVG…</source>
        <translation>Exporter en SVG…</translation>
    </message>
    <message>
        <source>Экспорт Mermaid…</source>
        <translation>Exporter en Mermaid…</translation>
    </message>
    <message>
        <source>Копировать как Mermaid</source>
        <translation>Copier en Mermaid</translation>
    </message>
    <message>
        <source>Текст схемы для README на GitHub, Notion, Obsidian — там он отображается как схема</source>
        <translation>Texte du diagramme pour un README sur GitHub, Notion ou Obsidian — ils l&apos;affichent sous forme de diagramme</translation>
    </message>
    <message>
        <source>Копировать в буфер</source>
        <translation>Copier dans le presse-papiers</translation>
    </message>
    <message>
        <source>Скопировать PNG — можно сразу вставить в Telegram, Slack, Word (%1)</source>
        <translation>Copier en PNG — à coller directement dans Telegram, Slack ou Word (%1)</translation>
    </message>
    <message>
        <source>Горизонтальная схема</source>
        <translation>Disposition horizontale</translation>
    </message>
    <message>
        <source>Располагать блоки слева направо</source>
        <translation>Disposer les blocs de gauche à droite</translation>
    </message>
    <message>
        <source>Чёрно-белая схема</source>
        <translation>Noir et blanc</translation>
    </message>
    <message>
        <source>Ч/Б</source>
        <translation>N/B</translation>
    </message>
    <message>
        <source>Чёрно-белая схема — для печати и документов (%1)</source>
        <translation>Diagramme en noir et blanc — pour l&apos;impression et les documents (%1)</translation>
    </message>
    <message>
        <source>Прозрачный фон</source>
        <translation>Fond transparent</translation>
    </message>
    <message>
        <source>Увеличить</source>
        <translation>Zoom avant</translation>
    </message>
    <message>
        <source>Уменьшить</source>
        <translation>Zoom arrière</translation>
    </message>
    <message>
        <source>Реальный размер</source>
        <translation>Taille réelle</translation>
    </message>
    <message>
        <source>По размеру окна</source>
        <translation>Ajuster à la fenêtre</translation>
    </message>
    <message>
        <source>Как в системе</source>
        <translation>Comme le système</translation>
    </message>
    <message>
        <source>Светлая</source>
        <translation>Clair</translation>
    </message>
    <message>
        <source>Тёмная</source>
        <translation>Sombre</translation>
    </message>
    <message>
        <source>Как в превью</source>
        <translation>Comme l&apos;aperçu</translation>
    </message>
    <message>
        <source>Всегда светлая</source>
        <translation>Toujours clair</translation>
    </message>
    <message>
        <source>Всегда тёмная</source>
        <translation>Toujours sombre</translation>
    </message>
    <message>
        <source>Синтаксис…</source>
        <translation>Syntaxe…</translation>
    </message>
    <message>
        <source>О программе Birch</source>
        <translation>À propos de Birch</translation>
    </message>
    <message>
        <source>Установить команду birch…</source>
        <translation>Installer la commande birch…</translation>
    </message>
    <message>
        <source>Файл</source>
        <translation>Fichier</translation>
    </message>
    <message>
        <source>Правка</source>
        <translation>Édition</translation>
    </message>
    <message>
        <source>Вид</source>
        <translation>Affichage</translation>
    </message>
    <message>
        <source>Тема</source>
        <translation>Thème</translation>
    </message>
    <message>
        <source>Язык</source>
        <translation>Langue</translation>
    </message>
    <message>
        <source>Экспорт картинки</source>
        <translation>Export d&apos;image</translation>
    </message>
    <message>
        <source>Тема картинки</source>
        <translation>Thème de l&apos;image</translation>
    </message>
    <message>
        <source>Справка</source>
        <translation>Aide</translation>
    </message>
    <message>
        <source>Панель инструментов</source>
        <translation>Barre d&apos;outils</translation>
    </message>
    <message>
        <source>Схема появится здесь.
Опишите её слева, например:
[Старт] -&gt; [Готово]</source>
        <translation>Le diagramme apparaîtra ici.
Décrivez-le à gauche, par exemple&#xa0;:
[Début] -&gt; [Terminé]</translation>
    </message>
    <message>
        <source>Узлов: %1 · связей: %2 · %3 мс</source>
        <translation>Nœuds&#xa0;: %1 · flèches&#xa0;: %2 · %3 ms</translation>
    </message>
    <message>
        <source>Строка %1: %2</source>
        <translation>Ligne %1&#xa0;: %2</translation>
    </message>
    <message>
        <source>Нечего экспортировать — схема пуста</source>
        <translation>Rien à exporter — le diagramme est vide</translation>
    </message>
    <message>
        <source>Ошибка рендеринга</source>
        <translation>Erreur de rendu</translation>
    </message>
    <message>
        <source>В тексте есть ошибки — в картинку попали только корректные строки</source>
        <translation>Le texte contient des erreurs — seules les lignes correctes sont dans l&apos;image</translation>
    </message>
    <message>
        <source>Скачать PNG</source>
        <translation>Enregistrer en PNG</translation>
    </message>
    <message>
        <source>Изображение PNG (*.png)</source>
        <translation>Image PNG (*.png)</translation>
    </message>
    <message>
        <source>Не удалось сохранить</source>
        <translation>Enregistrement impossible</translation>
    </message>
    <message>
        <source>Не удалось записать файл:
%1

%2</source>
        <translation>Impossible d&apos;écrire le fichier&#xa0;:
%1

%2</translation>
    </message>
    <message>
        <source>Сохранено: %1 (%2×%3)</source>
        <translation>Enregistré&#xa0;: %1 (%2×%3)</translation>
    </message>
    <message>
        <source>Экспорт SVG</source>
        <translation>Exporter en SVG</translation>
    </message>
    <message>
        <source>Векторное изображение SVG (*.svg)</source>
        <translation>Image vectorielle SVG (*.svg)</translation>
    </message>
    <message>
        <source>Сохранено: %1</source>
        <translation>Enregistré&#xa0;: %1</translation>
    </message>
    <message>
        <source>Картинка скопирована — вставьте её в Telegram, Slack, Word…</source>
        <translation>Image copiée — collez-la dans Telegram, Slack, Word…</translation>
    </message>
    <message>
        <source>В тексте есть ошибки — в Mermaid попали только корректные строки</source>
        <translation>Le texte contient des erreurs — seules les lignes correctes sont dans le diagramme Mermaid</translation>
    </message>
    <message>
        <source>Экспорт Mermaid</source>
        <translation>Exporter en Mermaid</translation>
    </message>
    <message>
        <source>Схема Mermaid (*.mmd)</source>
        <translation>Diagramme Mermaid (*.mmd)</translation>
    </message>
    <message>
        <source>Mermaid скопирован — вставьте его в README на GitHub, в Notion или Obsidian</source>
        <translation>Mermaid copié — collez-le dans un README sur GitHub, dans Notion ou Obsidian</translation>
    </message>
    <message>
        <source>Открыть схему</source>
        <translation>Ouvrir un diagramme</translation>
    </message>
    <message>
        <source>Схемы Birch (*.birch *.txt);;Все файлы (*)</source>
        <translation>Diagrammes Birch (*.birch *.txt);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Файл слишком большой</source>
        <translation>Fichier trop volumineux</translation>
    </message>
    <message>
        <source>Файл «%1» больше %2 МБ.</source>
        <translation>Le fichier «&#xa0;%1&#xa0;» dépasse %2 Mo.</translation>
    </message>
    <message>
        <source>Не удалось открыть</source>
        <translation>Ouverture impossible</translation>
    </message>
    <message>
        <source>%1

%2</source>
        <translation>%1

%2</translation>
    </message>
    <message>
        <source>Сохранить схему</source>
        <translation>Enregistrer le diagramme</translation>
    </message>
    <message>
        <source>Схемы Birch (*.birch)</source>
        <translation>Diagrammes Birch (*.birch)</translation>
    </message>
    <message>
        <source>Сохранить изменения?</source>
        <translation>Enregistrer les modifications&#xa0;?</translation>
    </message>
    <message>
        <source>В файле «%1» есть несохранённые изменения.</source>
        <translation>«&#xa0;%1&#xa0;» contient des modifications non enregistrées.</translation>
    </message>
    <message>
        <source>Без названия</source>
        <translation>Sans titre</translation>
    </message>
    <message>
        <source>Синтаксис Birch</source>
        <translation>Syntaxe de Birch</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Синтаксис Birch&lt;/h3&gt;&lt;table cellspacing=&apos;6&apos;&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[Текст]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;шаг (прямоугольник)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;{Условие?}&lt;/code&gt;&lt;/td&gt;&lt;td&gt;решение (ромб)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;(Старт)&lt;/code&gt;&lt;/td&gt;&lt;td&gt;начало / конец&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;стрелка; можно цепочкой: &lt;code&gt;[A] -&amp;gt; [B] -&amp;gt; [C]&lt;/code&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B] &quot;Да&quot;&lt;/code&gt;&lt;/td&gt;&lt;td&gt;подпись стрелки&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;;&lt;/code&gt; или Enter&lt;/td&gt;&lt;td&gt;конец инструкции&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;// …&lt;/code&gt; или &lt;code&gt;# …&lt;/code&gt;&lt;/td&gt;&lt;td&gt;комментарий&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;\n&lt;/code&gt;&lt;/td&gt;&lt;td&gt;перенос строки в тексте&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Рамка=&lt;/code&gt;&lt;/td&gt;&lt;td&gt;на отдельной строке: открыть рамку; &lt;code&gt;==&lt;/code&gt; — закрыть&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Рамка= [A]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;блок из рамки «Рамка»; &lt;code&gt;== [A]&lt;/code&gt; — общий блок&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;&lt;p&gt;Одинаковый текст — один и тот же узел, но в каждой рамке свои блоки: «[Шаг]» в двух рамках — два разных блока. Пример — «Файл → Открыть пример с рамками».&lt;/p&gt;&lt;p&gt;Если узел хоть раз записан как &lt;code&gt;{…}&lt;/code&gt; или &lt;code&gt;(…)&lt;/code&gt;, он останется этой фигурой.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Syntaxe de Birch&lt;/h3&gt;&lt;table cellspacing=&apos;6&apos;&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[Texte]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;étape (rectangle)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;{Condition?}&lt;/code&gt;&lt;/td&gt;&lt;td&gt;décision (losange)&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;(Début)&lt;/code&gt;&lt;/td&gt;&lt;td&gt;début / fin&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;flèche&#xa0;; en chaîne aussi&#xa0;: &lt;code&gt;[A] -&amp;gt; [B] -&amp;gt; [C]&lt;/code&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;[A] -&amp;gt; [B] &quot;Oui&quot;&lt;/code&gt;&lt;/td&gt;&lt;td&gt;étiquette de flèche&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;;&lt;/code&gt; ou Entrée&lt;/td&gt;&lt;td&gt;fin d&apos;instruction&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;// …&lt;/code&gt; ou &lt;code&gt;# …&lt;/code&gt;&lt;/td&gt;&lt;td&gt;commentaire&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;\n&lt;/code&gt;&lt;/td&gt;&lt;td&gt;saut de ligne dans le texte&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Cadre=&lt;/code&gt;&lt;/td&gt;&lt;td&gt;sur une ligne à part&#xa0;: ouvrir un cadre&#xa0;; &lt;code&gt;==&lt;/code&gt; le ferme&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td&gt;&lt;code&gt;=Cadre= [A]&lt;/code&gt;&lt;/td&gt;&lt;td&gt;bloc du cadre «&#xa0;Cadre&#xa0;»&#xa0;; &lt;code&gt;== [A]&lt;/code&gt; est un bloc commun&lt;/td&gt;&lt;/tr&gt;&lt;/table&gt;&lt;p&gt;Un même texte désigne le même nœud, mais chaque cadre a ses propres blocs&#xa0;: «&#xa0;[Étape]&#xa0;» dans deux cadres, ce sont deux blocs différents. Voir Fichier → Ouvrir l&apos;exemple avec cadres.&lt;/p&gt;&lt;p&gt;Si un nœud est écrit au moins une fois comme &lt;code&gt;{…}&lt;/code&gt; ou &lt;code&gt;(…)&lt;/code&gt;, il garde cette forme.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Язык интерфейса изменится после перезапуска Birch.</source>
        <translation>La langue de l&apos;interface changera au prochain démarrage de Birch.</translation>
    </message>
    <message>
        <source>&lt;h3&gt;Birch %1&lt;/h3&gt;&lt;p&gt;Блок-схемы из текста — без ИИ, полностью офлайн.&lt;/p&gt;&lt;p&gt;Graphviz %2 · Qt %3&lt;/p&gt;&lt;p&gt;Распространяется по лицензии &lt;a href=&quot;https://mozilla.org/MPL/2.0/&quot;&gt;Mozilla Public License 2.0&lt;/a&gt;.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Birch %1&lt;/h3&gt;&lt;p&gt;Des organigrammes à partir de texte — sans IA, entièrement hors ligne.&lt;/p&gt;&lt;p&gt;Graphviz %2 · Qt %3&lt;/p&gt;&lt;p&gt;Distribué sous la &lt;a href=&quot;https://mozilla.org/MPL/2.0/&quot;&gt;Mozilla Public License 2.0&lt;/a&gt;.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Крупнее</source>
        <translation>Plus grande</translation>
    </message>
    <message>
        <source>Мельче</source>
        <translation>Plus petite</translation>
    </message>
    <message>
        <source>Обычный размер</source>
        <translation>Taille par défaut</translation>
    </message>
    <message>
        <source>Шрифт редактора</source>
        <translation>Police de l’éditeur</translation>
    </message>
    <message>
        <source>Шрифт редактора: %1 пт</source>
        <translation>Police de l’éditeur&#xa0;: %1&#xa0;pt</translation>
    </message>
</context>
</TS>
