// Author: Jakub Lisowski

#include "include/Heaps/HeapsMain.h"
#include "include/DictionaryTrees/dTreeMain.h"
#include "include/Sorting/sortingMains.h"
#include "include/HashMaps/HashingMain.h"

int main()
{
    std::cout << "Welcome to structure testing function!\n"
        << "Choose next function to invoke:\n"
        << "1) Priority Queues Presentation\n"
        << "2) Dictionary - trees Presentation\n"
        << "3) Dictionary - Hash Maps Presentation\n"
        << "4) Sorting Methods Presentation\n";

    int choosenOption{};
    std::cin >> choosenOption;

    switch (choosenOption) {
        case 1:
            HeapsMain();
            break;
        case 2:
            dTreeMain();
            break;
        case 3:
            hashMain();
            break;
        case 4:
            sortMain();
            break;
        default:
            break;
    }

    return 0;
}