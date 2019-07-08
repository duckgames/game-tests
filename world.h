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
    std::map<int, Jump> jumpersMap;
    std::map<int, Draw> drawablesMap;
    std::map<int, Position> positionsMap;
    std::map<int, Controllable> controllablesMap;
    std::map<int, Move> moversMap;
    std::map<int, Follower> followersMap;
    std::map<int, Leader> leadersMap;
    std::map<int, BulletSpawnPoint> bulletSpawnPointsMap;
    std::map<int, BulletSpawnPoint> playerBulletSpawnPointsMap;

    std::unordered_set<int> waitingToFire;
    std::unordered_set<int> playerWaitingToFire;
    std::unordered_set<int> waitingForDeath;

    int screenWidth;
    int screenHeight;

    World(int screenWidth, int ScreenHeight);
    ~World() = default;

    unsigned int createEntity();
    void destroyEntity(unsigned int entity);

    void addJumpComponent(unsigned int entity, float maxHeight, float jumpSpeed, float fallSpeed);
    void addDrawComponent(unsigned int entity, sf::RectangleShape rectangleShape);
    void addPositionComponent(unsigned int entity, float x, float y);
    void addControllableComponent(unsigned int entity, float xSpeed, float ySpeed);
    void addMoveComponent(unsigned int entity, float xSpeed, float ySpeed);
    void addFollowerComponent(unsigned int entity, unsigned int owningEntity, float xOffset, float yOffset);
    void addLeaderComponent(unsigned int entity, std::vector<int> followers);
    void addBulletSpawnPointComponent(unsigned int entity, float rateOfFire, float bulletXSpeed, float bulletYSpeed, sf::RectangleShape bullet, bool forPlayer);

    unsigned int createJumper(float maxHeight, float jumpSpeed, float fallSpeed, sf::RectangleShape rectangleShape);
    unsigned int createControllable(float startX, float startY, float xSpeed, float ySpeed, sf::RectangleShape rectangleShape);
    unsigned int createMover(float startX, float startY, float xSpeed, float ySpeed, sf::RectangleShape rectangleShape);
    unsigned int createFollower(int owningEntity, float xOffset, float yOffset);
    unsigned int createBulletSpawnPoint(int owningEntity, float xOffset, float yOffset, float rateOfFire, sf::RectangleShape spawnPoint, sf::RectangleShape bullet);
    unsigned int createPlayerBulletSpawnPoint(int owningEntity, float xOffset, float yOffset, float rateOfFire, sf::RectangleShape spawnPoint, sf::RectangleShape bullet);
    unsigned int createPlayerBullet(int spawnPoint);
    unsigned int createEnemyBullet(int spawnPoint);
    unsigned int createEnemy(float startX, float startY, float xSpeed, float ySpeed, float rateOfFire, sf::RectangleShape enemy, sf::RectangleShape spawnPoint, sf::RectangleShape bullet);
};
#endif //UNTITLED_WORLD_H
