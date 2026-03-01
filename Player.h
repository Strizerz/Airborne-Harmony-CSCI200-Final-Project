#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "RotatingRectangle.h"
#include "RotatingTriangle.h"
#include "Camera.h"

class Player {
public:
    Player();
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window, const Camera& camera);
    void handleInput();

    sf::Vector3f getPosition() const;
    sf::Vector3f getRotation() const; // X=Pitch, Y=Yaw, Z=Roll
    sf::Vector3f getForwardVector() const;
    float getSpeed() const;

    void setPosition(const sf::Vector3f& pos);
    void setRotation(const sf::Vector3f& rot);

private:
    sf::Vector3f position;
    sf::Vector3f rotation; // Pitch (X), Yaw (Y), Roll (Z - Visual only mostly)
    sf::Vector3f visualRotation; // For HUD ship graphic limits
    
    float speed;
    float rotationSpeed;
    float maxPitch;
    float maxRoll;
    // Model Parts
    RotatingRectangle wing;
    // RotatingRectangle body;
    RotatingTriangle tail;

    float maxYaw; 

public: 
    bool checkCollision(float wavesHeight) const;

};

#endif // PLAYER_H
