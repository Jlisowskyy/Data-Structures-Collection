//
// Created by Jlisowskyy on 1/31/24.
//

#ifndef HASHINGMAIN_H
#define HASHINGMAIN_H

static constexpr bool displayPlainMap = false;
static constexpr bool displayExpandiblePlainMap = true;

static constexpr bool displayHashrateTests = false;

void PlainMapTest();
void ExpandiblePlainMapTest();
void HashRateTest();

inline int hashMain() {

    if constexpr (displayPlainMap) PlainMapTest();
    if constexpr (displayExpandiblePlainMap) ExpandiblePlainMapTest();

    if constexpr (displayHashrateTests) HashRateTest();

    return 0;
}

#endif //HASHINGMAIN_H
