//
// Created by Jlisowskyy on 1/11/24.
//

#ifndef DTREEMAIN_H
#define DTREEMAIN_H

#include <iostream>

void SPLAYTreeMain();
void TRIETreeMain();
void RSTTreeMain();
void PATRICIAMain();
void BSTTreeMain();
void AVLTreeMain();
void RBTreeMain();

inline int dTreeMain() {
    std::cout << "Choose type of the structure to be tested:\n"
        << "1) Splay Tree\n"
        << "2) TRIE Tree\n"
        << "3) RST Tree\n"
        << "4) PATRICIA Tree\n"
        << "5) BST Tree\n"
        << "6) AVL Tree\n"
        << "7) Red-Black Tree\n";

    int choosenOption{};
    std::cin >> choosenOption;

    switch (choosenOption) {
        case 1:
            SPLAYTreeMain();
            break;
        case 2:
            TRIETreeMain();
            break;
        case 3:
            RSTTreeMain();
            break;
        case 4:
            PATRICIAMain();
            break;
        case 5:
            BSTTreeMain();
            break;
        case 6:
            AVLTreeMain();
            break;
        case 7:
            RBTreeMain();
            break;
        default:
            break;
    }

    return 0;
}

#endif //DTREEMAIN_H
