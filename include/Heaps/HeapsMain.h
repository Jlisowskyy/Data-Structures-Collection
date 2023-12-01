//
// Created by Jlisowskyy on 11/8/23.
//

#ifndef MINI_PROJECTS_HEAPSMAIN_H
#define MINI_PROJECTS_HEAPSMAIN_H

#include "_baseBeapT.h"
#include "_baseHeapT.h"
#include "heapTesters.h"

static constexpr bool displayBeap = false;
static constexpr bool displayHeap = false;
static constexpr bool displayLeftistHeap = true;

inline int HeapsMain()
{
    if constexpr (displayLeftistHeap) {
        MergingHeapTest();
    }

    if constexpr (displayBeap) {
        HeapTest<_baseBeapT>();
    }

    if constexpr (displayHeap) {
        HeapTest<_baseHeapT>();
    }

    return EXIT_SUCCESS;
}

#endif //MINI_PROJECTS_HEAPSMAIN_H