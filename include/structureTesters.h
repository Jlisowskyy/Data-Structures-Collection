//
// Created by Jlisowskyy on 2/2/24.
//

#ifndef STRUCTURETESTERS_H
#define STRUCTURETESTERS_H

#include <chrono>
#include <random>
#include <iostream>
#include <vector>

// Note: expects the tree to operate on size_t
template<class BTreeT>
void BinaryTreeTester(const bool interactiveMode = false) {
    static constexpr size_t maxStep = 5;
    static constexpr size_t initSeq = 1;

    // defaults
    static constexpr size_t elemCountDef = 10;
    static constexpr size_t triesDef = 3;

    size_t elemCount{};
    size_t removeActions{};
    size_t tries{};

    if (interactiveMode) {
        std::cout <<  "Welcome to structure testing main function - binary tree edition!\n"
        << "Provide your parameters to display the structures:\n"
        << "1) Element count - defines how many elements will be added to the tree:\n";
        std::cin >> elemCount;

        std::cout << "2) Remove actions count - defines how many remove operations will be performed on fully built tree:\n";
        std::cin >> removeActions;

        std::cout << "3) Attempt count - defines how many times whole procedure will be repeated:\n";
        std::cin >> tries;
    }

    elemCount = elemCount > 0 ? elemCount : elemCountDef;
    tries = tries > 0 ? tries : triesDef;

    std::default_random_engine eng(std::chrono::steady_clock::now().time_since_epoch().count());

    for (size_t t = 0; t < tries; ++t) {
        std::cout << "--------------------------------------------------------------------------------------------\n";
        std::vector<size_t> elems{};
        BTreeT map{};

        size_t elem = initSeq;
        for (size_t i = 0; i < elemCount; ++i) {
            elems.push_back(elem);
            elem += 1 + eng() % maxStep;
        }

        std::shuffle(elems.begin(), elems.end(), eng);

        std::cout << "Inputs sequence:\n";
        for(auto e : elems) {
            std::cout << std::format("{} ", e);
            map.insert(e,e);
        }
        std::cout << "Tree:\n" << map;

        for (size_t i = 0; i < removeActions; ++i) {
            map.remove(elems[i]);
            std::cout << std::format("\nTree after removing {}:\n", elems[i]) << map;
        }

        std::cout << std::format("Access tests for first and last element from sequence:\n{}\n{}\n", map[elems[0]], map[elems[elemCount-1]]);
    }
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

template<class BalancedBSTTreeT>
void performInsertRecursiveVsNonRecu(const bool interactive = false) {
    std::default_random_engine eng(std::chrono::steady_clock::now().time_since_epoch().count());

    static constexpr size_t attemptsCountDef = 5;
    static constexpr size_t elemCountDef = static_cast<size_t>(1e+7);

    static constexpr size_t maxStep = 5;
    static constexpr size_t initSeq = 1;

    size_t attemptsCount{};
    size_t elemCount{};

    if (interactive) {
        std::cout << "Welcome to the Balanced BST tree insertion type comparison!\n"
            << "Provide your parameters to begin the test!\n"
            << "    1) Attempt count - defines how many attempts per inssertion type will be performed\n";
        std::cin >> attemptsCount;
        std::cout << "    2) Elements count - defines how many elements per test will be used\n";
        std::cin >> elemCount;
    }

    attemptsCount = attemptsCount > 0 ? attemptsCount : attemptsCountDef;
    elemCount = elemCount > 0 ? elemCount : elemCountDef;

    double sumRecu{};
    double sumNRecu{};

    for (size_t i = 0; i < attemptsCount; ++i) {
        BalancedBSTTreeT map{};

        size_t elem = initSeq;
        std::vector<size_t> elems(elemCount);
        for (size_t j = 0; j < elemCount; ++j) {
            elems[j] = elem;
            elem += 1 + eng() % maxStep;
        }

        // insert non recu - sequence
        auto t1 = std::chrono::steady_clock::now();
        for (auto e : elems) map.insert(e, e);
        auto t2 = std::chrono::steady_clock::now();

        // insert recu - sequence
        map.clean();
        auto t5 = std::chrono::steady_clock::now();
        for (auto e : elems) map.insertRecursive(e, e);
        auto t6 = std::chrono::steady_clock::now();

        std::shuffle(elems.begin(), elems.end(), eng);

        // insert non recu - random data
        map.clean();
        auto t3 = std::chrono::steady_clock::now();
        for (auto e : elems) map.insert(e, e);
        auto t4 = std::chrono::steady_clock::now();

        // insert recu - random data
        map.clean();
        auto t7 = std::chrono::steady_clock::now();
        for (auto e : elems) map.insertRecursive(e, e);
        auto t8 = std::chrono::steady_clock::now();

        const double timeSeqNR = (t2.time_since_epoch() - t1.time_since_epoch()).count()*1e-6;
        const double timeShuffleNR = (t4.time_since_epoch() - t3.time_since_epoch()).count()*1e-6;
        const double timeSeqR = (t6.time_since_epoch() - t5.time_since_epoch()).count()*1e-6;
        const double timeShuffleR = (t8.time_since_epoch() - t7.time_since_epoch()).count()*1e-6;

        sumNRecu += timeSeqNR + timeShuffleNR;
        sumRecu += timeSeqR + timeShuffleR;

        std::cout << std::format("Attempt number {}\n", i)
            << "    Non recursive result:\n"
            << std::format("        Time spent on sequenced data: {}ms\n        Insertion per ms ons sequenced data: {}\n", timeSeqNR, elemCount/timeSeqNR)
            << std::format("        Time spent on shuffled data: {}ms\n        Insertion per ms ons shuffled data: {}\n", timeShuffleNR, elemCount/timeShuffleNR)
            << "    Recursive result:\n"
            << std::format("        Time spent on sequenced data: {}ms\n        Insertion per ms ons sequenced data: {}\n", timeSeqR, elemCount/timeSeqR)
            << std::format("        Time spent on shuffled data: {}ms\n        Insertion per ms ons shuffled data: {}\n", timeShuffleR, elemCount/timeShuffleR);
    }

    std::cout << "----------------------\nSummary:\n"
        << std::format("    Total time spent with non recursive insertions: {}\n    Average insertion per ms: {}\n", sumNRecu, 2 * attemptsCount * elemCount / sumNRecu)
        << std::format("    Total time spent with recursive insertions: {}\n    Average insertion per ms: {}\n", sumRecu, 2 * attemptsCount * elemCount / sumRecu);
}

template<class BTreeT>
void PerformInteractiveTest() {
    std::cout << "Welcome to interactive binary tree tester!\n"
        << "Provide some numbers to add them to the structure!\n"
        << "Positive integeres means that provided number will be added\n"
        << "In contrast negative ones will means that provided number will be removed\n"
        << "And finally 0 number exits the program!\n";
    BTreeT map{};
    int num{};
    do {
        std::cout << "\nWrite some numbers:\n";
        std::cin >> num;

        if (num > 0) {
            map.insert(num, num);
            std::cout << "Tree afer the operation:\n\n" << map;
        }
        else if (num < 0) {
            map.remove(std::abs(num));
            std::cout << "Tree afer the operation:\n\n" << map;
        }
    }while(num != 0);
}

#endif //STRUCTURETESTERS_H
