//
// Created by Jlisowskyy on 10/28/23.
// Author: Jakub Lisowski
//

#ifndef HANOI_STACK_H
#define HANOI_STACK_H

#include <cstdio>
#include <exception>
#include <iostream>
#include <tuple>

template<class numT>
class stack{
public:
    stack(): _arr{ nullptr }, _arrSize{ 0 }, _used{ 0 } {}
    explicit stack(size_t maxSize): _arr { new numT[maxSize] }, _arrSize{ maxSize }, _used{ 0 } {}
    ~stack() { delete[] _arr; }

    void push(numT x){
        if (isFull()) [[unlikely]]
            throw std::out_of_range("[ ERROR ] Stack overflowed\n");

        _arr[_used++] = x;
    }

    void pop(){
        if (isEmpty())[[unlikely]] return;

        --_used;
    }

    numT top(){
        if (isEmpty())[[unlikely]]
            throw std::out_of_range("[ ERROR ] Stack is empty\n");

        return _arr[_used - 1];
    }

    numT topNPop(){
        auto ret = top();
        pop();

        return ret;
    }

    [[nodiscard]] bool isEmpty() const{
        return _used == 0;
    }

    [[nodiscard]] bool isFull() const {
        return  _used == _arrSize;
    }

    std::tuple<const numT*, size_t> getUnderlyingData(){
        return std::tuple(_arr, _used);
    }

private:
    numT* _arr;
    size_t _arrSize;
    size_t _used;
};

#endif //HANOI_STACK_H
