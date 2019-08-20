#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#include "world.h"
#include "system.h"
#include "controller.h"

#define CONTROLLER_AXIS_DEADZONE 20.0f
#define MAX_CONTROLLERS 4

static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;

sf::Texture textureAtlas;

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

static void requestButtonPress(sf::RenderWindow *window, int controllerNumber, GameInput *input, GameInput *oldInput, GameButtonState *gameButtonState, GameButtonState *oldGameButtonState, const char *buttonName) {
    sf::Event event;

    bool waiting = true;
    printf("Press Controller %d %s\n ", controllerNumber, buttonName);
    while (waiting) {
        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::JoystickButtonPressed:
                    if (event.joystickButton.joystickId == controllerNumber) {
                        input->buttons[controllerNumber][event.joystickButton.button] = gameButtonState;
                        oldInput->buttons[controllerNumber][event.joystickButton.button] = oldGameButtonState;
                        waiting = false;
                    }
                    break;
            }
        }
    }
}

static void SFMLSetButtons(sf::RenderWindow *window, int controllerNumber, GameInput *input, GameInput *oldInput) {
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].actionUp, &oldInput->controllers[controllerNumber].actionUp, "Action Up");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].actionDown, &oldInput->controllers[controllerNumber].actionDown, "Action Down");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].actionLeft, &oldInput->controllers[controllerNumber].actionLeft, "Action Left");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].actionRight, &oldInput->controllers[controllerNumber].actionRight, "Action Right");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].leftShoulder, &oldInput->controllers[controllerNumber].leftShoulder, "Left Shoulder");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].rightShoulder, &oldInput->controllers[controllerNumber].rightShoulder, "Right Shoulder");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].back, &oldInput->controllers[controllerNumber].back, "Back");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].start, &oldInput->controllers[controllerNumber].start, "Start");
}


void SFMLRenderDrawables(sf::RenderWindow *window, World *world) {
    sf::Sprite sprite;
    sprite.setTexture(textureAtlas);
    for (auto drawable : world->drawablesMap) {
        Position *position;
        position = &world->positionsMap[drawable.first];

        sprite.setTextureRect(sf::IntRect(
                drawable.second.x,
                drawable.second.y,
                drawable.second.width,
                drawable.second.height
        ));
        sprite.setPosition(position->x, position->y);

        window->draw(sprite);
    }
}

void SFMLRenderHitboxes(sf::RenderWindow *window, World *world, int playerEntity) {
    for (auto entity : world->collideWithPlayer) {
        Collider *collider = &world->collidersMap[entity];

        sf::RectangleShape rectangleShape;
        rectangleShape.setPosition(collider->x, collider->y);
        rectangleShape.setSize(sf::Vector2f(collider->width, collider->height));
        rectangleShape.setFillColor(sf::Color::Transparent);
        rectangleShape.setOutlineColor(sf::Color::Red);
        rectangleShape.setOutlineThickness(2.0f);

        window->draw(rectangleShape);
    }


    sf::RectangleShape rectangleShape;
    rectangleShape.setFillColor(sf::Color::Transparent);
    rectangleShape.setOutlineColor(sf::Color::Green);
    rectangleShape.setOutlineThickness(2.0f);

    for (auto entity : world->collideWithEnemy) {
        Collider *collider = &world->collidersMap[entity];
        rectangleShape.setPosition(collider->x, collider->y);
        rectangleShape.setSize(sf::Vector2f(collider->width, collider->height));

        window->draw(rectangleShape);
    }

    Collider *collider = &world->collidersMap[playerEntity];
    rectangleShape.setPosition(collider->x, collider->y);
    rectangleShape.setSize(sf::Vector2f(collider->width, collider->height));
    window->draw(rectangleShape);
}

