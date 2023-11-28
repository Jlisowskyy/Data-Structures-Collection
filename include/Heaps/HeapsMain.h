//
// Created by Jlisowskyy on 11/8/23.
//

#ifndef MINI_PROJECTS_HEAPSMAIN_H
#define MINI_PROJECTS_HEAPSMAIN_H

#include <cstdlib>
#include <functional>
#include <string>

#include "Beap.h"
#include "Heap.h"

static const bool displayBeap = false;
static const bool displayHeap = true;

int HeapsMain()
{
    if constexpr (displayBeap) {

    }

    if constexpr (displayHeap) {
        auto stringToPairs = [](const std::string str) {
            auto pairs = new std::pair<char, void*>[str.size()];

            for(size_t i = 0; i < str.size(); ++i) {
                pairs[i] = std::make_pair(str[i], nullptr);
            }

            return std::make_tuple(pairs, str.size());
        };

        using charHeap = Heap<char, void*, std::greater<>, INT8_MAX>;

        auto inits = stringToPairs("MINIPWEDU");
;       charHeap ch1(std::get<0>(inits), std::get<1>(inits));
        std::cout << ch1;

        inits = stringToPairs("dermatoglyphics");
        charHeap ch2(std::get<0>(inits), std::get<1>(inits));
        std::cout << ch2;

        inits = stringToPairs("uncopyrightable");
        charHeap ch3(std::get<0>(inits), std::get<1>(inits));
        std::cout << ch3;
    }

    return EXIT_SUCCESS;
}

#endif //MINI_PROJECTS_HEAPSMAIN_H
