//
// Created by Jlisowskyy on 11/26/23.
//

#ifndef HEAP_H
#define HEAP_H

#include <string>
#include <iostream>

#include "ArrayBasedStructure.h"
#include "HeapHelpers.h"

static constexpr bool IsMemSafe = false;

template<typename PrioT, typename ItemT, typename PriorityFunction, PrioT MostSignificantPrio>
class Heap: public TArrayBasedStructure<std::pair<PrioT, ItemT>, IsMemSafe> {
    // ------------------------------
    // Type creation/copying
    // ------------------------------
public:
    using mPair = std::pair<PrioT, ItemT>;
private:
    using base = TArrayBasedStructure<mPair, IsMemSafe>;
    using base::GetSize;
    using base::GetEndP;
    using base::AddLast;
    using base::RemoveLast;
    using base::RemoveAndReturn;
    using base::GetItem;
public:

    Heap(): TArrayBasedStructure<mPair, IsMemSafe>() {
        // Ading Sentinel
        AddLast(std::make_pair(MostSignificantPrio, ItemT{}));
    }

    Heap(const mPair* const items, const size_t size):
        TArrayBasedStructure<mPair, IsMemSafe>(items, size, size+1, 1){

        // Adding Sentinel
        GetItem(0) = std::make_pair(MostSignificantPrio, ItemT{});
        _createHeapDownToUp();
    }

    Heap(const Heap& other): TArrayBasedStructure<mPair, IsMemSafe>(other) {}
    Heap(Heap&& other) noexcept(true): TArrayBasedStructure<mPair, IsMemSafe>(std::move(other)){}

    Heap& operator=(Heap&& other) noexcept(true) {
        static_cast<TArrayBasedStructure<mPair, IsMemSafe> &>(*this) = std::move(other);
        return *this;
    }

    Heap& operator=(const Heap& other) {
        static_cast<TArrayBasedStructure<mPair, IsMemSafe> &>(*this).operator=(other);
        return *this;
    }

    ~Heap() = default;

    // ------------------------------
    // Class static methods
    // ------------------------------

    static Heap HeapUpToDownFactory(const mPair* const items, const size_t size) {
        Heap ret{size};
        ret._createHeapUpToDown(items, size);
        return ret;
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
        return GetEndP()-1;
    }

    [[nodiscard]] size_t GetLastIndex() const
        // offset ready for HeapIndexClass
    {
        return GetEndP() - 2;
    }

    Heap& Insert(const mPair& pair) {
        const size_t ind = GetEndP();
        AddLast(pair);
        _upHeap(ind);

        return *this;
    }

    [[nodiscard]] const mPair& Max() const
        // when heap is empty behaviour is undefined
    {
        return GetItem(1);
    }

    Heap& DeleteMax(mPair& out)
        // when heap is empty behaviour is undefined
    {
        out = GetItem(1);

        GetItem(1) = RemoveAndReturn();
        _downHeap(1);
        return *this;
    }

    Heap& DeleteMax()
        // when heap is empty behaviour is undefined
    {
        GetItem(1) = RemoveAndReturn();
        _downHeap(1);
        return *this;
    }

    [[nodiscard]] bool IsEmpty() const{
        return GetEndP() == 1;
    }

    [[nodiscard]] HeapIndex Search(PrioT prio)
        // when heap is empty behaviour is undefined
        // Return: index of found element. Output can be invalid
    {
        HeapIndex ret{};
        ret.index = _search(prio, 1);
        return ret;
    }

    Heap& Delete(const HeapIndex index, mPair& out)
        // when heap is empty or index is out of range behaviour is undefined
    {
        const size_t i = index;
        out = GetItem(i);
        _delete(i);

        return *this;
    }

    Heap& Delete(const HeapIndex index)
    // when heap is empty or index is out of range behaviour is undefined
    {
        const size_t i = index;
        _delete(i);
        return *this;
    }

    Heap& Replace(const HeapIndex ind, const mPair& newItem, mPair& oItem)
        // when heap is empty or index is out of range behaviour is undefined
    {
        const size_t i = ind;
        oItem = GetItem(i);;
        _replace(i, newItem);
        return *this;
    }

    Heap& Replace(const HeapIndex ind, const mPair& newItem)
    // when heap is empty or index is out of range behaviour is undefined
    {
        const size_t i = ind;
        _replace(i, newItem);
        return *this;
    }

    const mPair& operator[](const HeapIndex ind) const
        // when heap is empty or index is out of range behaviour is undefined
    {
        return GetItem(ind);;
    }

