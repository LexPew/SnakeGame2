#pragma once

//Handles game structure and loop
//Init
//Handle Events
//Handle Realtime Inputs
//Handle Updating
//Handle Drawing
//Shutdown / Exit
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Snake.h"
#include <vector>

//Window size (1:1 ratio)
#define windowSize 960
#define gridSize 48

class Game
{
//Main game functions and variables
private:
	//Pointer to gameWindow
	sf::RenderWindow* gameWindow{ nullptr };

public:
	//Main game functions
	bool Initialize();
	void Loop();

private:
	//Resource functions
	bool LoadResources();

	//Loop functions
	void ProcessInput();
	void CreateSnakes();
	void Update();
	void CalculateWaterTank();
	void Display();
	void DrawSnake();
	void Shutdown();

	//Other functions
	void CalculateFramerate();
	void AddApple();
	int RandomInt(int min, int max);
	void SpawnAppleRandomly();
	void DrawApples();

	void CheckAppleCollision(sf::Vector2f& newHeadPosition, Snake* snake);

	void ResetGameState();

private:
	
	//Snake linked list vector
	std::vector<Snake*> snakes;
	const int startingSnakeCount{ 5 };

	//Clocks
	sf::Clock waterClock;
	sf::Clock tickClock; 
	sf::Clock fpsClock;
	sf::Time elapsedTime;
	int fps{ 0 };

	//Tick rate for simulation in seconds, default to 20hz
	float tickRate = .18;


	//Textures
	sf::Texture gridTexture;
	sf::Texture foregroundTexture;
	sf::Texture waterTankTexture;
	sf::Texture appleTexture;
	sf::Texture snakeBodyTexture;

	//Fonts
	sf::Font defaultFont;

	//Shapes
	sf::RectangleShape gridRect = sf::RectangleShape(sf::Vector2f(windowSize, windowSize));
	sf::RectangleShape foregroundRect = sf::RectangleShape(sf::Vector2f(windowSize, windowSize));
	sf::RectangleShape waterTankRect = sf::RectangleShape(sf::Vector2f(windowSize, windowSize));
	sf::RectangleShape appleRect = sf::RectangleShape(sf::Vector2f(gridSize, gridSize));
	sf::RectangleShape snakeBodyRect = sf::RectangleShape(sf::Vector2f(gridSize, gridSize));

	//Text
	sf::Text fpsText;
};

