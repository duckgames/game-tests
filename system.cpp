//
// Created by kevin on 11/11/18.
//

#include <cstdio>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "math.h"
#include "components.h"
#include "system.h"
#include "controller.h"

void System::renderDrawables(sf::RenderWindow *window) {
    for(auto drawable : world->drawablesMap) {
        Position *position;
        position = &world->positionsMap[drawable.first];
        drawable.second.rectangleShape.setPosition(position->x, position->y);

        window->draw(drawable.second.rectangleShape);
    }
}

void System::renderHitboxes(sf::RenderWindow *window) {
    for (auto entity : world->collideWithPlayer) {
        Collider *collider = &world->collidersMap[entity];

        sf::RectangleShape rectangleShape;
        rectangleShape.setPosition(collider->x, collider->y);
        rectangleShape.setSize(sf::Vector2f(collider->width, collider->height));
        rectangleShape.setFillColor(sf::Color::Green);

        window->draw(rectangleShape);
    }

    for (auto entity : world->collideWithEnemy) {
        Collider *collider = &world->collidersMap[entity];

        sf::RectangleShape rectangleShape;
        rectangleShape.setPosition(collider->x, collider->y);
        rectangleShape.setSize(sf::Vector2f(collider->width, collider->height));
        rectangleShape.setFillColor(sf::Color::Red);

        window->draw(rectangleShape);
    }
}

void System::jumpers(float delta) {
    /*
    unsigned int entity;
    Jump *jump;
    Draw *draw;

    for(entity = 0; entity < MAX_ENTITIES; ++entity) {
        if((world->mask[entity] & JUMP_MASK) == JUMP_MASK) {
            jump = &(world->jump[entity]);
            draw = &(world->draw[entity]);

            if (jump->isJumping) {
                float toAdd = jump->jumpSpeed * delta;

                if (jump->currentJumpHeight + toAdd < jump->maxHeight) {
                    jump->currentJumpHeight += toAdd;
                }
                else {
                    jump->currentJumpHeight = jump->maxHeight;
                    jump->isJumping = false;
                    jump->isFalling = true;
                    continue;
                }
            }
            if (jump->isFalling) {
                float toSubtract = jump->fallSpeed * delta;

                if (jump->currentJumpHeight - toSubtract > 0.0f) {
                    jump->currentJumpHeight -= toSubtract;
                }
                else {
                    jump->currentJumpHeight = 0.0f;
                    jump->isFalling = false;
                }
            }

            draw->rectangleShape.setPosition(draw->rectangleShape.getPosition().x, jump->currentJumpHeight);
            draw = &(world->draw[entity]);
            window->draw(draw->rectangleShape);
        }
    }
     */
}

// Note there is a nullptr check on the keyboard input - this is so keyboard input can be disabled by passing a nullptr
// for the keyboardInput parameter.
void System::updateControllables(float delta, GameControllerInput *padInput, GameControllerInput *keyboardInput) {
    Position *position;
    Collider *collider;

    for (auto controllable: world->controllablesMap) {
        position = &world->positionsMap[controllable.first];
        collider = &world->collidersMap[controllable.first];

        if (padInput->stickAverageX != 0 || padInput->stickAverageY != 0) {
            position->x += (padInput->stickAverageX * controllable.second.xSpeed) * delta;
            position->y += (padInput->stickAverageY * controllable.second.ySpeed) * delta;
        }
        else if (padInput->povX != 0 || padInput->povY != 0) {
            position->x += (padInput->povX * controllable.second.xSpeed) * delta;
            position->y += (padInput->povY * controllable.second.ySpeed) * delta;
        }
        else if (keyboardInput != nullptr) {
            position->x += (keyboardInput->stickAverageX * controllable.second.xSpeed) * delta;
            position->y += (keyboardInput->stickAverageY * controllable.second.ySpeed) * delta;
        }

        if (padInput->actionLeft.endedDown || keyboardInput->actionLeft.endedDown) {
            for (int waiting: world->playerWaitingToFire) {
                world->createPlayerBullet(waiting);
            }

            world->playerWaitingToFire.clear();
        }

        collider->x = position->x;
        collider->y = position->y;
    }
}

