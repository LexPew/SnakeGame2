#pragma once

//Handles game structure and loop
//Init
//Handle Events
//Handle Realtime Inputs
//Handle Updating
//Handle Drawing
//Shutdown / Exit
#include <SFML/Graphics.hpp>

//Window size (1:1 ratio)
#define windowSize 1000

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
	void Display();
	void Shutdown();

	//Other functions
	void CalculateFramerate();

private:
	
	//Tick rate for simulation in seconds, default to 20hz
	float tickRate = 0.05;

	sf::Vector2i movementDirection;

	//Textures
	sf::Texture gridTexture;
	sf::Texture foregroundTexture;
	sf::Texture appleTexture;

	//Fonts
	sf::Font defaultFont;

	//Shapes
	sf::RectangleShape gridRect;
	sf::RectangleShape foregroundRect;
	sf::RectangleShape appleRect;

	//Text
	sf::Text fpsText;
};

