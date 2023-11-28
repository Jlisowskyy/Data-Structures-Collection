//
// Created by Jlisowskyy on 11/26/23.
//

#ifndef HEAP_H
#define HEAP_H

#include <string>
#include <iostream>

#include "ArrayBasedStructure.h"

static constexpr bool IsMemSafe = true;

template<typename PrioT, typename ItemT, typename PriorityFunction, PrioT MostSignificantPrio>
class Heap: public TArrayBasedStructure<std::pair<PrioT, ItemT>, IsMemSafe> {
    // ------------------------------
    // class creation
    // ------------------------------
public:
    using mPair = std::pair<PrioT, ItemT>;
    using base = TArrayBasedStructure<mPair, IsMemSafe>;
    using base::GetSize;
    using base::GetEndP;
    using base::AddLast;
    using base::RemoveLast;
    using base::RemoveAndReturn;

    Heap(): TArrayBasedStructure<mPair, IsMemSafe>() {
        // Ading Sentinel
        AddLast(std::make_pair(MostSignificantPrio, ItemT()));
    }

    Heap(const mPair* const items, const size_t size):
        TArrayBasedStructure<mPair, IsMemSafe>(items, size, size+1, 1){

        // Adding Sentinel
        (*this)[0] = std::make_pair(MostSignificantPrio, nullptr);
        CreateHeapDownToUp();
    }

    static Heap HeapUpToDownFactory(const mPair* const items, const size_t size) {
        Heap ret{size};
        return ret.CreateHeapUpToDown(items, size);
    }

    static void ChangePrintLevelSpacing(const unsigned int newSize) {
        PrintSpaceDist = newSize;
        SpacingString = std::string(newSize, '\n');
    }

    // ------------------------------
    // class interaction
    // ------------------------------

    [[nodiscard]] size_t MemSize() const {
        return GetSize();
    }

    [[nodiscard]] size_t ElementsCount() const {
        return GetEndP();
    }

    Heap& Insert(const mPair& pair) {
        const size_t ind = GetEndP();
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

    template<typename T1, typename T2, typename SortT, T1 mPrio>
    friend std::ostream& operator <<(std::ostream& out, Heap<T1, T2, SortT, mPrio> hp) {
        static auto printOffset = [&](size_t off){
            for (size_t z = 0; z < off; ++z) {
                out << ' ';
            }
        };

        if (hp.GetEndP() == 1) out << "[ Empty heap ]";

        std::ostringstream str{};
        auto max = hp.Max();
        str << std::get<0>(max);
        const size_t elemStringSize = str.str().length();
        const size_t spacingSize = elemStringSize + PrintSpaceDist*2;
        const size_t height = std::log2(hp.GetEndP() - 1) + 1; // EndP = n + 1;
        const size_t MaxLastRowElements = (1 << (height - 1));
        const size_t beforeLastRowElements = MaxLastRowElements - 1;
        const size_t LastRowElements = hp.GetEndP() - beforeLastRowElements - 1;
        const size_t LastLayerChars = LastRowElements * elemStringSize + (LastRowElements - 1) * spacingSize;

        size_t elemPerLayer = 1;
        for(size_t i = 1; i <= height; ++i) {
            const size_t firstElemDist = LastLayerChars / (elemPerLayer * 2);
            const size_t interElemDist = 2 * firstElemDist;

            printOffset(firstElemDist);
            for(size_t j = 0; j < elemPerLayer; ++j) {
                out << std::get<0>(hp[elemPerLayer + j]);
                printOffset(interElemDist);
            }

            out << SpacingString;
            elemPerLayer *= 2;
        }
        return out;
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

        const size_t maxInd = GetEndP();
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

        const size_t maxInd = GetEndP;
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

    // Private constructor used only inside UpToDown factory.
    explicit Heap(const size_t initSize): TArrayBasedStructure<mPair, IsMemSafe>(initSize+1) {
        // Ading Sentinel
        AddLast(std::make_pair(MostSignificantPrio, ItemT()));
    }

    void CreateHeapUpToDown(const mPair* const items, const size_t size) {
        for(size_t i = 0; i < size; ++i) {
            Insert(items[i]);
        }
    }

    // Expects all elements to be actually copied inside underlying array.
    void CreateHeapDownToUp() {
        for(int i = GetEndP() / 2; i >= 1; --i)
            DownHeap(i);
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
    inline static unsigned int PrintSpaceDist = 4;
    inline static std::string SpacingString = std::string(PrintSpaceDist, '\n');
};

#endif //HEAP_H