void System::updateMovers(float delta) {
    Position *position;

    for (auto mover: world->moversMap) {
        position = &world->positionsMap[mover.first];

        position->x += mover.second.xSpeed * delta;
        position->y += mover.second.ySpeed * delta;

        if (position->y < 0.0f || position->y > 1080.0f) {
            world->waitingForDeath.insert(mover.first);
        }
    }
}

void System::updateFollowers() {
    Position *position;
    Position *ownerPosition;

    for (auto follower: world->followersMap) {
        position = &world->positionsMap[follower.first];
        ownerPosition = &world->positionsMap[follower.second.owningEntity];

        position->x = ownerPosition->x + follower.second.xOffset;
        position->y = ownerPosition->y + follower.second.yOffset;
    }
}

void System::updateBulletSpawnPoints(float delta) {
    for (auto spawnPoint: world->bulletSpawnPointsMap) {
        world->bulletSpawnPointsMap[spawnPoint.first].timeElapsed += delta;

        if (spawnPoint.second.timeElapsed >= spawnPoint.second.rateOfFire) {
            world->waitingToFire.insert(spawnPoint.first);
            world->bulletSpawnPointsMap[spawnPoint.first].timeElapsed = 0.0f;
        }
    }

    for (auto spawnPoint: world->playerBulletSpawnPointsMap) {
        world->playerBulletSpawnPointsMap[spawnPoint.first].timeElapsed += delta;

        if (spawnPoint.second.timeElapsed >= spawnPoint.second.rateOfFire) {
            world->playerWaitingToFire.insert(spawnPoint.first);
            world->playerBulletSpawnPointsMap[spawnPoint.first].timeElapsed = 0.0f;
        }
    }
}

void System::processWaitingToFire() {
    for (int waiting: world->waitingToFire) {
        world->createEnemyBullet(waiting);
    }

    world->waitingToFire.clear();
}

void System::clearDeadEntities() {
    for (auto entity: world->waitingForDeath) {
        world->destroyEntity(entity);
    }

    world->waitingForDeath.clear();
}

void System::enforceScreenBoundaries() {
    enforceScreenYBoundaries();
    enforceScreenXBoundaries();
}

void System::updatePlayerCollisions() {
    for (auto entity: world->collideWithPlayer) {
        Position *position = &world->positionsMap[entity];
        Collider *collider = &world->collidersMap[entity];

        collider->x = position->x;
        collider->y = position->y;
    }
}

void System::updateEnemyCollisions() {
    for (auto entity: world->collideWithEnemy) {
        Position *position = &world->positionsMap[entity];
        Collider *collider = &world->collidersMap[entity];

        collider->x = position->x;
        collider->y = position->y;
    }
}

void System::enforceScreenXBoundaries() {
    for (auto entity: world->enforceScreenXBoundaries) {
        Position *position;
        Draw *draw;

        position = &world->positionsMap[entity];
        draw = &world->drawablesMap[entity];

        if (position->x < 0) {
            position->x = 0;
        }
        else if (position->x > world->screenWidth - draw->rectangleShape.getSize().x) {
            position->x = world->screenWidth - draw->rectangleShape.getSize().x;
        }
    }
}

void System::enforceScreenYBoundaries() {
    for (auto entity: world->enforceScreenYBoundaries) {
        Position *position;
        Draw *draw;

        position = &world->positionsMap[entity];
        draw = &world->drawablesMap[entity];

        if (position->y < 0) {
            position->y = 0;
        }
        else if (position->y > world->screenHeight - draw->rectangleShape.getSize().y) {
            position->y = world->screenHeight - draw->rectangleShape.getSize().y;
        }
    }
}