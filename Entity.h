#pragma once
#include <SFML/Graphics.hpp>

class Entity
{
	virtual void Update() {};
	virtual void Draw(sf::RenderWindow& drawWindow) {};
};


