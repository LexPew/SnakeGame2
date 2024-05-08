#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Menu.h"

int main()
{

    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Water Snakes", sf::Style::Default);
    if (!window.isOpen()) {
        std::cerr << "Couldn't initialize game, window state: " << window.isOpen() << "\n";
        return 0;
    }

    bool returnToMenu = true;
    while (returnToMenu) {
        Menu menu(&window);
        menu.Display();

        if (menu.GetSelectedOption() == 0)
        {
            Game snakeGame(&window);
            if (snakeGame.Initialize())
            {
                snakeGame.Loop();
                returnToMenu = true;
            }
        }
        else 
        {
            returnToMenu = false;
        }
    }
    window.close();

    return 0;
}
