//
// Created by kevin on 11/11/18.
//

#ifndef UNTITLED_WORLD_H
#define UNTITLED_WORLD_H

#include "components.h"
#include "EntityPool.h"
#include <map>
#include <unordered_set>

static const int MAX_ENTITIES = 1000;

class World {
public:
    int entities[MAX_ENTITIES];
    std::map<int, Jump> jump;
    std::map<int, Draw> draw;
    std::map<int, Position> position;
    std::map<int, Controllable> controllable;
    std::map<int, Move> move;
    std::map<int, Follower> followers;
    std::map<int, BulletSpawnPoint> bulletSpawnPoints;
    std::map<int, BulletSpawnPoint> playerBulletSpawnPoints;

    std::unordered_set<int> waitingToFire;
    std::unordered_set<int> playerWaitingToFire;

    World();
    ~World() = default;
    unsigned int createEntity();
    void destroyEntity(unsigned int entity);
    unsigned int createJumper(float maxHeight, float jumpSpeed, float fallSpeed, sf::RectangleShape rectangleShape);
    unsigned int createControllable(float startX, float startY, float xSpeed, float ySpeed, sf::RectangleShape rectangleShape);
    unsigned int createMover(float startX, float startY, float xSpeed, float ySpeed, sf::RectangleShape rectangleShape);
    unsigned int createFollower(int owningEntity, float xOffset, float yOffset);
    unsigned int createBulletSpawnPoint(int owningEntity, float xOffset, float yOffset, float rateOfFire, sf::RectangleShape rectangleShape);
    unsigned int createPlayerBulletSpawnPoint(int owningEntity, float xOffset, float yOffset, float rateOfFire, sf::RectangleShape spawnPoint, sf::RectangleShape bullet);
    unsigned int createPlayerBullet(int spawnPoint);
};
#endif //UNTITLED_WORLD_H
