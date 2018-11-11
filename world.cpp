//
// Created by kevin on 11/11/18.
//

#include <cstdio>
#include "world.h"

World::World() {
    unsigned int entity;

    for(entity = 0; entity < MAX_ENTITIES; ++entity) {
        mask[entity] = COMPONENT_NONE;
    }
}

unsigned int World::createEntity() {
    unsigned int entity;

    for(entity = 0; entity < MAX_ENTITIES; ++entity) {
        if (mask[entity] == COMPONENT_NONE) {
            return entity;
        }
    }

    printf("Entity limit exceeded.\n");
    return MAX_ENTITIES;
}

void World::destroyEntity(unsigned int entity) {
    mask[entity] = COMPONENT_NONE;
}

unsigned int World::createJumper(float maxHeight, float jumpSpeed, float fallSpeed) {
    unsigned int entity = createEntity();
    mask[entity] = COMPONENT_JUMP;

    jump[entity].maxHeight = maxHeight;
    jump[entity].jumpSpeed = jumpSpeed;
    jump[entity].fallSpeed = fallSpeed;
    jump[entity].isJumping = false;
    jump[entity].isFalling = false;

    return entity;
}