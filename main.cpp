// Author: Jakub Lisowski
#include "include/btree/btreeMain.h"
#include "include/hanoi/hanoiMain.h"
#include "include/Heaps/HeapsMain.h"
#include "include/DictionaryTrees/dTreeMain.h"

static constexpr bool displayBtree = false;
static constexpr bool displayHanoi = false;
static constexpr bool displayHeaps = false;
static constexpr bool displayDictionaryTrees = true;

int main()
{
    if constexpr (displayBtree) BtreeMain();
    if constexpr (displayHanoi) HanoirMain();
    if constexpr (displayHeaps) HeapsMain();
    if constexpr (displayDictionaryTrees) dTreeMain();

    return 0;
}