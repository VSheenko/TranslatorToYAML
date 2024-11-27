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
    ├── Expr.cpp
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
var a := 1

var arr := array(
    1,
    ?[- 6 3],
    @{test = 1; test2 = 2; }
)

! Это комментарий

var dict := @{
    d = a;
    dd = arr;
}

{{!--
var a := 2
--}}

var b := ?[* + 5 ?[/ 125 25] ?[- -6 -9]]

@{fgh = 765;}

var bigarr := array(
    23 + 7,
    23 + 17,
    "Hello" + ", " + "World!",
    ?[+ 5 3] + 2,
    a + 9,
    arr,
    b + 3,
    sqrt(100),
    max(12, 321, 54, -12.34, 87, -32),
    sqrt(12) + 3
)

var bigdict := @{
    d1 = 23 + 7;
    d2 = a + 19;
    d3 = dict;
    d4 = "Hello" + ", " + "World!";
    d5 = ?[+ 5 3] + 2;
    d6 = arr;
}
```
Вызов программы:

![doc/img/s1.png](https://github.com/VSheenko/TranslatorToYAML/blob/master/doc/img/s1.png)

Результат выполнения:

![doc/img/s2.png](https://github.com/VSheenko/TranslatorToYAML/blob/master/doc/img/s2.png)

# Тесты
![doc/img/s3.png](https://github.com/VSheenko/TranslatorToYAML/blob/master/doc/img/s3.png)
