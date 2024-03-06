#pragma once
#include "SFML/graphics.hpp"
class SnakeNode
{
public:
	//Snakes location
	sf::Vector2f position = sf::Vector2f(0,0);
	//Holds the snakes last position
	sf::Vector2f lastPosition = position;
	//Next element in the linked list
	SnakeNode* nextElement = nullptr;

	void Move(sf::Vector2f& newPosition)
	{
		lastPosition = position;
		position = newPosition;
	}
};

