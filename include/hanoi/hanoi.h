//
// Created by Jlisowskyy on 10/28/23.
// Author: Jakub Lisowski
//

#ifndef HANOI_HANOI_H
#define HANOI_HANOI_H

#include "stack.h"


// WARN: becomes unusable after game
class hanoi
{
public:
    // can be used to change data type used inside stacks
    using dtype = uint;

    explicit hanoi(size_t towerLevels): _towerLevels{ towerLevels }
    {
        for(auto & _tower : _towers){
            _tower = new stack<dtype>(towerLevels);
        }

        for(dtype i = towerLevels; i > 0; --i)
            _towers[0]->push(i);
    }

    ~hanoi(){
        for(int i = 0; i < towersCount; ++i){
            delete _towers[i];
        }
    }

    // display simple towers imitation ; does not align foundation to integer length
    void displayTowers(){
        std::tuple<const dtype*, size_t> towerArrays[towersCount];

        for (size_t i = 0; i < towersCount; ++i)
            towerArrays[i] = _towers[i]->getUnderlyingData();

        for(size_t i = _towerLevels; i > 0 ; --i){
            std::cout << "  ";

            for(size_t j = 0; j < towersCount - 1; ++j){
                if (std::get<1>(towerArrays[j]) >= i )
                    std::cout << std::get<0>(towerArrays[j])[i - 1];
                else
                    std::cout << '|';

                std::cout << "     ";
            }

            if (std::get<1>(towerArrays[2]) >= i )
                std::cout << std::get<0>(towerArrays[2])[i - 1];
            else
                std::cout << '|';

            std::cout << "     \n";
        }

        std::cout << "_____ _____ _____\n";
    }

    // true - recursive solution
    // false - non-recursive solution with go instruction
    template<bool showEveryMove = false>
    void playGame(bool isRecursive = true)
    {
        if constexpr (showEveryMove)
            displayTowers();

        if (isRecursive) hanoiRecu<showEveryMove>(_towerLevels, 0, 1, 2);
        else hanoiNonRecu<showEveryMove>();
    }

    template<bool showEveryMove = false>
    void playGamePerfNonRecuSol()
    {
        if constexpr (showEveryMove)
            displayTowers();

        hanoiPerfNonRecu<showEveryMove>();
    }

private:

    // the most popular recursive solution
    template<bool showEveryMove = false>
    void hanoiRecu(size_t elemsToMove, size_t srcTower, size_t tmpTower, size_t dstTower)
    {
        if (elemsToMove <= 0) return;
        hanoiRecu<showEveryMove>(elemsToMove - 1, srcTower, dstTower, tmpTower);

        auto elem = _towers[srcTower]->top();
        _towers[srcTower]->pop();
        _towers[dstTower]->push(elem);

        if constexpr (showEveryMove)
            displayTowers();

        hanoiRecu<showEveryMove>(elemsToMove - 1, tmpTower, srcTower, dstTower);
    }

    // more creative non-recursive solution, actually slower than recursive one
    template<bool showEveryMove = false>
    void hanoiNonRecu()
    {
        stack<size_t> argHolder(4 * _towerLevels);
        size_t elemsToMove = _towerLevels;
        size_t srcTower = 0;
        size_t tmpTower = 1;
        size_t dstTower = 2;

        static const auto pop4 = [&]() -> void
        {
            dstTower = argHolder.topNPop();
            tmpTower = argHolder.topNPop();
            srcTower = argHolder.topNPop();
            elemsToMove = argHolder.topNPop();
        };

        static const auto push4 = [&]() -> void
        {
            argHolder.push(elemsToMove);
            argHolder.push(srcTower);
            argHolder.push(tmpTower);
            argHolder.push(dstTower);
        };

        static const auto move = [&](size_t src, size_t dst) -> void
        {
            auto ret = _towers[src]->topNPop();
            _towers[dst]->push(ret);
        };

        while (elemsToMove > 0)
        {
            push4();
            elemsToMove--;
            std::swap(tmpTower, dstTower);
            continue;

adrRet:     pop4();
            move(srcTower, dstTower);
            if constexpr (showEveryMove)
                displayTowers();

            elemsToMove--;
            std::swap(tmpTower, srcTower);
        }

        if (!argHolder.isEmpty())
            goto adrRet;
    }

    // sightly upgraded hanoiNonRecu but still slower than recursive one
    template<bool showEveryMove = false>
    void hanoiPerfNonRecu()
    {
        stack<size_t> argHolder(4 * _towerLevels);
        size_t elemsToMove = _towerLevels;
        size_t srcTower = 0;
        size_t tmpTower = 1;
        size_t dstTower = 2;

        auto pop4 = [&]() -> void
        {
            dstTower = argHolder.topNPop();
            tmpTower = argHolder.topNPop();
            srcTower = argHolder.topNPop();
            elemsToMove = argHolder.topNPop();
        };

        auto push4 = [&]() -> void
        {
            argHolder.push(elemsToMove);
            argHolder.push(srcTower);
            argHolder.push(tmpTower);
            argHolder.push(dstTower);
        };

       auto move = [&](size_t src, size_t dst) -> void
        {
            auto ret = _towers[src]->topNPop();
            _towers[dst]->push(ret);
        };

        while (elemsToMove > 0)
        {
            push4();
            --elemsToMove;
            std::swap(tmpTower, dstTower);

            while(elemsToMove == 0 && !argHolder.isEmpty())
            {
                pop4();
                move(srcTower, dstTower);
                if constexpr (showEveryMove)
                    displayTowers();

                --elemsToMove;
                std::swap(tmpTower, srcTower);
            }
        }
    }

public:
    static const int towersCount = 3;
private:
    stack<dtype>* _towers[towersCount];
    size_t _towerLevels;
};

#endif //HANOI_HANOI_H