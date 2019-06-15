//
// Created by kevin on 11/11/18.
//

#ifndef UNTITLED_SYSTEMS_H
#define UNTITLED_SYSTEMS_H

#include "world.h"

#define JUMP_MASK (COMPONENT_JUMP)
#define DRAW_MASK (COMPONENT_DRAW)

class System {
public:
    System(World *world): world(world) {}
    ~System() = default;
    void jumpers(float delta, sf::RenderWindow *window);

private:
    World *world;
};

#endif //UNTITLED_SYSTEMS_H
