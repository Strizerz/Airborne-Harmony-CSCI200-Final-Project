#ifndef STARS_H
#define STARS_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "Camera.h"

class Stars {
public:
    Stars(int numStars, float radius);
    void draw(sf::RenderWindow& window, const Camera& camera);

private:
    std::vector<sf::Vector3f> starVertices;
    std::vector<float> starPhases;
    std::vector<float> starFrequencies;
    sf::Clock clock;
};

#endif
