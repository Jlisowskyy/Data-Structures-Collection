//
// Created by Jlisowskyy on 1/31/24.
//

#include <iostream>
#include <random>
#include <chrono>
#include <unordered_map>
#include <map>

#include "../include/HashMaps/HashingMain.h"
#include "../include/HashMaps/plainHashMap.h"
#include "../include/structureTesters.h"
#include "../include/HashMaps/chainHashingMap.h"

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

using boostedMap = _chainHashingMapT<size_t, size_t, std::equal_to<>, Fast2PowHashFunction<size_t>,
        PlainHashBucketT<size_t, size_t, std::equal_to<>, Fast2PowHashFunction<size_t>>>;

using simpleMap = _chainHashingMapT<size_t, size_t>;

using fastListMap = _chainHashingMapT<size_t, size_t, std::equal_to<>, Fast2PowHashFunction<size_t>,
        LinkedListBucketT<size_t, size_t, std::equal_to<>>>;

void HashRateTest(const bool interactive) {
    static constexpr auto elementCountDef = static_cast<size_t>(1e+5);
    static constexpr auto accessCountDef = static_cast<size_t>(1e+8);
    static constexpr size_t elementStep = 5;
    static constexpr size_t initElem  = 1;
    static constexpr size_t tryPerMapDef = 5;

    std::vector<size_t> elems{};
    std::vector<size_t> accessIndexes{};
    std::default_random_engine eng(std::chrono::steady_clock::now().time_since_epoch().count());

    size_t elementCount{};
    size_t accessCount{};
    size_t tryPerMap{};

    if (interactive) {
        std::cout << "Welcome to the Hash Map Performanc Test!\n"
        << "Provide your parameteres of the test to begin:\n"
        << "    1) Element count - defines how many elements will be added to the maps:\n";
        std::cin >> elementCount;
        std::cout << "  2) Access count - defines how many accesses per test will be performed:\n";
        std::cin >> accessCount;
        std::cout << "  3) Attempt count - how many different attempts per map type will be performed:\n";
        std::cin >> tryPerMap;
    }

    elementCount = elementCount > 0 ? elementCount : elementCountDef;
    accessCount = accessCount > 0 ? accessCount : accessCountDef;
    tryPerMap = tryPerMap > 0 ? tryPerMap : tryPerMapDef;



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
    std::cout << "ChainMap with list buckets test:\n";
    performAccessTest<fastListMap, true>(tryPerMap, accessIndexes, elems);

    std::cout << "-------------------------------------------------------\n";
    std::cout << "ChainMap with hash buckets test:\n";
    performAccessTest<boostedMap, true, true>(tryPerMap, accessIndexes, elems);
    std::cout << "-------------------------------------------------------\n";
}
