//
// Created by Jlisowskyy on 11/8/23.
//

#ifndef MINI_PROJECTS_HEAPSMAIN_H
#define MINI_PROJECTS_HEAPSMAIN_H

#include <functional>

#include "_baseBeapT.h"
#include "_baseHeapT.h"
#include "_baseLeftistHeapT.h"
#include "_baseBinomialQueueT.h"
#include "heapTesters.h"

static constexpr bool displayBeap = false;
static constexpr bool displayHeap = false;
static constexpr bool displayLeftistHeap = false;
static constexpr bool displayBinomialQueue = true;

inline int HeapsMain()
{
    if constexpr (displayBinomialQueue) {
        // TODO: TEMPORARY

        using Binomial = _baseBinominialQueue<size_t, size_t, std::greater<>>;
        Binomial tester{};

        static constexpr size_t range = 13 + 1;
        for(size_t i = 1; i < range; ++i) {
            tester.Insert(i, i);
        }

        std::cout << tester;
    }

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