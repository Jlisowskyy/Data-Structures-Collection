// Author: Jakub Lisowski

#include "btree.h"
#include <cstdlib>
#include <ctime>

int BtreeMain(){
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

    testerek->display(std::cout, &btree<int>::treeNode::displayLeftOrder);
    std::cout << std::endl;
    testerek->displaySize();

    std::cout << "\nState of counter: " << counter1;
    std::cout << "\n\n-----------------------------------------------------------------------------------------------------\n\n";

    testerek2 = *testerek;
    testerek2.display(std::cout, &btree<int>::treeNode::displayLeftOrder);
    std::cout << std::endl;
    testerek2.displaySize();

    std::cout << "\n\n-----------------------------------------------------------------------------------------------------\n\n";
   
    delete testerek;
    testerek2.display(std::cout, &btree<int>::treeNode::displayLeftOrder);
    std::cout << std::endl;
    testerek2.displaySize();


    std::cout << "\n\n-----------------------------------------------------------------------------------------------------\n\n";

    std::cout << "Before merging trees: \n";

    testerek2.display(std::cout, &btree<int>::treeNode::displayLeftOrder);
    std::cout << std::endl;
    testerek2.displaySize();

    testerek3.display(std::cout, &btree<int>::treeNode::displayLeftOrder);
    std::cout << std::endl;
    testerek3.displaySize();

    std::cout << "After merging trees: \n";

    testerek2.moveTree(testerek3);


    testerek2.display(std::cout, &btree<int>::treeNode::displayLeftOrder);
    std::cout << std::endl;
    testerek2.displaySize();

    testerek3.display(std::cout, &btree<int>::treeNode::displayLeftOrder);
    std::cout << std::endl;
    testerek3.displaySize();


    return 0;
}

template<typename T>
int btree<T>::treeNode::space = 0;