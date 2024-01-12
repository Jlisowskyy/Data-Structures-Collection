//
// Created by Jlisowskyy on 1/11/24.
//

#ifndef DTREEMAIN_H
#define DTREEMAIN_H

#include "Splay.h"

static constexpr bool displaySplayTree = true;

inline int dTreeMain() {
    if constexpr (displaySplayTree) {
        // TODO: Temporarty showcase

        using mSplay = SplayTree<int, int, std::greater<>>;

        mSplay tree{};
        for (int i = 0; i < 10; ++i)
            tree.Add(i+1, i);

        std::cout << "Simple 10-element sequence input\n";
        // tree.Print();

        std::cout << "After Search(1) invoc\n";
        tree.Contains(1);
        // tree.Print();

        std::cout << "After Search(2) invoc\n";
        tree.Contains(2);
        tree.Print();
    }

    return 0;
}

#endif //DTREEMAIN_H
