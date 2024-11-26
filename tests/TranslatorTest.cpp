#include <gtest/gtest.h>
#include "../headers/TranslatorToYAML.h"

std::string GetString(fs::path path) {
    if (!fs::exists(path)) {
        throw std::runtime_error("Source file does not exist");
    }

    std::ifstream input(path);
    if (!input.is_open()) {
        throw std::runtime_error("Could not open source file");
    }

    std::string res;
    std::string line;
    while (std::getline(input, line)) {
        res += line + "\n";
    }

    return res;
}

void SetString(fs::path path, const std::string& str) {
    std::ofstream output(path);
    if (!output.is_open()) {
        throw std::runtime_error("Could not open destination file");
    }

    output << str;
}

fs::path source = fs::current_path() / ".." / ".." / "tests" / "input.txt";
fs::path destination = fs::current_path() / ".." / ".." / "tests" / "output.yaml";

TEST(TranslatorTest, ArrayValues) {
    std::string input = "array(1, 2, 3.3)";
    std::string expected = "array:\n  - 1\n  - 2\n  - 3.300000\n";

    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    translator.TranslateToYaml();

    ASSERT_EQ(expected, GetString(destination));
}


TEST(TranslatorTest, ArrayStrings) {
    std::string input = "array(\"one\", \"two\", \"three\")";
    std::string expected = "array:\n  - \"one\"\n  - \"two\"\n  - \"three\"\n";

    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    translator.TranslateToYaml();

    ASSERT_EQ(expected, GetString(destination));
}


TEST(TranslatorTest, ArrayValueString) {
    std::string input = "array(1, \"two\", 3)";
    std::string expected = "array:\n  - 1\n  - \"two\"\n  - 3\n";

    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    translator.TranslateToYaml();

    ASSERT_EQ(expected, GetString(destination));
}


TEST(TranslatorTest, Expr) {
    std::string input = "?[+ 2 3]";
    std::string expected = "expr: 5\n";

    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    translator.TranslateToYaml();

    ASSERT_EQ(expected, GetString(destination));
}


TEST(TranslatorTest, ExprExpr) {
    std::string input = "?[* 2 / ?[* 3 7] ?[+ / 35 7 - 10 ?[* 2 + 1.8 2.2]]]";
    std::string expected = "expr: 6\n";

    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    translator.TranslateToYaml();

    ASSERT_EQ(expected, GetString(destination));
}

TEST(TranslatorTest, ArrayExpr) {
    std::string input = "array(?[+ 2 3], 2, ?[* 3 + 2 ?[* 2 2]])";
    std::string expected = "array:\n  - 5\n  - 2\n  - 18\n";

    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    translator.TranslateToYaml();

    ASSERT_EQ(expected, GetString(destination));

}

TEST(TranslatorTest, Dict) {
    std::string input = "@{first = 567; second = 456; third = \"123\"; fourth = ?[+ 2 3]; }";
    std::string expected = "dict:\n  first: 567\n  second: 456\n  third: \"123\"\n  fourth: 5\n";

    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    translator.TranslateToYaml();

    ASSERT_EQ(expected, GetString(destination));
}


TEST(TranslatorTest, Var) {
    std::string input = "var a := 2\n"
                        "var b := \"two\"\n"
                        "var c := ?[+ 7 3]\n"
                        "var d := array(1, c, b)\n"
                        "var dict := @{first = a; second = 456;}\n";
    std::string expected = "first: 2\n"
                           "\n"
                           "b: \"two\"\n"
                           "\n"
                           "c: 10\n"
                           "\n"
                           "d:\n"
                           "  - 1\n"
                           "  - 10\n"
                           "  - \"two\"\n"
                           "\n"
                           "dict:\n"
                           "  first: 2\n"
                           "  second: 456\n";

    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    translator.TranslateToYaml();

    ASSERT_EQ(expected, GetString(destination));
}

TEST(TranslatorTest, Exceptions) {
    std::string input = "array(1, 2, 3.3";
    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    ASSERT_THROW(translator.TranslateToYaml(), std::runtime_error);

    input = "array(1 2, 3.3)";
    SetString(source, input);
    ASSERT_THROW(translator.TranslateToYaml(), std::runtime_error);

    input = "[+ 2 4]";
    SetString(source, input);
    ASSERT_THROW(translator.TranslateToYaml(), std::runtime_error);

    input = "?[+ 2 4";
    SetString(source, input);
    ASSERT_THROW(translator.TranslateToYaml(), std::runtime_error);

    input = {"{\n a := 2}"};
    SetString(source, input);
    ASSERT_THROW(translator.TranslateToYaml(), std::runtime_error);
}

TEST(TranslatorTest, Empty) {
    std::string input = "";
    std::string expected = "";

    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    translator.TranslateToYaml();

    ASSERT_EQ(expected, GetString(destination));
}

TEST(TranslatorTest, Comment) {
    std::string input = "! Comment\nvar a := 2";
    std::string expected = "a: 2\n";

    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    translator.TranslateToYaml();

    ASSERT_EQ(expected, GetString(destination));
}

TEST(TranslatorTest, BlockComment) {
    std::string input = "{{!--\nvar aa := 2\n--}}";
    std::string expected = "";

    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    translator.TranslateToYaml();

    ASSERT_EQ(expected, GetString(destination));
}

TEST(TranslatorTest, CombineTest) {
    std::string input = "var a := 1\n"
                        "\n"
                        "var arr := array(\n"
                        "    1,\n"
                        "    ?[- 6 3],\n"
                        "    @{test = 1; test2 = 2; }\n"
                        ")\n"
                        "\n"
                        "! Это комментарий\n"
                        "\n"
                        "var dict := @{\n"
                        "    d = a;\n"
                        "    dd = arr;\n"
                        "}\n"
                        "\n"
                        "{{!--\n"
                        "var a := 2\n"
                        "--}}\n"
                        "\n"
                        "var b := ?[* + 5 ?[/ 125 25] ?[- -6 -9]]"
                        "@{"
                        "fgh = 765;}";

    std::string expected = "d: 1\n"
                           "\n"
                           "dd:\n"
                           "  - 1\n"
                           "  - 3\n"
                           "  - dict: {d: 1, test2: 2}\n"
                           "\n"
                           "dict:\n"
                           "  d: 1\n"
                           "  dd: [1, 3, dict: {d: 1, test2: 2}]\n"
                           "\n"
                           "b: 30\n"
                           "\n"
                           "dict:\n"
                           "  fgh: 765\n";

    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    translator.TranslateToYaml();

    ASSERT_EQ(expected, GetString(destination));
}
