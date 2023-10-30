
int HanoirMain() {
// parameters
const size_t towerLevels = 20;
const bool displayTowers = false;

// configs
const bool timeRecu = false; // standardowe rozwiazanie rekurencyjne
const bool timeNonRecu = false; // rozwiazanie nie rekurencyjne z zajec
const bool timerMyNonRecu = false; // poprawione rozwiazanie nie rekurencyjne
const bool timeAll = true;


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

    if constexpr (timerMyNonRecu || timeAll){
        hanoi game(towerLevels);
        timer t;
        game.playGameMySol<displayTowers>();
    }

}