int main() {
    World world(SCREEN_WIDTH, SCREEN_HEIGHT);
    System system(&world);

    textureAtlas.loadFromFile("../assets/texture-atlas.png");

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML works!");
    window.setVerticalSyncEnabled(true);

    int player = world.createControllable(window.getSize().x / 2, window.getSize().y - 16, 15.0f, 15.0f);

    int follower1 = world.createPlayerBulletSpawnPoint(player, 16.0f, 0.0f, 0.02f);
    int follower2 = world.createPlayerBulletSpawnPoint(player, -16.0f, 0.0f, 0.02f);
    std::vector<int> playerFollowers;
    playerFollowers.push_back(follower1);
    playerFollowers.push_back(follower2);
    world.addLeaderComponent(player, playerFollowers);

    world.createEnemy(250.0f, 0.0f, 0.0f, 75.0f, 0.2f);
    world.createEnemy(500.0f, 0.0f, 0.0f, 75.0f, 0.5f);

    sf::Clock tickClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    sf::Time duration = sf::Time::Zero;
/*
    sf::Texture vertTexture;
    sf::Texture horizTexture;
    vertTexture.loadFromFile("../assets/backgrounds/background.png");
    horizTexture.loadFromFile("../assets/backgrounds/background-horiz.png");

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


    FollowingBackground followingBackground = FollowingBackground(
            &window,
            &horizTexture,
            -500.0f,
            SCREEN_HEIGHT);
*/
    GameInput input[2] = {};
    GameInput *newInput = &input[0];
    GameInput *oldInput = &input[1];

    for (int i = 0; i < MAX_CONTROLLERS; i++) {
        if (sf::Joystick::isConnected(i)) {
            SFMLSetButtons(&window, i, newInput, oldInput);
        }
    }

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;

        timeSinceLastUpdate += tickClock.restart();
        while (timeSinceLastUpdate >= timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;

            // Check keys which don't map to game controller buttons here
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                    window.close();

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Delete)
                    world.destroyEntity(player);
            }

            // Check keys which map to game controller buttons here
            SFMLProcessGameControllerButton(&oldInput->keyboard.actionUp, &newInput->keyboard.actionUp, sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));
            SFMLProcessGameControllerButton(&oldInput->keyboard.actionDown, &newInput->keyboard.actionDown, sf::Keyboard::isKeyPressed(sf::Keyboard::Space));
            SFMLProcessGameControllerButton(&oldInput->keyboard.actionLeft, &newInput->keyboard.actionLeft, sf::Keyboard::isKeyPressed(sf::Keyboard::LControl));
            SFMLProcessGameControllerButton(&oldInput->keyboard.actionRight, &newInput->keyboard.actionRight, sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt));

            SFMLProcessGameControllerButton(&oldInput->keyboard.moveUp, &newInput->keyboard.moveUp,sf::Keyboard::isKeyPressed(sf::Keyboard::Up));
            SFMLProcessGameControllerButton(&oldInput->keyboard.moveDown, &newInput->keyboard.moveDown, sf::Keyboard::isKeyPressed(sf::Keyboard::Down));
            SFMLProcessGameControllerButton(&oldInput->keyboard.moveLeft, &newInput->keyboard.moveLeft, sf::Keyboard::isKeyPressed(sf::Keyboard::Left));
            SFMLProcessGameControllerButton(&oldInput->keyboard.moveRight, &newInput->keyboard.moveRight, sf::Keyboard::isKeyPressed(sf::Keyboard::Right));

            SFMLProcessGameControllerButton(&oldInput->keyboard.leftShoulder, &newInput->keyboard.leftShoulder, sf::Keyboard::isKeyPressed(sf::Keyboard::Z));
            SFMLProcessGameControllerButton(&oldInput->keyboard.rightShoulder, &newInput->keyboard.rightShoulder, sf::Keyboard::isKeyPressed(sf::Keyboard::X));

            SFMLProcessGameControllerButton(&oldInput->keyboard.back, &newInput->keyboard.back, sf::Keyboard::isKeyPressed(sf::Keyboard::F2));
            SFMLProcessGameControllerButton(&oldInput->keyboard.start, &newInput->keyboard.start, sf::Keyboard::isKeyPressed(sf::Keyboard::F1));

            // Process the arrow keys as if they were a controller axis, so movement related code can treat keyboard input identically.
            // The initial value is zero. If Up or Left are pressed, the respective axis will be set to -100.0f. If Down or Right
            // are pressed, 100.0f will be added to the existing amount (either 0.0f, or -100.0f, depending on whether Up/Left were
            // pressed). As such if Up/Down or Left/Right are both pressed, the value of that axis will be 0.0f.
            float xAxisValue = 0.0f;
            float yAxisValue = 0.0f;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                yAxisValue = -100.0f;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                yAxisValue += 100.0f;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                xAxisValue = -100.0f;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                xAxisValue += 100.0f;
            }

            newInput->keyboard.stickAverageX = xAxisValue;
            newInput->keyboard.stickAverageY = yAxisValue;
/*
            if (newInput->keyboard.moveUp.endedDown)
                printf("KEYBOARD up\n");

            if (newInput->keyboard.moveDown.endedDown)
                printf("KEYBOARD down\n");

            if (newInput->keyboard.moveLeft.endedDown)
                printf("KEYBOARD left\n");

            if (newInput->keyboard.moveRight.endedDown)
                printf("KEYBOARD right\n");
*/
            newInput->mouseX = sf::Mouse::getPosition(window).x;
            newInput->mouseY = sf::Mouse::getPosition(window).y;
            SFMLProcessGameControllerButton(&oldInput->mouseButtons[0], &newInput->mouseButtons[0], sf::Mouse::isButtonPressed(sf::Mouse::Left));
            SFMLProcessGameControllerButton(&oldInput->mouseButtons[1], &newInput->mouseButtons[1], sf::Mouse::isButtonPressed(sf::Mouse::Middle));
            SFMLProcessGameControllerButton(&oldInput->mouseButtons[2], &newInput->mouseButtons[2], sf::Mouse::isButtonPressed(sf::Mouse::Right));

