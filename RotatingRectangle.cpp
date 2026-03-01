// RotatingRectangle.cpp
#include "RotatingRectangle.h"
#include "Camera.h"

#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

RotatingRectangle::RotatingRectangle(float width, float height, float depth, const sf::Color& color)
    : position(0.0f, 0.0f, 0.0f),
      rotationSpeed(5.0f), dampingFactor(0.98f),
      levelOffFactor(0.005f), maxPitch(0.5), maxRoll(0.5),
      angleX(0.0f), angleY(0.0f), angleXPrev(0.0f), angleYPrev(0.0f),
      originalPosition(0.0f, 0.0f, 0.0f), originalAngleX(0.0f), originalAngleY(0.0f),
      rectangleWidth(width), rectangleHeight(height), rectangleDepth(depth), rectangleColor(color), divider(1.35f),
      allowRotationX(false), allowRotationY(false), cumulativeAngleX(0.0f), cumulativeAngleY(0.0f) {

    // Initialize your rectangleVertices and rectangleEdges here
    rectangleVertices = {
        { -rectangleWidth / 2, -rectangleHeight / 2, -rectangleDepth / 2 },
        { -rectangleWidth / 2, rectangleHeight / 2, -rectangleDepth / 2 },
        { rectangleWidth / 2, -rectangleHeight / 2, -rectangleDepth / 2 },
        { rectangleWidth / 2, rectangleHeight / 2, -rectangleDepth / 2 },
        { -rectangleWidth / 2, -rectangleHeight / 2, rectangleDepth / 2 },
        { -rectangleWidth / 2, rectangleHeight / 2, rectangleDepth / 2 },
        { rectangleWidth / 2, -rectangleHeight / 2, rectangleDepth / 2 },
        { rectangleWidth / 2, rectangleHeight / 2, rectangleDepth / 2 }
    };

    // Define the rectangle's edges
    rectangleEdges = {
        {0, 1}, {1, 3}, {3, 2}, {2, 0}, // Front face
        {4, 5}, {5, 7}, {7, 6}, {6, 4}, // Back face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Connecting lines
    };

    // Store the original position and angles
    originalPosition = position;
    originalAngleX = angleX;
    originalAngleY = angleY;
}

void RotatingRectangle::resetVertices() {
    // Reset vertices to their original positions
    rectangleVertices = {
        { -rectangleWidth / 2, -rectangleHeight / 2, -rectangleDepth / 2 },
        { -rectangleWidth / 2, rectangleHeight / 2, -rectangleDepth / 2 },
        { rectangleWidth / 2, -rectangleHeight / 2, -rectangleDepth / 2 },
        { rectangleWidth / 2, rectangleHeight / 2, -rectangleDepth / 2 },
        { -rectangleWidth / 2, -rectangleHeight / 2, rectangleDepth / 2 },
        { -rectangleWidth / 2, rectangleHeight / 2, rectangleDepth / 2 },
        { rectangleWidth / 2, -rectangleHeight / 2, rectangleDepth / 2 },
        { rectangleWidth / 2, rectangleHeight / 2, rectangleDepth / 2 }
    };

    // Store the original position and angles
    originalPosition = position;
    originalAngleX = angleX;
    originalAngleY = angleY;
    cumulativeAngleX = 0.0f;
    cumulativeAngleY = 0.0f;
}

void RotatingRectangle::setPosition(const sf::Vector3f& newPosition) {
    position = newPosition;
}

sf::Vector3f RotatingRectangle::getPosition() const {
    return position;
}

