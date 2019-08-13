//
// Created by kevin on 11/11/18.
//

#ifndef UNTITLED_SYSTEMS_H
#define UNTITLED_SYSTEMS_H

#include "world.h"
#include "controller.h"

class System {
public:
    System(World *world): world(world) {}
    ~System() = default;

    void jumpers(float delta);
    void updateControllables(float delta, GameControllerInput *input, GameControllerInput *keyboardInput);
    void updateMovers(float delta);
    void updateProjectiles(float delta);
    void updateFollowers();
    void updateBulletSpawnPoints(float delta);
    void processWaitingToFire();
    void clearDeadEntities();
    void enforceScreenBoundaries();
    void updatePlayerCollisions(int playerEntity);
    void updateEnemyCollisions();
    void processPendingCollisions();

private:
    void enforceScreenXBoundaries();
    void enforceScreenYBoundaries();

private:
    World *world;
};

#endif //UNTITLED_SYSTEMS_H
