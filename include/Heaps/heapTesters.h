//
// Created by Jlisowskyy on 12/1/23.
//

#ifndef HEAPTESTERS_H
#define HEAPTESTERS_H

#include <cstdlib>
#include <functional>
#include <climits>
#include <cfloat>
#include <string>

#include "../Debuggers.hpp"

template<template<typename PrioT, typename , typename , PrioT MostSignificantPrio> class HeapT>
void HeapTest() {
   auto stringToPairs = [](const std::string& str) {
        auto pairs = new std::pair<char, int>[str.size()];

        for(size_t i = 0; i < str.size(); ++i) {
            pairs[i] = std::make_pair(str[i], i + 1);
        }

        return std::make_tuple(pairs, str.size());
   };

    using charHeap = HeapT<char, int, std::greater<>, INT8_MAX>;
    std::cout << "-----------------------------------------------------------------------------\n"
              << "                           Character heap examples\n"
              << "-----------------------------------------------------------------------------\n";
    static constexpr const char* names[] = { "MINIPWEDU", "dermatoglyphics", "uncopyrightable" };

    for (const auto& name: names) {
        auto inits = stringToPairs(name);

        std::cout << "\nCharacter used to build heap: " << name << "\nUpToDown-built heap:\n\n";
        charHeap ch = std::move(charHeap::UpToDownFactory(std::get<0>(inits), std::get<1>(inits)));
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

    using intHeap = HeapT<int, int, std::greater<>, INT_MAX>;

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

    std::pair<int, int>* intArrs[] = { getSimpleNArray(31), getSimpleNArray(63),
                                        getSimpleNArray(127), getRandomElementsNArray(72),
                                        getRandomElementsNArray(49), getRandomElementsNArray(25) };
    const static int ranges[] = { 31, 63, 127, 72, 49, 25 };

    for(size_t i = 0; i < 6; ++i){
        std::cout << "UpToDown-built heap:\n\n";
        intHeap ih = intHeap::UpToDownFactory(intArrs[i], ranges[i]);
        std::cout << ih;

        std::cout << "\nDownToUp-built heap:\n\n";
        intHeap ih1(intArrs[i], ranges[i]);
        std::cout << ih1;
    }

    for (auto & intArr : intArrs) {
        delete[] intArr;
    }

    using doubleHeap = HeapT<double, double, std::greater<>, DBL_MAX>;
    std::cout << "-----------------------------------------------------------------------------\n"
              << "                 Floating-point times and complexity measures\n"
              << "-----------------------------------------------------------------------------\n";

    static constexpr size_t testRanges[] = { static_cast<size_t>(1e+5), static_cast<size_t>(1e+6), static_cast<size_t>(2e+6) };

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
        auto h2 = std::move(doubleHeap::UpToDownFactory(arr, range));
        T2.Stop();

        delete[] arr;
    }
}

template<template<typename, typename , typename> class MergingHeapT>
void MergingHeapTest() {
   auto stringToPairs = [](const std::string& str) {
        auto pairs = new std::pair<char, int>[str.size()];

        for(size_t i = 0; i < str.size(); ++i) {
            pairs[i] = std::make_pair(str[i], i + 1);
        }

        return std::make_tuple(pairs, str.size());
   };

    using charHeap = MergingHeapT<char, int, std::greater<>>;
    std::cout << "-----------------------------------------------------------------------------\n"
              << "                           Character heap examples\n"
              << "-----------------------------------------------------------------------------\n";
    static constexpr const char* names[] = { "MINIPWEDU", "dermatoglyphics", "uncopyrightable" };

    charHeap Merged{};
    for (const auto& name: names) {
        auto inits = stringToPairs(name);

        std::cout << "\nCharacter used to build heap: " << name << "\nAcquired heap:\n\n";
        charHeap ch1(std::get<0>(inits), std::get<1>(inits));
        std::cout << ch1;

        Merged.Merge(ch1);
        delete[] std::get<0>(inits);
    }

    std::cout << "Merged all previous structures: \n" << Merged;

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
        std::pair<char, int> out;
        mini.Search('D', out);
        std::cout << "D - found with value: " << out.second << std::endl;
        std::pair<char, int> pair1;
        std::pair<char, int> pair2;

        mini.Delete('I', pair1);
        mini.Delete('I', pair2);
        std::cout << "Deleted items: " << pair1.first << ' ' << pair2.first << "\nCurrent Tree:\n" << mini;
        mini.Insert(pair1).Insert(pair2);
        std::cout << "\nInserted again. Tree:\n" << mini;
        mini.Replace('I', std::make_pair('Z', 0));
        std::cout << "\nReplaced some I-priority item to Z:\n" << mini;

        delete[] std::get<0>(inits);
    }

    std::cout << "-----------------------------------------------------------------------------\n"
              << "                           Int heap examples\n"
              << "-----------------------------------------------------------------------------\n";

    using intHeap = MergingHeapT<int, int, std::greater<>>;

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

    std::pair<int, int>* intArrs[] = { getRandomElementsNArray(31), getRandomElementsNArray(63),
                                        getRandomElementsNArray(127), getRandomElementsNArray(72),
                                        getRandomElementsNArray(49), getRandomElementsNArray(25) };
    const static int ranges[] = { 31, 63, 127, 72, 49, 25 };

    for(size_t i = 0; i < 6; ++i){
        std::cout << "\nHeap created from " << ranges[i] << " integer elements:\n";
        intHeap ih1(intArrs[i], ranges[i]);
        std::cout << ih1;
    }

    for (auto & intArr : intArrs) {
        delete[] intArr;
    }

    using doubleHeap = MergingHeapT<double, double, std::greater<>>;
    std::cout << "-----------------------------------------------------------------------------\n"
              << "                 Floating-point times and complexity measures\n"
              << "-----------------------------------------------------------------------------\n";

    static constexpr size_t testRanges[] = { 1<<5, 1<<10, 1<<15, 1<<20, 1<<25 };

    srand(time(nullptr));
    for (auto range: testRanges) {
        std::cout << "-----------------------------";
        std::cout << "\nMeasuring heap building time with: " << range << " double precision floating point priorities\n";
        std::cout << "-----------------------------\n";

        auto* arr = new std::pair<double, double>[range];
        for (size_t i = 0; i < range; ++i) {
            arr[i].first = (((double)rand())/RAND_MAX) * 1e+7;
        }

        Timer T1("Double mergin heap creating", false);
        doubleHeap h1(arr, range);
        T1.Stop();

        delete[] arr;
    }
}

#endif //HEAPTESTERS_H