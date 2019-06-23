#include <SFML/Graphics.hpp>
#include <iostream>
#include <x86intrin.h>

#include "background.h"
#include "following-background.h"
#include "world.h"
#include "system.h"
#include "controller.h"

#define CONTROLLER_AXIS_DEADZONE 20.0f
#define MAX_CONTROLLERS 4

void updateBackground(float delta, Background *background) {
    background->testUpdate2(delta);
    background->draw();
}

void updateFollowingBackground(FollowingBackground *background) {
    background->draw();
}

static void SFMLProcessGameControllerButton(GameButtonState *oldState, GameButtonState *newState, bool value) {
    newState->endedDown = value;
    newState->halfTransitionCount += ((newState->endedDown == oldState->endedDown) ? 0 : 1);
}

static float SFMLProcessGameControllerAxis(float value) {
    if (value < -CONTROLLER_AXIS_DEADZONE || value > CONTROLLER_AXIS_DEADZONE) {
        return value;
    }
    return 0.0f;
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

    for (int i = 0; i < MAX_CONTROLLERS; i++) {
        if (sf::Joystick::isConnected(i)) {
            UNTITLED_CONTROLLER_H::setButtons(&window, i, newInput, oldInput);
        }
    }

    ulong total = 0;
    ulong loops = 0;
    ulong result = 0;
    while (window.isOpen() && loops < 7000) {
        sf::Event event;

        timeSinceLastUpdate += tickClock.restart();
        while (timeSinceLastUpdate >= timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;

            sf::Joystick::update();

            for (int i = 0; i < MAX_CONTROLLERS; i++) {
                if (sf::Joystick::isConnected(i)) {
                    // Controller button input
                    for (int j = 0; j < 10; j++) {
                        if (oldInput->buttons[i][j] != nullptr && newInput->buttons[i][j] != nullptr) {
                            SFMLProcessGameControllerButton(oldInput->buttons[i][j], newInput->buttons[i][j],
                                                            sf::Joystick::isButtonPressed(i, j));
                        }
                    }

                    // Dpad input
                    SFMLProcessGameControllerButton(&oldInput->controllers[i].moveUp, &newInput->controllers[i].moveUp,
                                                    (sf::Joystick::getAxisPosition(i, sf::Joystick::PovY) < 0));
                    SFMLProcessGameControllerButton(&oldInput->controllers[i].moveDown, &newInput->controllers[i].moveDown,
                                                    (sf::Joystick::getAxisPosition(i, sf::Joystick::PovY) > 0));
                    SFMLProcessGameControllerButton(&oldInput->controllers[i].moveLeft, &newInput->controllers[i].moveLeft,
                                                    (sf::Joystick::getAxisPosition(i, sf::Joystick::PovX) < 0));
                    SFMLProcessGameControllerButton(&oldInput->controllers[i].moveRight, &newInput->controllers[i].moveRight,
                                                    (sf::Joystick::getAxisPosition(i, sf::Joystick::PovX) > 0));

                    // Analog stick input

                    newInput->controllers[i].stickAverageX = SFMLProcessGameControllerAxis(
                            sf::Joystick::getAxisPosition(i, sf::Joystick::X));
                    newInput->controllers[i].stickAverageY = SFMLProcessGameControllerAxis(
                            sf::Joystick::getAxisPosition(i, sf::Joystick::Y));

                    newInput->controllers[i].stickAverageU = SFMLProcessGameControllerAxis(
                            sf::Joystick::getAxisPosition(i, sf::Joystick::U));
                    newInput->controllers[i].stickAverageV = SFMLProcessGameControllerAxis(
                            sf::Joystick::getAxisPosition(i, sf::Joystick::V));

                    // Analog trigger input
                    newInput->controllers[i].stickAverageR = SFMLProcessGameControllerAxis(
                            sf::Joystick::getAxisPosition(i, sf::Joystick::R));
                    newInput->controllers[i].stickAverageZ = SFMLProcessGameControllerAxis(
                            sf::Joystick::getAxisPosition(i, sf::Joystick::Z));

                    GameControllerInput *newController = &newInput->controllers[i];
                    if (newController->actionUp.endedDown)
                        printf("CONTROLLER %d action up\n", i);

                    if (newController->actionDown.endedDown)
                        printf("CONTROLLER %d action down\n", i);

                    if (newController->actionLeft.endedDown)
                        printf("CONTROLLER %d action left\n", i);

                    if (newController->actionRight.endedDown)
                        printf("CONTROLLER %d action right\n", i);

                    if (newController->leftShoulder.endedDown)
                        printf("CONTROLLER %d left shoulder\n", i);

                    if (newController->rightShoulder.endedDown)
                        printf("CONTROLLER %d right shoulder\n", i);

                    if (newController->back.endedDown)
                        printf("CONTROLLER %d back\n", i);

                    if (newController->start.endedDown)
                        printf("CONTROLLER %d start\n", i);


                    if (newController->moveUp.endedDown)
                        printf("CONTROLLER %d up\n", i);

                    if (newController->moveDown.endedDown)
                        printf("CONTROLLER %d down\n", i);

                    if (newController->moveLeft.endedDown)
                        printf("CONTROLLER %d left\n", i);

                    if (newController->moveRight.endedDown)
                        printf("CONTROLLER %d right\n", i);
/*
                    printf("axis x: %f\n", newInput->controllers.stickAverageX);
                    printf("axis y: %f\n", newInput->controllers.stickAverageY);

                    printf("axis u: %f\n", newInput->controllers.stickAverageU);
                    printf("axis v: %f\n", newInput->controllers.stickAverageV);

                    printf("axis r: %f\n", newInput->controllers.stickAverageR);
                    printf("axis z: %f\n", newInput->controllers.stickAverageZ);
*/
                }
            }

            window.clear();
            updateFollowingBackground(&followingBackground);
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