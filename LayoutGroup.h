#pragma once
#include <SFML/Graphics.hpp>
class LayoutGroup
{
public:
	//Render window to render the text to when function called
	sf::RenderWindow* renderWindow;
	//Vector to hold the text
	std::vector<sf::Text*> texts;
	//Layout variables
	float startX;
	float startY;
	float spacing;
public:
	//Constructor for the layout group given the start x and y and spacing it will caulcate the layout
	LayoutGroup(float p_StartXPosition, float p_StartYPosition, float p_Spacing, bool p_AutoSpacing,  sf::RenderWindow* p_RenderWindow)
	{
		//Assign variables
		startX = p_StartXPosition;
		startY = p_StartYPosition;
		spacing = p_Spacing;
		renderWindow = p_RenderWindow;
	}
	~LayoutGroup() {
		renderWindow = nullptr;
		texts.clear();
	}

	void AddText(sf::Text* text)
	{
		texts.push_back(text);
		UpdateLayoutGroup();
	}

	virtual void UpdateLayoutGroup()
	{
		//Grab width of first text
		float firstTextWidthCentre = texts[0]->getPosition().x + texts[0]->getLocalBounds().width /2;
		//Handle updating layout group, default to vertical layout
		float yPos = startY;
		float xPos = startX;
	
		for (sf::Text* text : texts)
		{

			if (text != texts[0])
			{
				xPos = firstTextWidthCentre - text->getLocalBounds().width / 2;
			}
			text->setPosition(xPos, yPos);

			yPos += text->getLocalBounds().height + spacing; // Update yPos for next text
		}
	};

	void Draw()
	{
		for (sf::Text* text : texts)
		{
			renderWindow->draw(*text);
		}
	}
};

