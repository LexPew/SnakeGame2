#include "Game.h"
#include <iostream>


//Grid settings

//Grid size so windowSize / gridSize = N grids
#define gridSize 50

int minRange = gridSize * 2;
int maxRange = windowSize - minRange;


//Clocks

//Tick clock handles tick rate for simulation
sf::Clock tickClock;
//FPS clock handles time elapsed between each frame
sf::Clock fpsClock;
sf::Time elapsedTime;
int fps{ 0 };





//Main Game Functions
bool Game::Initialize()
{
	//Try loading the resources if we cant return early otherwise try opening the game window
	if (!LoadResources())
	{
		return false;
	}
	//Try creating a new game window then assigning it to the gameWindow pointer if we cant return early and display error
	gameWindow = new sf::RenderWindow(sf::VideoMode(windowSize, windowSize), "Snake Game", sf::Style::Default);
	if (!gameWindow->isOpen())
	{
		std::cerr << "Couldn't initilize game, window state: " << gameWindow->isOpen() << "\n";
		return false;
	}

	//Game initialization
	gameWindow->setFramerateLimit(120);


	//Init shapes and textures
	gridRect = sf::RectangleShape(sf::Vector2f(windowSize, windowSize));
	foregroundRect = sf::RectangleShape(sf::Vector2f(windowSize, windowSize));
	appleRect = sf::RectangleShape(sf::Vector2f(gridSize, gridSize));

	gridRect.setTexture(&gridTexture);
	foregroundRect.setTexture(&foregroundTexture);
	appleRect.setTexture(&appleTexture);

	//Init text
	fpsText.setPosition(10, 0);
	fpsText.setFont(defaultFont);
	fpsText.setString("FPS Counter");

	//Reset the deltaClock
	tickClock.restart();


	std::cout << "Game succesfully initialized!\n";
	return true;



}

//Loads the game resources returning whether it was succesfull or not
bool Game::LoadResources()
{
	// Try loading the resource files
	if (!gridTexture.loadFromFile("Resources/Sprites/Grid.png"))
	{
		std::cerr << "Couldn't load file: Grid.png\n";
		return false;
	}

	if (!foregroundTexture.loadFromFile("Resources/Sprites/Foreground.png"))
	{
		std::cerr << "Couldn't load file: Foreground.png\n";
		return false;
	}

	if (!defaultFont.loadFromFile("Resources/Fonts/pixelated.ttf"))
	{
		std::cerr << "Couldn't load file: pixelated.ttf\n";
		return false;
	}

	if (!appleTexture.loadFromFile("Resources/Sprites/Apple.png"))
	{
		std::cerr << "Couldn't load file: Apple.png\n";
		return false;
	}

	std::cout << "Resources succesfully loaded!\n";
	return true;
}



//Handles the main game loop
void Game::Loop()
{
	//Whilst the game window is open then run the game loop
	while (gameWindow->isOpen())
	{
		ProcessInput();
		Update();
		Display();
	}

}




//Loop Functions
void Game::ProcessInput()
{
	//Process Sf Event
	sf::Event event;
	while (gameWindow->pollEvent(event))
	{
		switch (event.type)
		{
		//When event close recieved call shutdown
		case sf::Event::Closed:
			Shutdown();
				break;
		}
	}
}

void Game::Update()
{
	CalculateFramerate();
	
	//Checks that enough time has passed
	if (tickClock.getElapsedTime().asSeconds() < tickRate)
	{
		//If it has restart the clock and update the game
		tickClock.restart();
	}
	else
	{
		//Return early since we have elapsed enough time
		return;
	}

}


void Game::Display()
{
	//Clears the game window with black
	gameWindow->clear(sf::Color::Black);

	//Draw the grid first, everything will be rendered on top of this.
	gameWindow->draw(gridRect);

	//Draw the foreground last before UI, etc
	gameWindow->draw(foregroundRect);

	//Draw the fps text
	gameWindow->draw(fpsText);
	//Dislay all elements on the screen
	gameWindow->display();
}

void Game::Shutdown()
{
	//Handle shutdown properly
	gameWindow->close();
}

//Other functions

//Calucates the games framerate
void Game::CalculateFramerate()
{
	elapsedTime = fpsClock.restart();
	fps = 1 / elapsedTime.asSeconds();
	fpsText.setString("FPS: " + std::to_string(fps));
}

