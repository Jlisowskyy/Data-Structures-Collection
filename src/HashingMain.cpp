//
// Created by Jlisowskyy on 1/31/24.
//

#include "../include/HashMaps/HashingMain.h"
#include "../include/HashMaps/plainHashMap.h"

#include <iostream>
#include <random>
#include <chrono>
#include <unordered_map>
#include <map>

void PlainMapTest() {
    static constexpr size_t keys[] = {
        1, 2 ,3 , 4, 5, 6, 7, 8, 9, 10
    };

    static constexpr size_t sizes[] = {
        50, 100, 150, 200
    };

    for (size_t size: sizes) {
        _basePlainMapT<size_t, size_t> map(size);
        std::cout << std::format("\n\nStarting new map test with size: {}...\n", size);

        for (size_t key: keys) {
            bool result = map.insert(key, key);

            std::cout << std::format("Adding new key: {} to map...\nStatus: {}\n", key, result);
        }
    }

}

void ExpandiblePlainMapTest() {
    static constexpr size_t keys[] = {
        1, 2 ,3 , 4, 5, 6, 7, 8, 9, 10
    };
    static constexpr size_t sizeStep = 10;

    bool collisionDetected;
    size_t size = 20;

    do{
        collisionDetected = false;

        _baseExpandiblePlainMapT<size_t, size_t> map(size);
        std::cout << std::format("\n\nStarting new map test with size: {}...\n", size);

        for (size_t key: keys) {
            bool result = map.insert(key, key);
            if (!result) collisionDetected = true;

            std::cout << std::format("Adding new key: {} to map...\nStatus: {}\n", key, result);
        }

        size += sizeStep;
    }while(collisionDetected);

}

void HashRateTest() {
    static constexpr auto elementCount = static_cast<size_t>(1e+6);
    static constexpr auto accessCount = static_cast<size_t>(1e+7);
    static constexpr size_t elementStep = 5;
    static constexpr size_t initElem  = 1;
    static constexpr size_t tryPerMap = 5;

    std::vector<size_t> elems{};
    std::vector<size_t> accessIndexes{};
    std::default_random_engine eng(std::chrono::steady_clock::now().time_since_epoch().count());

    size_t elem = initElem;
    for (size_t i = 0; i < elementCount; ++i) {
        elems.push_back(elem);
        elem += 1 + eng() % elementStep;
    }

    for (size_t i = 0;i < accessCount; ++i) {
        accessIndexes.push_back(eng()%elementCount);
    }

    // ------------------------------
    // Initializing test
    // ------------------------------

    std::cout << std::format("Starting hashrate test with {} distinct elements:\nFor each map {} tries will be conducted...\n", elementCount, tryPerMap);

    std::cout << "-------------------------------------------------------\n";
    std::cout << "Unordered map test:\n";
    performHashTest<std::unordered_map<size_t, size_t>>(tryPerMap, elems);

    std::cout << "-------------------------------------------------------\n";
    std::cout << "map test:\n";
    performHashTest<std::map<size_t, size_t>>(tryPerMap, elems);

    std::cout << "-------------------------------------------------------\n";
    std::cout << "ChainMap with list buckets test:\n";
    performHashTest<_chainHashingMapT<size_t, size_t, std::equal_to<size_t>, BaseHashFunction<size_t, true>, LinkedListBucketT<size_t, size_t, std::equal_to<size_t>>>>(tryPerMap, elems);

    std::cout << "-------------------------------------------------------\n";
    std::cout << "ChainMap with hash buckets test:\n";
    performHashTest<_chainHashingMapT<size_t, size_t>>(tryPerMap, elems);
    std::cout << "-------------------------------------------------------\n";

    // ------------------------------
    // Access time test
    // ------------------------------

    std::cout << std::format("Starting access time test with {} distinct elements:\nFor each map {} access attemps will be conducted...\n", elementCount, accessCount);

    std::cout << "-------------------------------------------------------\n";
    std::cout << "Unordered map test:\n";
    performAccessTest<std::unordered_map<size_t, size_t>>(tryPerMap, accessIndexes, elems);

    std::cout << "-------------------------------------------------------\n";
    std::cout << "map test:\n";
    performAccessTest<std::map<size_t, size_t>>(tryPerMap, accessIndexes, elems);

    std::cout << "-------------------------------------------------------\n";
    std::cout << "ChainMap with list buckets test:\n";
    performAccessTest<_chainHashingMapT<size_t, size_t, std::equal_to<size_t>, BaseHashFunction<size_t, true>,
        LinkedListBucketT<size_t, size_t, std::equal_to<size_t>>>, true>(tryPerMap, accessIndexes, elems);

    std::cout << "-------------------------------------------------------\n";
    std::cout << "ChainMap with hash buckets test:\n";
    performAccessTest<_chainHashingMapT<size_t, size_t>, true>(tryPerMap, accessIndexes, elems);
    std::cout << "-------------------------------------------------------\n";
}
