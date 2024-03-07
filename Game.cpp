#include "Game.h"
#include <iostream>


//Grid settings

//Grid size so windowSize / gridSize = N grids
#define gridSize 48
const int gridNumber = 20;
const int spriteSize = gridSize;
//Clocks

//Tick clock handles tick rate for simulation
sf::Clock tickClock;
//FPS clock handles time elapsed between each frame
sf::Clock fpsClock;
sf::Time elapsedTime;
int fps{ 0 };

//Apple collectables
const int maxApples = 5;
sf::Vector2f applePositions[maxApples];
int currentApples = 0;

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
	appleRect = sf::RectangleShape(sf::Vector2f(spriteSize, spriteSize));
	snakeBodyRect = sf::RectangleShape(sf::Vector2f(spriteSize, spriteSize));


	gridRect.setTexture(&gridTexture);
	foregroundRect.setTexture(&foregroundTexture);
	appleRect.setTexture(&appleTexture);
	snakeBodyRect.setTexture(&snakeBodyTexture);

	//Init text
	fpsText.setPosition(10, 0);
	fpsText.setFont(defaultFont);
	fpsText.setString("FPS Counter");

	//Reset the deltaClock
	tickClock.restart();

	//Add the snake to a valid position
	int snakeX = 3 + rand() % 8; // Random number between 3 and 10
	int snakeY = 3 + rand() % 8; // Random number between 3 and 10

	snake.head->position = sf::Vector2f(snakeX * gridSize, snakeY * gridSize);
	SnakeNode* node2 = new SnakeNode;

	snake.head->nextElement = node2;


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
			switch (event.key.code)
			{
				// Check for key presses to change the snake's direction
			case sf::Keyboard::A:
				if (movementDirection != sf::Vector2f(1, 0))
				{
					movementDirection = sf::Vector2f(-1, 0); // Move left
				}
				break;
			case sf::Keyboard::D:
				if (movementDirection != sf::Vector2f(-1, 0))
				{
					movementDirection = sf::Vector2f(1, 0); // Move right
				}
				break;
			case sf::Keyboard::W:
				if (movementDirection != sf::Vector2f(0, 1))
				{
					movementDirection = sf::Vector2f(0, -1); // Move up
				}
				break;
			case sf::Keyboard::S:
				if (movementDirection != sf::Vector2f(0, -1))
				{
					movementDirection = sf::Vector2f(0, 1); // Move down
				}
				break;
			}
			break;
		}
	}
}

void Game::Update()
{
	CalculateFramerate();
	
	//Checks that enough time has passed
	if (tickClock.getElapsedTime().asSeconds() >= tickRate)
	{
		//If it has restart the clock and update the game
		tickClock.restart();

		SpawnAppleRandomly();
		MoveSnake();

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
	
	//Draw apples
	DrawApples();

	//Draw snake body
	//Start the search at the head
	SnakeNode* currentNode = snake.head;

	//While the node we are checking is not null continue to its next element
	while (currentNode != nullptr)
	{
		snakeBodyRect.setPosition(currentNode->position);
		gameWindow->draw(snakeBodyRect);
		currentNode = currentNode->nextElement;
	}


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

void Game::SpawnAppleRandomly()
{
	//Spawn a new apple every now and then based on rand number up until apple limit
	if (rand() % 10 == 1 && currentApples < maxApples)
	{
		AddApple();
	}
}
void Game::AddApple()
{
	//If we have reached the apple limit dont try spawning a new one
	if (currentApples > maxApples)
	{
		return;
	}

	int freeAppleIndex = -1;
	// Loop through the applePositions array
	for (int a = 0; a < 5; a++)
	{
		// Check if the current apple position is empty (0, 0)
		if (applePositions[a] == sf::Vector2f(0, 0))
		{	
			freeAppleIndex = a;
		}
	}
	//Couldn't find free position return
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

		// Generate random X and Y coordinates for the new apple position
		int appleX = 3 + rand() % 8; // Random number between 3 and 10
		int appleY = 3 + rand() % 8; // Random number between 3 and 10

		// Calculate the new apple position based on the gridSize
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
		//Check its not spawning on snake either
		SnakeNode* currentNode = snake.head;

		//While the node we are checking is not null continue to its next element
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

void Game::AddSnakeBody()
{
	SnakeNode* nodeToAdd = new SnakeNode;
	
	SnakeNode* currentNode = snake.head;

	snake.GetElement(snake.Length() -1 )->nextElement = nodeToAdd;

}
void Game::MoveSnake()
{
	//Move the snake head to a new position check for collision with apples or walls
	sf::Vector2f newHeadPosition = snake.head->position + sf::Vector2f(movementDirection.x * gridSize,
		movementDirection.y * gridSize);

	snake.MoveSnake(newHeadPosition);

	for (int a = 0; a < maxApples; a++)
	{
		if (applePositions[a] == newHeadPosition)
		{
			applePositions[a] = sf::Vector2f(0, 0);
			currentApples--;
			AddSnakeBody();
			//AddApple();
		}
	}
}
