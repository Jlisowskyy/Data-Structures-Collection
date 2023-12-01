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
class _baseHeapT: public TArrayBasedStructure<std::pair<PrioT, ItemT>, IsMemSafe> {
    // ------------------------------
    // Type creation/copying
    // ------------------------------
public:
    using mPair = std::pair<PrioT, ItemT>;
private:
    using base = TArrayBasedStructure<mPair, IsMemSafe>;
    using base::GetElemCount;
    using base::GetEndP;
    using base::AddLast;
    using base::RemoveLast;
    using base::RemoveAndReturn;
    using base::GetItem;
public:

    _baseHeapT(): TArrayBasedStructure<mPair, IsMemSafe>() {
        // Ading Sentinel
        AddLast(std::make_pair(MostSignificantPrio, ItemT{}));
    }

    _baseHeapT(const mPair* const items, const size_t size):
        TArrayBasedStructure<mPair, IsMemSafe>(items, size, size+1, 1){

        // Adding Sentinel
        GetItem(0) = std::make_pair(MostSignificantPrio, ItemT{});
        _createHeapDownToUp();
    }

    _baseHeapT(const _baseHeapT& other): TArrayBasedStructure<mPair, IsMemSafe>(other) {}
    _baseHeapT(_baseHeapT&& other) noexcept(true): TArrayBasedStructure<mPair, IsMemSafe>(std::move(other)){}

    _baseHeapT& operator=(_baseHeapT&& other) noexcept(true) {
        base::operator=(std::move(other));
        return *this;
    }

    _baseHeapT& operator=(const _baseHeapT& other) {
        base::operator=(other);
        return *this;
    }

    ~_baseHeapT() = default;

    // ------------------------------
    // Class static methods
    // ------------------------------

    static _baseHeapT UpToDownFactory(const mPair* const items, const size_t size) {
        _baseHeapT ret{size};
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
        return GetElemCount();
    }

    [[nodiscard]] size_t ElementsCount() const {
        return GetEndP()-1;
    }

    [[nodiscard]] size_t GetLastIndex() const
        // offset ready for HeapIndexClass
    {
        return GetEndP() - 2;
    }

    _baseHeapT& Insert(const mPair& pair) {
        _insert(pair);
        return *this;
    }

    [[nodiscard]] const mPair& Max() const
        // when heap is empty behaviour is undefined
    {
        return GetItem(1);
    }

    _baseHeapT& DeleteMax(mPair& out)
        // when heap is empty behaviour is undefined
    {
        out = GetItem(1);

        _deleteMax();
        return *this;
    }

    _baseHeapT& DeleteMax()
        // when heap is empty behaviour is undefined
    {
        _deleteMax();
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

    _baseHeapT& Delete(const HeapIndex index, mPair& out)
        // when heap is empty or index is out of range behaviour is undefined
    {
        const size_t i = index;
        out = GetItem(i);
        _delete(i);

        return *this;
    }

    _baseHeapT& Delete(const HeapIndex index)
    // when heap is empty or index is out of range behaviour is undefined
    {
        const size_t i = index;
        _delete(i);
        return *this;
    }

    _baseHeapT& Replace(const HeapIndex ind, const mPair& newItem, mPair& oItem)
        // when heap is empty or index is out of range behaviour is undefined
    {
        const size_t i = ind;
        oItem = GetItem(i);
        _replace(i, newItem);
        return *this;
    }

    _baseHeapT& Replace(const HeapIndex ind, const mPair& newItem)
    // when heap is empty or index is out of range behaviour is undefined
    {
        const size_t i = ind;
        _replace(i, newItem);
        return *this;
    }

    const mPair& operator[](const HeapIndex ind) const
        // when heap is empty or index is out of range behaviour is undefined
    {
        return GetItem(ind);
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
    }

public:
    friend std::ostream& operator <<(std::ostream& out, const _baseHeapT& hp) {
        return hp._print(out, hp);
    }

    // -------------------------------
    // implementation-components
    // -------------------------------
private:

    void _insert(const mPair& pair) {
        const size_t ind = GetEndP();
        AddLast(pair);
        _upHeap(ind);
    }

    void _deleteMax() {
        GetItem(1) = RemoveAndReturn();
        _downHeap(1);
    }

    std::ostream& _print(std::ostream& out, const _baseHeapT& hp) const {
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
        out << std::setw(elemStringSize) << std::setfill(' ');
        const size_t height = std::log2(hp.GetEndP() - 1) + 1; // EndP = n + 1;
        const size_t MaxLastRowElements = 1 << height - 1;
        const size_t beforeLastRowElements = MaxLastRowElements - 1;
        const size_t LastRowElements = hp.GetEndP() - beforeLastRowElements - 1;
        const size_t LastRowSpacing = PrintSpaceDist + elemStringSize;
        const size_t LastLayerChars = MaxLastRowElements * elemStringSize + (MaxLastRowElements - 1) * LastRowSpacing;

        size_t elemPerLayer = 1;
        for(size_t i = 1; i <= height-1; ++i) {
            const size_t firstElemDist = std::floor((double)(LastLayerChars - elemPerLayer*elemStringSize) / (double)(elemPerLayer * 2));
            const size_t interElemDist = elemPerLayer == 1 ? 0 :
                std::floor((double)(LastLayerChars - elemPerLayer*elemStringSize - 2*firstElemDist) / (double)(elemPerLayer - 1));

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
            printOffset(LastRowSpacing);
        }
        out << std::endl;
        return out;
    }

    void _delete(size_t i) {
        if (i == GetEndP()-1) [[unlikely]]{
            RemoveLast();
            return;
        }

        GetItem(i) = RemoveAndReturn();
        _downHeap(i);
    }

    void _replace(size_t i, const mPair& item) {
        GetItem(i) = item;

        if (pred(item.first, GetItem(_getParent(i)).first)) _upHeap(i);
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
    explicit _baseHeapT(const size_t initSize): TArrayBasedStructure<mPair, IsMemSafe>(initSize+1) {
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

    // ------------------------------
    // private class Fields
    // ------------------------------

    PriorityFunction pred{};
    inline static unsigned int PrintSpaceDist = 3;
    inline static std::string SpacingString = std::string(PrintSpaceDist, '\n');
};

#endif //HEAP_H