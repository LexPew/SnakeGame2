#pragma once
#include "Snake.h"
#include "Apple.h"
class ArtificialUnIntelligentSnake : public Snake
{
    std::vector<Apple> appleList;
public:
    ArtificialUnIntelligentSnake(int p_TopWaterBounds, std::vector<Snake*>* p_SnakeList, char p_SnakeChar)
        : Snake(p_TopWaterBounds, p_SnakeList, p_SnakeChar) {}

    void SetAppleCollectablesList(std::vector<Apple>& p_AppleList)
    {
        appleList = p_AppleList;
    }

private:

    // Overriding UpdateInput function to implement intelligent AI movement logic
    void UpdateInput() override
    {

    }


    sf::Vector2f FindNearestCollectiblePosition()
    {
        sf::Vector2f nearestApplePosition;
        float appleDist = 100000; // Ensure first loop will check
        for (const Apple& apple : appleList) // Assuming appleList is a vector of Apple objects
        {
            float magnitudeDistance = std::sqrt(std::pow((apple.position.x - GetHead()->position.x), 2)
                + std::pow((apple.position.y - GetHead()->position.y), 2));
            if (magnitudeDistance < appleDist)
            {
                nearestApplePosition = apple.position;
                appleDist = magnitudeDistance; // Update the distance to the nearest apple
            }
        }
        return nearestApplePosition;
    }
`
};

