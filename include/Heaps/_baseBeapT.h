//
// Created by Jlisowskyy on 11/8/23.
//

#ifndef MINI_PROJECTS_BEAP_H
#define MINI_PROJECTS_BEAP_H

#include <cstdlib>
#include <tuple>
#include <cmath>

// TODO: temporary
#define DEBUG_

#include "ArrayBasedStructure.h"
#include "HeapHelpers.h"

static constexpr bool IsMemSafe1 = false;

template<typename PrioT, typename ItemT, typename PriorityFunction, PrioT MostSignificantPrio>
class _baseBeapT: public TArrayBasedStructure<std::pair<PrioT, ItemT>, IsMemSafe1> {
    // ------------------------------
    // Type creation/copying
    // ------------------------------
public:
    using mPair = std::pair<PrioT, ItemT>;
private:
    using base = TArrayBasedStructure<mPair, IsMemSafe1>;
    using base::GetElemCount;
    using base::GetEndP;
    using base::AddLast;
    using base::RemoveLast;
    using base::RemoveAndReturn;
    using base::GetItem;
public:
    _baseBeapT(): TArrayBasedStructure<mPair, IsMemSafe1>() {
        // Ading Sentinel
        AddLast(std::make_pair(MostSignificantPrio, ItemT{}));
    }

    _baseBeapT(const mPair* const items, const size_t size):
        TArrayBasedStructure<mPair, IsMemSafe1>(items, size, size+1, 1){

        // Adding Sentinel
        GetItem(0) = std::make_pair(MostSignificantPrio, ItemT{});
        _createBeapFromDownToUp();
    }

    _baseBeapT(const _baseBeapT& other): TArrayBasedStructure<mPair, IsMemSafe1>(other) {}
    _baseBeapT(_baseBeapT&& other) noexcept(true): TArrayBasedStructure<mPair, IsMemSafe1>(std::move(other)){}

    _baseBeapT& operator=(_baseBeapT&& other) noexcept(true) {
        base::operator=(std::move(other));
        return *this;
    }

    _baseBeapT& operator=(const _baseBeapT& other) {
        base::operator=(other);
        return *this;
    }

    ~_baseBeapT() = default;

    // ------------------------------
    // Class static methods
    // ------------------------------

    static _baseBeapT UpToDownFactory(const mPair* const items, const size_t size) {
        _baseBeapT ret{size};
        ret._createBeapFromUpToDown(items, size);
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

    _baseBeapT& Insert(const mPair& pair) {
        _insert(pair);
        return *this;
    }

    [[nodiscard]] const mPair& Max() const
        // when heap is empty behaviour is undefined
    {
        return GetItem(1);
    }

    _baseBeapT& DeleteMax(mPair& out)
        // when heap is empty behaviour is undefined
    {
        out = GetItem(1);

        _deleteMax();
        return *this;
    }

    _baseBeapT& DeleteMax()
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
        ret.index = _search(prio);
        return ret;
    }

    _baseBeapT& Delete(const HeapIndex index, mPair& out)
        // when heap is empty or index is out of range behaviour is undefined
    {
        const size_t i = index;
        out = GetItem(i);
        _delete(i);

        return *this;
    }

    _baseBeapT& Delete(const HeapIndex index)
    // when heap is empty or index is out of range behaviour is undefined
    {
        const size_t i = index;
        _delete(i);
        return *this;
    }

    _baseBeapT& Replace(const HeapIndex ind, const mPair& newItem, mPair& oItem)
        // when heap is empty or index is out of range behaviour is undefined
    {
        const size_t i = ind;
        oItem = GetItem(i);
        _replace(i, newItem);
        return *this;
    }

    _baseBeapT& Replace(const HeapIndex ind, const mPair& newItem)
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
    friend std::ostream& operator <<(std::ostream& out, const _baseBeapT& hp) {
        return hp._print(out, hp);
    }

    // ----------------------------------
    // Debug and testing procedures
    // ----------------------------------

#ifdef DEBUG_
    static void Test() {
        static constexpr size_t cords[] = { 0, 1, 2, 5, 7 };

        for (const auto cord: cords) {
            auto [row, col] = _getBeapPos(cord);
            const size_t index = _getIndex(row, col);

            std::cout << "Testing cords output on cord: " << cord
                << "\nRows cord: " << row << "\nCols cord: " << col
                << "\nIndex of acquired cords: " << index << "\n\n";
        }
    }
#endif // DEBUG_

    // -------------------------------
    // implementation-components
    // -------------------------------
private:

    void _createBeapFromDownToUp() {
       for(size_t ind = _getPreLastRowLastElement(); ind >= 1; --ind) {
           _downBeap(ind);
       }
    }

    void _createBeapFromUpToDown(const mPair* const items, const size_t size) {
        for(size_t i  = 0 ; i < size; ++i) {
            _insert(items[i]);
        }
    }

    size_t _search(PrioT prio) const{
        return  _searchAct(prio);
    }

