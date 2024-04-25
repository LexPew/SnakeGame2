#include <iostream>
#include "Game.h"

int main()
{
    //Initialize Random
    srand(clock());
    //Create new window
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(windowSize, windowSize), "Snake Game", sf::Style::Default);
    if (!window->isOpen())
    {
        std::cerr << "Couldn't initilize game, window state: " << window->isOpen() << "\n";
        return 0;
    }

    //Create a new game object named snakeGame
    Game snakeGame(window);

    if (snakeGame.Initialize()) {
        snakeGame.Loop();
    }
    std::cout << "SnakeGame: Finished" << std::endl;

    return 0;
}
