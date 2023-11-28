//
// Created by Jlisowskyy on 11/26/23.
//

#ifndef ARRAYBASEDSTRUCTURE_H
#define ARRAYBASEDSTRUCTURE_H

#include <cstring>

class ArrayBasedStructure {
protected:
    ArrayBasedStructure(): ElemCount{ InitalSize }, EndP{ 0 } {}
    ArrayBasedStructure(const size_t elemCount, const size_t lastItemPos):
        ElemCount{ elemCount }, EndP { lastItemPos } {}

    [[nodiscard]] bool ShouldExpand() const{
        return EndP == ElemCount;
    }

    size_t UpdateSize() {
        return ElemCount *= 2;
    }

    size_t ElemCount;
    size_t EndP;
    static constexpr size_t InitalSize { 128 };
};

template<typename T, bool IsMemSafe>
class TArrayBasedStructure: public ArrayBasedStructure {
protected:

    // Basic construction procedure. That is pointers are null valued and uses inital array length: 128.
    TArrayBasedStructure() {
        Array = new T[InitalSize];
    }

    // Configurable construction. Allocates base array with length of desiredCount and pastes mem into array,
    // beggining with cpyStartIndex. In this situation LastItemPos = cpyStartIndex + memCount.
    // Depending on chosen safty options performs checks or not.
    TArrayBasedStructure(const T* mem, const size_t memCount, const size_t desiredCount, const size_t cpyStartIndex) noexcept(false)
        :ArrayBasedStructure(desiredCount, cpyStartIndex + memCount){

        if constexpr (IsMemSafe) {
            if (desiredCount == 0 || memCount == 0) {
                throw std::runtime_error("[ ERROR ] Passed arguments to TArrayBasedStructure construction are not legal.");
            }

            if (EndP > desiredCount)
                throw std::runtime_error("[ ERROR ] Buffer overflow detected on TArrayBasedStructure construction.");

            if (mem == nullptr)
                throw std::runtime_error("[ ERROR ] Passed mem chunk to TArrayBasedStructure is null.");
        }

        Array = new T[desiredCount];
        memcpy(Array + cpyStartIndex, mem, memCount);
    }

    // Created fundational array with passed initial size.
    explicit TArrayBasedStructure(const size_t initSize): ArrayBasedStructure(initSize, 0) {
        Array = new T[initSize];
    }

    // Simplified version of above one. Just uses copy of passed memory as initializing array.
    TArrayBasedStructure(const T* mem, const size_t elemCount): TArrayBasedStructure(mem, elemCount, elemCount, 0) {}

    TArrayBasedStructure& PasteArrayInto(const T* const mem, const size_t elemCount) {
        if constexpr (IsMemSafe) {
            if (mem == nullptr)
                throw std::runtime_error("[ ERROR ] Passed mem chunk is null.");

            // TODO: temporary solution: do adaptive resizing
            if (const size_t neededSize = elemCount + EndP; neededSize > ElemCount) {
                const size_t coef = std::ceil(std::log2(neededSize / static_cast<double>(ElemCount)));

                _expandArray(ElemCount << coef);
            }
        }

        memcpy(Array + EndP, mem, elemCount * sizeof(T));
        EndP += elemCount;
        return *this;
    }

    TArrayBasedStructure& ExpandArray(const size_t newElemCount) {
        if constexpr (IsMemSafe) {
            if (newElemCount <= 0)
                throw std::runtime_error("[ ERROR ] Passed newElemCount to ExpandArray method is not positive.");

            if (newElemCount <= ElemCount) return *this;
        }

        _expandArray(newElemCount);
        return *this;
    }

    TArrayBasedStructure& RemoveLast() {
        if constexpr (IsMemSafe){
            if (EndP == 0)
                return *this;
        }

        --EndP;
        return *this;
    }

    T RemoveAndReturn(){
        if constexpr (IsMemSafe){
            if (EndP == 0)
                throw std::runtime_error("Array is empty");
        }

        return Array[EndP--];
    }

    TArrayBasedStructure& AddLast(const T& item) {
        if (ShouldExpand()) _expandArray();

        Array[EndP++] = item;
        return *this;
    }

    T& operator[](size_t ind) {
        return Array[ind];
    }

    const T& operator[](size_t ind) const {
        return Array[ind];
    }

    [[nodiscard]] size_t GetSize() const {
        return ElemCount;
    }

    [[nodiscard]] size_t GetEndP() const {
        return EndP;
    }

private:
    void _expandArray() {
        const size_t oSize = ElemCount;
        T* NewArray = new T[UpdateSize()];

        // TODO: find consensus here when no mem

        mempcpy(NewArray, Array, oSize * sizeof(T));
        delete[] Array;
        Array = NewArray;
    }

    void _expandArray(const size_t newElemCount) {
        ElemCount = newElemCount;
        const T* arr = Array;
        Array = new T[ElemCount];

        // TODO: find consensus here when no mem

        memcpy(Array, arr, ElemCount * sizeof(T));
        delete[] arr;
    }

    T* Array;
};

#endif //ARRAYBASEDSTRUCTURE_H