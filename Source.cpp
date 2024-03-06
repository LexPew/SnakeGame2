#include <iostream>
#include "Game.h"

int main()
{
    //Create a new game object named snakeGame
    Game snakeGame;

    //Initialize Random
    srand(time(NULL));
    //Then if it succesfully initializes then call the loop function
    if (snakeGame.Initialize()) {
        snakeGame.Loop();
    }
    std::cout << "SnakeGame: Finished" << std::endl;

    return 0;
}
