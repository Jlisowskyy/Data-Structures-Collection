//
// Created by Jlisowskyy on 11/8/23.
//

#ifndef MINI_PROJECTS_HEAPSMAIN_H
#define MINI_PROJECTS_HEAPSMAIN_H

#include <functional>

#include "_baseBeapT.h"
#include "_baseHeapT.h"
#include "_baseLeftistHeapT.h"
#include "heapTesters.h"

static constexpr bool displayBeap = true;
static constexpr bool displayHeap = false;
static constexpr bool displayLeftistHeap = false;

inline int HeapsMain()
{
    if constexpr (displayLeftistHeap) {
        MergingHeapTest<_baseLeftistHeapT>();
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