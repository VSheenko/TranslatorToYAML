#include "../headers/TranslatorToYAML.h"

TranslatorToYAML::TranslatorToYAML(const fs::path &source, const fs::path &destination) {
    if (!fs::exists(source)) {
        throw std::runtime_error("Source file does not exist");
    }

    if (!fs::exists(destination)) {
        fs::create_directories(destination);
    }

    this->source = source;
    this->destination = destination;
}

void TranslatorToYAML::TranslateToYaml() {
    std::ifstream input(source);
    if (!input.is_open()) {
        throw std::runtime_error("Could not open source file");
    }

    Parser parser(&input);
    parser.Parse();

    Container* root = parser.GetRoot();
    root->TranslateToYaml(std::cout, "");
}
