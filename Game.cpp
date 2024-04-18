#include "Game.h"
#include <iostream>
#include <random>
//Grid settings

//Grid size so windowSize / gridSize = N grids

const int gridNumber = 20;
const int spriteSize = gridSize;

//Apple collectables
const int maxApples = 5;
sf::Vector2f applePositions[maxApples];
int currentApples = 0;

float waterTimer;


//Movement
bool hasUpdatedMovement = true;
//Water level temp var

sf::RectangleShape waterLevelRect(sf::Vector2f(768, 768));
int waterTopBounds = 48 * 2;
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

	gridRect.setTexture(&gridTexture);
	foregroundRect.setTexture(&foregroundTexture);
	waterTankRect.setTexture(&waterTankTexture);
	appleRect.setTexture(&appleTexture);
	snakeBodyRect.setTexture(&snakeBodyTexture);

	waterLevelRect.setFillColor(sf::Color(0, 0, 255, 100));
	waterLevelRect.setPosition(96, 96);
	//Init text
	fpsText.setPosition(10, 0);
	fpsText.setFont(defaultFont);
	fpsText.setString("FPS Counter");

	//Reset the clocks
	tickClock.restart();
	waterClock.restart();


	//Create a new snake
	CreateSnake();

	//Fill out apple position array with blank elements, then add a valid element to start with
	for (int a = 0; a < 5; a++)
	{
		applePositions[a] = sf::Vector2f(0, 0);
	}
	AddApple();


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

	if (!waterTankTexture.loadFromFile("Resources/Sprites/Water.png"))
	{
		std::cerr << "Couldn't load file: Water.png\n";
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

	if (!snakeBodyTexture.loadFromFile("Resources/Sprites/SnakeBody.png"))
	{
		std::cerr << "Couldn't load file: SnakeBody.png\n";
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
	// Process SFML events
	sf::Event event;
	while (gameWindow->pollEvent(event))
	{
		switch (event.type)
		{
			// When event close received, call shutdown
		case sf::Event::Closed:
			Shutdown();
			break;
		case sf::Event::KeyPressed:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				ResetGameState();
			}
			if (hasUpdatedMovement)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::A:
					snake->ChangeDirection(sf::Vector2f(-1, 0)); // Move left
					break;
				case sf::Keyboard::D:
					snake->ChangeDirection(sf::Vector2f(1, 0)); // Move right
					break;
				case sf::Keyboard::W:
					snake->ChangeDirection(sf::Vector2f(0, -1)); // Move up
					break;
				case sf::Keyboard::S:
					snake->ChangeDirection(sf::Vector2f(0, 1)); // Move down
					break;
				}

				hasUpdatedMovement = false;
			}		
			break;
		}
	}
}

void Game::Update()
{
	CalculateFramerate();
	CalculateWaterTank();
	//Checks that enough time has passed
	if (tickClock.getElapsedTime().asSeconds() >= tickRate)
	{
		//If it has restart the clock and update the game
		tickClock.restart();
			if (snake != nullptr)
			{
				SpawnAppleRandomly();

				//Move the snake and update the movement
				if (!snake->Move(waterTopBounds))\
				{
					ResetGameState();
				}
				snake->CheckSteps();  
				hasUpdatedMovement = true;
				CheckAppleCollision(snake->snakeHead->position);
			}
		}
	else
	{
		//Return early since we have elapsed enough time
		return;
	}

}