/*
            printf("Mouse x: %d\n", newInput->mouseX);
            printf("Mouse y: %d\n", newInput->mouseY);

            if (newInput->mouseButtons[0].endedDown)
                printf("Mouse 1 down\n");

            if (newInput->mouseButtons[1].endedDown)
                printf("Mouse 2 down\n");

            if (newInput->mouseButtons[2].endedDown)
                printf("Mouse 3 down\n");
*/

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

                    // Dpad input as button
                    SFMLProcessGameControllerButton(&oldInput->controllers[i].moveUp, &newInput->controllers[i].moveUp,
                                                    (sf::Joystick::getAxisPosition(i, sf::Joystick::PovY) < 0));
                    SFMLProcessGameControllerButton(&oldInput->controllers[i].moveDown, &newInput->controllers[i].moveDown,
                                                    (sf::Joystick::getAxisPosition(i, sf::Joystick::PovY) > 0));
                    SFMLProcessGameControllerButton(&oldInput->controllers[i].moveLeft, &newInput->controllers[i].moveLeft,
                                                    (sf::Joystick::getAxisPosition(i, sf::Joystick::PovX) < 0));
                    SFMLProcessGameControllerButton(&oldInput->controllers[i].moveRight, &newInput->controllers[i].moveRight,
                                                    (sf::Joystick::getAxisPosition(i, sf::Joystick::PovX) > 0));

                    // Dpad input as POV axis
                    newInput->controllers[i].povX = SFMLProcessGameControllerAxis(sf::Joystick::getAxisPosition(i, sf::Joystick::PovX));
                    newInput->controllers[i].povY = SFMLProcessGameControllerAxis(sf::Joystick::getAxisPosition(i, sf::Joystick::PovY));

                    // Analog stick input

                    newInput->controllers[i].stickAverageX = SFMLProcessGameControllerAxis(sf::Joystick::getAxisPosition(i, sf::Joystick::X));
                    newInput->controllers[i].stickAverageY = SFMLProcessGameControllerAxis(sf::Joystick::getAxisPosition(i, sf::Joystick::Y));

                    newInput->controllers[i].stickAverageU = SFMLProcessGameControllerAxis(sf::Joystick::getAxisPosition(i, sf::Joystick::U));
                    newInput->controllers[i].stickAverageV = SFMLProcessGameControllerAxis(sf::Joystick::getAxisPosition(i, sf::Joystick::V));

                    // Analog trigger input
                    newInput->controllers[i].stickAverageR = SFMLProcessGameControllerAxis(
                            sf::Joystick::getAxisPosition(i, sf::Joystick::R));
                    newInput->controllers[i].stickAverageZ = SFMLProcessGameControllerAxis(
                            sf::Joystick::getAxisPosition(i, sf::Joystick::Z));

/*
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

                    printf("controller %d, axis x: %f\n", i, newInput->controllers[i].stickAverageX);
                    printf("controller %d, axis y: %f\n", i, newInput->controllers[i].stickAverageY);

                    printf("controller %d, axis u: %f\n", i, newInput->controllers[i].stickAverageU);
                    printf("controller %d, axis v: %f\n", i, newInput->controllers[i].stickAverageV);

                    printf("controller %d, axis r: %f\n", i, newInput->controllers[i].stickAverageR);
                    printf("controller %d, axis z: %f\n", i, newInput->controllers[i].stickAverageZ);
*/
                }
            }

            window.clear();

            system.clearDeadEntities();
            system.processWaitingToFire();
            system.updateMovers(timePerFrame.asSeconds());
            system.updateControllables(timePerFrame.asSeconds(), &newInput->controllers[0], &newInput->keyboard);
            system.updateProjectiles(timePerFrame.asSeconds());
            system.enforceScreenBoundaries();
            system.updateFollowers();
            system.updateBulletSpawnPoints(timePerFrame.asSeconds());
            system.updatePlayerCollisions(player);
            system.updateEnemyCollisions();
            system.processPendingCollisions();

            SFMLRenderDrawables(&window, &world);
            SFMLRenderHitboxes(&window, &world, player);

            window.display();

            GameInput *temp = newInput;
            newInput = oldInput;
            oldInput = temp;

            float currentTime = clock.restart().asSeconds();
            float fps = 1.0f / currentTime;
            std::cout << fps << std::endl;
        }
    }

    return 0;
}