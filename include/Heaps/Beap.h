//
// Created by Jlisowskyy on 11/8/23.
//

#ifndef MINI_PROJECTS_BEAP_H
#define MINI_PROJECTS_BEAP_H

#include <cstdlib>
#include <tuple>
#include <cmath>

template<typename T>
class Beap
{
public:
    Beap(size_t initSize = 0)
    {
        size_t i = k2i(initSize);
        _arrSize = i * (i - 1) / 2 + 1;
        _arr = (std::tuple<size_t, T>*) malloc(sizeof(std::tuple<size_t, T>) * _arrSize);
        _hL = 0;
    }

//    Beap()

    void insert(size_t priority, T* value)
    {

    }

    const T& Max()
    {

    }

    T* deleteMax()
    {

    }



private:

    inline size_t ij2k(size_t i, size_t j)
    {
        return i * (i - 1)  / 2 + j;
    }

    inline size_t k2i(size_t k)
    {
        return ceil((1 + sqrt(1 + 8 * k)) / 2);
    }

    inline std::tuple<size_t, size_t> k2ij(size_t k)
    {
        size_t i = k2i(k);
        size_t j = k - i * (i - 1) / 2;

        return std::make_tuple(i, j);
    }

    std::tuple<size_t, T> _arr;
    size_t _arrSize;
    size_t _hL;
};

#endif //MINI_PROJECTS_BEAP_H