    size_t _searchN(PrioT prio) const{
        /*
         *              OTHER IMPLEMENTATION TYPE
         *
         */

        size_t ind = GetEndP() - 1;
        auto [row, col] = _getBeapPos(ind);
        if (row != col) {
            ind -= col;
            row = col = row-1;
        }

        static auto leftBorderCleaning = [&] {
            size_t lChild = ind + row;

            const size_t range = GetEndP();
            while(lChild < range) {

            }
            if (lChild >= GetEndP() || pred(prio, GetItem(lChild).first)) [[unlikely]]
                // element not found we have no possible paths to go.
            {
                return 0;
            }
            ind = lChild;
            ++row;
        };

        while(GetItem(ind).first != prio) {
            if (col == 1)
                // left parrent dont exists
            {
                leftBorderCleaning();
            }
        }

        return ind;
    }

    size_t _searchAct(PrioT prio) const{
        const size_t ep = GetEndP();
        size_t ind = ep;
        auto [row, col] = _getBeapPos(ind);
        const size_t lowerRange = ind - col + 1;
        if (row != col) {
            ind -= col;
            row = col = row-1;
        }

        while(ind != 0 && ind != lowerRange) {
            if (pred(prio, GetItem(ind).first))
                // we are bigger than currently chosen priority
            {
                if (col != 1)
                    // otherwise there is no left parent
                {
                    ind -= row;
                    --row; --col;
                }
                else return 0; // we cant go back
            }
            else if (pred(GetItem(ind).first, prio))
                // we are smaller than currently chosen priority
            {
                if (const size_t lChild = ind + row; lChild < ep)
                    // left child exists
                {
                    ind = lChild;
                    ++row;
                }
                else
                    // left child doesnt exists
                {
                    --col; --ind;
                }
            }
            else return ind; // we are not bigger and not smalelr so equal
        }

        return 0; // came to dead end
    }

    void _replace(const size_t i, const mPair& item){
        auto [row, col] = _getBeapPos(i);

        if (size_t lChild = i + row; lChild < GetEndP())
            // we are sure that at lest one child exists
        {
            if (size_t rChild = lChild + 1; rChild < GetEndP())
                // both childrens exists
            {
                GetItem(i) = item;

                // Any of childs has bigger priority
                if (pred(GetItem(lChild).first, item.first) || pred(GetItem(rChild).first, item.first)) _downBeap(i);
                else _upBeap(i);
                return;
            }

            if (pred(GetItem(lChild).first, item.first))
                // edge situation where we are at last - 1 row and our only child is last item in array.
                // We just needt to swap them.
            {
                GetItem(i) = GetItem(lChild);
                GetItem(lChild) = item;
                return;
            }
        }

        GetItem(i) = item;
        _upBeap(i);
    }

    void _delete(size_t i) {
        if (i == GetEndP()-1) [[unlikely]]{
            RemoveLast();
            return;
        }

        GetItem(i) = RemoveAndReturn();
        _downBeap(i);
    }

    std::ostream& _print(std::ostream& out, const _baseBeapT& bp) const {
        static auto printOffset = [&](const size_t off){
            for (size_t z = 0; z < off; ++z) {
                out << ' ';
            }
        };

        if (bp.GetEndP() == 1) {
            out << "[ Empty heap ]";
            return out;
        }

        const size_t elemStringSize = bp._findMaxPrint(); // slow but needed
        out << std::setw(elemStringSize) << std::setfill(' ');
        const auto [lRow, lCol] = _getBeapPos(bp.GetEndP() - 1);
        const size_t AllElements = bp.GetEndP() - 1;
        const size_t height = lRow;
        const size_t MaxLastRowElements = lRow;
        const size_t beforeLastRowElements = AllElements - lCol;
        const size_t LastRowElements = AllElements - beforeLastRowElements;
        const size_t LastRowSpacing = PrintSpaceDist + elemStringSize;
        const size_t LastLayerChars = MaxLastRowElements * elemStringSize + (MaxLastRowElements - 1) * LastRowSpacing;

        size_t elemPerLayer = 1;
        size_t prevLayers = 0;
        for(size_t i = 1; i <= height-1; ++i) {
            const size_t firstElemDist = std::floor((double)(LastLayerChars - elemPerLayer*elemStringSize) / (double)(elemPerLayer + 1));
            const size_t interElemDist = elemPerLayer == 1 ? 0 :
                std::floor((double)(LastLayerChars - elemPerLayer*elemStringSize - 2*firstElemDist) / (double)(elemPerLayer - 1));

            printOffset(firstElemDist);

            for(size_t j = 0; j < elemPerLayer; ++j) {
                out << std::get<0>(bp.GetItem(prevLayers + 1 + j));
                printOffset(interElemDist);
            }

            out << SpacingString;
            prevLayers += elemPerLayer;
            elemPerLayer++;
        }

        // cleaning last line
        for(size_t j = 0; j < LastRowElements; ++j) {
            out << std::get<0>(bp.GetItem(prevLayers + 1 + j));
            printOffset(LastRowSpacing);
        }
        out << std::endl;
        return out;
    }

