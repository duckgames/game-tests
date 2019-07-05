//
// Created by kevin on 11/11/18.
//

#ifndef UNTITLED_WORLD_H
#define UNTITLED_WORLD_H

#include "components.h"
#include <map>

static const int MAX_ENTITIES = 100;

class World {
public:
    int entities[MAX_ENTITIES];
    std::map<int, Jump> jump;
    std::map<int, Draw> draw;
    std::map<int, Position> position;
    std::map<int, Controllable> controllable;

    World();
    ~World() = default;
    unsigned int createEntity();
    void destroyEntity(unsigned int entity);
    unsigned int createJumper(float maxHeight, float jumpSpeed, float fallSpeed, sf::RectangleShape rectangleShape);
    unsigned int createControllable(float startX, float startY, float xSpeed, float ySpeed, sf::RectangleShape rectangleShape);
};
#endif //UNTITLED_WORLD_H
