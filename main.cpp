// Author: Jakub Lisowski

#include "include/btree/btreeMain.h"
#include "include/hanoi/hanoiMain.h"
#include "include/Heaps/HeapsMain.h"
#include "include/DictionaryTrees/dTreeMain.h"
#include "include/Sorting/sortingMains.h"

static constexpr bool displayBtree = false;
static constexpr bool displayHanoi = false;
static constexpr bool displayHeaps = false;
static constexpr bool displayDictionaryTrees = true;
static constexpr bool displaySorting = false;

int main()
{
    if constexpr (displayBtree) BtreeMain();
    if constexpr (displayHanoi) HanoirMain();
    if constexpr (displayHeaps) HeapsMain();
    if constexpr (displayDictionaryTrees) dTreeMain();
    if constexpr (displaySorting) sortMain();

    return 0;
}