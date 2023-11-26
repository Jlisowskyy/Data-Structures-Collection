//
// Created by Jlisowskyy on 11/26/23.
//

#ifndef ARRAYBASEDSTRUCTURE_H
#define ARRAYBASEDSTRUCTURE_H

#include <cstring>

class ArrayBasedStructure {
protected:
    ArrayBasedStructure(): LastItemPos{ 0 }, ActualSize{ InitalSize } {}

    [[nodiscard]] bool ShouldExpand() const{
        return LastItemPos == ActualSize;
    }

    size_t UpdateSize() {
        return ActualSize *= 2;
    }

    size_t LastItemPos;
    size_t ActualSize;
    static constexpr size_t InitalSize { 128 };
};

template<typename T, bool IsMemSafe>
class TArrayBasedStructure: public ArrayBasedStructure {
protected:
    TArrayBasedStructure() {
        Array = new T[InitalSize];
    }

    TArrayBasedStructure& RemoveLast() {
        if constexpr (IsMemSafe){
            if (LastItemPos == 0)
                return *this;
        }

        --LastItemPos;
        return *this;
    }

    T RemoveAndReturn() noexcept(false){
        if constexpr (IsMemSafe){
            if (LastItemPos == 0)
                throw std::runtime_error("Array is empty");
        }

        return Array[LastItemPos--];
    }

    TArrayBasedStructure& AddLast(const T& item) {
        if (ShouldExpand()) ExpandArray();

        Array[LastItemPos++] = item;
        return *this;
    }

    T& operator[](size_t ind) {
        return Array[ind];
    }

    const T& operator[](size_t ind) const {
        return Array[ind];
    }

    [[nodiscard]] size_t GetSize() const {
        return ActualSize;
    }

    [[nodiscard]] size_t GetLastElemPos() const {
        return LastItemPos;
    }

private:
    void ExpandArray() {
        const size_t oSize = ActualSize;
        T* NewArray = new T[UpdateSize()];

        mempcpy(NewArray, Array, oSize * sizeof(T));
        delete[] Array;
        Array = NewArray;
    }

    T* Array;
};

#endif //ARRAYBASEDSTRUCTURE_H