    void _deleteMax() {
        GetItem(1) = RemoveAndReturn();
        _downBeap(1);
    }

    void _insert(const mPair& pair) {
        const size_t ind = GetEndP();
        AddLast(pair);
        _upBeap(ind);
    }

    void _upBeap(size_t index)
        // Basic simplest implementation
    {
        static constexpr size_t fCord = 1; // First elemen inside the row

        mPair elem = GetItem(index);
        auto [row, col] = _getBeapPos(index);

        while(true) {
            if (col == row)
                // We are on the last element of actul row, there is only left parent or we are at root
            {
                if (size_t pCord = index - row; pred(elem.first, GetItem(pCord).first) ){
                    --row; --col;
                    GetItem(index) = GetItem(pCord);
                    index = pCord;
                }
                else break;
            }
            else if (col == 1)
                // We are at the first element of row, so there is only right parent possible.
                // We are sure that we are not inside root because this is always processed in previous if statement.
            {
                if (size_t pCord = index - row + 1; pred(elem.first, GetItem(pCord).first) ){
                    GetItem(index) = GetItem(pCord);
                    index = pCord;
                    --row;
                }
                else break;
            }
            else
                // We are not at the first element neither the last.
            {
                if (auto [lCord, rCord] = std::make_pair(index - row, index - row + 1); pred(GetItem(lCord).first, GetItem(rCord).first ))
                    // left parent is bigger, we swap with smaller one to preserve heap rule
                {
                    if (pred(elem.first, GetItem(rCord).first)) {
                        GetItem(index) = GetItem(rCord);
                        index = rCord;
                        --row;
                    }
                    else break;
                }
                else
                    // right is bigger, we swap with smaller one to preserve heap rule
                {
                    if (pred(elem.first, GetItem(lCord).first)) {
                        GetItem(index) = GetItem(lCord);
                        index = lCord;
                        --row; --col;
                    }
                    else break;
                }
            }
        }

        GetItem(index) = elem;
    }

    void _downBeap(size_t index)
        // Basic simplest implementation
    {
        static constexpr size_t fCord = 1; // First elemen inside the row

        mPair elem = GetItem(index);
        auto [row, col] = _getBeapPos(index);

        size_t lChild = index + row;
        const size_t range = GetEndP();
        while(lChild < range)
            // We check whether left child exists
        {
            if (size_t rChild = lChild + 1; rChild < range)
                // both childs exists
            {
                if (pred(GetItem(rChild).first, GetItem(lChild).first))
                    // right child is bigger
                {
                    if (pred(GetItem(rChild).first, elem.first)) {
                        GetItem(index) = GetItem(rChild);
                        index = rChild;
                        ++row; ++col;
                    }
                    else break;
                }
                else
                    // left child is bigger or equal
                {
                    if (pred(GetItem(lChild).first, elem.first)) {
                        GetItem(index) = GetItem(lChild);
                        index = lChild;
                        ++row;
                    }
                    else break;
                }
            }
            else
                // only left child exists
            {
                if (pred(GetItem(lChild).first, elem.first)) {
                    GetItem(index) = GetItem(lChild);
                    index = lChild;
                    ++row;
                }
                else break;
            }
            lChild = index + row;
        }

        GetItem(index) = elem;
    }

    // Private constructor used only inside UpToDown factory.
    explicit _baseBeapT(const size_t initSize): TArrayBasedStructure<mPair, IsMemSafe1>(initSize+1) {
        // Ading Sentinel
        AddLast(std::make_pair(MostSignificantPrio, ItemT()));
    }

    [[nodiscard]] constexpr size_t _getPreLastRowLastElement() const {
        auto [lRow, lCol] = _getBeapPos(GetEndP() - 1);
        return lRow == lCol ? (GetEndP() - 1) : (GetEndP() - 1) - lCol;
    }

    static constexpr size_t _getRowPos(const size_t ind) {
        // row = (-1 + 8k) / 2, where k = ind and equation comes from second-grade polynomial root
        return  static_cast<size_t>(std::ceil(0.5 * (-1.0 + std::sqrt(1.0 + 8.0 * static_cast<double>(ind)))));
    }

    static constexpr size_t _getColPos(const size_t ind, const size_t rowPos) {
        return ind - (rowPos - 1)*rowPos/2;
    }

    static constexpr std::pair<size_t, size_t> _getBeapPos(const size_t ind) {
        size_t rPos = _getRowPos(ind);
        return std::make_pair(rPos, _getColPos(ind, rPos));
    }

    static constexpr size_t _getIndex(const size_t rPos, const size_t cPos) {
        const size_t elementsOnPreviousRows = (rPos - 1) * rPos / 2;
        return elementsOnPreviousRows + cPos;
    }

    // ------------------------------
    // private class Fields
    // ------------------------------

    PriorityFunction pred{};
    inline static unsigned int PrintSpaceDist = 3;
    inline static std::string SpacingString = std::string(PrintSpaceDist, '\n');
};

#endif //MINI_PROJECTS_BEAP_H