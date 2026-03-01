#include "Camera.h"
#include "Waves.h"

#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>




using namespace std;

Waves::Waves(int numRows, int numCols, float spacing, float cubeSize) {
    // Initialize waves vertices and edges based on your grid parameters
    // Implement this based on your requirements

    //calculate the total size of the waves
    float totalWidth = numCols * (cubeSize + spacing);
    float totalDepth = numRows * (cubeSize + spacing);
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            float x = -totalWidth / 2 + col * (cubeSize + spacing);
            float z = -totalDepth / 2 + row * (cubeSize + spacing);

            // Create the vertices for each cube in the grid
            sf::Vector3f vertex1(x, 0.0f, z);
            sf::Vector3f vertex2(x + cubeSize, 0.0f, z);
            sf::Vector3f vertex3(x, 0.0f, z + cubeSize);
            sf::Vector3f vertex4(x + cubeSize, 0.0f, z + cubeSize);

            // Add edges for the cube
            // Define the edges as pairs of vertex indices
            wavesEdges.push_back({wavesVertices.size(), wavesVertices.size() + 1});
            wavesEdges.push_back({wavesVertices.size() + 1, wavesVertices.size() + 3});
            wavesEdges.push_back({wavesVertices.size() + 3, wavesVertices.size() + 2});
            wavesEdges.push_back({wavesVertices.size() + 2, wavesVertices.size()});

            // Add the vertices to the wavesVertices vector
            wavesVertices.push_back(vertex1);
            wavesVertices.push_back(vertex2);
            wavesVertices.push_back(vertex3);
            wavesVertices.push_back(vertex4);
        }
    }
    waveAmplitude = 5.0f;
    waveFrequency = 0.1f;
    waveSpeed = 2.0f;
    // Set the gradient colors for the wave
    waveColorStart = sf::Color(255, 0, 0);  // Start color (blue)
    waveColorEnd = sf::Color(255, 255, 255);  // End color (white)
}

void Waves::rotate(const sf::Vector3f& rotation) {
    // Implement your rotation logic here
    for (sf::Vector3f& vertex : wavesVertices) {
        // Apply the same rotation to each vertex
        float x = vertex.x;
        float y = vertex.y;
        float z = vertex.z;

        // Apply rotation (rotating around the Y-axis)
        float newX = cos(rotation.y) * x - sin(rotation.y) * z;
        float newZ = sin(rotation.y) * x + cos(rotation.y) * z;

        // Update the vertex position
        vertex.x = newX;
        vertex.y = y;
        vertex.z = newZ;
    }
}
 

void Waves::translate(const sf::Vector3f& translation) {
    // Implement your translation logic here
    for (sf::Vector3f& vertex : wavesVertices) {
        vertex += translation;
    }
}

bool Waves::isWithinScreenBounds(float x, float y, const sf::Vector2u& screenSize) {
    return (x >= -1 && x <= screenSize.x && y >= -1 && y <= screenSize.y);
}

