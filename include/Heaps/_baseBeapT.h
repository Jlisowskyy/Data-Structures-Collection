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

        // TODO: CREATION PROCEDURE
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

    static _baseBeapT BeapUpToDownFactory(const mPair* const items, const size_t size) {
        _baseBeapT ret{size};

        // TODO: Beap creation procedure

        return ret;
    }

    static void ChangePrintLevelSpacing(const unsigned int newSize) {
        PrintSpaceDist = newSize;
        SpacingString = std::string(newSize, '\n');
    }

    // ------------------------------
    // class interaction
    // ------------------------------


//
//     [[nodiscard]] size_t MemSize() const {
//         return GetElemCount();
//     }
//
//     [[nodiscard]] size_t ElementsCount() const {
//         return GetEndP()-1;
//     }
//
//     [[nodiscard]] size_t GetLastIndex() const
//         // offset ready for HeapIndexClass
//     {
//         return GetEndP() - 2;
//     }
//
//     _baseBeapT& Insert(const mPair& pair) {
//         _insert(pair);
//         return *this;
//     }
//
//     [[nodiscard]] const mPair& Max() const
//         // when heap is empty behaviour is undefined
//     {
//         return GetItem(1);
//     }
//
//     _baseBeapT& DeleteMax(mPair& out)
//         // when heap is empty behaviour is undefined
//     {
//         out = GetItem(1);
//
//         _deleteMax();
//         return *this;
//     }
//
//     _baseBeapT& DeleteMax()
//         // when heap is empty behaviour is undefined
//     {
//         _deleteMax();
//         return *this;
//     }
//
//     [[nodiscard]] bool IsEmpty() const{
//         return GetEndP() == 1;
//     }
//
//     [[nodiscard]] HeapIndex Search(PrioT prio)
//         // when heap is empty behaviour is undefined
//         // Return: index of found element. Output can be invalid
//     {
//         HeapIndex ret{};
//         ret.index = _search(prio, 1);
//         return ret;
//     }
//
//     _baseBeapT& Delete(const HeapIndex index, mPair& out)
//         // when heap is empty or index is out of range behaviour is undefined
//     {
//         const size_t i = index;
//         out = GetItem(i);
//         _delete(i);
//
//         return *this;
//     }
//
//     _baseBeapT& Delete(const HeapIndex index)
//     // when heap is empty or index is out of range behaviour is undefined
//     {
//         const size_t i = index;
//         _delete(i);
//         return *this;
//     }
//
//     _baseBeapT& Replace(const HeapIndex ind, const mPair& newItem, mPair& oItem)
//         // when heap is empty or index is out of range behaviour is undefined
//     {
//         const size_t i = ind;
//         oItem = GetItem(i);
//         _replace(i, newItem);
//         return *this;
//     }
//
//     _baseBeapT& Replace(const HeapIndex ind, const mPair& newItem)
//     // when heap is empty or index is out of range behaviour is undefined
//     {
//         const size_t i = ind;
//         _replace(i, newItem);
//         return *this;
//     }
//
//     const mPair& operator[](const HeapIndex ind) const
//         // when heap is empty or index is out of range behaviour is undefined
//     {
//         return GetItem(ind);
//     }
//
//     // ------------------------------
//     // Printing methods
//     // ------------------------------
// private:
//     [[nodiscard]] size_t _findMaxPrint() const{
//         if (IsEmpty()) return 0;
//         std::ostringstream str{};
//
//         str << std::get<0>(GetItem(1));
//         size_t max = str.str().length();
//         str = std::ostringstream{};
//
//         for (size_t i = 2; i < GetEndP(); ++i) {
//             str << std::get<0>(GetItem(i));
//             if (str.str().length() > max) {
//                 max = str.str().length();
//             }
//
//             str = std::ostringstream{};
//         }
//
//         return max;
//     }
//
// public:
//     friend std::ostream& operator <<(std::ostream& out, const _baseBeapT& hp) {
//         return hp._print(out, hp);
//     }

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
    void _upHeap(size_t index) {
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
                break;
            }

            if (col == 1)
                // We are at the first element of row, so there is only right parent possible.
                // We are sure that we are not inside root because this is always processed in previous if statement.
            {
                // auto [rCol, rRow] = std::make_pair(row - 1, fCord);
                if (size_t pCord = index - row + 1; pred(elem.first, GetItem(pCord).first) ){
                    GetItem(index) = GetItem(pCord);
                    index = pCord;
                    --row;
                }
                break;
            }


            // We are not at the first element neither the last.
            auto [lCord, rCord] = std::make_pair(index - row, index - row + 1);
            if (pred(GetItem(lCord).first, GetItem(rCord).first ))

        }
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