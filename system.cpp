//
// Created by kevin on 11/11/18.
//

#include <iostream>
#include <cmath>
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

    for (auto controllable: world->controllablesMap) {
        position = &world->positionsMap[controllable.first];

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
    }
}

void System::updateProjectiles(float delta) {
    Position *position;
    Direction *direction;

    for (auto projectile: world->directionsMap) {
        position = &world->positionsMap[projectile.first];
        direction = &world->directionsMap[projectile.first];

        position->x += direction->xSpeed * direction->dirNormalX * delta;
        position->y += direction->ySpeed * direction->dirNormalY * delta;

        if (position->y < 0.0f || position->y > world->screenHeight ||
            position->x + position->x < 0.0f || position->x > world->screenWidth) {
            world->waitingForDeath.insert(projectile.first);
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

void System::updatePlayerCollisions(int playerEntity) {
    Position *playerPosition = &world->positionsMap[playerEntity];
    Collider *playerCollider = &world->collidersMap[playerEntity];

    float playerColliderX = playerPosition->x - (playerCollider->width / 2) + playerCollider->xOffset;
    float playerColliderY = playerPosition->y - (playerCollider->height / 2) + playerCollider->yOffset;

    for (auto entity: world->collideWithPlayer) {
        Position *position = &world->positionsMap[entity];
        Collider *collider = &world->collidersMap[entity];
        float colliderX = position->x - (collider->width / 2) + collider->xOffset;
        float colliderY = position->y - (collider->height / 2) + collider->yOffset;

        if (playerColliderX + playerCollider->width > colliderX &&
            playerColliderX < colliderX + collider->width &&
            playerColliderY < colliderY + collider->height &&
            playerColliderY + playerCollider->height > colliderY) {
            world->pendingCollisions.emplace_back(std::pair<int, int>(playerEntity, entity));
        }
    }
}

void System::updateEnemyCollisions() {
    for (auto entity: world->collideWithEnemy) {
        Position *position = &world->positionsMap[entity];
        Collider *collider = &world->collidersMap[entity];

        float colliderX = position->x - (collider->width / 2) + collider->xOffset;
        float colliderY = position->y - (collider->height / 2) + collider->yOffset;

        for (auto enemy: world->enemies) {
            Position *enemyPosition = &world->positionsMap[enemy];
            Collider *enemyCollider = &world->collidersMap[enemy];
            float enemyColliderX = enemyPosition->x - (enemyCollider->width / 2) + enemyCollider->xOffset;
            float enemyColliderY = enemyPosition->y - (enemyCollider->width / 2) + enemyCollider->yOffset;

            if (enemyColliderX + enemyCollider->width > colliderX &&
                enemyColliderX < colliderX + collider->width &&
                enemyColliderY < colliderY + collider->height &&
                enemyColliderY + enemyCollider->height > colliderY) {
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

        if (position->y > world->screenHeight + background->height) {
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

void System::updateScore() {
    for (auto toAdd: world->scoresToAdd) {
        Score *score = &world->scoresMap[toAdd];
        world->score += score->points;
    }

    world->scoresToAdd.clear();
}

void System::updateAttractables() {
    Position *attractorPosition;
    Position *attractablePosition;
    Attractable *attractable;

    for (auto attbl: world->attractablesMap) {
        attractable = &world->attractablesMap[attbl.first];
        attractorPosition = &world->positionsMap[attbl.second.attractorEntity];
        attractablePosition = &world->positionsMap[attbl.first];
        float diffX = attractorPosition->x - attractablePosition->x;
        float diffY = attractorPosition->y - attractablePosition->y;
        attractable->distanceToAttractor = sqrt((diffX * diffX) + (diffY * diffY));
        attractable->diffX = diffX;
        attractable->diffY = diffY;

        if (attractable->distanceToAttractor < world->attractorsMap[attbl.second.attractorEntity].radius) {
            world->beingAttracted.insert(attbl.first);
        }
    }

    Direction *direction;
    Attractor *attractor;

    for (auto attracted: world->beingAttracted) {
        direction = &world->directionsMap[attracted];
        attractable = &world->attractablesMap[attracted];
        attractor = &world->attractorsMap[attractable->attractorEntity];

        direction->dirNormalX = attractable->diffX / attractable->distanceToAttractor;
        direction->dirNormalY = attractable->diffY / attractable->distanceToAttractor;
        direction->xSpeed = attractor->speed;
        direction->ySpeed = attractor->speed;
    }
}

void System::updateAnimations(float delta) {
    for (auto entity: world->animationsMap) {
        Animation *animation = &world->animationsMap[entity.first];
        animation->timeElapsed += delta;

        if (animation->timeElapsed >= animation->frameDuration) {
            int nextFrame = (animation->currentFrame < animation->totalFrames - 1 ? (animation->currentFrame + 1) : 0);
            world->drawablesMap[entity.first] = animation->drawables[nextFrame];
            world->drawablesMap[entity.first] = animation->drawables[nextFrame];
            animation->currentFrame = nextFrame;
            animation->timeElapsed = 0;
        }
    }
}

void System::enforceScreenXBoundaries() {
    for (auto entity: world->enforceScreenXBoundaries) {
        Position *position = &world->positionsMap[entity];
        Draw *draw = &world->drawablesMap[entity];
        int halfWidth = draw->width / 2;

        if (position->x < halfWidth) {
            position->x = halfWidth;
        } else if (position->x > world->screenWidth - halfWidth) {
            position->x = world->screenWidth - halfWidth;
        }
    }
}

void System::enforceScreenYBoundaries() {
    for (auto entity: world->enforceScreenYBoundaries) {
        Position *position = &world->positionsMap[entity];
        Draw *draw = &world->drawablesMap[entity];
        int halfHeight = draw->height / 2;

        if (position->y < halfHeight) {
            position->y = halfHeight;
        } else if (position->y > world->screenHeight - halfHeight) {
            position->y = world->screenHeight - halfHeight;
        }
    }
}