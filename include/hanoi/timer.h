//
// Created by Jlisowskyy on 10/28/23.
// Author: Jakub Lisowski
//

#ifndef HANOI_TIMER_H
#define HANOI_TIMER_H

#include <chrono>
#include <iostream>

class timer{
public:
    timer(): start{ std::chrono::steady_clock::now() } {}
    ~timer()
    {
        if (!wasWritten) Stop();
    }

    void Stop()
    {
        auto st = std::chrono::steady_clock::now();

        std::cout << "Timer spent: " << (double)(st.time_since_epoch() - start.time_since_epoch()).count() * 1e-6 << std::endl;
        wasWritten = true;
    }

private:
    bool wasWritten = false;
    std::chrono::time_point<std::chrono::steady_clock> start;
};

#endif //HANOI_TIMER_H
