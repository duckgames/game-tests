//
// Created by kevin on 11/11/18.
//

#ifndef UNTITLED_WORLD_H
#define UNTITLED_WORLD_H

#include "components.h"

static const int MAX_ENTITIES = 100;

class World {
public:
    int mask[MAX_ENTITIES];
    Jump jump[MAX_ENTITIES];

    World();
    ~World() = default;
    unsigned int createEntity();
    void destroyEntity(unsigned int entity);
    unsigned int createJumper(float maxHeight, float jumpSpeed, float fallSpeed);
};
#endif //UNTITLED_WORLD_H
