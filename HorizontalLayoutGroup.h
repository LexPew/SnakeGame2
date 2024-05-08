#pragma once
#include "LayoutGroup.h"
class HorizontalLayoutGroup : public LayoutGroup 
{
public:
	using LayoutGroup::LayoutGroup;
	// Inherited via LayoutGroup
	void UpdateLayoutGroup() override
	{
		float xPos = startX;
		for (sf::Text* text : texts)
		{
			text->setPosition(xPos, startY);
			xPos += spacing;
		}
	};

};

