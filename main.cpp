#include "btree.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(nullptr));
    btree<int>* testerek = new btree<int>() , testerek2, testerek3;
    int counter1 = 0;
    int counter3 = 0;

    for (int i = 0; i < 25; ++i) {
        int temp = (rand() % 150);
        if (testerek->insert(temp)) {
            ++counter1;
        }

        temp = (rand() % 25);
        if (testerek3.insert(temp)) {
            ++counter3;
        }
    }
    
    testerek->Display(std::cout, &btree<int>::treenode::DisplayLeftOrder);
    std::cout << std::endl;
    testerek->DisplaySize();

    std::cout << "\nState of counter: " << counter1;
    std::cout << "\n\n-----------------------------------------------------------------------------------------------------\n\n";

    testerek2 = *testerek;
    testerek2.Display(std::cout, &btree<int>::treenode::DisplayLeftOrder);
    std::cout << std::endl;
    testerek2.DisplaySize();

    std::cout << "\n\n-----------------------------------------------------------------------------------------------------\n\n";
   
    delete testerek;
    testerek2.Display(std::cout, &btree<int>::treenode::DisplayLeftOrder);
    std::cout << std::endl;
    testerek2.DisplaySize();


    std::cout << "\n\n-----------------------------------------------------------------------------------------------------\n\n";

    std::cout << "Before merging trees: \n";

    testerek2.Display(std::cout, &btree<int>::treenode::DisplayLeftOrder);
    std::cout << std::endl;
    testerek2.DisplaySize();

    testerek3.Display(std::cout, &btree<int>::treenode::DisplayLeftOrder);
    std::cout << std::endl;
    testerek3.DisplaySize();

    std::cout << "After merging trees: \n";

    testerek2.MoveTree(testerek3);


    testerek2.Display(std::cout, &btree<int>::treenode::DisplayLeftOrder);
    std::cout << std::endl;
    testerek2.DisplaySize();

    testerek3.Display(std::cout, &btree<int>::treenode::DisplayLeftOrder);
    std::cout << std::endl;
    testerek3.DisplaySize();


    return 0;
}