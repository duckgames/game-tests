//
// Created by kevin on 11/11/18.
//

#include <iostream>
#include "math.h"
#include "components.h"
#include "system.h"
#include "controller.h"

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
        } else if (padInput->povX != 0 || padInput->povY != 0) {
            position->x += (padInput->povX * controllable.second.xSpeed) * delta;
            position->y += (padInput->povY * controllable.second.ySpeed) * delta;
        } else if (keyboardInput != nullptr) {
            position->x += (keyboardInput->stickAverageX * controllable.second.xSpeed) * delta;
            position->y += (keyboardInput->stickAverageY * controllable.second.ySpeed) * delta;
        }

        if ((padInput->actionLeft.endedDown || keyboardInput->actionLeft.endedDown) &&
            (padInput->actionRight.endedDown || keyboardInput->actionRight.endedDown)) {
            for (int waiting: world->playerWaitingToFire) {
                world->createPlayerBullet(waiting);
            }

            world->playerWaitingToFire.clear();
        }
        else if (padInput->actionLeft.endedDown || keyboardInput->actionLeft.endedDown) {
            for (int waiting: world->playerWaitingToFire) {
                BulletSpawnPoint *bulletSpawnPoint = &world->playerBulletSpawnPointsMap[waiting];
                bulletSpawnPoint->angle -= 10.0f;

                world->createPlayerBullet(waiting);
            }

            world->playerWaitingToFire.clear();
        }
        else if (padInput->actionRight.endedDown || keyboardInput->actionRight.endedDown) {
            for (int waiting: world->playerWaitingToFire) {
                BulletSpawnPoint *bulletSpawnPoint = &world->playerBulletSpawnPointsMap[waiting];
                bulletSpawnPoint->angle += 10.0f;

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
    Draw *draw;

    for (auto mover: world->moversMap) {
        position = &world->positionsMap[mover.first];
        draw = &world->drawablesMap[mover.first];

        position->x += mover.second.xSpeed * delta;
        position->y += mover.second.ySpeed * delta;

        if (position->y + draw->height < 0.0f || position->y > 1080.0f) {
            world->waitingForDeath.insert(mover.first);
        }
    }
}

void System::updateProjectiles(float delta) {
    Position *position;
    Direction *direction;

    for (auto projectile: world->directionsMap) {
        position = &world->positionsMap[projectile.first];
        direction = &world->directionsMap[projectile.first];

        position->x += direction->xMove * delta;
        position->y += direction->yMove * delta;

        if (position->y < 0.0f || position->y > 1080.0f) {
            world->waitingForDeath.insert(projectile.first);
        }
    }
}

void System::updateFollowers() {
    Position *position;
    Position *ownerPosition;
    Draw *drawable;
    Draw *ownerDrawable;

    for (auto follower: world->followersMap) {
        position = &world->positionsMap[follower.first];
        drawable = &world->drawablesMap[follower.first];
        ownerPosition = &world->positionsMap[follower.second.owningEntity];
        ownerDrawable = &world->drawablesMap[follower.second.owningEntity];

        float x = ownerPosition->x + (ownerDrawable->width / 2) - (drawable->width / 2);
        float y = ownerPosition->y + (ownerDrawable->height / 2) - (drawable->height / 2);

        position->x = x + follower.second.xOffset;
        position->y = y + follower.second.yOffset;
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

void System::updatePlayerCollisions(int playerEntity) {
    Position *playerPosition = &world->positionsMap[playerEntity];
    Collider *playerCollider = &world->collidersMap[playerEntity];

    playerCollider->x = playerPosition->x;
    playerCollider->y = playerPosition->y;

    for (auto entity: world->collideWithPlayer) {
        Position *position = &world->positionsMap[entity];
        Collider *collider = &world->collidersMap[entity];

        collider->x = position->x;
        collider->y = position->y;

        if (playerCollider->x + playerCollider->width > collider->x &&
            playerCollider->x < collider->x + collider->width &&
            playerCollider->y < collider->y + collider->height &&
            playerCollider->y + playerCollider->height > collider->y) {
            world->pendingCollisions.emplace_back(std::pair<int, int>(playerEntity, entity));
        }
    }
}

void System::updateEnemyCollisions() {
    for (auto entity: world->collideWithEnemy) {
        Position *position = &world->positionsMap[entity];
        Collider *collider = &world->collidersMap[entity];

        collider->x = position->x;
        collider->y = position->y;

        for (auto enemy: world->enemies) {
            // Enemies collide with the player, so their collider position will have been updated in updatePlayerCollisions()
            Collider *enemyCollider = &world->collidersMap[enemy];
            if (enemyCollider->x + enemyCollider->width > collider->x &&
                enemyCollider->x < collider->x + collider->width &&
                enemyCollider->y < collider->y + collider->height &&
                enemyCollider->y + enemyCollider->height > collider->y) {
                world->pendingCollisions.emplace_back(std::pair<int, int>(enemy, entity));
            }
        }
    }
}

void System::updateInfiniteBackgrounds(float delta) {
    Position *position;
    InfiniteBackground *background;

    for (auto entity: world->infiniteBackgroundsMap) {
        position = &world->positionsMap[entity.first];
        background = &world->infiniteBackgroundsMap[entity.first];

        position->x += background->xSpeed * delta;
        position->y += background->ySpeed * delta;

        if (position->y > 1080.0f) {
            position->y = background->startY;
        }
    }
}

void System::processPendingCollisions() {
    for (auto collision: world->pendingCollisions) {
        Collider *collider1 = &world->collidersMap[collision.first];
        Collider *collider2 = &world->collidersMap[collision.second];

        Health *health1 = &world->healthMap[collision.first];
        Health *health2 = &world->healthMap[collision.second];

        health1->currentHealth -= collider2->damage;
        health2->currentHealth -= collider1->damage;

        if (health1->currentHealth <= 0) {
            world->destroyEntity(collision.first);
        }

        if (health2->currentHealth <= 0) {
            world->destroyEntity(collision.second);
        }
    }

    world->pendingCollisions.clear();
}

void System::enforceScreenXBoundaries() {
    for (auto entity: world->enforceScreenXBoundaries) {
        Position *position;
        Draw *draw;

        position = &world->positionsMap[entity];
        draw = &world->drawablesMap[entity];

        if (position->x < 0) {
            position->x = 0;
        } else if (position->x > world->screenWidth - draw->width) {
            position->x = world->screenWidth - draw->width;
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
        } else if (position->y > world->screenHeight - draw->height) {
            position->y = world->screenHeight - draw->height;
        }
    }
}