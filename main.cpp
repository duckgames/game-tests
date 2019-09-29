#include <SFML/Graphics.hpp>
#include "lib/lua/sol.hpp"
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
sf::Font font;

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

static void requestButtonPress(sf::RenderWindow *window, int controllerNumber, GameInput *input, GameInput *oldInput,
                               GameButtonState *gameButtonState, GameButtonState *oldGameButtonState,
                               const char *buttonName) {
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
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].actionUp,
                       &oldInput->controllers[controllerNumber].actionUp, "Action Up");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].actionDown,
                       &oldInput->controllers[controllerNumber].actionDown, "Action Down");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].actionLeft,
                       &oldInput->controllers[controllerNumber].actionLeft, "Action Left");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].actionRight,
                       &oldInput->controllers[controllerNumber].actionRight, "Action Right");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].leftShoulder,
                       &oldInput->controllers[controllerNumber].leftShoulder, "Left Shoulder");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].rightShoulder,
                       &oldInput->controllers[controllerNumber].rightShoulder, "Right Shoulder");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].back,
                       &oldInput->controllers[controllerNumber].back, "Back");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].start,
                       &oldInput->controllers[controllerNumber].start, "Start");
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

// If this is being called to create a follower entity, the owning entity should be the ID of the owner (>= 0 && < MAX_ENTITIES).
// If it is not a follower entity, any negative value.
int createEntity(World *world, sol::table entityData, int owningEntity) {
    int entity = world->createEntity();

    // Add Draw Component
    sol::optional<sol::table> drawExists = entityData["components"]["draw"];
    if (drawExists != sol::nullopt) {
        std::string textureAtlasLocation = entityData["components"]["draw"]["textureAtlasLocation"];
        world->addDrawComponent(entity, world->textureAtlasLocationMap.at(textureAtlasLocation));
    }

    // Add Position Component
    sol::optional<sol::table> positionExists = entityData["components"]["position"];
    if (positionExists != sol::nullopt) {
        world->addPositionComponent(entity,
                                    static_cast<float>(entityData["components"]["position"]["x"]),
                                    static_cast<float>(entityData["components"]["position"]["y"])
        );
    }

    // Add Direction Component
    sol::optional<sol::table> directionExists = entityData["components"]["direction"];
    if (directionExists != sol::nullopt) {
        world->addDirectionComponent(entity,
                                     static_cast<float>(entityData["components"]["direction"]["xMove"]),
                                     static_cast<float>(entityData["components"]["direction"]["yMove"])
        );
    }

    // Add Controllable Component
    sol::optional<sol::table> controllableExists = entityData["components"]["controllable"];
    if (controllableExists != sol::nullopt) {
        world->addControllableComponent(entity,
                                        static_cast<float>(entityData["components"]["controllable"]["xSpeed"]),
                                        static_cast<float>(entityData["components"]["controllable"]["ySpeed"])
        );
    }

    // Add Move Component
    sol::optional<sol::table> moveExists = entityData["components"]["move"];
    if (moveExists != sol::nullopt) {
        world->addMoveComponent(entity,
                                static_cast<float>(entityData["components"]["move"]["xSpeed"]),
                                static_cast<float>(entityData["components"]["move"]["ySpeed"])
        );
    }

    // Add Leader Component
    sol::optional<sol::table> leaderExists = entityData["components"]["leader"];
    if (leaderExists != sol::nullopt) {
        std::vector<int> followers;

        sol::table followerTable = entityData["components"]["leader"]["followers"];
        unsigned int followerIndex = 0;
        while (true) {
            sol::optional<sol::table> followerExists = followerTable[followerIndex];
            if (followerExists == sol::nullopt) {
                break;
            } else {
                sol::table follower = followerTable[followerIndex];
                // Recursively call createEntity function to create a follower of the current entity
                followers.emplace_back(createEntity(world, follower, entity));
            }
            followerIndex++;
        }

        world->addLeaderComponent(entity, followers);
    }

    // Add Follower Component
    if (owningEntity >= 0) {
        sol::optional<sol::table> followerExists = entityData["components"]["follower"];
        if (followerExists != sol::nullopt) {
            world->addFollowerComponent(entity,
                                        owningEntity,
                                        static_cast<float>(entityData["components"]["follower"]["xOffset"]),
                                        static_cast<float>(entityData["components"]["follower"]["yOffset"])
            );
        }
    }

    // Add BulletSpawnPoint Component
    sol::optional<sol::table> bulletSpawnPointExists = entityData["components"]["bulletSpawnPoint"];
    if (bulletSpawnPointExists != sol::nullopt) {
        std::string textureAtlasLocation = entityData["components"]["bulletSpawnPoint"]["textureAtlasLocation"];
        world->addBulletSpawnPointComponent(entity,
                                            static_cast<float>(entityData["components"]["bulletSpawnPoint"]["rateOfFire"]),
                                            static_cast<float>(entityData["components"]["bulletSpawnPoint"]["velocity"]),
                                            static_cast<float>(entityData["components"]["bulletSpawnPoint"]["angle"]),
                                            world->textureAtlasLocationMap.at(textureAtlasLocation),
                                            static_cast<bool>(entityData["components"]["bulletSpawnPoint"]["forPlayer"])
        );
    }

    // Add Collider Component
    sol::optional<sol::table> colliderExists = entityData["components"]["collider"];
    if (colliderExists != sol::nullopt) {
        world->addColliderComponent(entity,
                                    static_cast<float>(entityData["components"]["collider"]["x"]),
                                    static_cast<float>(entityData["components"]["collider"]["y"]),
                                    static_cast<float>(entityData["components"]["collider"]["width"]),
                                    static_cast<float>(entityData["components"]["collider"]["height"]),
                                    static_cast<int>(entityData["components"]["collider"]["damage"])
        );
    }

    // Add Health Component
    sol::optional<sol::table> healthExists = entityData["components"]["health"];
    if (healthExists != sol::nullopt) {
        world->addHealthComponent(entity, static_cast<int>(entityData["components"]["health"]["initialHealth"]));
    }

    // Add Score Component
    sol::optional<sol::table> scoreExists = entityData["components"]["score"];
    if (scoreExists != sol::nullopt) {
        world->addHealthComponent(entity, static_cast<int>(entityData["components"]["score"]["points"]));
    }

    // Add Animation Component
    sol::optional<sol::table> animationExists = entityData["components"]["animation"];
    if (animationExists != sol::nullopt) {
        sol::table frames = entityData["components"]["animation"]["frames"];
        int numFrames = static_cast<int>(entityData["components"]["animation"]["numFrames"]);
        TextureAtlasLocation textureAtlasLocations[numFrames];

        for (int i = 0; i < numFrames; i++) {
            sol::table frame = frames[i];
            std::string textureAtlasLocation = frame["frame"];
            textureAtlasLocations[i] = world->textureAtlasLocationMap.at(textureAtlasLocation);
        }

        world->addAnimationComponent(entity,
                                     static_cast<int>(entityData["components"]["animation"]["numFrames"]),
                                     static_cast<int>(entityData["components"]["animation"]["startFrame"]),
                                     static_cast<float>(entityData["components"]["animation"]["frameDuration"]),
                                     static_cast<bool>(entityData["components"]["animation"]["loop"]),
                                     textureAtlasLocations
        );
    }

    bool isEnemy = entityData["isEnemy"];
    bool enforceXBoundary = entityData["enforceXBoundary"];
    bool enforceYBoundary = entityData["enforceYBoundary"];

    if (isEnemy) {
        world->canCollideWithPlayer(entity);
        world->enemies.insert(entity);
    }

    if (enforceXBoundary) {
        world->addXBoundaryEnforcement(entity);
    }

    if (enforceYBoundary) {
        world->addYBoundaryEnforcement(entity);
    }

    return entity;
}

