#include "TranslatorToYAML.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: TranslatorToYAML <source> <destination>" << std::endl;
        return 1;
    }

    fs::path source = argv[1];
    fs::path destination = argv[2];

    if (source.is_relative())
        source = fs::absolute(source);

    if (destination.is_relative())
        destination = fs::absolute(destination);

    if (!fs::exists(source)) {
        std::cerr << "Source file does not exist" << std::endl;
        return 1;
    }

    try {
        TranslatorToYAML translator(source, destination);
        translator.TranslateToYaml();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
