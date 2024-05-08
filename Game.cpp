#include "Game.h"
#include <iostream>
#include <JMathLib/JMath.h>
#include "SnakePlayer2.h"
#include "HorizontalLayoutGroup.h"
#include "Apple.h"
//Grid settings

//Grid size so windowSize / gridSize = N grids

const int gridNumber = 20;
const int spriteSize = gridSize;

//Apple collectables
const int maxApples = 5;
std::vector<Apple> apples;

float waterTimer;

//Water level temp var

sf::RectangleShape waterLevelRect(sf::Vector2f(768, 768));
int waterTopBounds = 2 * gridSize;


// Test h layout group
HorizontalLayoutGroup* hLayoutGroup;
std::vector<sf::Text*> snakeScoreTexts;
//Main Game Functions
bool Game::Initialize()
{

	//Try loading the resources if we cant return early otherwise try opening the game window
	if (!LoadResources())
	{
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


	Countdown();



	//Reset the clocks
	tickClock.restart();
	waterClock.restart();


	//Create a new snake
	SetupSnakes();
	//Fill out apple position array with blank elements, then add a valid element to start with
	AddApple();

	//Init Snake Scores
	hLayoutGroup = new HorizontalLayoutGroup(96, 912, 200, false, gameWindow);

	for(Snake* snake : snakes)
	{
		sf::Text* newSnakeText = new sf::Text;
		newSnakeText->setFont(defaultFont);
		newSnakeText->setString("Snake " + std::to_string(snake->GetSnakeChar()) + " : " + std::to_string(snake->GetScore()));
		newSnakeText->setFillColor(snake->GetSnakeColour());
		newSnakeText->setOutlineColor(sf::Color::Black);
		newSnakeText->setOutlineThickness(2);
		snakeScoreTexts.push_back(newSnakeText);
		hLayoutGroup->AddText(newSnakeText);
	}

	std::cout << "Game succesfully initialized!\n";
	return true;



}

void Game::Countdown()
{
	sf::Text countdownText;
	countdownText.setFont(defaultFont);
	countdownText.setCharacterSize(48);
	countdownText.setFillColor(sf::Color::White);

	// Countdown before starting the game
	sf::Clock countdownClock;
	for (int i = 3; i >= 1; --i)
	{
		while (countdownClock.getElapsedTime().asSeconds() < 1.f)
		{
			// Update countdown text
			countdownText.setString(std::to_string(i));
			countdownText.setPosition(gameWindow->getSize().x / 2 - countdownText.getLocalBounds().width / 2, gameWindow->getSize().y / 2 - countdownText.getLocalBounds().height / 2);

			gameWindow->clear();
			gameWindow->draw(countdownText);
			gameWindow->display();
		}
		countdownClock.restart();
	}

	// Show "Go!" message
	countdownText.setString("Go!");
	countdownText.setPosition(gameWindow->getSize().x / 2 - countdownText.getLocalBounds().width / 2, gameWindow->getSize().y / 2 - countdownText.getLocalBounds().height / 2);

	gameWindow->clear();
	gameWindow->draw(countdownText);
	gameWindow->display();
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
	// Set isRunning flag to true initially
	isRunning = true;

	// While the game window is open and the game is running
	while (gameWindow->isOpen() && isRunning)
	{
		ProcessInput();
		if (!isPaused)
		{
			Update();
			Display();
		}
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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			{
				isPaused = !isPaused;
			}
			break;
		}
		for (Snake* snake : snakes)
		{
			snake->UpdateInput();
		}

	}

}

