//
// Created by Jlisowskyy on 2/1/24.
//

#include "../include/DictionaryTrees/dTreeMain.h"

#include "../include/DictionaryTrees/Splay.h"
#include "../include/DictionaryTrees/binaryTRIALStructures.h"
#include "../include/DictionaryTrees/BSTTree.h"

#include <chrono>
#include <random>

void SPLAYTreeMain() {
    // // TODO: Temporarty showcase
    //
    // using mSplay = SplayTreeT<int, int, std::greater<>>;
    //
    // mSplay tree{};
    // for (int i = 0; i < 10; ++i)
    //     tree.Add(i+1, i);
    //
    // std::cout << "Simple 10-element sequence input\n";
    // tree.Print();
    //
    // std::cout << "After Search(1) invoc\n";
    // tree.Contains(1);
    // tree.Print();
    //
    // std::cout << "After Search(2) invoc\n";
    // tree.Contains(2);
    // tree.Print();

    using mSplay = SplayTreeT<char, char, std::greater<>>;
    mSplay tree{};

    for(auto elem : {'J', 'A', 'K', 'U'}) {
        tree.Add(elem, elem);
        std::cout << "-------------------\n";
        tree.Print();
    }

    std::cout << "-------------------\n";
    tree.Delete('A');
    tree.Print();
    // tree.Contains('U');
    // tree.Print();
}

void TRIETreeMain() {
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

void RSTTreeMain() {
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

void PATRICIAMain() {
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

void BSTTreeMain() {
    static constexpr size_t elemCount = 10;
    static constexpr size_t maxStep = 5;
    static constexpr size_t initSeq = 1;
    static constexpr size_t tries = 3;
    static constexpr size_t removeActions = 4;

    std::default_random_engine eng(std::chrono::steady_clock::now().time_since_epoch().count());

    for (size_t t = 0; t < tries; ++t) {
        std::cout << "--------------------------------------------------------------------------------------------\n";
        std::vector<size_t> elems{};
        BSTTreeT<size_t, size_t> map{};

        size_t elem = initSeq;
        for (size_t i = 0; i < elemCount; ++i) {
            elems.push_back(elem);
            elem += 1 + eng() % maxStep;
        }

        std::shuffle(elems.begin(), elems.end(), eng);

        std::cout << "Inputs sequence:\n";
        for(auto e : elems) {
            std::cout << std::format("{} ", e);
            map.insert(e,e);
        }
        std::cout << "Tree:\n" << map;

        for (size_t i = 0; i < removeActions; ++i) {
            map.remove(elems[i]);
            std::cout << std::format("\nTree after removing {}:\n", elems[i]) << map;
        }
    }
}
