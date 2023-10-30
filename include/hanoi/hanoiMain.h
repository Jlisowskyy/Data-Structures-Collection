// Author: Jakub Lisowski

#include "hanoi.h"
#include "timer.h"

// parameters
const size_t towerLevels = 10;
const bool displayTowers = true;

// configs
const bool timeRecu = true;
const bool timeNonRecu = false;
const bool timerPerfNonRecu = false;
const bool timeAll = false;

int HanoirMain() {

    if constexpr (timeRecu || timeAll){
        hanoi game(towerLevels);
        timer t;
        game.playGame<displayTowers>(true);
    }

    if constexpr (timeNonRecu || timeAll){
        hanoi game(towerLevels);
        timer t;
        game.playGame<displayTowers>(false);
    }

    if constexpr (timerPerfNonRecu || timeAll){
        hanoi game(towerLevels);
        timer t;
        game.playGamePerfNonRecuSol<displayTowers>();
    }

    return 0;
}