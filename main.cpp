#include "Camera.h"
#include "Waves.h"
#include "Player.h"
#include "RotatingTriangle.h"
#include "RotatingRectangle.h"
#include "Stars.h"

#include <ctime>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

Color generateRainbowColor(int increment, float speed) {
    int red = static_cast<int>(sin(speed * 0.3 * increment + 0) * 127 + 128);
    int green = static_cast<int>(sin(speed * 0.3 * increment + 2) * 127 + 128);
    int blue = static_cast<int>(sin(speed * 0.3 * increment + 4) * 127 + 128);

    return Color(red, green, blue);
}



int main() {
    srand(time(NULL));

    Clock songSwitchTimer;
    Time songSwitchInterval = seconds(rand() % 31 + 30); // Random interval between 30 and 60 seconds
    Clock stopwatch;

    Clock clock;
    float deltaTime;

    ContextSettings antialiasing;
    antialiasing.antialiasingLevel = 8;
    RenderWindow window(VideoMode(800, 600), "AirbonHarmony", Style::Default, antialiasing);



    float moveSpeed = 0.1f;
    float rotationSpeed = 0.03f;


    vector<String> songList = { "Aerial Jingles", "Low Cloud Beat", "Jungle Rumble" };
    std::string currentSongTitle = "Song: ";

    // Create a waves of cubes using the Waves class
    Waves waves(100, 100, 0.0f, 10.0f); // 5 rows, 5 columns, 2.0f spacing, 1.0f cube size
    Vector3f wavesPosition(5.0f, 0.0f, 5.0f);
    waves.setPosition(wavesPosition);

    Waves mainMenuWaves(100, 100, 0.0f, 10.0f);
    Vector3f mainMenuWavesPosition(50.0f, 0.0f, 0.0f);
    mainMenuWaves.setPosition(mainMenuWavesPosition);
    mainMenuWaves.setWaveAmplitude(1.0f);
    mainMenuWaves.setWaveFrequency(1.0f);
    mainMenuWaves.setWaveSpeed(1.0f);

    bool mainMenu = true; //debugging
    cout << "Press Enter to start" << endl;


    Music mainMenuMusic;
    if (!mainMenuMusic.openFromFile("contents/menu_music.ogg")) {
        cout << "Error loading music: " << mainMenuMusic.getStatus() << endl;
    }
    mainMenuMusic.setLoop(true);
    mainMenuMusic.play();

    SoundBuffer spookBuffer;
    if (!spookBuffer.loadFromFile("contents/spook wind.wav")) {
        cout << "Error loading sound" << endl;
    }
    Sound spookSound;
    spookSound.setBuffer(spookBuffer);
    spookSound.setVolume(100);
    spookSound.setLoop(true);

    Music gameMusic1;
    if (!gameMusic1.openFromFile("contents/Song1Game.wav")) {
        cout << "Error loading music: " << gameMusic1.getStatus() << endl;
    }
    gameMusic1.setLoop(true);
    gameMusic1.setVolume(100);

    Music gameMusic2;
    if (!gameMusic2.openFromFile("contents/Song2Game.wav")) {
        cout << "Error loading music: " << gameMusic2.getStatus() << endl;
    }
    gameMusic2.setLoop(true);
    gameMusic2.setVolume(100);

    Music gameMusic3;
    if (!gameMusic3.openFromFile("contents/Song3Game.wav")) {
        cout << "Error loading music: " << gameMusic3.getStatus() << endl;
    }
    gameMusic3.setLoop(true);
    gameMusic3.setVolume(100);

    Camera camera(Vector3f(0.0f, 50.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f), moveSpeed, rotationSpeed);
    Camera mainMenuCamera(Vector3f(0.0f, 50.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f), moveSpeed, rotationSpeed); //
    Vector3f mainMenuCameraPosition = mainMenuCamera.getPosition();
    Vector3f mainMenuCameraRotation = mainMenuCamera.getRotation();

    window.setFramerateLimit(60);

    Font font;
    if (!font.loadFromFile("contents/Freedom.ttf")) {
        cout << "Error loading font" << endl;
    }

    Font timerFont;
    if (!timerFont.loadFromFile("contents/PaulFont.ttf")) {
        cout << "Error loading font" << endl;
    }

    // Title text
    Text titleText;
    titleText.setFont(font);
    titleText.setString("Airborne Harmony");
    titleText.setCharacterSize(60);
    titleText.setFillColor(Color::White);
    titleText.setPosition(200, 100);

    // "Press Enter to play" button text
    Text playText;
    playText.setFont(font);
    playText.setString("Press Enter to play");
    playText.setCharacterSize(30);
    playText.setFillColor(Color::White);
    playText.setOrigin(playText.getLocalBounds().width / 2.0f, playText.getLocalBounds().height / 2.0f);
    playText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    Text controlsText;
    controlsText.setFont(font);
    controlsText.setString("Hold C for Controls");
    controlsText.setCharacterSize(30);
    controlsText.setFillColor(Color::White);
    controlsText.setOutlineColor(Color::Red);
    controlsText.setOutlineThickness(2.0f);
    controlsText.setOrigin(controlsText.getLocalBounds().width / 2.0f, controlsText.getLocalBounds().height / 2.0f);
    controlsText.setPosition(window.getSize().x / 2.0f + (window.getSize().x / 4.0f) , window.getSize().y / 2.0f +window.getSize().y / 3.0f );

    Text controls;
    controls.setFont(font);
    controls.setString("W to move up\n S to move down \n A to move left \n D to move right \n Q to speed up \n E to slow down \n Enter to start game \n Esc to exit \n All music is created by me");
    controls.setCharacterSize(30);
    controls.setFillColor(Color::Black);
    controls.setOutlineColor(Color::Green);
    controls.setOutlineThickness(2.0f);
    controls.setOrigin(controls.getLocalBounds().width / 2.0f, controls.getLocalBounds().height / 2.0f);
    controls.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    Text description;
    description.setFont(font);
    description.setString("Flight simulator to help you relax \n with calming music and visuals");
    description.setCharacterSize(30);
    description.setFillColor(Color::White);
    description.setOutlineColor(Color::Red);
    description.setOutlineThickness(2.0f);
    description.setOrigin(description.getLocalBounds().width / 2.0f, description.getLocalBounds().height / 2.0f);
    description.setPosition(window.getSize().x / 2.0f, window.getSize().y / 7.0f);

    Text currentSongText;
    currentSongText.setFont(font);
    currentSongText.setString("Now Playing: " + currentSongTitle);
    currentSongText.setCharacterSize(20);
    currentSongText.setFillColor(Color::White);
    currentSongText.setPosition(window.getSize().x - currentSongText.getLocalBounds().width - 10, 10);
    


    float scaleFactor = 1.0f;
    bool breathingIn = true;
    int colorIncrement = 1;

    Player player;
    Stars stars(1500, 2000.0f); // Generate 1500 stars evenly in the sky

    Text stopwatchText;
    stopwatchText.setFont(timerFont);
    stopwatchText.setCharacterSize(20);
    stopwatchText.setFillColor(Color::Red);
    stopwatchText.setPosition(10, 10); 
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Enter) {
                    if (mainMenu) {
                        songSwitchTimer.restart();
                        //choose random song to play
                        int songNumber = rand() % 3 + 1;
                        switch (songNumber) {
                            case 1:
                                gameMusic1.play();
                                gameMusic1.setVolume(100);
                                currentSongTitle = songList[0];
                                break;
                            case 2:
                                gameMusic2.play();
                                gameMusic2.setVolume(100);
                                currentSongTitle = songList[1];
                                break;
                            case 3:
                                gameMusic3.play();
                                gameMusic3.setVolume(100);
                                currentSongTitle = songList[2];
                                break;
                        }

                        mainMenu = false;
                        spookSound.play();
                        //gradually stop music and darken screen using a giant square that darkens up on the screen
                        for (int i = 0; i < 100; i++) {
                            window.clear();
                            sf::Vector3f pos = mainMenuCamera.getPosition();
                            pos.z += 0.0050f;
                            mainMenuCamera.setPosition(pos);

                            stars.draw(window, mainMenuCamera); // Draw stars in main menu transition
                            mainMenuWaves.draw(window, mainMenuCamera);
                            
                            mainMenuMusic.setVolume(100-i);
                            spookSound.setVolume(i);
                            window.draw(titleText);
                            window.draw(playText);
                            RectangleShape darkenScreen(Vector2f(window.getSize().x, window.getSize().y));
                            darkenScreen.setFillColor(Color(0, 0, 0, i*2.55f));
                            window.draw(darkenScreen);
                            window.display();
                        }
                        stopwatch.restart();
                        mainMenuMusic.stop();
                        
                    } else {
                        mainMenu = true;
                        mainMenuMusic.play();
                        mainMenuMusic.setVolume(100);
                    }
                }
            }
        }

        if (mainMenu) {
            Time frameTime = clock.restart();
            deltaTime = frameTime.asSeconds();
            window.clear();

            if (breathingIn) {
                scaleFactor += 0.0005f;
                if (scaleFactor >= 1.1f) {
                    breathingIn = false;
                }
            } else {
                scaleFactor -= 0.0005f;
                if (scaleFactor <= 1.05f) {
                    breathingIn = true;
                }
            }

            FloatRect textBounds = titleText.getLocalBounds();
            float xPos = (window.getSize().x) / 2.0f;
            float yPos = (window.getSize().y) / 2.0f;
            titleText.setPosition(xPos, yPos-window.getSize().y/3.0f);
            //set origin to center of text
            titleText.setOrigin(textBounds.width/2.0f, textBounds.height/2.0f);

            titleText.setOutlineThickness(2);
            titleText.setOutlineColor(generateRainbowColor(colorIncrement,0.1f));
            playText.setScale(scaleFactor, scaleFactor);
            mainMenuWaves.setColor(generateRainbowColor(colorIncrement, 0.1f));
            
            sf::Vector3f pos = mainMenuCamera.getPosition();
            pos.z += 0.0050f;
            mainMenuCamera.setPosition(pos);

            stars.draw(window, mainMenuCamera); // Draw stars in main menu
            mainMenuWaves.draw(window, mainMenuCamera);
            mainMenuWaves.update(deltaTime);
            
            colorIncrement++;
            
            window.draw(titleText);
            window.draw(playText);
            window.draw(controlsText);

            if (Keyboard::isKeyPressed(Keyboard::C)) {
                window.clear();
                
                window.draw(controls);
                window.draw(controlsText);
                window.draw(description);
            }
            
            window.display();
        } else if (!mainMenu) {

            //switch between 3 songs randomly every 30 seconds to a minute
            // Check if it's time to switch the song
            
            if (songSwitchTimer.getElapsedTime() >= songSwitchInterval) {
                // Choose a random song
                int songNumber = rand() % 3 + 1;
                switch (songNumber) {
                    case 1:
                        gameMusic1.play();
                        gameMusic2.stop();
                        gameMusic3.stop();
                        break;
                    case 2:
                        gameMusic1.stop();
                        gameMusic2.play();
                        gameMusic3.stop();
                        break;
                    case 3:
                        gameMusic1.stop();
                        gameMusic2.stop();
                        gameMusic3.play();
                        break;
                }

                // Reset the timer and choose a new random interval
                songSwitchTimer.restart();
                songSwitchInterval = seconds(rand() % 31 + 30);
            }

            Time elapsedTime = stopwatch.getElapsedTime();
            int seconds = static_cast<int>(elapsedTime.asSeconds());
            int minutes = seconds / 60;
            seconds %= 60;

            // Format the time as "MM:SS"
            std::string timeString = (minutes < 10 ? "0" : "") + std::to_string(minutes) +
                                     ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);

            stopwatchText.setString("Time: " + timeString);
 
            window.clear();

            Vector3f cameraPosition = camera.getPosition(); // Get current camera pos (if modified by something else)
            // But we want Chase Camera logic.
            // Update Player
            player.handleInput();
            player.update(deltaTime);

            // Dynamically adjust the pitch of the songs based on speed
            float speedPitch = 1.0f + (player.getSpeed() - 0.1f) * 2.5f;
            gameMusic1.setPitch(speedPitch);
            gameMusic2.setPitch(speedPitch);
            gameMusic3.setPitch(speedPitch);

            // Start collision check
            float wavesHeight = 10.0f; // Cube size is 10.0f, so y=10 is approx surface if cubes are at y=0 base
            if (player.checkCollision(0.0f)) {
                 camera.setGameOver(true);
            }

            // Calculate Chase Camera Position
            sf::Vector3f playerPos = player.getPosition();
            sf::Vector3f playerRot = player.getRotation();

            sf::Vector3f forward = player.getForwardVector();

            // Stationary camera behind the player
            float distance = 10.0f; 
            float height = 4.0f; 
            
            sf::Vector3f chasePos = playerPos - (forward * distance);
            chasePos.y += height;

            camera.setPosition(chasePos);
            // Lock camera rotation to follow Player entirely (rigid frame orbit)
            camera.setRotation(playerRot);

            waves.update(deltaTime);

            stars.draw(window, camera);
            waves.draw(window, camera);
            
            player.draw(window, camera);

            window.draw(stopwatchText);

            if (camera.getGameOver()) {
                window.clear();
                Text gameOverText;
                gameOverText.setFont(font);
                gameOverText.setString("Game Over");
                gameOverText.setCharacterSize(60);
                gameOverText.setFillColor(Color::White);
                gameOverText.setPosition(200, 100);
                window.draw(gameOverText);
                window.draw(currentSongText);
                window.display();
                //slowly fade music and stop it
                for (int i = 0; i < 100; i++) {
                    gameMusic1.setVolume(100-i);
                    gameMusic2.setVolume(100-i);
                    gameMusic3.setVolume(100-i);
                    //sleep for 0.1 seconds
                    sleep(milliseconds(10));
                }
                gameMusic1.stop();
                gameMusic2.stop();
                gameMusic3.stop();
                //press enter to restart text
                Text restartText;
                restartText.setFont(font);
                restartText.setString("Press Enter to restart");
                restartText.setCharacterSize(30);
                restartText.setFillColor(Color::White);
                restartText.setOrigin(restartText.getLocalBounds().width / 2.0f, restartText.getLocalBounds().height / 2.0f);
                restartText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
                window.draw(restartText);
                //wait for enter to be pressed
                while(!Keyboard::isKeyPressed(Keyboard::Enter)) {
                    window.display();
                    
                }
        
                if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                
                camera.setGameOver(false);
                stopwatch.restart();
                // Reset Player
                player.setPosition(Vector3f(0.0f, 50.0f, 0.0f));
                player.setRotation(Vector3f(0.0f, 0.0f, 0.0f));
                
                //gradually stop music and darken screen using a giant square that darkens up on the screen
                    for (int i = 0; i < 100; i++) {
                        window.clear();
                        sf::Vector3f pos = mainMenuCamera.getPosition();
                        pos.z += 0.0050f;
                        mainMenuCamera.setPosition(pos);

                        stars.draw(window, mainMenuCamera);
                        mainMenuWaves.draw(window, mainMenuCamera);
                        
                        spookSound.setVolume(i);
                    }
                    mainMenuMusic.stop();
                    spookSound.stop();
                //sleep
                sleep(milliseconds(100));
                    
                }  
            }
        window.display();
        }
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    }
    return 0;
}