#pragma once
#include <SFML/Graphics.hpp>

#include <JMathLib/JMath.h>
class Apple
{
public:
	sf::Color appleColour{sf::Color::Green};
	sf::Vector2f position{0, 0};

    //Collectable Settings
    int maxCollectablePoints{ 100 };
    int collectablePoints{ 0 };
    
    int maxTailGrowth{ 3 };
    int tailGrowth{ 1 };

    Apple(sf::Vector2f p_StartPosition)
    {
        position = p_StartPosition;
        collectablePoints = JMath::RandomInt(1, maxCollectablePoints);
        // Ensure floating-point division by casting one of the operands to float
        float percentage = static_cast<float>(collectablePoints) / maxCollectablePoints;

        if (percentage <= .75) {
            tailGrowth = 2;
        }
        else if (percentage > .75)
        {
            tailGrowth = maxTailGrowth;
        }


        int appleR = JMath::Lerp(appleColour.r, 255, percentage);
        int appleG = JMath::Lerp(appleColour.g, 215, percentage);
        int appleB = JMath::Lerp(appleColour.b, 0, percentage);
        appleColour = sf::Color(appleR, appleG, appleB);
    }

};

