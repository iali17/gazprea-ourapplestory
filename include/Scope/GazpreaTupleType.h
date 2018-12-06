//
// Created by lepoidev on 11/4/18.
//

#ifndef GAZPREABASE_TUPLESYMBOL_H
#define GAZPREABASE_TUPLESYMBOL_H
#include "Symbol.h"
#include "GazpreaType.h"
#include <unordered_map>

class GazpreaTupleType : public GazpreaType {
public:
    GazpreaTupleType(const std::string &name, llvm::Type *typeDef, std::unordered_map<std::string, int> *stringRefMap,
                     std::vector<llvm::Type *> *members);
    int getOffsetFromString(std::string str);
    std::vector<llvm::Type *> *getMembers() const;
    std::unordered_map<std::string, int> *getStringRefMap() const;

protected:
    std::unordered_map<std::string, int> *stringRefMap;
    std::vector<llvm::Type *>            *members;

};
#endif //GAZPREABASE_TUPLESYMBOL_H
