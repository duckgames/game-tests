#include <SFML/Graphics.hpp>
#include <iostream>
#include <x86intrin.h>

#include "background.h"
#include "following-background.h"
#include "world.h"
#include "system.h"
#include "controller.h"

#define CONTROLLER_AXIS_DEADZONE 20.0f

#define CONTROLLER_360_A 0

void updateBackground(float delta, Background *background, sf::RenderWindow *window) {
    background->testUpdate2(delta);
    background->draw();
}

void updateFollowingBackground(FollowingBackground *background, sf::RenderWindow *window) {
    background->draw();
}

static void SFMLProcessGameControllerButton(GameButtonState *oldState, GameButtonState *newState, bool value) {
    newState->endedDown = value;
    newState->halfTransitionCount += ((newState->endedDown == oldState->endedDown) ? 0 : 1);
}

int main() {
    float stickAverageX = 0.0f;
    float stickAverageY = 0.0f;
    float rotation = 0.0f;
    float speed = 1000.0f;
    unsigned int screenHeight = 1080;

    World world;
    System system(&world);

    sf::Texture specialBrew;
    sf::RenderWindow window(sf::VideoMode(1920, screenHeight), "SFML works!");
    window.setVerticalSyncEnabled(true);

    specialBrew.loadFromFile("specialbrew.png");

    sf::RectangleShape spritealBrew(sf::Vector2f(32, 32));
    spritealBrew.setTexture(&specialBrew, true);

  //  sf::Vector2f position = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);
    spritealBrew.setPosition(window.getSize().x / 2, screenHeight - specialBrew.getSize().y);

    unsigned int jumper = world.createJumper(50.0f, 200.0f, 200.0f, spritealBrew);

    sf::Clock tickClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    sf::Time duration = sf::Time::Zero;

	sf::Texture vertTexture;
	sf::Texture horizTexture;
	vertTexture.loadFromFile("../assets/backgrounds/background.png");
	horizTexture.loadFromFile("../assets/backgrounds/background-horiz.png");
/*
    Background background = Background(
			&window,
			&vertTexture,
            sf::Vector2f(window.getSize().x / 2, 0.0f),
            -500.0f,
			Y_AXIS);

	Background backgroundHoriz = Background(
			&window,
			&horizTexture,
			sf::Vector2f(0.0f, window.getSize().y / 2),
			-500.0f,
			X_AXIS);
*/

    FollowingBackground followingBackground = FollowingBackground(
            &window,
            &horizTexture,
            -500.0f,
            screenHeight);

    GameInput input[2] = {};
    GameInput *newInput = &input[0];
    GameInput *oldInput = &input[1];

    UNTITLED_CONTROLLER_H::calibrate(&window);

    printf("Action Up: %d\n", BUTTON_ACTION_UP);
    printf("Action Down: %d\n", BUTTON_ACTION_DOWN);
    printf("Action Left: %d\n", BUTTON_ACTION_LEFT);
    printf("Action Right: %d\n", BUTTON_ACTION_RIGHT);

    printf("Move Up: %d\n", BUTTON_MOVE_UP);
    printf("Move Down: %d\n", BUTTON_MOVE_DOWN);
    printf("Move Left: %d\n", BUTTON_MOVE_LEFT);
    printf("Move Right: %d\n", BUTTON_MOVE_RIGHT);

    printf("Left Shoulder: %d\n", BUTTON_LEFT_SHOULDER);
    printf("Right Shoulder: %d\n", BUTTON_RIGHT_SHOULDER);

    printf("Back: %d\n", BUTTON_BACK);
    printf("Start: %d\n", BUTTON_START);

	ulong total = 0;
	ulong loops = 0;
	ulong result = 0;
    while (window.isOpen() && loops < 7000)
    {
        sf::Event event;

        GameControllerInput *oldController = &oldInput->controller;
        GameControllerInput *newController = &newInput->controller;

        SFMLProcessGameControllerButton(&(oldController->actionUp), &(newController->actionUp), sf::Joystick::isButtonPressed(0, CONTROLLER_360_A));
        SFMLProcessGameControllerButton(&(oldController->actionDown), &(newController->actionDown), sf::Joystick::isButtonPressed(0, 1));
        SFMLProcessGameControllerButton(&(oldController->actionLeft), &(newController->actionLeft), sf::Joystick::isButtonPressed(0, 2));
        SFMLProcessGameControllerButton(&(oldController->actionRight), &(newController->actionRight), sf::Joystick::isButtonPressed(0, 3));

        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                    else if (event.key.code == sf::Keyboard::Space) {
                        if (!world.jump[jumper].isJumping && !world.jump[jumper].isFalling) {
                            world.jump[jumper].isJumping = true;
                        }
                    }
                    break;

                case sf::Event::JoystickConnected:
                    break;

                case sf::Event::JoystickDisconnected:
                    break;

                case sf::Event::JoystickMoved:
                    if (event.joystickMove.axis == sf::Joystick::Axis::X) {
                        stickAverageX = event.joystickMove.position;
                    }
                    else if (event.joystickMove.axis == sf::Joystick::Axis::Y) {
                        stickAverageY = event.joystickMove.position;
                    }
                    else if (event.joystickMove.axis == sf::Joystick::Axis::Z) {
                        rotation -= (100 + event.joystickMove.position) / 200;
                    }
                    else if (event.joystickMove.axis == sf::Joystick::Axis::R) {
                        rotation += (100 + event.joystickMove.position) / 200;
                    }
                    break;
            }
        }

        timeSinceLastUpdate += tickClock.restart();
        while (timeSinceLastUpdate >= timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;

            if (stickAverageX > CONTROLLER_AXIS_DEADZONE || stickAverageX < -CONTROLLER_AXIS_DEADZONE) {
                if (stickAverageX < 0) {
                //    position.x += -speed * timePerFrame.asSeconds();
                    followingBackground.moveX(timePerFrame.asSeconds(), false);
                }
                else {
                //    position.x += speed * timePerFrame.asSeconds();
                    followingBackground.moveX(timePerFrame.asSeconds(), true);
                }
            }

            if (stickAverageY > CONTROLLER_AXIS_DEADZONE || stickAverageY < -CONTROLLER_AXIS_DEADZONE) {
                if (stickAverageY < 0) {
                //    position.y += -speed * timePerFrame.asSeconds();
                    followingBackground.moveY(timePerFrame.asSeconds(), false);
                }
                else {
                //    position.y += speed * timePerFrame.asSeconds();
                    followingBackground.moveY(timePerFrame.asSeconds(), true);
                }
            }

            window.clear();
            updateFollowingBackground(&followingBackground, &window);
            system.jumpers(timePerFrame.asSeconds(), &window);
            window.display();

            GameInput *temp = newInput;
            newInput = oldInput;
            oldInput = temp;

            /*
            ulong t1 = __rdtsc();
            ulong t2 = __rdtsc();
            //       background.testUpdate2(timePerFrame.asSeconds());
            ulong t3 = __rdtsc();
            result = (t3 - t2) - (t2 - t1);

            total += result;
            loops++;

            //		backgroundHoriz.update(timePerFrame.asSeconds());
            //       spritealBrew.setPosition(position);

            window.clear();
            //       background.draw();
            //       backgroundHoriz.draw();
            followingBackground.draw();
            window.draw(spritealBrew);
            window.display();
             */
        }

	//	std::cout << total / loops << std::endl;
	}

    return 0;
}