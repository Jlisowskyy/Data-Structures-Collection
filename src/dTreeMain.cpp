//
// Created by Jlisowskyy on 2/1/24.
//

#include "../include/DictionaryTrees/dTreeMain.h"

#include "../include/DictionaryTrees/Splay.h"
#include "../include/DictionaryTrees/binaryTRIALStructures.h"
#include "../include/DictionaryTrees/BSTTree.h"

#include "../include/structureTesters.h"

void SPLAYTreeMain() {
    BinaryTreeTester<SplayTreeT<size_t, size_t>>(true);
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
    const binaryPATRICIA patricia{
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
    BinaryTreeTester<BSTTreeT<size_t, size_t>>(true);
}

void AVLTreeMain() {

}

void RBTreeMain() {

}


