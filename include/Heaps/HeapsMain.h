//
// Created by Jlisowskyy on 11/8/23.
//

#ifndef MINI_PROJECTS_HEAPSMAIN_H
#define MINI_PROJECTS_HEAPSMAIN_H

#include <cstdlib>
#include <functional>
#include <climits>
#include <cfloat>
#include <string>

#include "_baseBeapT.h"
#include "_baseHeapT.h"
#include "../Debuggers.hpp"

static constexpr bool displayBeap = false;
static constexpr bool displayHeap = true;

inline int HeapsMain()
{
    if constexpr (displayBeap) {
        using charBeap = _baseBeapT<char, char, std::greater<>, INT8_MAX>;

        charBeap ch;
        ch.Test();
    }

    if constexpr (displayHeap) {
        auto stringToPairs = [](const std::string& str) {
            auto pairs = new std::pair<char, int>[str.size()];

            for(size_t i = 0; i < str.size(); ++i) {
                pairs[i] = std::make_pair(str[i], i + 1);
            }

            return std::make_tuple(pairs, str.size());
        };

        using charHeap = _baseHeapT<char, int, std::greater<>, INT8_MAX>;
        std::cout << "-----------------------------------------------------------------------------\n"
                  << "                           Character heap examples\n"
                  << "-----------------------------------------------------------------------------\n";
        static constexpr const char* names[] = { "MINIPWEDU", "dermatoglyphics", "uncopyrightable" };

        for (const auto& name: names) {
            auto inits = stringToPairs(name);

            std::cout << "\nCharacter used to build heap: " << name << "\nUpToDown-built heap:\n\n";
            charHeap ch = std::move(charHeap::HeapUpToDownFactory(std::get<0>(inits), std::get<1>(inits)));
            std::cout << ch;

            std::cout << "\nDownToUp-built heap:\n\n";
            charHeap ch1(std::get<0>(inits), std::get<1>(inits));
            std::cout << ch1;

            delete[] std::get<0>(inits);
        }

        std::cout << "-----------------------------------------------------------------------------\n"
                  << "                  Character heap - based operation examples\n"
                  << "-----------------------------------------------------------------------------\n";

        {
            const auto inits = stringToPairs("MINIPWEDU");
            charHeap mini(std::get<0>(inits), std::get<1>(inits));
            std::cout << "Our tree:\n" << mini;

            std::cout << "Max elements: " << mini.Max().first << std::endl;
            mini.DeleteMax();
            std::cout << "W - removed: \n" << mini;
            std::cout << "W - added: \n" << mini.Insert(std::make_pair('W', 6));
            const auto result  = mini.Search('D');
            std::cout << "D - found with value: " << mini[result].second << std::endl;
            std::pair<char, int> pair1;
            std::pair<char, int> pair2;

            mini.Delete(HeapIndex(0), pair1); mini.Delete(HeapIndex(5), pair2);
            std::cout << "Deleted items: " << pair1.first << ' ' << pair2.first << "\nCurrent Tree:\n" << mini;
            mini.Insert(pair1).Insert(pair2);
            std::cout << "\nInserted again. Tree:\n" << mini;
            mini.Replace(HeapIndex(mini.GetLastIndex()), std::make_pair('Z', 0));
            std::cout << "\nReplaced last item to Z:\n" << mini;


            delete[] std::get<0>(inits);
        }


        std::cout << "-----------------------------------------------------------------------------\n"
                  << "                           Int heap examples\n"
                  << "-----------------------------------------------------------------------------\n";

        using intHeap = _baseHeapT<int, int, std::greater<>, INT_MAX>;

        auto getRandomElementsNArray = [](const size_t arrSize) {
            const auto arr = new std::pair<int, int>[arrSize];

            for(size_t i = 0; i < arrSize; ++i) {
                arr[i].first = rand();
            }

            return arr;
        };

        auto getSimpleNArray = [](const size_t arrSize) {
            const auto arr = new std::pair<int, int>[arrSize];

            for(size_t i = 0; i < arrSize; ++i) {
                arr[i].first = static_cast<int>(i);
            }

            return arr;
        };

        std::pair<int, int>* intArrs[] = { getSimpleNArray(32), getSimpleNArray(64),
                                            getSimpleNArray(128), getRandomElementsNArray(72),
                                            getRandomElementsNArray(49), getRandomElementsNArray(25) };
        const static int ranges[] = { 32, 64, 128, 72, 49, 25 };

        for(size_t i = 0; i < 6; ++i){
            std::cout << "UpToDown-built heap:\n\n";
            intHeap ih = intHeap::HeapUpToDownFactory(intArrs[i], ranges[i]);
            std::cout << ih;

            std::cout << "\nDownToUp-built heap:\n\n";
            intHeap ih1(intArrs[i], ranges[i]);
            std::cout << ih1;
        }

        for (auto & intArr : intArrs) {
            delete[] intArr;
        }

        using doubleHeap = _baseHeapT<double, double, std::greater<>, DBL_MAX>;
        std::cout << "-----------------------------------------------------------------------------\n"
                  << "                 Floating-point times and complexity measures\n"
                  << "-----------------------------------------------------------------------------\n";

        static constexpr size_t testRanges[] = { static_cast<size_t>(1e+6), static_cast<size_t>(1e+7), static_cast<size_t>(1e+8) };

        for (auto range: testRanges) {
            std::cout << "-----------------------------";
            std::cout << "\nMeasuring heap building time with: " << range << " double precision floating point priorities\n";
            std::cout << "-----------------------------\n";

            auto* arr = new std::pair<double, double>[range];
            for (size_t i = 0; i < range; ++i) {
                arr[i].first = static_cast<double>(i);
            }

            Timer T1("Down to up heap building", false);
            doubleHeap h1(arr, range);
            T1.Stop();

            Timer T2("Up to down heap building", false);
            auto h2 = std::move(doubleHeap::HeapUpToDownFactory(arr, range));
            T2.Stop();

            delete[] arr;
        }

    }

    return EXIT_SUCCESS;
}

#endif //MINI_PROJECTS_HEAPSMAIN_H
