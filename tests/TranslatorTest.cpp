#include <gtest/gtest.h>
#include "TranslatorToYAML.h"
#include <fstream>


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
    std::string expected = "";

    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    translator.TranslateToYaml();

    ASSERT_EQ(expected, GetString(destination));
}


TEST(TranslatorTest, ExprExpr) {
    std::string input = "var num := ?[* 2 / ?[* 3 7] ?[+ / 35 7 - 10 ?[* 2 + 1.8 2.2]]]\n"
                        "array(?[num])";
    std::string expected = "array:\n  - 6\n";

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
                        "var d := array(1, ?[c], ?[b])\n"
                        "?[d]"
                        "@{first = ?[a]; second = 456;}\n";
    std::string expected ="d:\n"
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
    std::string input = "! Comment\nvar a := 2\narray(?[a])";
    std::string expected = "array:\n  - 2\n";

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
    std::string input = "var e1 := ?[+ \"1\" \"2\"]\n"
                        "var e2 := ?[+ \"k.djfhg;klfjdhg3w45lisudyr@13123::(U(*@$: SELD\" \"1244\"]\n"
                        "var e3 := ?[\"\"]\n"
                        "var e4 := ?[+ \"12\" \"34\"]\n"
                        "var e5 := ?[sqrt 16]\n"
                        "var e6 := ?[max -1 -2]\n"
                        "\n"
                        "array(\n"
                        "    ?[e1],\n"
                        "    ?[e2],\n"
                        "    ?[e5],\n"
                        "    ?[e6]\n"
                        ")\n"
                        "\n"
                        "\n"
                        "var a := 4\n"
                        "var s := \"Hello, \"\n"
                        "var s2 := ?[\"World!\"]\n"
                        "\n"
                        "array(\n"
                        "    ?[+ s2 s],\n"
                        "    ?[56]\n"
                        ")\n"
                        "\n"
                        "var testarr := array(\n"
                        "    ?[\"Hello\"],\n"
                        "    ?[2345]\n"
                        ")\n"
                        "\n"
                        "@{\n"
                        "    a2 = 5;\n"
                        "    s3 = \"Hello, \";\n"
                        "    s4 = ?[+ \"World!\" \" OK\"];\n"
                        "    arr = array(\n"
                        "        ?[+ s s2],\n"
                        "        ?[54]\n"
                        "    );\n"
                        "    num = ?[* / 16 a + 2 3];\n"
                        "    str = ?[+ \"Och\" \"ko\"];\n"
                        "    test = ?[testarr];\n"
                        "    testd = @{ d1 = \"d1\"; d2 = 12321; d3 = ?[s]; };\n"
                        "}";

    std::string expected = "array:\n"
                           "  - \"12\"\n"
                           "  - \"k.djfhg;klfjdhg3w45lisudyr@13123::(U(*@$: SELD1244\"\n"
                           "  - 4\n"
                           "  - -1\n"
                           "\n"
                           "array:\n"
                           "  - \"World!Hello, \"\n"
                           "  - 56\n"
                           "\n"
                           "dict:\n"
                           "  a2: 5\n"
                           "  s3: \"Hello, \"\n"
                           "  s4: \"World! OK\"\n"
                           "  arr: [\"Hello, World!\", 54]\n"
                           "  num: 20\n"
                           "  str: \"Ochko\"\n"
                           "  testarr: [\"Hello\", 2345]\n"
                           "  testd: {d1: \"d1\", d2: 12321, d3: \"Hello, \"}\n";

    SetString(source, input);

    TranslatorToYAML translator(source, destination);
    translator.TranslateToYaml();

    ASSERT_EQ(expected, GetString(destination));
}
