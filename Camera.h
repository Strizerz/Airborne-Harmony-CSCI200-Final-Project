#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

class Camera {
private:
    sf::Vector3f position;
    sf::Vector3f rotation;
    float moveSpeed;
    float rotationSpeed;
    float moveMultiplier;
    bool wToggle;
    bool sToggle;
    bool aToggle;
    bool dToggle;
    bool gameOver;
    

public:
    Camera(sf::Vector3f initialPos, sf::Vector3f initialRot, float moveSpd, float rotSpd);

    void setPosition(sf::Vector3f newPosition);
    void setRotation(sf::Vector3f newRotation);

    float getmoveMultiplier() const;
    bool getGameOver() const;
    void setGameOver(bool gameOver);

    void handleInput();

    sf::Vector3f getPosition() const;

    sf::Vector3f getRotation() const;
};

#endif
