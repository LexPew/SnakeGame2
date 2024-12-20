#pragma once
#include "Snake.h"
class SnakePlayer2 : public Snake
{
public:
	SnakePlayer2(int p_TopWaterBounds, std::vector<Snake*>* p_SnakeList, char p_SnakeChar) : Snake(p_TopWaterBounds, p_SnakeList, p_SnakeChar){}
	void UpdateInput() override
	{

			//if (!hasUpdatedMovement) { return; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				ChangeDirection(sf::Vector2f(-1, 0)); // Move left
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				ChangeDirection(sf::Vector2f(1, 0)); // Move right
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				ChangeDirection(sf::Vector2f(0, -1)); // Move up
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				ChangeDirection(sf::Vector2f(0, 1)); // Move down
			}
	}
};