int loadLevel(int levelNumber, World *world) {
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);
    std::string levelName = "level" + std::to_string(levelNumber);
    lua.script_file("../scripts/" + levelName + ".lua");

    sol::table enemies = lua["enemies"];
    unsigned int enemyIndex = 0;
    while (true) {
        sol::optional<sol::table> enemyExists = enemies[enemyIndex];
        if (enemyExists == sol::nullopt) {
            break;
        } else {
            sol::table enemy = enemies[enemyIndex];
            createEntity(world, enemy, -1);
        }
        enemyIndex++;
    }
}

int main() {
    World world(SCREEN_WIDTH, SCREEN_HEIGHT);
    System system(&world);

    font.loadFromFile("../assets/Ubuntu-M.ttf");
    textureAtlas.loadFromFile("../assets/texture-atlas.png");

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML works!");
    window.setVerticalSyncEnabled(true);

    TextureAtlasLocation background = world.textureAtlasLocationMap.at("background");
    world.createInfiniteBackground((window.getSize().x / 2) - (background.w / 2), -background.h + window.getSize().y,
                                   0.0f, 100.0f, background);

    int player = world.createControllable(window.getSize().x / 2, window.getSize().y - 16, 15.0f, 15.0f);

    int follower1 = world.createPlayerBulletSpawnPoint(player, 16.0f, 0.0f, 0.02f);
    int follower2 = world.createPlayerBulletSpawnPoint(player, -16.0f, 0.0f, 0.02f);
    std::vector<int> playerFollowers;
    playerFollowers.push_back(follower1);
    playerFollowers.push_back(follower2);
    world.addLeaderComponent(player, playerFollowers);

    world.createEnemy(250.0f, 0.0f, 0.0f, 75.0f, 0.2f);
    world.createEnemy(500.0f, 0.0f, 0.0f, 75.0f, 0.5f);

    loadLevel(1, &world);

    sf::Clock tickClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    sf::Time duration = sf::Time::Zero;

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
            SFMLProcessGameControllerButton(&oldInput->keyboard.actionUp, &newInput->keyboard.actionUp,
                                            sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));
            SFMLProcessGameControllerButton(&oldInput->keyboard.actionDown, &newInput->keyboard.actionDown,
                                            sf::Keyboard::isKeyPressed(sf::Keyboard::Space));
            SFMLProcessGameControllerButton(&oldInput->keyboard.actionLeft, &newInput->keyboard.actionLeft,
                                            sf::Keyboard::isKeyPressed(sf::Keyboard::LControl));
            SFMLProcessGameControllerButton(&oldInput->keyboard.actionRight, &newInput->keyboard.actionRight,
                                            sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt));

            SFMLProcessGameControllerButton(&oldInput->keyboard.moveUp, &newInput->keyboard.moveUp,
                                            sf::Keyboard::isKeyPressed(sf::Keyboard::Up));
            SFMLProcessGameControllerButton(&oldInput->keyboard.moveDown, &newInput->keyboard.moveDown,
                                            sf::Keyboard::isKeyPressed(sf::Keyboard::Down));
            SFMLProcessGameControllerButton(&oldInput->keyboard.moveLeft, &newInput->keyboard.moveLeft,
                                            sf::Keyboard::isKeyPressed(sf::Keyboard::Left));
            SFMLProcessGameControllerButton(&oldInput->keyboard.moveRight, &newInput->keyboard.moveRight,
                                            sf::Keyboard::isKeyPressed(sf::Keyboard::Right));

            SFMLProcessGameControllerButton(&oldInput->keyboard.leftShoulder, &newInput->keyboard.leftShoulder,
                                            sf::Keyboard::isKeyPressed(sf::Keyboard::Z));
            SFMLProcessGameControllerButton(&oldInput->keyboard.rightShoulder, &newInput->keyboard.rightShoulder,
                                            sf::Keyboard::isKeyPressed(sf::Keyboard::X));

            SFMLProcessGameControllerButton(&oldInput->keyboard.back, &newInput->keyboard.back,
                                            sf::Keyboard::isKeyPressed(sf::Keyboard::F2));
            SFMLProcessGameControllerButton(&oldInput->keyboard.start, &newInput->keyboard.start,
                                            sf::Keyboard::isKeyPressed(sf::Keyboard::F1));

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
            SFMLProcessGameControllerButton(&oldInput->mouseButtons[0], &newInput->mouseButtons[0],
                                            sf::Mouse::isButtonPressed(sf::Mouse::Left));
            SFMLProcessGameControllerButton(&oldInput->mouseButtons[1], &newInput->mouseButtons[1],
                                            sf::Mouse::isButtonPressed(sf::Mouse::Middle));
            SFMLProcessGameControllerButton(&oldInput->mouseButtons[2], &newInput->mouseButtons[2],
                                            sf::Mouse::isButtonPressed(sf::Mouse::Right));

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
                    SFMLProcessGameControllerButton(&oldInput->controllers[i].moveDown,
                                                    &newInput->controllers[i].moveDown,
                                                    (sf::Joystick::getAxisPosition(i, sf::Joystick::PovY) > 0));
                    SFMLProcessGameControllerButton(&oldInput->controllers[i].moveLeft,
                                                    &newInput->controllers[i].moveLeft,
                                                    (sf::Joystick::getAxisPosition(i, sf::Joystick::PovX) < 0));
                    SFMLProcessGameControllerButton(&oldInput->controllers[i].moveRight,
                                                    &newInput->controllers[i].moveRight,
                                                    (sf::Joystick::getAxisPosition(i, sf::Joystick::PovX) > 0));

                    // Dpad input as POV axis
                    newInput->controllers[i].povX = SFMLProcessGameControllerAxis(
                            sf::Joystick::getAxisPosition(i, sf::Joystick::PovX));
                    newInput->controllers[i].povY = SFMLProcessGameControllerAxis(
                            sf::Joystick::getAxisPosition(i, sf::Joystick::PovY));

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
            system.updateInfiniteBackgrounds(timePerFrame.asSeconds());
            system.updateMovers(timePerFrame.asSeconds());
            system.updateControllables(timePerFrame.asSeconds(), &newInput->controllers[0], &newInput->keyboard);
            system.updateProjectiles(timePerFrame.asSeconds());
            system.enforceScreenBoundaries();
            system.updateFollowers();
            system.updateBulletSpawnPoints(timePerFrame.asSeconds());
            system.updatePlayerCollisions(player);
            system.updateEnemyCollisions();
            system.processPendingCollisions();
            system.updateScore();
            system.updateAnimations(timePerFrame.asSeconds());

            SFMLRenderDrawables(&window, &world);
            SFMLRenderHitboxes(&window, &world, player);

            sf::Text text;

            std::string scoreString = "SCORE: ";
            scoreString.append(std::to_string(world.score));
            text.setString(scoreString);
            text.setFont(font);
            text.setCharacterSize(24);
            window.draw(text);

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