    // ------------------------------
    // Printing methods
    // ------------------------------
private:
    [[nodiscard]] size_t _findMaxPrint() const{
        if (IsEmpty()) return 0;
        std::ostringstream str{};

        str << std::get<0>(GetItem(1));
        size_t max = str.str().length();
        str = std::ostringstream{};

        for (size_t i = 2; i < GetEndP(); ++i) {
            str << std::get<0>(GetItem(i));
            if (str.str().length() > max) {
                max = str.str().length();
            }

            str = std::ostringstream{};
        }

        return max;
    };

public:
    friend std::ostream& operator <<(std::ostream& out, const Heap& hp) {
        static auto printOffset = [&](const size_t off){
            for (size_t z = 0; z < off; ++z) {
                out << ' ';
            }
        };

        if (hp.GetEndP() == 1) {
            out << "[ Empty heap ]";
            return out;
        }

        const size_t elemStringSize = hp._findMaxPrint(); // slow but needed
        const size_t height = std::log2(hp.GetEndP() - 1) + 1; // EndP = n + 1;
        const size_t MaxLastRowElements = 1 << height - 1;
        const size_t beforeLastRowElements = MaxLastRowElements - 1;
        const size_t LastRowElements = hp.GetEndP() - beforeLastRowElements - 1;
        const size_t LastLayerChars = MaxLastRowElements * elemStringSize + (MaxLastRowElements - 1) * PrintSpaceDist;

        size_t elemPerLayer = 1;
        for(size_t i = 1; i <= height-1; ++i) {
            const size_t firstElemDist = (LastLayerChars - elemPerLayer*elemStringSize) / (elemPerLayer * 2);
            const size_t interElemDist = elemPerLayer == 1 ? 0 :
                (LastLayerChars - elemPerLayer*elemStringSize - 2*firstElemDist) / (elemPerLayer - 1);

            printOffset(firstElemDist);
            for(size_t j = 0; j < elemPerLayer; ++j) {
                out << std::get<0>(hp.GetItem(elemPerLayer + j));
                printOffset(interElemDist);
            }

            out << SpacingString;
            elemPerLayer *= 2;
        }

        // cleaning last line
        for(size_t j = 0; j < LastRowElements; ++j) {
            out << std::get<0>(hp.GetItem(elemPerLayer + j));
            printOffset(PrintSpaceDist);
        }
        out << std::endl;
        return out;
    }

    // -------------------------------
    // implementation-components
    // -------------------------------
private:

    void _delete(size_t i) {
        if (i == GetEndP()-1) {
            RemoveLast();
            return;
        }

        GetItem(i) = RemoveAndReturn();
        _downHeap(i);
    }

    void _replace(size_t i, const mPair& item) {
        GetItem(i) = item;

        if (i == 1) _downHeap(i);
        else if (pred(item.first, GetItem(_getParent(i)).first)) _upHeap(i);
        else _downHeap(i);
    }

    size_t _search(PrioT prio, size_t ind) {
        if (prio == GetItem(ind).first) return ind;

        if (pred(prio, GetItem(ind).first)) {
            return 0;
        }

        if (const size_t lChild = _getLeftChild(ind) ;  lChild < GetEndP()) {
            if (const size_t lChildResult = _search(prio, lChild) ; lChildResult != 0) return lChildResult;

            if (const size_t rChild = lChild + 1 ; lChild + 1 < GetEndP()) {
                if (const size_t rChildResult = _search(prio, rChild); rChildResult != 0) return rChildResult;
            }
        }

        return 0;
    }

    void _upHeap(size_t i) {
        mPair elem = GetItem(i);

        for(size_t pInd = _getParent(i); pred(elem.first, GetItem(pInd).first); pInd = _getParent(i) ) {
            GetItem(i) = GetItem(pInd);
            i = pInd;
        }

        GetItem(i) = elem;
    }

    void _downHeap(size_t i) {
        mPair elem = GetItem(i);

        const size_t maxInd = GetEndP();
        for (size_t childInd = _getLeftChild(i); childInd < maxInd; childInd = _getLeftChild(i)) {
            if (const size_t rChild = childInd + 1; rChild < maxInd) {
                if (pred(GetItem(rChild).first, GetItem(childInd).first))
                    childInd = rChild;
            }

            if (pred(GetItem(childInd).first, elem.first)) {
                GetItem(i) = GetItem(childInd);
                i = childInd;
            }
            else break;
        }

        GetItem(i) = elem;
    }

    // Private constructor used only inside UpToDown factory.
    explicit Heap(const size_t initSize): TArrayBasedStructure<mPair, IsMemSafe>(initSize+1) {
        // Ading Sentinel
        AddLast(std::make_pair(MostSignificantPrio, ItemT()));
    }

    void _createHeapUpToDown(const mPair* const items, const size_t size) {
        for(size_t i = 0; i < size; ++i) {
            Insert(items[i]);
        }
    }

    // Expects all elements to be actually copied inside underlying array.
    void _createHeapDownToUp() {
        const size_t height = std::log2(GetEndP() - 1) + 1; // EndP = n + 1;
        const size_t MaxLastRowElements = 1 << height - 1;
        const size_t beforeLastRowElements = MaxLastRowElements - 1;

        for(int i = beforeLastRowElements; i >= 1; --i)
            _downHeap(i);
    }

    static constexpr size_t _getParent(const size_t index) {
        return index / 2;
    }

    static constexpr size_t _getLeftChild(const size_t index) {
        return 2 * index;
    }

    static constexpr size_t _getRightChild(const size_t index) {
        return _getLeftChild(index) + 1;
    }

    PriorityFunction pred{};
    inline static unsigned int PrintSpaceDist = 3;
    inline static std::string SpacingString = std::string(PrintSpaceDist, '\n');
};

#endif //HEAP_H