void Game::Update()
{
	if (!isRunning) {
		return;
	}
	CalculateFramerate();
	CalculateWaterTank();

	// Update the score texts
	for (int i = 0; i < snakeScoreTexts.size(); i++)
	{
		snakeScoreTexts[i]->setString("Snake " + std::to_string(snakes[i]->GetSnakeChar()) + " : " + std::to_string(snakes[i]->GetScore()));
		hLayoutGroup->UpdateLayoutGroup();
	}

	// Checks that enough time has passed
	if (tickClock.getElapsedTime().asSeconds() >= tickRate)
	{
		tickClock.restart();
		bool isAnyAlive = false;
		int snakesAlive = 0;
		// Find the snake with the highest score
		int highestScore = -1;
		Snake* winner = nullptr;

		for (Snake* snake : snakes)
		{
			if (snake->IsAlive())
			{
				snakesAlive++;
				isAnyAlive = true;

			}
			if (snake->GetScore() > highestScore)
			{
				highestScore = snake->GetScore();
				winner = snake;
			}
		}

		if (!isAnyAlive)
		{
			// If no snake is alive, end the game and declare the winner
			EndGame(winner);
		}
		else if (snakesAlive == 1)
		{
			for (Snake* snake : snakes)
			{
				if (snake->IsAlive()) {
					EndGame(snake);
				}

			}

		}
		else
		{
			// Continue the game
			SpawnAppleRandomly();
			for (Snake* snake : snakes)
			{

				if (snake->IsAlive())
				{
					snake->UpdateFlags();
					snake->Update();
				}

			}

			CheckAppleCollision();
		}
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
	DrawSnake();
	gameWindow->draw(waterLevelRect);
	//Draw the water tank
	gameWindow->draw(waterTankRect);
	//Draw the foreground last before UI, etc
	gameWindow->draw(foregroundRect);

	//Draw the fps text
	gameWindow->draw(fpsText);
	hLayoutGroup->Draw();
	//Dislay all elements on the screen
	gameWindow->display();
}

void Game::Shutdown()
{
	isRunning = false;
	snakeScoreTexts.clear();
	snakes.clear();

	delete hLayoutGroup;
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
		for (Snake* snake : snakes)
		{
			snake->UpdateWaterBounds(waterTopBounds);
		}


		//Remove any apples outside of the new bounds
		for (int a = 0; a < apples.size(); a++)
		{

			if (apples[a].position.y <= waterTopBounds - 48)
			{
				apples[a].position = sf::Vector2f(0, 0);
			}
		}
	}
}
void Game::CalculateFramerate()
{
	elapsedTime = fpsClock.restart();
	fps = 1 / elapsedTime.asSeconds();
	fpsText.setString("FPS: " + std::to_string(fps));
}

void Game::ResetGameState()
{

	Countdown();
	// Reset water level
	waterTopBounds = gridSize * 2;
	waterClock.restart();
	waterLevelRect.setPosition(96, 96);
	apples.clear();

	// Recreate the snake
	SetupSnakes();
}

