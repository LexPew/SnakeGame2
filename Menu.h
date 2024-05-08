#pragma once
#include <SFML/Graphics.hpp>
#include "LayoutGroup.h"
class Menu {
public:
    Menu(sf::RenderWindow* window);
    ~Menu();

    void Display();
    int GetSelectedOption();

private:
    sf::RenderWindow* window;
    LayoutGroup* menuLayout;
    sf::Texture backgroundTexture;
    sf::RectangleShape background;
    sf::Font font;
    sf::Text titleText;
    sf::Text startText;
    sf::Text exitText;
    int selectedOption;
};
