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

//Window size (1:1 ratio)
#define windowSize 960

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
	void Update();
	void MoveSnake();
	void SpawnAppleRandomly();
	void Display();
	void DrawApples();
	void Shutdown();

	//Other functions
	void CalculateFramerate();
	void AddApple();

private:
	
	//Snake linked list
	Snake snake;

	//Tick rate for simulation in seconds, default to 20hz
	float tickRate = .15;

	sf::Vector2f movementDirection;

	//Textures
	sf::Texture gridTexture;
	sf::Texture foregroundTexture;
	sf::Texture appleTexture;
	sf::Texture snakeBodyTexture;

	//Fonts
	sf::Font defaultFont;

	//Shapes
	sf::RectangleShape gridRect;
	sf::RectangleShape foregroundRect;
	sf::RectangleShape appleRect;
	sf::RectangleShape snakeBodyRect;

	//Text
	sf::Text fpsText;
};

