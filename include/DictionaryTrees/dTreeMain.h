//
// Created by Jlisowskyy on 1/11/24.
//

#ifndef DTREEMAIN_H
#define DTREEMAIN_H

static constexpr bool displaySplayTree = false;
static constexpr bool displayTrieTree = false;
static constexpr bool displayRstTree = false;
static constexpr bool displayPATRICIATree = false;
static constexpr bool displayBSTTree = true;

void SPLAYTreeMain();
void TRIETreeMain();
void RSTTreeMain();
void PATRICIAMain();
void BSTTreeMain();

inline int dTreeMain() {
    if constexpr (displaySplayTree) SPLAYTreeMain();
    if constexpr (displayTrieTree) TRIETreeMain();
    if constexpr (displayRstTree) RSTTreeMain();
    if constexpr (displayPATRICIATree) PATRICIAMain();
    if constexpr (displayBSTTree) BSTTreeMain();

    return 0;
}

#endif //DTREEMAIN_H
