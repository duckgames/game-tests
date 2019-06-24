//
// Created by kevin on 11/11/18.
//

#include <SFML/Graphics.hpp>

#ifndef UNTITLED_COMPONENTS_H
#define UNTITLED_COMPONENTS_H

typedef enum
{
    COMPONENT_NONE = 0,
    COMPONENT_JUMP = 1 << 0,
    COMPONENT_DRAW = 2 << 0,
    COMPONENT_POSITION = 3 << 0,
    COMPONENT_CONTROLLABLE = 4 << 0
};

typedef struct {
    bool isJumping;
    bool isFalling;
    float maxHeight;
    float jumpSpeed;
    float fallSpeed;
    float currentJumpHeight;
} Jump;

typedef struct {
    sf::RectangleShape rectangleShape;
} Draw;

typedef struct {
    float x;
    float y;
} Position;

typedef struct {
    float xSpeed;
    float ySpeed;
} Controllable;

#endif //UNTITLED_COMPONENTS_H
