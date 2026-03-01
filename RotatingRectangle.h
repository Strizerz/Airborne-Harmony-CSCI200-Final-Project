#ifndef ROTATING_RECTANGLE_H
#define ROTATING_RECTANGLE_H

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <vector>

class RotatingRectangle{
public:
    RotatingRectangle(float width, float height, float depth, const sf::Color& color);

    void update(float multiplier);
    virtual void draw(sf::RenderWindow& window, const class Camera& camera) const;
    void setRotationSpeed(float speed);
    void appendQuad(sf::VertexArray& vertices, const sf::Vector3f& v1, const sf::Vector3f& v2, const sf::Vector3f& v3, const sf::Vector3f& v4, sf::RenderWindow& window, const class Camera& camera) const;
    void setPosition(const sf::Vector3f& position);
    sf::Vector3f getPosition() const;
    virtual void resetVertices();
    void setVisualRotation(float pitch, float roll);
    void setAbsoluteTransform(const sf::Vector3f& pos, const sf::Vector3f& rot);
    void setColor(const sf::Color& color);

protected:
    void translateToOrigin();
    void translateBackToOriginal();
    sf::Vector3f calculateCentroid() const;

    bool vectorsEqual(const sf::Vector3f& v1, const sf::Vector3f& v2) const;

    float cumulativeAngleX;
    float cumulativeAngleY;
    float rotationSpeed;
    float dampingFactor;
    float levelOffFactor;
    float maxPitch;
    float maxRoll;
    float angleX;
    float angleY;
    float angleXPrev;
    float angleYPrev;
    float rectangleWidth;
    float rectangleHeight;
    float rectangleDepth;
    sf::Color rectangleColor;
    float divider;
    std::vector<sf::Vector3f> rectangleVertices;
    std::vector<std::pair<size_t, size_t>> rectangleEdges;
    std::vector<sf::Vector3f> prevRectangleVertices;
    bool allowRotationX;
    bool allowRotationY;
    sf::Vector3f position; // Added position attribute
    sf::Vector3f originalPosition; // Added original position attribute
    float originalAngleX; // Added original angleX attribute
    float originalAngleY; // Added original angleY attribute

    const float epsilon = 0.001f;
};

#endif // ROTATING_RECTANGLE_H
