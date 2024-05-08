#include "Menu.h"

Menu::Menu(sf::RenderWindow* window) : window(window), selectedOption(0) {
    if (!font.loadFromFile("Resources/Fonts/pixelated.ttf")) {
        //TODO: Handle font loading error
    }

    if (!backgroundTexture.loadFromFile("Resources/Sprites/GameboyMainScreen.png")) {
        //TODO: Handle texture loading error
    }

    background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
    background.setTexture(&backgroundTexture);
    titleText.setFont(font);
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::White);
    titleText.setString("--Water Snakes--");

    startText.setFont(font);
    startText.setCharacterSize(60);
    startText.setFillColor(sf::Color::White);
    startText.setString("--Start Game--");

    exitText.setFont(font);
    exitText.setCharacterSize(60);
    exitText.setFillColor(sf::Color::White);
    exitText.setString("--Exit--");

    // Create a layout group for menu items

    menuLayout = new LayoutGroup((window->getSize().x/2 - titleText.getLocalBounds().width / 2), window->getSize().y / 5, 50, true, window);
    menuLayout->AddText(&titleText);
    menuLayout->AddText(&startText);
    menuLayout->AddText(&exitText);
}

Menu::~Menu() {
    delete menuLayout;
}

void Menu::Display() {
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Down) {
                    selectedOption = (selectedOption + 1) % 2;

                    menuLayout->UpdateLayoutGroup();
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    selectedOption = (selectedOption - 1 + 2) % 2;

                    menuLayout->UpdateLayoutGroup();
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    return;
                }
            }
        }

        window->clear(sf::Color::Black);
        window->draw(background);
        menuLayout->Draw();
        // Highlight the selected option
        if (selectedOption == 0) {
            startText.setFillColor(sf::Color::Red);
            exitText.setFillColor(sf::Color::White);
        }
        else {
            startText.setFillColor(sf::Color::White);
            exitText.setFillColor(sf::Color::Red);
        }
        window->display();
    }
}

int Menu::GetSelectedOption() {
    return selectedOption;
}
