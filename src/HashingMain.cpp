//
// Created by Jlisowskyy on 1/31/24.
//

#include "../include/HashMaps/HashingMain.h"
#include "../include/HashMaps/plainHashMap.h"

#include <iostream>

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
            bool result = map.Insert(key, key);

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
            bool result = map.Insert(key, key);
            if (!result) collisionDetected = true;

            std::cout << std::format("Adding new key: {} to map...\nStatus: {}\n", key, result);
        }

        size += sizeStep;
    }while(collisionDetected);

}

void HashRateTest() {}
