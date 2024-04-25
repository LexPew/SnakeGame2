#include "Snake.h"

#define gridSize 48


void Snake::Move()
{
	//Calculate head position
	sf::Vector2f newHeadPosition = snakeHead->position + sf::Vector2f(movementDirection.x * gridSize, movementDirection.y * gridSize);
	UpdateSnakePosition(newHeadPosition);
	isAlive = CheckCollisionBounds() && CheckCollisionSelf();
	CheckSteps();

}

void Snake::UpdateSnakePosition(sf::Vector2f& newHeadPosition)
{
	//Move the snake linked list positions
	SnakeNode* currentNode = snakeHead;
	currentNode->Move(newHeadPosition);
	while (currentNode->nextElement != nullptr)
	{
		//Move the next element of the current node to its current position.
		currentNode->nextElement->Move(currentNode->lastPosition);
		//Update the currentnode to the next element and repeat
		currentNode = currentNode->nextElement;
	}
}
bool Snake::CheckCollisionBounds()
{
	//Check if the snake is out of bounds
	if (snakeHead->position.x > 17 * gridSize || snakeHead->position.x < 2 * gridSize
		|| snakeHead->position.y > 17 * gridSize || snakeHead->position.y < topWaterBounds)
	{
		return false;
	}
	else
	{
		//If we arent out of bounds regen breath if near water line
		if (snakeHead->position.y <= topWaterBounds)
		{
			movementStepsLeft = defaultMovementSteps;
		}
		else if (movementStepsLeft > 0)
		{
			movementStepsLeft--;
		}
		return true;
	}


}
bool Snake::CheckCollisionSelf()
{
	SnakeNode* currentNode = snakeHead->nextElement;

	while (currentNode != nullptr)
	{
		if (snakeHead->position == currentNode->position)
		{
			std::cerr << "Hit ourselves";
			//Return false since we bumped into ourselves
			return false;
		}
		currentNode = currentNode->nextElement;
	}
	return true;
}

void Snake::CheckSteps()
{
	if (movementStepsLeft <= 0)
	{
		if (snakeHead->nextElement != nullptr)
		{
			SnakeNode* currentNode = snakeHead;
			SnakeNode* previousNode = currentNode;

			while (currentNode->nextElement != nullptr)
			{
				previousNode = currentNode;
				currentNode = currentNode->nextElement;
			}
			previousNode->nextElement = nullptr;
			delete currentNode;
		}
	}
}
