// Author: Jakub Lisowski

#ifndef PARALLEL_NUM_DEBUGGERS_H_
#define PARALLEL_NUM_DEBUGGERS_H_

#include <chrono>

class Timer {
// --------------------------------
// class creation/destruction
// --------------------------------
public:
    Timer(const char* YourName = nullptr, bool WriteOnDeath = true);
    ~Timer();

// ------------------------------
// class interaction
// ------------------------------

    void Start();
    long long Stop();
    void Stop_and_Start();
    void CalculateAverageTime(unsigned AmountOfTries, bool Verbose = false);
    void InvalidateLastRun();

// ------------------------------
// private methods
// ------------------------------

    void WriteMessage(long long SpentTime);
	void ShortenAverageToThisRun();

// ------------------------------
// private fields
// ------------------------------

    std::chrono::steady_clock::time_point begin;
    const char* TimerName;
    const bool WriteOnDeath;
    bool IsComputingAverage = false;
    bool VerboseAverage = false;
    unsigned RunsToIncludeInAverage = 0;
    unsigned RunsDone = 0;
    long long RunsSum = 0;
    long long LastRun = 0;
    const unsigned TimerID;
    static unsigned TimerCount;

};

#endif