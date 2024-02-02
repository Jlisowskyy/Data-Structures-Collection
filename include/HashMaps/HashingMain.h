//
// Created by Jlisowskyy on 1/31/24.
//

#ifndef HASHINGMAIN_H
#define HASHINGMAIN_H

#include <chrono>
#include <vector>
#include <iostream>

#include "chainHashingMap.h"

static constexpr bool displayPlainMap = false;
static constexpr bool displayExpandiblePlainMap = false;
static constexpr bool displayHashrateTests = true;

void PlainMapTest();
void ExpandiblePlainMapTest();
void HashRateTest();

inline int hashMain() {

    if constexpr (displayPlainMap) PlainMapTest();
    if constexpr (displayExpandiblePlainMap) ExpandiblePlainMapTest();

    if constexpr (displayHashrateTests) HashRateTest();

    return 0;
}

template<class hashmap, bool printBuckets = false>
void performHashTest(const size_t attemptCount, const std::vector<size_t>& elems) {
    double sum{};

    for (size_t i = 0; i < attemptCount; ++i) {
        hashmap map{};

        auto t1 = std::chrono::steady_clock::now();
        for (const auto num : elems) map.insert(std::make_pair(num, num));
        auto t2 = std::chrono::steady_clock::now();

        const double time = (t2.time_since_epoch() - t1.time_since_epoch()).count()*1e-6;
        sum += time;

        std::cout << std::format("Attempt number {}: {}ms\n", i + 1, time);

        if constexpr (printBuckets) {
            std::cout << std::format("Bucket ration in this run: {}\n\n", map.load_factor());
        }
    }

    std::cout << std::format("Average time after {} attempts: {}ms\n", attemptCount, sum/attemptCount);
    std::cout << std::format("Average hashrate: {} insertsPerMs\n", 5 * elems.size() / sum);
}

template<class hashmap, bool check = false, bool get = false>
void performAccessTest(const size_t attemptCount, const std::vector<size_t>& indexes, const std::vector<size_t>& elems) {
    double sum{};

    for (size_t i = 0; i < attemptCount; ++i) {
        hashmap map{};
        for (const auto elem : elems) map.insert(std::make_pair(elem, elem));

        auto t1 = std::chrono::steady_clock::now();

        if constexpr (get) {
            for (const auto ind : indexes) ++map.get(elems[ind]);
        }
        if constexpr (!get) {
            for (const auto ind : indexes) ++map[elems[ind]];
        }

        auto t2 = std::chrono::steady_clock::now();

        const double time = (t2.time_since_epoch() - t1.time_since_epoch()).count()*1e-6;
        sum += time;

        std::cout << std::format("Attempt number {}: {}ms\n", i + 1, time);

        if constexpr (check) {
            std::cout << std::format("Maximal load on bucket: {}\n", map.getMaximalBucketLoad());
        }
    }

    std::cout << std::format("Average time after {} attempt: {}ms\n", attemptCount, sum/attemptCount);
    std::cout << std::format("Average access: {} accessesPerMs\n", 5 * indexes.size() / sum);
}

#endif //HASHINGMAIN_H
