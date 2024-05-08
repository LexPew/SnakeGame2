#pragma once
#include "SnakeNode.h"
#include <iostream>
#include <JMath.h>

/*-- - SNAKE CLASS - ---
*
*	Handles all Snake related functions including movement, state, scoring and a singularly linked list.
*
*/

class Snake
{
private:
#pragma region VARIABLES
	//--GENERAL--
	SnakeNode* snakeHead;
	std::vector<Snake*>* otherSnakes;

	//--STATE--
	bool isAlive{ true };
	bool updatedIsAlive{ true };

	//--MOVEMENT--
	sf::Vector2f movementDirection;
	sf::Vector2f newMovementDirection;

	const int defaultMovementSteps{ 100 };
	int movementStepsLeft;

	int stepsTakenSinceOutOfBreath{ 0 };
	int topWaterBounds{ 0 };

	//--SCORE--
	int snakeScore{ 0 };
	float scoreLossPercentage = .05f;

	//--VISUAL--
	char snakeSuffix{ 1 };
	sf::Color snakeColour{ sf::Color::Black };

#pragma endregion

public:
#pragma region MAIN FUNCTIONS

	/*
	 * @brief Constructor for the Snake class.
	 *
	 * @param p_TopWaterBounds Top boundary for the water area.
	 * @param p_SnakeList Pointer to a vector containing other snakes in the game.
	 * @param p_SnakeChar Character representing the snake.
	 */
	Snake(int p_TopWaterBounds, std::vector<Snake*>* p_SnakeList, char p_SnakeChar)
	{
		// Initialize the first element
		snakeHead = new SnakeNode;
		movementStepsLeft = defaultMovementSteps;
		otherSnakes = p_SnakeList;
		UpdateWaterBounds(p_TopWaterBounds);

		// Generate random colour
		snakeColour.r = JMath::RandomInt(150, 255);
		snakeColour.g = JMath::RandomInt(150, 255);

		snakeSuffix = p_SnakeChar;

		isAlive = true;
	}
	/*
	* @brief Destructor for the Snake class.
	*/
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

	/*
	 * @brief Function to handle input for updating snake direction.
	 */
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
	/*
	 * @brief Update function for the snake.
	 */
	virtual void Update()
	{
		//Dont call if not alive
		if (!isAlive) return;
		Move();
	}
	/*
	* @brief Update flags related to snake state.
	*/
	void UpdateFlags()
	{
		isAlive = updatedIsAlive;
	}


#pragma endregion

#pragma region ADDITIONAL FUNCTIONS
	/*
	 * @brief Update water bounds.
	 *
	 * @param p_TopWaterBounds Top boundary for the water area.
	 */
	void UpdateWaterBounds(int p_TopWaterBounds)
	{
		topWaterBounds = p_TopWaterBounds;
	}

	/*
	 * @brief Handles changing the direction of the snake.
	 *
	 * @param newDirection Direction in which the snake will go if it doesn't conflict with movement rules.
	 */
	void ChangeDirection(const sf::Vector2f& newDirection)
	{
		//Checks that we arent going back on ourselves and updates the newMovementDirection.
		if (newDirection.x != -movementDirection.x || newDirection.y != -movementDirection.y) {
			newMovementDirection = newDirection;
		}

	}

	/*
	 * @brief Adds score to the snake.
	 *
	 * @param p_ScoreToAdd Score to add to the snake.
	 */
	void AddScore(int p_ScoreToAdd) {
		snakeScore += p_ScoreToAdd;
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
	int GetScore()
	{
		return snakeScore;
	}
	char GetSnakeChar()
	{
		return snakeSuffix;
	}
#pragma endregion 

private:
#pragma region LINKED LIST
	  /*
		* @brief Returns the length of the snake.
		*
		* @return Length of the snake from 1 to N.
		*/
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
	/*
	 * @brief Gets the element at the specified index.
	 *
	 * @param elementIndex Index of the element to retrieve.
	 * @return SnakeNode* Pointer to the node at the specified index.
	 *                   Returns nullptr if index is out of range.
	 */
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
	/*
	 * @brief Adds a new body segment to the snake in the linked list.
	 */
public:
	void AddSnakeBody()
	{
		SnakeNode* nodeToAdd = new SnakeNode;

		SnakeNode* currentNode = snakeHead;

		GetElement(Length() - 1)->nextElement = nodeToAdd;
	}
#pragma endregion
private:
#pragma region MOVEMENT FUNCTIONS
	/**
	 * @brief Updates the snakes movement, checking for collision, etc via collision functions.
	 */
	void Move();
	/**
	 * @brief Updates the position of the snake's head to the specified position and updates all subsequent snake nodes.
	 *
	 * @param newHeadPosition The new position of the snake's head.
	 */
	void UpdateSnakePosition(sf::Vector2f& newHeadPosition);
	/*
	 * @brief Checks if the snake collides with the bounds of the game area or the water.
	 *
	 * @return true if collision with bounds occurs, false otherwise.
	 */
	bool CheckCollisionBounds();
	/**
	 * @brief Checks if the snake collides with itself or with other snakes.
	 *
	 * @return true if collision with itself or other snakes occurs, false otherwise.
	 */
	bool CheckCollision();
	/*
	 * @brief Checks the steps taken by the snake, and if we are out of breath removes segments and score.
	 */
	void CheckSteps();
#pragma endregion

};




