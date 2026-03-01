#include "Player.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Player::Player() 
    : position(0.0f, 50.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), 
      speed(0.1f), rotationSpeed(0.03f), maxPitch(0.785f), maxRoll(0.523f), maxYaw(0.523f), // 30 degrees approx
      wing(3.0f, 0.25f, 1.0f, sf::Color(75, 73, 73)),
      tail(1.0f, 4.0f, sf::Color(0, 255, 0))
{
    // Initialize model parts if needed (colors/sizes set in initializer list)
}

void Player::handleInput() {
    bool pitching = false;
    bool rolling = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        float pIn = -rotationSpeed * 0.5f;
        rotation.x += pIn * cos(rotation.z);
        rotation.y += pIn * sin(rotation.z);
        if (visualRotation.x > -maxPitch) visualRotation.x += pIn;
        pitching = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        float pIn = rotationSpeed * 0.5f;
        rotation.x += pIn * cos(rotation.z);
        rotation.y += pIn * sin(rotation.z);
        if (visualRotation.x < maxPitch) visualRotation.x += pIn;
        pitching = true;
    }

    // Keep world Pitch in [-PI, PI] boundary
    rotation.x = std::fmod(rotation.x, 2 * M_PI);
    if (rotation.x > M_PI) rotation.x -= 2 * M_PI;
    if (rotation.x < -M_PI) rotation.x += 2 * M_PI;

    if (!pitching) {
        if (visualRotation.x > 0.001f) visualRotation.x -= 0.005f;
        if (visualRotation.x < -0.001f) visualRotation.x += 0.005f;
        
        // Auto-level world pitch smoothly to the horizon
        if (rotation.x > 0.001f) rotation.x -= 0.0025f;
        if (rotation.x < -0.001f) rotation.x += 0.0025f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        rotation.y += rotationSpeed * 0.5f; // Turn Right (Positive Yaw)
        rotation.z += rotationSpeed; // Bank Right (Roll Camera)
        if (visualRotation.z < maxRoll) visualRotation.z += rotationSpeed; 
        rolling = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        rotation.y -= rotationSpeed * 0.5f; // Turn Left (Negative Yaw)
        rotation.z -= rotationSpeed; // Bank Left (Roll Camera)
        if (visualRotation.z > -maxRoll) visualRotation.z -= rotationSpeed;
        rolling = true;
    }

    // Keep world Roll in [-PI, PI] boundary
    rotation.z = std::fmod(rotation.z, 2 * M_PI);
    if (rotation.z > M_PI) rotation.z -= 2 * M_PI;
    if (rotation.z < -M_PI) rotation.z += 2 * M_PI;

    if (!rolling) {
        if (visualRotation.z > 0.001f) visualRotation.z -= 0.01f;
        if (visualRotation.z < -0.001f) visualRotation.z += 0.01f;
        
        // Auto-level world roll smoothly to the horizon
        if (rotation.z > 0.001f) rotation.z -= 0.005f;
        if (rotation.z < -0.001f) rotation.z += 0.005f;
    }

    // Temporary Speed Control (Q/E) Flipped!
    bool speedAction = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) { // Speed up
        speed += 0.005f;
        if (speed > 0.3f) speed = 0.3f;
        speedAction = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { // Slow down
        speed -= 0.005f;
        if (speed < 0.02f) speed = 0.02f;
        speedAction = true;
    }

    if (!speedAction) {
        float baselineSpeed = 0.1f;
        if (speed > baselineSpeed + 0.002f) speed -= 0.002f;
        else if (speed < baselineSpeed - 0.002f) speed += 0.002f;
        else speed = baselineSpeed;
    }
}

void Player::update(float deltaTime) {
    // Move Forward based on Rotation
    // Forward Vector calculation:
    // X = sin(Yaw) * cos(Pitch)
    // Y = -sin(Pitch)
    // Z = cos(Yaw) * cos(Pitch)

    sf::Vector3f forward(
        sin(rotation.y) * cos(rotation.x),
        -sin(rotation.x),
        cos(rotation.y) * cos(rotation.x)
    );

    // Player translates forward naturally
    position += forward * speed;

    // Map speed (0.02 to 0.3) to Z offset 
    float targetZ = 10.0f + (speed - 0.1f) * 20.0f;
    sf::Vector3f staticHudOffset(0.0f, 0.4f, targetZ);

    // Apply speed shake if going fast (reduced jittle)
    if (speed > 0.15f) {
        float shakeIntensity = (speed - 0.15f) * 0.25f; // Decreased shake
        staticHudOffset.x += ((rand() % 100) / 100.0f - 0.5f) * shakeIntensity;
        staticHudOffset.y += ((rand() % 100) / 100.0f - 0.5f) * shakeIntensity;
    }

    wing.setAbsoluteTransform(staticHudOffset, visualRotation);
    tail.setAbsoluteTransform(staticHudOffset, visualRotation);

    // Speed coloring for the tail (Triangle)
    sf::Color tailColor(0, 255, 0); // default green
    if (speed > 0.1f) {
        // Interpolate Green - Red
        float t = (speed - 0.1f) / 0.2f; // 0 to 1
        if (t > 1.0f) t = 1.0f;
        tailColor.r = static_cast<sf::Uint8>(255 * t);
        tailColor.g = static_cast<sf::Uint8>(255 * (1.0f - t));
        tailColor.b = 0;
    } else if (speed < 0.1f) {
        // Interpolate Green - Blue
        float t = (0.1f - speed) / 0.08f; // 0 to 1
        if (t > 1.0f) t = 1.0f;
        tailColor.r = 0;
        tailColor.g = static_cast<sf::Uint8>(255 * (1.0f - t));
        tailColor.b = static_cast<sf::Uint8>(255 * t);
    }
    tail.setColor(tailColor);
}

void Player::draw(sf::RenderWindow& window, const Camera& camera) {
    wing.draw(window, camera);
    tail.draw(window, camera);
}

bool Player::checkCollision(float wavesHeight) const {
    // Check if y is below waves height + buffer
    return position.y < (wavesHeight + 2.0f);
}

sf::Vector3f Player::getPosition() const {
    return position;
}

sf::Vector3f Player::getRotation() const {
    return rotation;
}

void Player::setPosition(const sf::Vector3f& pos) {
    position = pos;
}

void Player::setRotation(const sf::Vector3f& rot) {
    rotation = rot;
}

sf::Vector3f Player::getForwardVector() const {
    return sf::Vector3f(
        sin(rotation.y) * cos(rotation.x),
        -sin(rotation.x),
        cos(rotation.y) * cos(rotation.x)
    );
}

float Player::getSpeed() const {
    return speed;
}
