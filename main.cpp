// Author: Jakub Lisowski

#include "include/hanoi/hanoiMain.h"
#include "include/Heaps/HeapsMain.h"
#include "include/DictionaryTrees/dTreeMain.h"
#include "include/Sorting/sortingMains.h"
#include "include/HashMaps/HashingMain.h"

int main()
{
    std::cout << "Welcome to structure testing function!\n"
        << "Choose next function to invoke:\n"
        << "1) Hanoi Tower Presentation\n"
        << "2) Priority Queues Presentation\n"
        << "3) Dictionary - trees Presentation\n"
        << "4) Dictionary - Hash Maps Presentation\n"
        << "5) Sorting Methods Presentation\n";

    int choosenOption{};
    std::cin >> choosenOption;

    switch (choosenOption) {
        case 1:
            HanoirMain();
            break;
        case 2:
            HeapsMain();
            break;
        case 3:
            dTreeMain();
            break;
        case 4:
            hashMain();
            break;
        case 5:
            sortMain();
            break;
        default:
            break;
    }

    return 0;
}