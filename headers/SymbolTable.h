#ifndef TRANSLATORTOYAML_SYMBOLTABLE_H
#define TRANSLATORTOYAML_SYMBOLTABLE_H

#include "objs.h"
#include <unordered_map>

class SymbolTable {
private:
    SymbolTable();
    static SymbolTable* instance;

    std::vector<Object*> values;
    std::unordered_map<std::string, size_t> table;

public:
    ~SymbolTable();
    static SymbolTable* GetTable();
    bool Contains(const std::string& key);
    size_t Add(const std::string& key, Object* obj);
    size_t Add(Object* obj);
    Object* GetObjByInd(size_t index);
    size_t GetInd(const std::string& key);
    void SetObjByInd(size_t index, Object* obj);
    void DelObjByInd(size_t index);

};



#endif //TRANSLATORTOYAML_SYMBOLTABLE_H
