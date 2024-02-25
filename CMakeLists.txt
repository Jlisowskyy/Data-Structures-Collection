cmake_minimum_required(VERSION 3.26)
project(DataTypes)

set(CMAKE_CXX_STANDARD 20)

add_executable(DataTypes
        main.cpp
        include/Heaps/_baseBeapT.h
        include/Heaps/HeapsMain.h
        include/Heaps/_baseHeapT.h
        include/Heaps/ArrayBasedStructure.h
        src/Debuggers.cpp
        include/Heaps/HeapHelpers.h
        include/Heaps/PriorityQueue.h
        include/Heaps/_baseBeapT.h
        include/Heaps/heapTesters.h
        include/Heaps/_baseLeftistHeapT.h
        include/Heaps/_baseBinomialQueueT.h
        include/DictionaryTrees/Splay.h
        include/DictionaryTrees/dTreeMain.h
        include/DictionaryTrees/_AVLcore.h
        include/bTreeHelpers.h
        include/DictionaryTrees/_SplayCore.h
        include/arrayBasedTreeHelpers.h
        include/DictionaryTrees/binaryTRIALStructures.h
        include/Sorting/sortingMains.h
        include/Sorting/listSorting.h
        include/Sorting/indexedSorting.h
        include/HashMaps/chainHashingMap.h
        include/HashMaps/plainHashMap.h
        include/HashMaps/HashFunctions.h
        include/HashMaps/HashingMain.h
        src/HashingMain.cpp
        include/linkedListHelpers.h
        src/dTreeMain.cpp
        include/DictionaryTrees/BSTTree.h
        include/DictionaryTrees/AVLTree.h
        include/structureTesters.h
        include/simpleStructures.h
)

#add_compile_options(DataTypes -fsanitize=address,undefined -DDEBUG_)
#add_compile_options(DataTypes -O3;-march=native)