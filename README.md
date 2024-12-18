# Задание
Разработать инструмент командной строки для учебного конфигурационного языка, синтаксис которого приведен далее. Этот инструмент преобразует текст из входного формата в выходной. Синтаксические ошибки выявляются с выдачей сообщений.

Входной текст на учебном конфигурационном языке принимается из файла, путь к которому задан ключом командной строки. Выходной текст на языке yaml попадает в файл, путь к которому задан ключом командной строки.

## Синтаксис конфигурационного языка

### Коментарии
```
 ! Однострочный комментарий
```

```
{{!-- Многострочный
комментарий --}}
```

### Имена и значения

```
имя: [a-z][a-z0-9_]*
значения:
    - строка
    - число
    - массив
    - словарь
```

### Массив
```
array( значение, значение, значение, ... )
```

### Словарь
```
@{ имя = значение;
   имя = значение;
   ...
   имя = значение;
}
```

### Объявление констант
```
var имя := значение
```

### Макрос префиксного вычисления на этапе трансляции
```
?[+ имя 1]
```

### Для константных вычислений определены операции и функции
```
1. Сложение
2. max()
3. sqrt()
```

# Структура проекта
```
TranslatorToYaml/
├── CMakeLists.txt         # Файл для сборки проекта
├── main.cpp               # Точка входа
├── headers/  
    ├── objs.h/            # Классы, представляющие типы данных
        ├── Array.h 
        ├── Container.h
        ├── Dict.h
        ├── Expr.h
        ├── ExprObj.h
        ├── ExprStr.h
        ├── ExprValue.h
        ├── Object.h
        ├── Str.h
        ├── objs.h
        └── Value.h
    ├── Lexer.h
    ├── Parser.h
    ├── SymbolTable.h      # Класс таблицы символов
    ├── Token.h            # Объявление токенов
    └── TralatorToYaml.h
├── sources/
    ├── objs
        ├── Array.cpp
        ├── Container.cpp
        ├── Dict.cpp
        ├── Expr.cpp
        ├── ExprObj.cpp
        ├── ExprStr.cpp
        ├── ExprValue.cpp
        ├── Object.cpp
        ├── Str.cpp
        └── Value.cpp
    ├── Lexer.cpp
    ├── Parser.cpp
    ├── SymbolTable.cpp
    └── TralatorToYaml.cpp
└── tests/                 # Тесты
    ├── CMakeLists.txt
    ├── TranslatorToYamlTest.cpp
    ├── input.txt
    └── output.yaml
```

## Структура иерархии классов для "типов данных"

![s5](https://github.com/user-attachments/assets/fd9358dd-2fc1-4c89-9d7a-d4ffdcd7f0a6)

# Сборка
Сборка осущесвтляется как для обычного проекта CMake
## Требования
- OS: Windows x64
- Компилятор: GCC
- CMake 3.28+


# Пример использования

```bash
TranslatorToYaml <input> <output>
<input> - путь к файлу с входным текстом
<output> - путь к файлу для выходного текста
```
---
Входной текст:
```txt
var e1 := ?[+ "1" "2"]
var e2 := ?[+ "k.djfhg;klfjdhg3w45lisudyr@13123::(U(*@$: SELD" "1244"]
var e3 := ?[""]
var e4 := ?[+ "12" "34"]
var e5 := ?[sqrt 16]
var e6 := ?[max -1 -2]

array(
    ?[e1],
    ?[e2],
    ?[e5],
    ?[e6]
)


var a := 4
var s := "Hello, "
var s2 := ?["World!"]

array(
    ?[+ s2 s],
    ?[56]
)

var testarr := array(
    ?["Hello"],
    ?[2345]
)

@{
    a2 = 5;
    s3 = "Hello, ";
    s4 = ?[+ "World!" " OK"];
    arr = array(
        ?[+ s s2],
        ?[54]
    );
    num = ?[* / 16 a + 2 3];
    str = ?[+ "Och" "ko"];
    test = ?[testarr];
    testd = @{ d1 = "d1"; d2 = 12321; d3 = ?[s]; };
}

```
Вызов программы:

![doc/img/s1.png](https://github.com/VSheenko/TranslatorToYAML/blob/master/doc/img/s1.png)

Результат выполнения:

![s4](https://github.com/user-attachments/assets/78305894-53d6-4d49-a524-f1314be2994b)

# Тесты
![doc/img/s3.png](https://github.com/VSheenko/TranslatorToYAML/blob/master/doc/img/s3.png)
