//
// Created by giacomo on 02/01/2022.
//

#include "yaucl/bpm/structures/declare/CNFDeclareDataAware.h"

CNFDeclareDataAware::CNFDeclareDataAware(const DeclareDataAware &singleton) : singleElementOfConjunction{singleton} {}

CNFDeclareDataAware::CNFDeclareDataAware(const std::vector<DeclareDataAware> &orig) : singleElementOfConjunction() {
    singleElementOfConjunction.reserve(orig.size());
    for (const auto& ref : orig)
        singleElementOfConjunction.emplace_back(ref);
}