void RotatingRectangle::update(float multiplier) {
    prevRectangleVertices = rectangleVertices;

    

    // space bar moves it on z axis forward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        angleX = angleX * (1.0f + levelOffFactor);
        angleY = angleY * (1.0f + levelOffFactor);

        for (size_t i = 0; i < rectangleVertices.size(); ++i) {
            // Interpolate each vertex back to its original position
            rectangleVertices[i] = prevRectangleVertices[i] * (1.0f - levelOffFactor);
        }
    } 
    // q moves it on z axis backward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        angleX = angleX * (1.0f - levelOffFactor);
        angleY = angleY * (1.0f - levelOffFactor);

        for (size_t i = 0; i < rectangleVertices.size(); ++i) {
            // Interpolate each vertex away from its original position
            rectangleVertices[i] = prevRectangleVertices[i] * (1.0f + levelOffFactor);
        }
    } 
    
    // Simulate pitch and roll based on keys
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && angleX < maxPitch) {
        angleX = std::min(angleX + rotationSpeed, maxPitch);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && angleY < maxRoll) {
        angleY = std::min(angleY + rotationSpeed, maxRoll);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && angleX > -maxPitch) {
        angleX = std::max(angleX - rotationSpeed, -maxPitch);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && angleY > -maxRoll) {
        angleY = std::max(angleY - rotationSpeed, -maxRoll);
    }
    

    // Apply damping when no keys are pressed to gradually return to center
if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
    !sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
    !sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
    !sf::Keyboard::isKeyPressed(sf::Keyboard::A))
 {

    // Reset angles to 0 when keys are released
    angleX = 0.14f;
    angleY = -0.0f;

    resetVertices();



// Update rectangle vertices based on pitch and roll angles
for (size_t i = 0; i < rectangleVertices.size(); ++i) {
    // Apply rotation around X-axis (pitch)
    float rotatedX = rectangleVertices[i].x;
    float rotatedY = cos(angleX) * rectangleVertices[i].y - sin(angleX) * rectangleVertices[i].z;
    float rotatedZ = sin(angleX) * rectangleVertices[i].y + cos(angleX) * rectangleVertices[i].z;

    // Apply rotation around Z-axis (roll)
    float finalX = cos(angleY) * rotatedX - sin(angleY) * rotatedY;
    float finalY = sin(angleY) * rotatedX + cos(angleY) * rotatedY;
    float finalZ = rotatedZ;

    rectangleVertices[i].x = finalX;
    rectangleVertices[i].y = finalY;
    rectangleVertices[i].z = finalZ;
}
}
    cumulativeAngleX += angleX;
    cumulativeAngleY += angleY;

    // Check if angles are close to 0, then interpolate back to 0
    if (std::abs(angleX) > 0.001f) {
        angleX = angleX * (1.0f - levelOffFactor);
    } else {
        angleX = 0.0f;
    }

    if (std::abs(angleY) > 0.001f) {
        angleY = angleY * (1.0f - levelOffFactor);
    } else {
        angleY = 0.0f;
    }

    angleX = angleX / divider;
    angleY = angleY / divider;

    //cout << "angleX: " << angleX << " angleY: " << angleY << endl;
    allowRotationX = (std::abs(angleX) > 0.001f) && (std::abs(cumulativeAngleX) < 0.5f);
    allowRotationY = (std::abs(angleY) > 0.001f) && (std::abs(cumulativeAngleY) < 2.0f);

    

    if (allowRotationX || allowRotationY) {

        //cout << fixed << setprecision(3) << "cumulativeAngleX: " << right << cumulativeAngleX << " cumulativeAngleY: " << right << cumulativeAngleY << endl;
        // Store the original position
        sf::Vector3f originalPosition = position;

        // Apply rotation around X-axis (pitch)
        for (size_t i = 0; i < rectangleVertices.size(); ++i) {
            float rotatedX = rectangleVertices[i].x;
            float rotatedY = cos(angleX) * rectangleVertices[i].y - sin(angleX) * rectangleVertices[i].z;
            float rotatedZ = sin(angleX) * rectangleVertices[i].y + cos(angleX) * rectangleVertices[i].z;

            rectangleVertices[i].x = rotatedX;
            rectangleVertices[i].y = rotatedY;
            rectangleVertices[i].z = rotatedZ;
        }

        // Apply rotation around Z-axis (roll)
        for (size_t i = 0; i < rectangleVertices.size(); ++i) {
            float rotatedX = cos(angleY) * rectangleVertices[i].x - sin(angleY) * rectangleVertices[i].y;
            float rotatedY = sin(angleY) * rectangleVertices[i].x + cos(angleY) * rectangleVertices[i].y;
            float rotatedZ = rectangleVertices[i].z;

            rectangleVertices[i].x = rotatedX;
            rectangleVertices[i].y = rotatedY;
            rectangleVertices[i].z = rotatedZ;
        }

        // Translate back to the original position
        sf::Vector3f translation = originalPosition - position;
        for (size_t i = 0; i < rectangleVertices.size(); ++i) {
            rectangleVertices[i] += translation;
        }

        // Update the position attribute
        position = originalPosition;
        }
        
}


