#pragma once
#include "SnakeNode.h"
#include <iostream>

class Snake
{
public:
	//Constructor
	Snake()
	{
		//Initilize the first element
		head = new SnakeNode;
	}
	//Destructor
	~Snake()
	{
		// Delete all the Snake Nodes
		SnakeNode* currentNode = head;
		while (currentNode != nullptr)
		{
			SnakeNode* nextNode = currentNode->nextElement;
			delete currentNode;
			currentNode = nextNode;
		}
	}

	//List functions, length, getElement ovverid
	//Returns the length from 1 -> N
	int Length()
	{
		int listSize = 0;

		//Start the length search at the head
		SnakeNode* currentNode = head;

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
			SnakeNode* currentNode = head;
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
	SnakeNode* head;

	void MoveSnake(sf::Vector2f& newMovePosition)
	{
		//Move all the bodys
		SnakeNode* currentNode = head;

		currentNode->Move(newMovePosition);
		//While the node we are checking is not null continue to its next element
		while (currentNode->nextElement != nullptr)
		{
			currentNode->nextElement->Move(currentNode->lastPosition);
			currentNode = currentNode->nextElement;
		}
	}
};


	