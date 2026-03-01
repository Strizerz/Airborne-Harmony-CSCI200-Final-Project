#ifndef ROTATING_TRIANGLE_H
#define ROTATING_TRIANGLE_H

#include "RotatingRectangle.h"

class RotatingTriangle : public RotatingRectangle {
public:
    RotatingTriangle(float sideLength, float height, const sf::Color& color);

    void draw(sf::RenderWindow& window, const class Camera& camera) const override;
    void resetVertices() override;

private:
    void initializeVertices();
    float sideLength;
    float height;
};

#endif // ROTATING_TRIANGLE_H