void RotatingRectangle::translateToOrigin() {
    sf::Vector3f centroid = calculateCentroid();

    for (size_t i = 0; i < rectangleVertices.size(); ++i) {
        rectangleVertices[i] -= centroid;
    }
}

void RotatingRectangle::translateBackToOriginal() {
    sf::Vector3f centroid = calculateCentroid();

    for (size_t i = 0; i < rectangleVertices.size(); ++i) {
        rectangleVertices[i] += centroid;
    }
}

sf::Vector3f RotatingRectangle::calculateCentroid() const {
    sf::Vector3f centroid(0.0f, 0.0f, 0.0f);

    for (const auto& vertex : rectangleVertices) {
        centroid += vertex;
    }

    centroid /= static_cast<float>(rectangleVertices.size());

    return centroid;
}

void RotatingRectangle::draw(sf::RenderWindow& window, const Camera& camera) const {

    // Draw the filled shape on all faces
    sf::VertexArray filledShape(sf::Triangles);

    // Front face
    appendQuad(filledShape, rectangleVertices[0], rectangleVertices[1], rectangleVertices[3], rectangleVertices[2], window, camera);

    // Back face
    appendQuad(filledShape, rectangleVertices[4], rectangleVertices[5], rectangleVertices[7], rectangleVertices[6], window, camera);

    // Bottom face
    appendQuad(filledShape, rectangleVertices[0], rectangleVertices[4], rectangleVertices[5], rectangleVertices[1], window, camera);

    // Top face
    appendQuad(filledShape, rectangleVertices[2], rectangleVertices[3], rectangleVertices[7], rectangleVertices[6], window, camera);

    // Left face
    appendQuad(filledShape, rectangleVertices[0], rectangleVertices[2], rectangleVertices[6], rectangleVertices[4], window, camera);

    // Right face
    appendQuad(filledShape, rectangleVertices[1], rectangleVertices[3], rectangleVertices[7], rectangleVertices[5], window, camera);

    window.draw(filledShape);

    // Disable face culling after rendering the filled shape
    

    // Draw the outline lines for the sides
    sf::VertexArray outlineSides(sf::Lines);
    const float outlineThickness = 3.0f; // Adjust this value for the thickness of the outline

    // Iterate through the edges to draw the outlines
    // Increased perspective focal length to reduce FOV distortion (fisheye effect) on close objects
    const float perspective = 500.0f;
    for (const auto& edge : rectangleEdges) {
        // Get the vertices for the current edge
        const sf::Vector3f& v1 = allowRotationX ? rectangleVertices[edge.first] : prevRectangleVertices[edge.first]; // question mark for if else
        const sf::Vector3f& v2 = allowRotationX ? rectangleVertices[edge.second] : prevRectangleVertices[edge.second]; // question mark for if else

        float z1 = (v1.z > 0.01f) ? v1.z : 0.01f;
        float z2 = (v2.z > 0.01f) ? v2.z : 0.01f;

        // Apply perspective projection
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



void RotatingRectangle::appendQuad(sf::VertexArray& vertices, const sf::Vector3f& v1, const sf::Vector3f& v2, const sf::Vector3f& v3, const sf::Vector3f& v4, sf::RenderWindow& window, const Camera& camera) const {
        // Apply perspective projection
        // Increased perspective focal length to reduce FOV distortion
        const float perspective = 500.0f;
        
        float z1 = (v1.z > 0.01f) ? v1.z : 0.01f;
        float z2 = (v2.z > 0.01f) ? v2.z : 0.01f;
        float z3 = (v3.z > 0.01f) ? v3.z : 0.01f;
        float z4 = (v4.z > 0.01f) ? v4.z : 0.01f;

        // Calculate projected positions with perspective division by z
        float proj1X = (v1.x / z1) * perspective;
        float proj1Y = (v1.y / z1) * perspective;
        
        float proj2X = (v2.x / z2) * perspective;
        float proj2Y = (v2.y / z2) * perspective;
        
        float proj3X = (v3.x / z3) * perspective;
        float proj3Y = (v3.y / z3) * perspective;
        
        float proj4X = (v4.x / z4) * perspective;
        float proj4Y = (v4.y / z4) * perspective;
        
        // Add vertices for the quad
        vertices.append(sf::Vertex(sf::Vector2f(window.getSize().x / 2 + proj1X, window.getSize().y / 2 + proj1Y), rectangleColor));
        vertices.append(sf::Vertex(sf::Vector2f(window.getSize().x / 2 + proj2X, window.getSize().y / 2 + proj2Y), rectangleColor));
        vertices.append(sf::Vertex(sf::Vector2f(window.getSize().x / 2 + proj3X, window.getSize().y / 2 + proj3Y), rectangleColor));

        vertices.append(sf::Vertex(sf::Vector2f(window.getSize().x / 2 + proj1X, window.getSize().y / 2 + proj1Y), rectangleColor));
        vertices.append(sf::Vertex(sf::Vector2f(window.getSize().x / 2 + proj3X, window.getSize().y / 2 + proj3Y), rectangleColor));
        vertices.append(sf::Vertex(sf::Vector2f(window.getSize().x / 2 + proj4X, window.getSize().y / 2 + proj4Y), rectangleColor));
    
}

void RotatingRectangle::setRotationSpeed(float speed) {
    rotationSpeed = speed;
}

bool RotatingRectangle::vectorsEqual(const sf::Vector3f& v1, const sf::Vector3f& v2) const {
    return std::abs(v1.x - v2.x) < epsilon && std::abs(v1.y - v2.y) < epsilon && std::abs(v1.z - v2.z) < epsilon;
}

void RotatingRectangle::setVisualRotation(float pitch, float roll) {
    angleX = pitch;
    angleY = roll;
}

void RotatingRectangle::setColor(const sf::Color& color) {
    rectangleColor = color;
}

void RotatingRectangle::setAbsoluteTransform(const sf::Vector3f& pos, const sf::Vector3f& rot) {
    // 1. Reset vertices to their local origin
    resetVertices();
    
    // 2. Rotate Pitch and Roll ONLY (The HUD doesn't translate or yaw visually because camera orbits behind it)
    for(size_t i = 0; i < rectangleVertices.size(); ++i) {
        sf::Vector3f v = rectangleVertices[i];
        
        // Dramatic multiplier for visuals
        float dramaticMultiplier = 1.35f;
        
        // Roll (Z) - invert rot.z to fix left/right banking
        float rz = -rot.z * dramaticMultiplier;
        float x1 = v.x * cos(rz) - v.y * sin(rz);
        float y1 = v.x * sin(rz) + v.y * cos(rz);
        float z1 = v.z;
        
        // Pitch (X) - invert rot.x to fix W/S inversion visually, and make it dramatic
        float rx = -rot.x * dramaticMultiplier;
        float x2 = x1;
        float y2 = y1 * cos(rx) - z1 * sin(rx);
        float z2 = y1 * sin(rx) + z1 * cos(rx);
        
        // No Yaw applied as it stays aligned with camera view natively
        // Translate deeply into view space 
        rectangleVertices[i] = sf::Vector3f(x2 + pos.x, y2 + pos.y, z2 + pos.z);
    }
    
    // allowRotationX flags are just to prevent drawing from previous state in some outlines. Force true.
    allowRotationX = true;
}
