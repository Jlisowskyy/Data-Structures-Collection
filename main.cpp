// Author: Jakub Lisowski
#include "include/btree/btreeMain.h"
#include "include/hanoi/hanoiMain.h"

const bool displayBtree = false;
const bool displayHanoi = true;

int main()
{
    if constexpr (displayBtree) BtreeMain();
    if constexpr (displayHanoi) HanoirMain();

    return 0;
}