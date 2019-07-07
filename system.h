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
    void renderDrawables(sf::RenderWindow *window);
    void jumpers(float delta);
    void updateControllables(float delta, GameControllerInput *input, GameControllerInput *keyboardInput, sf::RectangleShape bullet);
    void updateMovers(float delta);
    void updateFollowers();
    void updateBulletSpawnPoints(float delta);
    void processWaitingToFire();
    void clearDeadEntities();

private:
    World *world;
};

#endif //UNTITLED_SYSTEMS_H
