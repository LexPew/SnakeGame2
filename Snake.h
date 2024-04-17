#pragma once
#include "SnakeNode.h"
#include <iostream>


class Snake
{
public:

	SnakeNode* snakeHead;
	sf::Vector2f movementDirection;
	const int defaultMovementSteps{ 60 };
	int movementStepsLeft = defaultMovementSteps;
	int stepsTakenSinceOutOfBreath{ 0 };

public:
	//Constructor
	Snake()
	{
		//Initilize the first element
		snakeHead = new SnakeNode;
	}
	//Destructor
	~Snake()
	{
		// Delete all the Snake Nodes
		SnakeNode* currentNode = snakeHead;
		while (currentNode != nullptr)
		{
			SnakeNode* nextNode = currentNode->nextElement;
			delete currentNode;
			currentNode = nextNode;
		}

		snakeHead = nullptr;
	}

	//Move
	bool Move(int topBoundsWater)
	{
		//Calculate the new position based on the movement direction and grid size.
		sf::Vector2f newHeadPosition = snakeHead->position + sf::Vector2f(movementDirection.x * 48,
																			movementDirection.y * 48);
		//Assign the current node to the snake head
		SnakeNode* currentNode = snakeHead;

		//Move the head to the new position, this updates it's last position as well
		currentNode->Move(newHeadPosition);

		//Make sure that there is a next element to update.
		while (currentNode->nextElement != nullptr)
		{
			//Move the next element of the current node to its current position.
			currentNode->nextElement->Move(currentNode->lastPosition);
			//Update the currentnode to the next element and repeat
			currentNode = currentNode->nextElement;
		}
		if (newHeadPosition.y == (topBoundsWater))
		{
			movementStepsLeft = defaultMovementSteps;
		}
		else 
		{
			movementStepsLeft--;
		}

		//Now that we have updated all the positions check we havent hit any snake parts
		//Start at the snakehead next element
		currentNode = snakeHead->nextElement;

		while (currentNode != nullptr)
		{
			if (newHeadPosition == currentNode->position)
			{
				std::cerr << "Hit ourselves";
				//Return false since we bumped into ourselves
				return false;
			}
			currentNode = currentNode->nextElement;
		}

		//Check if the snake is out of bounds
		if (snakeHead->position.x > 17 * 48|| snakeHead->position.x < 2 * 48
			|| snakeHead->position.y > 17 * 48 || snakeHead->position.y < topBoundsWater)
		{
			return false;
		}
		return true;
	}
	//Handle direction
	void ChangeDirection(const sf::Vector2f& newDirection)
	{
		if (newDirection.x != -movementDirection.x || newDirection.y != -movementDirection.y) {
			movementDirection = newDirection;
		}

	}

	//Step check
	void CheckSteps()
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


	//Snake related functions
	//List functions, length, getElement ovverid
		//Returns the length from 1 -> N
	int Length()
	{
		int listSize = 0;

		//Start the length search at the head
		SnakeNode* currentNode = snakeHead;

		//While the node we are checking is not null continue to its next element
		while (currentNode != nullptr)
		{
			//Add to the size if the node is valid
			listSize++;

			//Set the current node to the next element in the linked list, while loop will repeat until nullptr
			currentNode = currentNode->nextElement;
		}

		//Run out of elements, return the list size
		return listSize;

	}

	SnakeNode* GetElement(int elementIndex)
	{
		//Check first that the element is not out of range
		if (elementIndex < 0 || elementIndex > Length() - 1)
		{
			std::cerr << "Tried accessing element in list out side of range, 0 to " << Length() - 1;
			return nullptr;
		}
		else
		{
			//Loop through from first element until i is less than elementIndex - 1 then return the nextElement of i.
			//So if 1 is input then 0, next element will be 1 index. Also check it is valid
			SnakeNode* currentNode = snakeHead;
			if (elementIndex == 0) {
				return currentNode;
			}
			else
			{
				for (int i = 0; i < elementIndex; i++)
				{
					currentNode = currentNode->nextElement;
				}
			}


			if (currentNode != nullptr) {
				return currentNode;
			}
			else
			{
				std::cerr << "Tried accessing element outside of range";
				return nullptr;
			}

		}
	}

	//Adds a body to the snake
	void AddSnakeBody()
	{
		SnakeNode* nodeToAdd = new SnakeNode;

		SnakeNode* currentNode = snakeHead;

		GetElement(Length() - 1)->nextElement = nodeToAdd;
	}
};
		

	

		