#pragma region Snake Functions
void Game::SetupSnakes()
{
	for (int snakeIndex = 0; snakeIndex < snakeCount; snakeIndex++)
	{
		Snake* tempSnake;
		if (snakeIndex == 0)
		{
			tempSnake = new Snake(waterTopBounds, &snakes, snakeIndex);
		}
		else if (snakeIndex == 1)
		{
			tempSnake = new SnakePlayer2(waterTopBounds,&snakes, snakeIndex);
		}
		else
		{
			//Snake Ai
			tempSnake = new Snake(waterTopBounds, &snakes, snakeIndex);
		}
		// Generate new snake position
		int snakeX = JMath::RandomInt(5, 15); // Random number between 3 and 10
		int snakeY = JMath::RandomInt(5, 15); // Random number between 3 and 10

		// Create new snake and set its position
		//snake = new Snake(waterTopBounds);

		SnakeNode* snode = new SnakeNode;
		tempSnake->GetHead()->nextElement = snode;
		tempSnake->GetHead()->position = sf::Vector2f(snakeX * gridSize, snakeY * gridSize);
		tempSnake->ChangeDirection(sf::Vector2f(1, 0)); // Move right
		snakes.push_back(tempSnake);
	}

	
}
void Game::DrawSnake()
{
	if (snakes.empty())
	{
		return;
	}
	//Loop through each snake drawing them
	for(Snake* snake : snakes)
	{
		if (snake->IsAlive())
		{
			//Start the search at the head
			SnakeNode* currentNode = snake->GetHead();

			//While the node we are checking is not null continue to its next element
			while (currentNode != nullptr)
			{
				if (snake == nullptr)
				{
					return;
				}
				snakeBodyRect.setPosition(currentNode->position);

				//Catch division by zero with if statment 
				if (snake->GetMovementStepsLeft() > 0)
				{
					float colourMultiplier = (static_cast<float>(snake->GetMovementStepsLeft()) / snake->GetDefaultMovementSteps());
					float colourValue = 255 * colourMultiplier;
					snakeBodyRect.setFillColor(sf::Color(snake->GetSnakeColour().r * colourMultiplier, snake->GetSnakeColour().g * colourMultiplier, -colourValue, 255));
				}
				else
				{
					snakeBodyRect.setFillColor(sf::Color(0, 0, 255, 255));
				}

				gameWindow->draw(snakeBodyRect);
				currentNode = currentNode->nextElement;
			}
		}
		
	
	}
}
#pragma endregion 
#pragma region Apple Functions
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
	if (apples.size() >= maxApples)
	{
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
		int appleX = JMath::RandomInt(3, 17);
		int appleY = JMath::RandomInt((waterTopBounds / 48) + 1, 17);
		newApplePosition = sf::Vector2f(appleX * gridSize, appleY * gridSize);

		// Check if the new position is already taken by another apple
		for (Apple& apple : apples)
		{
			if (newApplePosition == apple.position) {
				// Position is taken, set flag to true and exit loop
				positionTaken = true;
				break;
			}
		}
		//Check its not spawning on snake either, loop through
		for (Snake* snake : snakes) 
		{
			SnakeNode* currentNode = snake->GetHead();

			while (currentNode != nullptr)
			{
				if (newApplePosition == currentNode->position) {
					// Position is taken, set flag to true and exit loop
					positionTaken = true;
					break;
				}
				currentNode = currentNode->nextElement;

			}
		}
		

		// If the position is not taken, assign it to the current index in applePositions
		if (!positionTaken)
		{
			apples.push_back(Apple(newApplePosition));
		}
	} while (positionTaken); // Repeat until a valid position is found
	return;
}
void Game::DrawApples()
{
	if (apples.size() > 0)
	{
		// Loop through the applePositions array
		for(Apple& apple : apples)
		{

			// Set the position of the apple rectangle
			appleRect.setPosition(apple.position);
			appleRect.setFillColor(apple.appleColour);
			// Draw the apple rectangle on the game window
			gameWindow->draw(appleRect);
		}
	}

}
void Game::CheckAppleCollision()
{
	for(Snake* snake1 : snakes)
	//Check if we have hit an apple, if so then add a snake body
	for (int a = 0; a < apples.size(); a++)
	{
		if (snake1->GetHead()->position == apples[a].position)
		{
			snake1->AddScore(apples[a].collectablePoints);

			for (int additionCount = 0; additionCount < apples[a].tailGrowth; additionCount++)
			{
				snake1->AddSnakeBody();
			}

			apples.erase(apples.begin() + a);


			AddApple(); 

		}
	}
}

#pragma endregion
void Game::EndGame(Snake* winner)
{
	// Clear the game window
	gameWindow->clear(sf::Color::Black);

	// Display winner's information
	sf::Text winnerText;
	winnerText.setFont(defaultFont);
	winnerText.setCharacterSize(48);
	winnerText.setFillColor(sf::Color::White);
	winnerText.setString("Winner Snake: " + std::to_string(winner->GetSnakeChar()));
	winnerText.setPosition(gameWindow->getSize().x / 2 - winnerText.getLocalBounds().width / 2, gameWindow->getSize().y / 2 - winnerText.getLocalBounds().height / 2);

	sf::Text scoreText;
	scoreText.setFont(defaultFont);
	scoreText.setCharacterSize(36);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setString("Score: " + std::to_string(winner->GetScore()));
	scoreText.setPosition(gameWindow->getSize().x / 2 - scoreText.getLocalBounds().width / 2, winnerText.getPosition().y + 100);

	gameWindow->draw(winnerText);
	gameWindow->draw(scoreText);
	gameWindow->display();

	// Wait for a moment before resetting the game
	sf::sleep(sf::seconds(2));

	//ResetGameState();
	Shutdown();
}
