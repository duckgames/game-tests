//
// Created by kevin on 11/11/18.
//

#include <SFML/Graphics.hpp>

#ifndef UNTITLED_COMPONENTS_H
#define UNTITLED_COMPONENTS_H

typedef enum
{
    COMPONENT_NONE = 0,
    COMPONENT_USED = 1 << 0,
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

typedef struct {
    float xSpeed;
    float ySpeed;
} Move;

typedef struct {
    int owningEntity;
    float xOffset;
    float yOffset;
} Follower;

typedef struct {
    float rateOfFire;
    float timeElapsed;
    float bulletXSpeed;
    float bulletYSpeed;
    sf::RectangleShape bullet;
} BulletSpawnPoint;

#endif //UNTITLED_COMPONENTS_H
