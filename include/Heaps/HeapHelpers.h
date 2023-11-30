//
// Created by Jlisowskyy on 11/30/23.
//

#ifndef HEAPHELPERS_H
#define HEAPHELPERS_H



class HeapIndex
    // Invalid indexes should not be used inside any instance,
    // it will lead to undefined behaviour. Such invalid entity
    // could only be an output of search or other heap-related method.
    // That means after search index have to be invalidated.
{
public:
    explicit HeapIndex(const size_t i): index{ i + 1 } { };
    [[nodiscard]] operator size_t() const { return index; }
    [[nodiscard]] size_t operator()() const { return index; }
    [[nodiscard]] bool isValid() const { return  index != 0; }
private:
    template<typename PrioT, typename ItemT, typename PriorityFunction, PrioT MostSignificantPrio>
    friend class Heap;

    HeapIndex(): index{ 0 } {}
    size_t index;
};

#endif //HEAPHELPERS_H
