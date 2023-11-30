// Author: Jakub Lisowski
#include "include/btree/btreeMain.h"
#include "include/hanoi/hanoiMain.h"
#include "include/Heaps/HeapsMain.h"

static constexpr bool displayBtree = false;
static constexpr bool displayHanoi = false;
static constexpr bool displayHeaps = true;

int main()
{
    if constexpr (displayBtree) BtreeMain();
    if constexpr (displayHanoi) HanoirMain();
    if constexpr (displayHeaps) HeapsMain();

    return 0;
}