#pragma once
#include "SnakeNode.h"
#include <iostream>
#include <JMath.h>

class Snake
{

private:
	//General snake variables
	SnakeNode* snakeHead;
	int topWaterBounds{ 0 };
	bool isAlive{ true };
	std::vector<Snake*>* otherSnakes;
	//Movement variables
	sf::Vector2f movementDirection;
	const int defaultMovementSteps{ 100 };
	int movementStepsLeft;
	int stepsTakenSinceOutOfBreath{ 0 };
	//Colour variables
	sf::Color snakeColour{ sf::Color::Black };
public:
	#pragma region MAIN FUNCTIONS
		//Constructor
		Snake(int p_TopWaterBounds, std::vector<Snake*>* p_SnakeList)
		{
			//Initilize the first element
			snakeHead = new SnakeNode;
			movementStepsLeft = defaultMovementSteps;
			otherSnakes = p_SnakeList;
			UpdateWaterBounds(p_TopWaterBounds);

			//Generate random colour
			snakeColour.r = JMath::RandomInt(150, 255);
			snakeColour.g = JMath::RandomInt(150, 255);
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

		//Default input system
		virtual void UpdateInput()
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				ChangeDirection(sf::Vector2f(-1, 0)); // Move left
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				ChangeDirection(sf::Vector2f(1, 0)); // Move right
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				ChangeDirection(sf::Vector2f(0, -1)); // Move up
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				ChangeDirection(sf::Vector2f(0, 1)); // Move down
			}
		}
		virtual void Update()
		{
			//Dont call if not alive
			if (!isAlive) return;
			Move();
		}


	#pragma endregion
	#pragma region ADDITIONAL FUNCTIONS
	void UpdateWaterBounds(int p_TopWaterBounds)
	{
		topWaterBounds = p_TopWaterBounds;
	}

	//Handle direction
	void ChangeDirection(const sf::Vector2f& newDirection)
	{
		//Simple check to ensure we arent going back on ourselves even if input.
		if (newDirection.x != -movementDirection.x || newDirection.y != -movementDirection.y) {
			movementDirection = newDirection;
		}

	}

	void AddSnakeBody()
	{
		SnakeNode* nodeToAdd = new SnakeNode;

		SnakeNode* currentNode = snakeHead;

		GetElement(Length() - 1)->nextElement = nodeToAdd;
	}
	#pragma endregion
	#pragma region GETTERS
	SnakeNode* GetHead()
	{
		return snakeHead;
	}
	bool IsAlive() {
		return isAlive;
	}
	int GetMovementStepsLeft()
	{
		return movementStepsLeft;
	}
	const int GetDefaultMovementSteps() 
	{
		return defaultMovementSteps;
	}
	sf::Color GetSnakeColour()
	{
		return snakeColour;
	}
	#pragma endregion 
private:
	//Step check
	void CheckSteps();
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
	#pragma region MOVEMENT FUNCTIONS
	void Move();
	void UpdateSnakePosition(sf::Vector2f& newHeadPosition);
	bool CheckCollisionBounds();
	bool CheckCollision();

	#pragma endregion

};
		

	

		