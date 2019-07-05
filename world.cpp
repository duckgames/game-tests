//
// Created by kevin on 11/11/18.
//

#include <cstdio>
#include <SFML/Graphics/RectangleShape.hpp>
#include "world.h"

World::World() {
    unsigned int entity;

    for(entity = 0; entity < MAX_ENTITIES; ++entity) {
        entities[entity] = COMPONENT_NONE;
    }
}

unsigned int World::createEntity() {
    unsigned int entity;

    for(entity = 0; entity < MAX_ENTITIES; ++entity) {
        if (entities[entity] == COMPONENT_NONE) {
            entities[entity] = COMPONENT_USED;
            return entity;
        }
    }

    printf("Entity limit exceeded.\n");
    return MAX_ENTITIES;
}

void World::destroyEntity(unsigned int entity) {
    jump.erase(entity);
    draw.erase(entity);
    position.erase(entity);
    controllable.erase(entity);
    entities[entity] = COMPONENT_NONE;
}

unsigned int World::createJumper(float maxHeight, float jumpSpeed, float fallSpeed, sf::RectangleShape rectangleShape) {
    unsigned int entity = createEntity();

    jump[entity].maxHeight = maxHeight;
    jump[entity].jumpSpeed = jumpSpeed;
    jump[entity].fallSpeed = fallSpeed;
    jump[entity].isJumping = false;
    jump[entity].isFalling = false;

    draw[entity].rectangleShape = rectangleShape;

    return entity;
}

unsigned int World::createControllable(float startX, float startY, float xSpeed, float ySpeed, sf::RectangleShape rectangleShape) {
    unsigned int entity = createEntity();

    rectangleShape.setPosition(startX, startY);

    draw[entity].rectangleShape = rectangleShape;

    position[entity].x = startX;
    position[entity].y = startY;

    controllable[entity].xSpeed = xSpeed;
    controllable[entity].ySpeed = ySpeed;

    return entity;
}