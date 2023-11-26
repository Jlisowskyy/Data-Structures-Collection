//
// Created by Jlisowskyy on 11/26/23.
//

#ifndef HEAP_H
#define HEAP_H

#include "ArrayBasedStructure.h"

static constexpr bool IsMemSafe = true;

template<typename PrioT, typename ItemT, typename PriorityFunction, PrioT MostSignificantPrio>
class Heap: public TArrayBasedStructure<std::pair<PrioT, ItemT*>, IsMemSafe> {
    // ------------------------------
    // class creation
    // ------------------------------
public:
    using mPair = std::pair<PrioT, ItemT*>;
    using base = TArrayBasedStructure<mPair, IsMemSafe>;
    using base::GetSize;
    using base::GetLastElemPos;
    using base::AddLast;
    using base::RemoveLast;
    using base::RemoveAndReturn;

    Heap(): TArrayBasedStructure<mPair, IsMemSafe>() {

        // Ading Sentinel
        AddLast(std::make_pair(MostSignificantPrio, nullptr));
    }

    Heap(const mPair* const items, const size_t size): Heap() {
        CreateHeapDownToUp(items, size);
    }

    // ------------------------------
    // class interaction
    // ------------------------------

    [[nodiscard]] size_t MemSize() const {
        return GetSize();
    }

    [[nodiscard]] size_t ElementsCount() const {
        return GetLastElemPos();
    }

    Heap& Insert(const mPair& pair) {
        const size_t ind = GetLastElemPos();
        AddLast(pair);
        UpHeap(ind);

        return *this;
    }

    const mPair& Max() const {
        return (*this)[1];
    }

    mPair DeleteMax() {
        auto ret = (*this)[1];
        (*this)[1] = RemoveAndReturn();
        DownHeap(1);
        return ret;
    }

    void CreateHeapUpToDown(const mPair* const items, const size_t size) {
        for(size_t i = 0; i < size; ++i) {
            Insert(items[i]);
        }
    }

    void CreateHeapDownToUp(const mPair* const items, const size_t size) {

    }


    // -------------------------------
    // implementation-components
    // -------------------------------
private:
    void UpHeap(size_t i) {
        mPair elem = (*this)[i];

        for(size_t pInd = GetParent(i); pred(elem.first, (*this)[pInd].first); pInd = GetParent(i) ) {
            (*this)[i] = (*this)[pInd];
            i = pInd;
        }

        (*this)[i] = elem;
    }

    void DownHeap(size_t i) {
        mPair elem = (*this)[i];

        const size_t maxInd = GetLastElemPos;
        for (size_t childInd = GetLeftChild(i); childInd <= maxInd; childInd = GetLeftChild(i)) {
            if (const size_t rChild = childInd + 1; rChild <= maxInd) {
                if (pred(rChild, childInd))
                    childInd = rChild;
            }

            if (pred((*this)[childInd].first, elem.first)) {
                (*this)[i] = (*this)[childInd];
                i = childInd;
            }
            else break;
        }

        (*this)[i] = elem;
    }

    void MDownHeap(size_t i) {
        mPair elem = (*this)[i];

        const size_t maxInd = GetLastElemPos;
        for (size_t lChild = GetLeftChild(i); lChild <= maxInd; lChild = GetLeftChild(i)) {
            if (pred((*this)[lChild].first, elem.first)) {
                (*this)[i] = (*this)[lChild];
                i = lChild;
            }
            else if (const size_t rChild = lChild + 1; rChild <= maxInd) {
                    if (pred(rChild, lChild)) {
                        (*this)[i] = (*this)[rChild];
                        i = rChild;
                    }
            }
            else break;
        }

        (*this)[i] = elem;
    }

    static constexpr size_t GetParent(const size_t index) {
        return index / 2;
    }

    static constexpr size_t GetLeftChild(const size_t index) {
        return 2 * index;
    }

    static constexpr size_t GetRightChild(const size_t index) {
        return GetLeftChild(index) + 1;
    }

    PriorityFunction pred;
};

#endif //HEAP_H
