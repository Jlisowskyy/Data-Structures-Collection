//
// Created by Jlisowskyy on 11/30/23.
//

#ifndef _HEAPSAPI_H
#define _HEAPSAPI_H

// TODO: make API

// #include <cstdlib>
// #include <tuple>
// #include <cmath>
//
// #include "HeapHelpers.h"
//
// static constexpr bool IsMemSafe1 = false;
//
// template<template<typename PrioT, typename, typename , PrioT MostSignificantPrio> class _queueFundation>
// class PriorityQueue: _queueFundation {
//     // ------------------------------
//     // Type creation/copying
//     // ------------------------------
// public:
//     using mPair = std::pair<PrioT, ItemT>;
// private:
//     using base = TArrayBasedStructure<mPair, IsMemSafe1>;
//     using base::GetElemCount;
//     using base::GetEndP;
//     using base::AddLast;
//     using base::RemoveLast;
//     using base::RemoveAndReturn;
//     using base::GetItem;
// public:
//     _baseBeapT(): TArrayBasedStructure<mPair, IsMemSafe1>() {
//         // Ading Sentinel
//         AddLast(std::make_pair(MostSignificantPrio, ItemT{}));
//     }
//
//     _baseBeapT(const mPair* const items, const size_t size):
//         TArrayBasedStructure<mPair, IsMemSafe1>(items, size, size+1, 1){
//
//         // Adding Sentinel
//         GetItem(0) = std::make_pair(MostSignificantPrio, ItemT{});
//
//         // TODO: CREATION PROCEDURE
//     }
//
//     _baseBeapT(const _baseBeapT& other): TArrayBasedStructure<mPair, IsMemSafe1>(other) {}
//     _baseBeapT(_baseBeapT&& other) noexcept(true): TArrayBasedStructure<mPair, IsMemSafe1>(std::move(other)){}
//
//     _baseBeapT& operator=(_baseBeapT&& other) noexcept(true) {
//         base::operator=(std::move(other));
//         return *this;
//     }
//
//     _baseBeapT& operator=(const _baseBeapT& other) {
//         base::operator=(other);
//         return *this;
//     }
//
//     ~_baseBeapT() = default;
//
//     // ------------------------------
//     // Class static methods
//     // ------------------------------
//
//     static _baseBeapT BeapUpToDownFactory(const mPair* const items, const size_t size) {
//         _baseBeapT ret{size};
//         ret._createHeapUpToDown(items, size);
//         return ret;
//     }
//
//     static void ChangePrintLevelSpacing(const unsigned int newSize) {
//         PrintSpaceDist = newSize;
//         SpacingString = std::string(newSize, '\n');
//     }
//
//         // ------------------------------
//     // class interaction
//     // ------------------------------
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
//
//     // -------------------------------
//     // implementation-components
//     // -------------------------------
// private:
//
//
//     // ------------------------------
//     // private class Fields
//     // ------------------------------
//
//     PriorityFunction pred{};
//     inline static unsigned int PrintSpaceDist = 3;
//     inline static std::string SpacingString = std::string(PrintSpaceDist, '\n');
// };

#endif //_HEAPSAPI_H