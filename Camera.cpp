#include "Camera.h"

#include <cmath>
#include <iostream>
using namespace std;

Camera::Camera(sf::Vector3f initialPos, sf::Vector3f initialRot, float moveSpd, float rotSpd)
    : position(initialPos), rotation(initialRot), moveSpeed(moveSpd),moveMultiplier(1.0f), rotationSpeed(rotSpd)
    , wToggle(false), sToggle(false), aToggle(false), dToggle(false), gameOver(false) {}

void Camera::handleInput() {
    // Calculate forward vector based on Yaw and Pitch
    sf::Vector3f forward(
        sin(rotation.y) * cos(rotation.x),
        -sin(rotation.x),
        cos(rotation.y) * cos(rotation.x)
    );

    // Auto-move forward
    position -= forward * moveSpeed * moveMultiplier;
    
    // Check ground collision
    if (position.y < 0.0f) {
         // position.y = 0.0f; // Optional: Slide on ground
         gameOver = true;
         cout << "Game Over" << endl;
    }
}
sf::Vector3f Camera::getPosition() const {
    return position;
}

sf::Vector3f Camera::getRotation() const {
    return rotation;
}

float Camera::getmoveMultiplier() const {
    return moveMultiplier;
}

bool Camera::getGameOver() const {
    return gameOver;
}

void Camera::setGameOver(bool gameOver) {
    this->gameOver = gameOver;
}

void Camera::setPosition(sf::Vector3f newPosition) {
    position = newPosition;
}

void Camera::setRotation(sf::Vector3f newRotation) {
    rotation = newRotation;
}