void Waves::draw(sf::RenderWindow& window, const Camera& camera) {
    // Get the window size
    sf::Vector2u screenSize = window.getSize();
    const float fov = 300.0f;

    for (const auto& edge : wavesEdges) {
        int startVertexIndex = edge.first;
        int endVertexIndex = edge.second;

        sf::Vector3f startVertex = wavesVertices[startVertexIndex];
        sf::Vector3f endVertex = wavesVertices[endVertexIndex];

        // Apply waves transformations (translation and rotation)
        startVertex -= camera.getPosition();  // Translate
        endVertex -= camera.getPosition();    // Translate

        // Apply custom rotation logic
        // Rotate around the Y-axis (Yaw)
        float cosY = cos(-camera.getRotation().y);
        float sinY = sin(-camera.getRotation().y);

        float startX = cosY * startVertex.x - sinY * startVertex.z;
        float startZ = sinY * startVertex.x + cosY * startVertex.z;
        startVertex.x = startX;
        startVertex.z = startZ;

        float endX = cosY * endVertex.x - sinY * endVertex.z;
        float endZ = sinY * endVertex.x + cosY * endVertex.z;
        endVertex.x = endX;
        endVertex.z = endZ;

        // Rotate around X-axis (Pitch)
        float cosX = cos(-camera.getRotation().x);
        float sinX = sin(-camera.getRotation().x);

        float startY = startVertex.y * cosX - startVertex.z * sinX;
        float startZ_final = startVertex.y * sinX + startVertex.z * cosX;
        startVertex.y = startY;
        startVertex.z = startZ_final;

        float endY = endVertex.y * cosX - endVertex.z * sinX;
        float endZ_final = endVertex.y * sinX + endVertex.z * cosX;
        endVertex.y = endY;
        endVertex.z = endZ_final;

        // Rotate around Z-axis (Roll)
        float cosZ = cos(-camera.getRotation().z);
        float sinZ = sin(-camera.getRotation().z);

        float finalStartX = startVertex.x * cosZ - startVertex.y * sinZ;
        float finalStartY = startVertex.x * sinZ + startVertex.y * cosZ;
        startVertex.x = finalStartX;
        startVertex.y = finalStartY;

        float finalEndX = endVertex.x * cosZ - endVertex.y * sinZ;
        float finalEndY = endVertex.x * sinZ + endVertex.y * cosZ;
        endVertex.x = finalEndX;
        endVertex.y = finalEndY;

        // Check if behind camera
        if (startVertex.z <= 0.1f && endVertex.z <= 0.1f) continue;

        // Apply custom perspective projection (adjust perspective value as needed)
        float projectedStartX = (startVertex.x / startVertex.z) * fov;
        float projectedStartY = (startVertex.y / startVertex.z) * fov;
        float projectedEndX = (endVertex.x / endVertex.z) * fov;
        float projectedEndY = (endVertex.y / endVertex.z) * fov;

        // Center on screen
        projectedStartX += screenSize.x / 2.0f;
        projectedStartY = screenSize.y / 2.0f - projectedStartY;
        projectedEndX += screenSize.x / 2.0f;
        projectedEndY = screenSize.y / 2.0f - projectedEndY;

        // Check if at least one vertex of the line is within the screen bounds
        if (isWithinScreenBounds(projectedStartX, projectedStartY, screenSize) || 
            isWithinScreenBounds(projectedEndX, projectedEndY, screenSize)) {
            // Create vertices for drawing
            sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(projectedStartX, projectedStartY), vertexColor),
            sf::Vertex(sf::Vector2f(projectedEndX, projectedEndY), vertexColor)
        };

            // Draw the line segment on the screen
            window.draw(line, 2, sf::Lines);
        }
    }
}



void Waves::getPosition() {
    cout << wavesVertices[0].x << " " << wavesVertices[0].y << " " << wavesVertices[0].z << endl;
}
void Waves::setPosition(const sf::Vector3f& position) {
    // Calculate the translation offset based on the new position and the previous position
    sf::Vector3f translation = position - wavesPosition;

    // Update the wavesVertices to apply the translation
    for (sf::Vector3f& vertex : wavesVertices) {
        vertex += translation;
    }

    // Update the stored position of the waves
    wavesPosition = position;
}

void Waves::setColor(const sf::Color& color) {
    wavesColor = color;
    
}

// Modify the update function to set the vertex color based on gradient
void Waves::update(float deltaTime) {
    float currentTime = clock.getElapsedTime().asSeconds();
    for (sf::Vector3f& vertex : wavesVertices) {
        // Use a sine function to create a wave motion
        vertex.y = sin(currentTime * waveSpeed + vertex.x * waveFrequency) * waveAmplitude;

        // Normalize the height to the range [0, 1]
        float normalizedHeight = (vertex.y + waveAmplitude) / (2.0f * waveAmplitude);
        //cout << normalizedHeight << endl;

        // Set the vertex color based on linear interpolation (lerp) between blue and white
        vertexColor = lerpColor(sf::Color(0, 0, 255), sf::Color(0, 255, 255), normalizedHeight);
    }
    //cout << "BLUC" << endl;
}

void Waves::setWaveAmplitude(float amplitude) {
    waveAmplitude = amplitude;
}

void Waves::setWaveFrequency(float frequency) {
    waveFrequency = frequency;
}

void Waves::setWaveSpeed(float speed) {
    waveSpeed = speed;
}







// Function to linearly interpolate between two colors
sf::Color Waves::lerpColor(const sf::Color& color1, const sf::Color& color2, float t) {
    t = std::max(0.0f, std::min(1.0f, t));  // Clamp t between 0 and 1
    sf::Uint8 r = static_cast<sf::Uint8>((1.0f - t) * color1.r + t * color2.r);
    sf::Uint8 g = static_cast<sf::Uint8>((1.0f - t) * color1.g + t * color2.g);
    sf::Uint8 b = static_cast<sf::Uint8>((1.0f - t) * color1.b + t * color2.b);
    return sf::Color(r, g, b);
}





