#include "Stars.h"
#include <cmath>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Stars::Stars(int numStars, float radius) {
    for (int i = 0; i < numStars; ++i) {
        float theta = (rand() % 360) * M_PI / 180.0f;
        float phi = (rand() % 360) * M_PI / 180.0f;
        float r = radius + (rand() % (int)(radius * 0.5f)); // vary distance
        
        float x = r * sin(phi) * cos(theta);
        float y = r * cos(phi);
        float z = r * sin(phi) * sin(theta);
        starVertices.push_back(sf::Vector3f(x, y, z));

        // Randomly assign a unique twinkling speed and starting phase offset to each star
        starPhases.push_back((rand() % 100) / 100.0f * 2.0f * M_PI);
        starFrequencies.push_back(0.5f + (rand() % 100) / 100.0f * 2.5f);
    }
}

void Stars::draw(sf::RenderWindow& window, const Camera& camera) {
    float currentTime = clock.getElapsedTime().asSeconds();
    sf::Vector2u screenSize = window.getSize();
    const float fov = 300.0f;
    
    sf::VertexArray starPoints(sf::Points);

    for (size_t i = 0; i < starVertices.size(); ++i) {
        const auto& originalVertex = starVertices[i];
        sf::Vector3f vertex = originalVertex;
        
        // Rotate around Y-axis (Yaw)
        float cosY = cos(-camera.getRotation().y);
        float sinY = sin(-camera.getRotation().y);

        float x1 = cosY * vertex.x - sinY * vertex.z;
        float z1 = sinY * vertex.x + cosY * vertex.z;
        vertex.x = x1;
        vertex.z = z1;

        // Rotate around X-axis (Pitch)
        float cosX = cos(-camera.getRotation().x);
        float sinX = sin(-camera.getRotation().x);

        float y1 = vertex.y * cosX - vertex.z * sinX;
        float z2 = vertex.y * sinX + vertex.z * cosX;
        vertex.y = y1;
        vertex.z = z2;

        // Rotate around Z-axis (Roll)
        float cosZ = cos(-camera.getRotation().z);
        float sinZ = sin(-camera.getRotation().z);

        float x2 = vertex.x * cosZ - vertex.y * sinZ;
        float y2 = vertex.x * sinZ + vertex.y * cosZ;
        vertex.x = x2;
        vertex.y = y2;

        if (vertex.z <= 0.1f) continue;

        // Apply perspective projection
        float projectedX = (vertex.x / vertex.z) * fov;
        float projectedY = (vertex.y / vertex.z) * fov;

        projectedX += screenSize.x / 2.0f;
        projectedY = screenSize.y / 2.0f - projectedY;

        if (projectedX >= 0 && projectedX <= screenSize.x && projectedY >= 0 && projectedY <= screenSize.y) {
            float baseAlpha = 180.0f;
            
            // Unique twinkle logic 
            float twinkle = sin(currentTime * starFrequencies[i] + starPhases[i]);
            float glowMultiplier = 0.8f + 0.5f * twinkle; 
            
            float finalAlpha = baseAlpha * glowMultiplier;
            if (finalAlpha > 255.0f) finalAlpha = 255.0f;
            if (finalAlpha < 0.0f) finalAlpha = 0.0f;
            
            starPoints.append(sf::Vertex(sf::Vector2f(projectedX, projectedY), sf::Color(255, 255, 255, static_cast<sf::Uint8>(finalAlpha))));
        }
    }
    window.draw(starPoints);
}
