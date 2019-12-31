//
// Created by kevin on 11/11/18.
//

#include <vector>
#include "TextureAtlasLocation.h"

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
    int x;
    int y;
    int width;
    int height;
} Draw;

typedef struct {
    float x;
    float y;
} Position;

typedef struct {
    float xMove;
    float yMove;
} Direction;

typedef struct {
    float xSpeed;
    float ySpeed;
} Controllable;

typedef struct {
    float xSpeed;
    float ySpeed;
} Move;

typedef struct {
    std::vector<int> followers;
} Leader;

typedef struct {
    int owningEntity;
    float xOffset;
    float yOffset;
} Follower;

typedef struct {
    float velocity;
    float angle;
    float xOffset;
    float yOffset;
    TextureAtlasLocation bullet;
} BulletDefinition;

typedef struct {
    float rateOfFire;
    float timeElapsed;
    float angle;
    std::vector<BulletDefinition> bullets;
} BulletSpawnPoint;

typedef struct {
    float xOffset;
    float yOffset;
    float width;
    float height;
    int damage;
} Collider;

typedef struct {
    int initialHealth;
    int currentHealth;
} Health;

typedef struct {
    long points;
} Score;

typedef struct {
    std::string itemName;
} DropItem;

typedef struct {
    float radius;
    float speed;
} Attractor;

typedef struct {
} Attractable;

typedef struct {
    int currentFrame;
    int totalFrames;
    float frameDuration;
    float timeElapsed;
    bool loop;
    std::vector<Draw> drawables;
} Animation;

typedef struct {
    int follower;
    float startY;
    float xSpeed;
    float ySpeed;
} InfiniteBackground;
#endif //UNTITLED_COMPONENTS_H
