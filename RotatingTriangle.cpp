#include "RotatingTriangle.h"
#include <cmath>

RotatingTriangle::RotatingTriangle(float sideLength, float height, const sf::Color& color)
    : RotatingRectangle(sideLength, sideLength, height, color) {
    this->sideLength = sideLength;
    this->height = height;
    initializeVertices();
    setPosition(sf::Vector3f(5.0f, 0.0f, 0.0f)); 
    angleX = M_1_PI*3.2/2; //so it faces the other way
}



void RotatingTriangle::initializeVertices() {
    // Initialize your triangleVertices and triangleEdges here
    rectangleVertices = {
        { -sideLength / 2, -sideLength / 2, -height / 2 },
        { 0, sideLength / 2, -height / 2 },
        { sideLength / 2, -sideLength / 2, -height / 2 },
        { 0, 0, height / 2 }
    };
    
    // Define the triangle's edges
    rectangleEdges = {
        {0, 1}, {1, 2}, {2, 0}, // Base
        {0, 3}, {1, 3}, {2, 3}  // Connecting lines
    };
}


void RotatingTriangle::resetVertices() {
    // Reset the vertices to their original positions
    rectangleVertices = {
        { -sideLength / 2, -sideLength / 2, -height / 2 },
        { 0, sideLength / 2, -height / 2 },
        { sideLength / 2, -sideLength / 2, -height / 2 },
        { 0, 0, height / 2 }
    };

     // Store the original position and angles
    originalPosition = position;
    originalAngleX = angleX;
    originalAngleY = angleY;
    cumulativeAngleX = 0.0f;
    cumulativeAngleY = 0.0f;
}


void RotatingTriangle::draw(sf::RenderWindow& window, const Camera& camera) const {
    // Draw the filled shape on all faces
    sf::VertexArray filledShape(sf::Triangles);

    // Draw four triangles to form a pyramid
    appendQuad(filledShape, rectangleVertices[0], rectangleVertices[1], rectangleVertices[2], rectangleVertices[0], window, camera);

    // Left face
    appendQuad(filledShape, rectangleVertices[2], rectangleVertices[1], rectangleVertices[0], rectangleVertices[2], window, camera);

    // Right face
    appendQuad(filledShape, rectangleVertices[3], rectangleVertices[1], rectangleVertices[0], rectangleVertices[3], window, camera);

    // Back face
    appendQuad(filledShape, rectangleVertices[3], rectangleVertices[2], rectangleVertices[1], rectangleVertices[3], window, camera);

    window.draw(filledShape);
    
    // Draw the outline lines for the sides
    sf::VertexArray outlineSides(sf::Lines);
    const float outlineThickness = 3.0f; // Adjust this value for the thickness of the outline
    // Increased perspective focal length to reduce FOV distortion
    const float perspective = 500.0f;

    // Iterate through the edges to draw the outlines
    for (const auto& edge : rectangleEdges) {
        const sf::Vector3f& v1 = allowRotationX ? rectangleVertices[edge.first] : prevRectangleVertices[edge.first]; // question mark for if else
        const sf::Vector3f& v2 = allowRotationX ? rectangleVertices[edge.second] : prevRectangleVertices[edge.second]; // question mark for if else

        // Apply perspective projection
        float z1 = (v1.z > 0.01f) ? v1.z : 0.01f;
        float z2 = (v2.z > 0.01f) ? v2.z : 0.01f;

        float proj1X = (v1.x / z1) * perspective;
        float proj1Y = (v1.y / z1) * perspective;
        
        float proj2X = (v2.x / z2) * perspective;
        float proj2Y = (v2.y / z2) * perspective;

        // Add vertices for the outline lines
        outlineSides.append(sf::Vertex(sf::Vector2f(window.getSize().x / 2 + proj1X, window.getSize().y / 2 + proj1Y), sf::Color::White));
        outlineSides.append(sf::Vertex(sf::Vector2f(window.getSize().x / 2 + proj2X, window.getSize().y / 2 + proj2Y), sf::Color::White));

        // Create a thicker outline by extending the lines
        sf::Vector2f offset = outlineThickness * sf::Vector2f(proj2Y - proj1Y, proj1X - proj2X) / std::sqrt((proj2Y - proj1Y) * (proj2Y - proj1Y) + (proj1X - proj2X) * (proj1X - proj2X));

        outlineSides.append(sf::Vertex(sf::Vector2f(window.getSize().x / 2 + proj1X + offset.x, window.getSize().y / 2 + proj1Y + offset.y), sf::Color::White));
        outlineSides.append(sf::Vertex(sf::Vector2f(window.getSize().x / 2 + proj2X + offset.x, window.getSize().y / 2 + proj2Y + offset.y), sf::Color::White));
    }

    window.draw(outlineSides);
}

