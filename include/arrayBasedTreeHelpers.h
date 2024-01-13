//
// Created by Jlisowskyy on 1/12/24.
//

#ifndef ARRAYBASEDTREEHELPERS_H
#define ARRAYBASEDTREEHELPERS_H

#include <iostream>

template<class nodeT, size_t arrSize>
std::ostream& SimplestArrBasedRecursivePrint(const nodeT* const n, const size_t depth, std::ostream& out = std::cout)
{
    if (!n) return out;

    if constexpr (arrSize == 2) {
        SimplestArrBasedRecursivePrint<nodeT, arrSize>(n->next[0], depth+4, out);
        std::cout << std::string(depth, ' ') << *n << std::endl;
        SimplestArrBasedRecursivePrint<nodeT, arrSize>(n->next[1], depth+4, out);
    }
    else {
        std::cout << std::string(depth, ' ') << *n << std::endl;
        for(size_t i = 0; i < arrSize; ++i) {
            SimplestArrBasedRecursivePrint<nodeT, arrSize>(n->next[i], depth+4, out);
        }
    }

    return out;
}

template<class nodeT, size_t arrSize>
void CleanArrBasedTree(const nodeT* n) {
    if (!n) return;

    for (size_t i = 0; i < arrSize; ++i)
        CleanArrBasedTree<nodeT, arrSize>(n->next[i]);

    delete n;
}

template<class nodeT, size_t arrSize>
nodeT* CloneArrBasedTree(const nodeT* const n) {
    if (!n) return nullptr;

    nodeT* nRet = new nodeT(*n);
    for (size_t i = 0; i < arrSize; ++i) {
        nRet->next[i] = CloneArrBasedTree(n->next[i]);
    }

    return nRet;
}

#endif //ARRAYBASEDTREEHELPERS_H
