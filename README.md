# Birch

Десктопное приложение, которое строит блок-схемы по текстовому описанию. Без ИИ и без интернета: текст разбирает собственный парсер, раскладку и стрелки делает [Graphviz](https://graphviz.org), интерфейс написан на Qt 6. Работает на macOS, Windows и Linux.

![Birch в светлой теме](docs/screenshot-light.png)

```text
(Старт) -> [Ввод_данных] -> {Данные корректны?};
{Данные корректны?} -> [Сохранить] "Да" -> (Конец);
{Данные корректны?} -> [Показать_ошибку] "Нет" -> [Ввод_данных];
```

## Возможности

- Редактор слева, схема справа. Схема перерисовывается автоматически через 300 мс после паузы в наборе; рендер идёт в фоновом потоке, поэтому набор не подвисает.
- Ошибки подчёркиваются прямо в тексте: строка, позиция и понятное сообщение. Недописанная строка не ломает остальную схему.
- Интерактивный холст: масштаб через ⌘/Ctrl + колесо или щипок на трекпаде, прокрутка перетаскиванием, двойной щелчок вписывает схему в окно. Схема выводится как вектор и остаётся чёткой при любом масштабе.
- **Скачать PNG** (⌘/Ctrl+E) и **Копировать в буфер** (⌘/Ctrl+Shift+C): картинку можно сразу вставить в Telegram, Slack, Word или Discord. Есть экспорт в SVG и прозрачный фон.
- Светлая и тёмная темы следуют системной; тему можно выбрать и вручную. Тему картинки можно задать отдельно, например всегда светлую для документов.
- Файлы `.birch`: открытие, сохранение, открытие двойным щелчком (macOS, Linux). Несохранённый текст восстанавливается при следующем запуске.

## Синтаксис

| Запись | Значение |
|---|---|
| `[Текст]` | шаг (скруглённый прямоугольник) |
| `{Условие?}` | решение (ромб) |
| `(Старт)` | начало или конец |
| `[A] -> [B]` | стрелка; можно цепочкой: `[A] -> [B] -> [C]` |
| `[A] -> [B] "Да"` | подпись стрелки, ведущей в `[B]` |
| `;` или перевод строки | конец инструкции; строка, начинающаяся с `->`, продолжает предыдущую |
| `// …` или `# …` | комментарий |
| `_` | пробел в тексте узла; `\_` — само подчёркивание |
| `\n` | перенос строки внутри текста |
| `\]`, `\"`, `\\` | символ как есть |

Одинаковый текст обозначает один и тот же узел. Если узел хоть раз записан как `{…}` или `(…)`, он остаётся этой фигурой, даже если дальше написан как `[…]`. Стрелка `→` и кавычки `«»` и `“”` тоже поддерживаются.

## Установка зависимостей

Нужны компилятор с C++20, CMake ≥ 3.21, Qt ≥ 6.4 (модули Base и Svg) и Graphviz ≥ 2.42 с заголовками для разработки.

### macOS (Homebrew)

```bash
xcode-select --install          # компилятор, если ещё не установлен
brew install cmake qt graphviz
```

### Windows (vcpkg)

1. Установите [Visual Studio 2022](https://visualstudio.microsoft.com/) с рабочей нагрузкой «Разработка классических приложений на C++». CMake входит в её состав.
2. Установите vcpkg и задайте переменную `VCPKG_ROOT`:

   ```powershell
   git clone https://github.com/microsoft/vcpkg C:\vcpkg
   C:\vcpkg\bootstrap-vcpkg.bat
   setx VCPKG_ROOT C:\vcpkg      # затем откройте новое окно терминала
   ```

3. Больше ничего ставить не нужно: при первой настройке проекта vcpkg сам соберёт `qtbase`, `qtsvg` и `graphviz` из `vcpkg.json`. Первая сборка Qt занимает 30–60 минут, потом результат берётся из кэша.

<details>
<summary>Быстрее: Qt из официального установщика</summary>

Установите Qt 6 (MSVC 2022 64-bit, с модулем Qt SVG) через [Qt Online Installer](https://www.qt.io/download-qt-installer) и уберите `qtbase` и `qtsvg` из `vcpkg.json`. Тогда vcpkg соберёт только Graphviz, а путь к Qt передаётся при настройке:

```powershell
cmake --preset windows-vcpkg -DCMAKE_PREFIX_PATH=C:\Qt\6.8.0\msvc2022_64
```
</details>

### Linux (apt)

Ubuntu 24.04+ и Debian 13+:

```bash
sudo apt install build-essential cmake ninja-build pkg-config \
                 qt6-base-dev qt6-svg-dev libgl-dev libgraphviz-dev
```

В Ubuntu 22.04 и Debian 12 Qt старше 6.4. Поставьте свежий Qt через [Qt Online Installer](https://www.qt.io/download-qt-installer) или `pip install aqtinstall` и передайте путь к нему: `-DCMAKE_PREFIX_PATH=~/Qt/6.8.0/gcc_64`.

Fedora: `sudo dnf install gcc-c++ cmake ninja-build qt6-qtbase-devel qt6-qtsvg-devel graphviz-devel`.

## Сборка и запуск

Для каждой платформы есть пресет CMake (`CMakePresets.json`):

```bash
# macOS
cmake --preset macos && cmake --build --preset macos
open build/macos/Birch.app

# Linux
cmake --preset linux && cmake --build --preset linux
./build/linux/birch

# Windows (Developer PowerShell для VS 2022)
cmake --preset windows-vcpkg
cmake --build --preset windows-vcpkg
.\build\windows-vcpkg\Release\Birch.exe
```

При настройке CMake печатает строку вида `Birch 0.1.0: Qt 6.9.3, Graphviz 16.1.0, builtin plugins: TRUE`. Значение `builtin plugins: TRUE` означает, что плагины Graphviz встроены в приложение и ему не нужен установленный Graphviz во время работы.

Тесты (парсер, рендерер, около 45 проверок) собираются по умолчанию; отключаются флагом `-DBIRCH_BUILD_TESTS=OFF`:

```bash
ctest --test-dir build/macos --output-on-failure
```

Файл можно открыть из командной строки: `Birch путь/к/схеме.birch`.

## Упаковка

| Платформа | Команда | Результат |
|---|---|---|
| macOS | `cd build/macos && cpack` | `Birch-<версия>-macos-<arch>.dmg` |
| Windows | `cd build\windows-vcpkg && cpack -C Release` | `.zip`, а при установленном [NSIS](https://nsis.sourceforge.io) (`winget install NSIS.NSIS`) ещё и установщик `.exe` |
| Linux | `cd build/linux && cpack` | `.tar.gz` и `.deb` (зависимости определяются автоматически) |
| Linux, переносимый | `./packaging/linux/build-appimage.sh` | `Birch-<версия>-x86_64.AppImage` |

Что происходит при упаковке:

- **macOS.** `macdeployqt` копирует Qt и все библиотеки, включая Graphviz, внутрь `Birch.app`. Затем `packaging/macos/prune-bundle.cmake` удаляет лишнее (QtQuick, QML, PDF-плагин), докопирует то, что `macdeployqt` пропустил, убирает пути машины сборки и **проверяет, что бандл самодостаточен**: если какой-то библиотеки не хватает, упаковка завершится ошибкой, а не выдаст сломанный `.app`. В конце бандл подписывается ad-hoc. Размер `.dmg` около 30 МБ.
- **Windows.** `windeployqt` копирует Qt; DLL Graphviz и его зависимостей находятся по импортам `Birch.exe`.
- **Linux.** Стандартная раскладка `/usr`: бинарник `birch`, `.desktop`-файл, иконка, MIME-тип для `.birch`. Для `.deb` и `.tar.gz` библиотеки берутся из системы; AppImage содержит всё внутри.

### Совместимость пакетов

- **macOS.** Пакет работает на тех версиях macOS, под которые собраны библиотеки. Graphviz из Homebrew на свежей системе собран под самую новую macOS, поэтому `.dmg` с такой машины на старых системах не запустится. Для широкой совместимости соберите Qt и Graphviz с нужной минимальной версией (например, через vcpkg с `VCPKG_OSX_DEPLOYMENT_TARGET=12.0`) и передайте `-DCMAKE_OSX_DEPLOYMENT_TARGET=12.0`.
- **macOS, распространение.** Ad-hoc подписи достаточно для своей машины. Другим пользователям Gatekeeper покажет предупреждение: для распространения подпишите приложение сертификатом Developer ID (замените `-` в `cmake/Packaging.cmake` на имя сертификата) и нотаризуйте через `xcrun notarytool`.
- **Linux AppImage** работает на дистрибутивах с glibc не старше, чем на сборочной машине. Собирайте на самой старой системе, которую хотите поддерживать.

## Структура проекта

```text
Birch/
├── CMakeLists.txt            сборка: цели birch_core и Birch, платформенные настройки
├── CMakePresets.json         пресеты macos / linux / windows-vcpkg
├── vcpkg.json                зависимости для vcpkg (Windows)
├── cmake/
│   ├── FindGraphviz.cmake    поиск Graphviz: pkg-config, затем вручную
│   └── Packaging.cmake       установка, деплой Qt, CPack
├── src/
│   ├── main.cpp
│   ├── core/                 логика без виджетов, покрыта тестами
│   │   ├── Diagram.h         модель: узлы, связи, фигуры
│   │   ├── Parser.*          DSL → Diagram, ошибки с позициями
│   │   ├── Renderer.*        Diagram → Graphviz (cgraph + dot) → SVG → QImage/PNG
│   │   └── Theme.*           светлая и тёмная палитры схемы
│   ├── app/
│   │   └── RenderController.*  фоновый рендер, «побеждает последний запрос»
│   └── ui/
│       ├── MainWindow.*      окно, меню, экспорт, файлы, темы
│       ├── CodeEditor.*      редактор: номера строк, подчёркивание ошибок
│       ├── SyntaxHighlighter.*
│       └── PreviewView.*     интерактивный холст (вектор, зум, прокрутка)
├── resources/                иконки интерфейса, пример схемы
├── packaging/                Info.plist, .rc, .desktop, MIME, AppImage, иконки
├── tests/                    тесты парсера и рендерера (Qt Test)
└── docs/                     скриншоты
```

### Как устроен рендеринг

1. `Parser` превращает текст в `Diagram` и список ошибок. Инструкция применяется целиком или не применяется вовсе, поэтому при наборе схема не «мигает».
2. `Renderer` строит граф через C++ API Graphviz (`cgraph`), задаёт стили атрибутами (скругления, пастельные цвета, шрифт, стрелки), раскладывает граф движком `dot` и получает SVG через `gvRenderData`.
3. Ширину текста для Graphviz измеряет Qt, тем же шрифтом, которым потом рисуется SVG: в Graphviz зарегистрирован собственный плагин `textlayout`. Без него Graphviz оценивает ширину по встроенной таблице, и кириллица вылезает за рамки узлов.
4. Превью показывает SVG как вектор (`QGraphicsSvgItem`), а экспорт растеризует его в PNG с масштабом 2× для экранов Retina и HiDPI.

## Решение проблем

| Симптом | Что делать |
|---|---|
| `Could NOT find Graphviz` | Установите пакет для разработки (`libgraphviz-dev`, `graphviz-devel`) или укажите путь: `-DGRAPHVIZ_ROOT=/путь/к/graphviz`. |
| `Could NOT find Qt6Svg` / `SvgWidgets` | Установите модуль SVG: `qt6-svg-dev`, `qt6-qtsvg-devel`, в установщике Qt — «Qt SVG». |
| Ошибка «Layout type: "dot" not recognized» или пустая схема | Плагины Graphviz не встроены (`builtin plugins: FALSE`) и не найдены при запуске. Выполните `dot -c` (на Windows — в папке с `dot.exe`), чтобы Graphviz обновил список плагинов. |
| После смены параметров CMake ничего не меняется | Значения опций кэшируются: передайте их явно (`-DBIRCH_BUILD_TESTS=ON`) или удалите папку `build/<пресет>`. |
| macOS: «Birch повреждено» или «не удаётся проверить разработчика» | Пакет подписан ad-hoc. Откройте через контекстное меню → «Открыть» или выполните `xattr -dr com.apple.quarantine /Applications/Birch.app`. Для распространения нужна подпись Developer ID. |

Иконки приложения (`packaging/macos/app.icns`, `packaging/windows/app.ico`) генерируются из `resources/icons/app.svg` скриптом `packaging/generate-icons.sh`.
