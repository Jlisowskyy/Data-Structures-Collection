// Author: Jakub Lisowski
#include "include/btree/btreeMain.h"
#include "include/hanoi/hanoiMain.h"
#include "include/Heaps/HeapsMain.h"

static const bool displayBtree = false;
static const bool displayHanoi = false;
static const bool displayHeaps = true;

int main()
{
    if constexpr (displayBtree) BtreeMain();
    if constexpr (displayHanoi) HanoirMain();
    if constexpr (displayHeaps) HeapsMain();

    return 0;
}