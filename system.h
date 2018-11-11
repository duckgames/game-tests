//
// Created by kevin on 11/11/18.
//

#ifndef UNTITLED_SYSTEMS_H
#define UNTITLED_SYSTEMS_H

#include "world.h"

#define JUMP_MASK (COMPONENT_JUMP)

class System {
public:
    System(World *world): world(world) {}
    ~System() = default;
    void jump(float delta);

private:
    World *world;
};

#endif //UNTITLED_SYSTEMS_H
