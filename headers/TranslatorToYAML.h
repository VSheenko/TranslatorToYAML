#ifndef TRANSLATORTOYAML_TRANSLATORTOYAML_H
#define TRANSLATORTOYAML_TRANSLATORTOYAML_H

#include <filesystem>
#include "Parser.h"

namespace fs = std::filesystem;

class TranslatorToYAML {
private:
    fs::path source;
    fs::path destination;

public:
    TranslatorToYAML(const fs::path& source, const fs::path& destination);
    void TranslateToYaml();
};

#endif //TRANSLATORTOYAML_TRANSLATORTOYAML_H
