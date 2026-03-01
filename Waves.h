#ifndef WAVES_H
#define WAVES_H

#include <SFML/Graphics.hpp>
#include <random>


class Waves {
public:
    Waves(int numRows, int numCols, float spacing, float cubeSize);
    void rotate(const sf::Vector3f& rotation);
    void translate(const sf::Vector3f& translation);
    void draw(sf::RenderWindow& window, const class Camera& camera);
    void getPosition();
    void setPosition(const sf::Vector3f& position);
    bool isWithinScreenBounds(float x, float y, const sf::Vector2u& screenSize);
    void setColor(const sf::Color& color);
    void update(float deltaTime);
    sf::Color lerpColor(const sf::Color& color1, const sf::Color& color2, float t);
    void setWaveAmplitude(float amplitude);
    void setWaveFrequency(float frequency);
    void setWaveSpeed(float speed);

private:
    std::vector<sf::Vector3f> wavesVertices;
    std::vector<std::pair<int, int>> wavesEdges;
    sf::Vector3f wavesPosition;
    sf::Color wavesColor;
    sf::Color vertexColor;
    float waveAmplitude;
    float waveFrequency;
    float waveSpeed;
    sf::Clock clock;
    sf::Color waveColorStart;
    sf::Color waveColorEnd;
    std::vector<sf::Color> vertexColors;

    

    
};

#endif // WAVES_H
