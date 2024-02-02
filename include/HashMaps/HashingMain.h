//
// Created by Jlisowskyy on 1/31/24.
//

#ifndef HASHINGMAIN_H
#define HASHINGMAIN_H

#include <iostream>

void PlainMapTest();
void ExpandiblePlainMapTest();
void HashRateTest(bool interactive = false);

inline int hashMain() {
    std::cout << "Choose type of the structure to be tested:\n"
    << "1) Plain hash map\n"
    << "2) Expandible plain hash map\n"
    << "3) Comparison of chain hash map with unordered_map\n";

    int choosenOption{};
    std::cin >> choosenOption;

    switch (choosenOption) {
        case 1:
            PlainMapTest();
            break;
        case 2:
            ExpandiblePlainMapTest();
            break;
        case 3:
            HashRateTest(true);
            break;
        default:
            break;
    }

    return 0;
}

#endif //HASHINGMAIN_H