void Game::Display()
{
	/*system("cls");
	int appleCount = 0;
	for (sf::Vector2f apple : applePositions) {
		if (apple != sf::Vector2f(0, 0)) {
			appleCount++;
		}
	}
	std::cout << "Apple count : " << appleCount;*/
	//Clears the game window with black
	gameWindow->clear(sf::Color::Black);

	//Draw the grid first, everything will be rendered on top of this.
	gameWindow->draw(gridRect);
	
	//Draw apples
	DrawApples();

	//Draw snake body
	DrawSnake();
	gameWindow->draw(waterLevelRect);
	//Draw the water tank
	gameWindow->draw(waterTankRect);
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
void Game::CalculateWaterTank()
{
	// Calculate water timer
	waterTimer = 0.f + waterClock.getElapsedTime().asSeconds();

	// Calculate percentage filled
	float percentageFilled = (waterTimer / 90.f);

	// Calculate width of filled area
	int xFill = (0 + (128 * percentageFilled));

	// Set new texture rectangle size
	waterTankRect.setTextureRect(sf::IntRect(xFill, 0, 160, 160));

	int waterLevelOffset = (768 * percentageFilled);
	if (waterLevelOffset % 48 == 0 && waterLevelOffset != 0)
	{
		waterLevelRect.setPosition(96, 96 + waterLevelOffset);
		waterTopBounds = waterLevelOffset + 48;
		for (int a = 0; a < maxApples; a++)
		{

			if (applePositions[a].y <= waterTopBounds - 48)
			{
				applePositions[a] = sf::Vector2f(0, 0);
			}
		}
	}


}
void Game::DrawSnake()
{
	if (snake == nullptr)
	{
		return;
	}
	//Start the search at the head
	SnakeNode* currentNode = snake->snakeHead;

	//While the node we are checking is not null continue to its next element
	while (currentNode != nullptr)
	{
		if (snake == nullptr)
		{
			return;
		}
		snakeBodyRect.setPosition(currentNode->position);

		//Catch division by zero with if statment 
		if(snake->movementStepsLeft > 0)
		{
			float colourMultiplier = ((float)snake->movementStepsLeft / snake->defaultMovementSteps);
			float colourValue = 255 * colourMultiplier;
			snakeBodyRect.setFillColor(sf::Color(0, colourValue, -colourValue, 255));
		}
		else
		{
			snakeBodyRect.setFillColor(sf::Color(0, 0, 255, 255));
		}

		gameWindow->draw(snakeBodyRect);
		currentNode = currentNode->nextElement;
	}
}

//Calucates the games framerate
void Game::CalculateFramerate()
{
	elapsedTime = fpsClock.restart();
	fps = 1 / elapsedTime.asSeconds();
	fpsText.setString("FPS: " + std::to_string(fps));
}

void Game::SpawnAppleRandomly()
{
	//Spawn a new apple every now and then based on rand number up until apple limit
	if (rand() % 10 == 1)
	{
		AddApple();
	}
}
void Game::AddApple()
{
	//If we have reached the apple limit dont try spawning a new one
	if (currentApples >= maxApples)
	{
		return;
	}

	// Loop through the applePositions array finding a free position if we cant find one return
	int freeAppleIndex = -1;
	for (int a = 0; a < 5; a++)
	{
		// Check if the current apple position is empty (0, 0)
		if (applePositions[a] == sf::Vector2f(0, 0))
		{	
			freeAppleIndex = a;
		}
	}
	if (freeAppleIndex == -1) {
		return;
	}

	// Initialize variables
	sf::Vector2f newApplePosition;
	bool positionTaken = false;
	int attempts = 0;
	// Generate a new random position until it's not taken
	do
	{
		if (attempts >= 50) { break; }
		attempts++;

		// Generate random X and Y coordinates for the new apple position, then calculate the apple position on the grid and ensure its below the the water line
		int appleX = RandomInt(3, 17);
		int appleY = RandomInt((waterTopBounds / 48) + 1, 17);
		newApplePosition = sf::Vector2f(appleX * gridSize, appleY * gridSize);

		// Check if the new position is already taken by another apple
		for (const sf::Vector2f& position : applePositions)
		{
			if (newApplePosition == position) {
				// Position is taken, set flag to true and exit loop
				positionTaken = true;
				break;
			}
		}
		//Check its not spawning on snake either, loop through
		SnakeNode* currentNode = snake->snakeHead;

		while (currentNode != nullptr)
		{
			if (newApplePosition == currentNode->position) {
				// Position is taken, set flag to true and exit loop
				positionTaken = true;
				break;
			}
			currentNode = currentNode->nextElement;

		}

		// If the position is not taken, assign it to the current index in applePositions
		if (!positionTaken)
		{
			applePositions[freeAppleIndex] = newApplePosition;
			currentApples++;
		}
	} while (positionTaken); // Repeat until a valid position is found
	return;
}
int Game::RandomInt(int min, int max)
{
	std::random_device rd;
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rd);
}
void Game::DrawApples()
{
	// Loop through the applePositions array
	for (int a = 0; a < 5; a++)
	{
		// Check if the current apple position is not empty (0, 0)
		if (applePositions[a] != sf::Vector2f(0, 0))
		{
			// Set the position of the apple rectangle
			appleRect.setPosition(applePositions[a]);

			// Draw the apple rectangle on the game window
			gameWindow->draw(appleRect);
		}
	}
}
void Game::CheckAppleCollision(sf::Vector2f& newHeadPosition)
{
	//Check if we have hit an apple, if so then add a snake body
	for (int a = 0; a < maxApples; a++)
	{
		if (applePositions[a] == newHeadPosition)
		{
			applePositions[a] = sf::Vector2f(0, 0);
			currentApples--;
			snake->AddSnakeBody();
			AddApple();
		}
	}
}
void Game::ResetGameState()
{
	// Reset water level
	waterTopBounds = 48 * 2;
	waterClock.restart();
	waterLevelRect.setPosition(96, 96);
	// Clear apple positions and reset currentApples count
	for (int a = 0; a < maxApples; a++) {
		applePositions[a] = sf::Vector2f(0, 0);
	}
	currentApples = 0;

	// Recreate the snake
	CreateSnake();
}

void Game::CreateSnake() {
	// Delete the old snake if it exists
	if (snake != nullptr) {
		delete snake;
		snake = nullptr;
	}

	// Generate new snake position
	int snakeX = 3 + rand() % 16; // Random number between 3 and 10
	int snakeY = 3 + rand() % 16; // Random number between 3 and 10

	// Create new snake and set its position
	snake = new Snake;
	SnakeNode* snode = new SnakeNode;
	snake->snakeHead->nextElement = snode;
	snake->snakeHead->position = sf::Vector2f(snakeX * gridSize, snakeY * gridSize);
	snake->ChangeDirection(sf::Vector2f(1, 0)); // Move right
}
