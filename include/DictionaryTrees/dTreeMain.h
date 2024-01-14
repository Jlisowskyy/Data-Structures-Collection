//
// Created by Jlisowskyy on 1/11/24.
//

#ifndef DTREEMAIN_H
#define DTREEMAIN_H

#include "Splay.h"
#include "binaryTRIALStructures.h"

static constexpr bool displaySplayTree = false;
static constexpr bool displayTrieTree = false;
static constexpr bool displayRstTree = false;
static constexpr bool displayPATRICIATree = true;

inline int dTreeMain() {
    if constexpr (displaySplayTree) {
        // TODO: Temporarty showcase

        using mSplay = SplayTreeT<int, int, std::greater<>>;

        mSplay tree{};
        for (int i = 0; i < 10; ++i)
            tree.Add(i+1, i);

        std::cout << "Simple 10-element sequence input\n";
        tree.Print();

        std::cout << "After Search(1) invoc\n";
        tree.Contains(1);
        tree.Print();

        std::cout << "After Search(2) invoc\n";
        tree.Contains(2);
        tree.Print();
    }

    if constexpr (displayTrieTree) {
        binaryTRIE trie{
            0b0100u,
            0b1100u,
            0b1111u,
            0b0101u,
            0b0001u,
            0b0111u,
            0b1001u,
            0b1010u,
            0b1111u,
            0b1110u
        };

        trie.Print();
        std::cout << "---------------------------------------------------\n";

        trie.Delete(0b1100);
        trie.Print();

        std::cout << "---------------------------------------------------\n";

        trie.Delete(0b0001);
        trie.Print();

    }

    if constexpr (displayRstTree) {
        binaryRST<3> rst{
            0b0100,
            0b1100,
            0b1111,
            0b0101,
            0b0001,
            0b0111,
            0b1001,
            0b1010
        };

        rst.Print();
        rst.Delete(0b0100);
        std::cout << "---------------------------------------------------\n";
        rst.Print();

        rst.Delete(0b0001);
        std::cout << "---------------------------------------------------\n";
        rst.Print();

        rst.Delete(0b0111);
        std::cout << "---------------------------------------------------\n";
        rst.Print();
    }

    if constexpr (displayPATRICIATree) {
        binaryPATRICIA patricia{
            0b1111,
            0b0111,
            0b1010,
            0b0001,
            0b0100,
            0b1001,
            0b1100,
            0b0101,
        };

        patricia.Print();
    }

    return 0;
}

#endif //DTREEMAIN_H
