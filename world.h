//
// Created by kevin on 11/11/18.
//

#ifndef UNTITLED_WORLD_H
#define UNTITLED_WORLD_H

#include "components.h"
#include "EntityPool.h"
#include "TextureAtlasLocation.h"
#include <map>
#include <unordered_set>

static const int MAX_ENTITIES = 1000;

class World {
    public:
    int entities[MAX_ENTITIES];
    std::map<int, Jump> jumpersMap;
    std::map<int, Draw> drawablesMap;
    std::map<int, Position> positionsMap;
    std::map<int, Direction> directionsMap;
    std::map<int, Controllable> controllablesMap;
    std::map<int, Move> moversMap;
    std::map<int, Follower> followersMap;
    std::map<int, Leader> leadersMap;
    std::map<int, BulletSpawnPoint> bulletSpawnPointsMap;
    std::map<int, BulletSpawnPoint> playerBulletSpawnPointsMap;
    std::map<int, Collider> collidersMap;
    std::map<int, Health> healthMap;

    std::vector<std::pair<int, int>> pendingCollisions;

    std::unordered_set<int> enemies;
    std::unordered_set<int> collideWithPlayer;
    std::unordered_set<int> collideWithEnemy;
    std::unordered_set<int> waitingToFire;
    std::unordered_set<int> playerWaitingToFire;
    std::unordered_set<int> waitingForDeath;
    std::unordered_set<int> enforceScreenXBoundaries;
    std::unordered_set<int> enforceScreenYBoundaries;

    sf::Texture textureAtlas;
    std::map<std::string, TextureAtlasLocation> textureAtlasLocationMap;

    int screenWidth;
    int screenHeight;
    
    World(int screenWidth, int ScreenHeight);
    ~World() = default;
    
    unsigned int createEntity();
    void destroyEntity(unsigned int entity);
    
    void addJumpComponent(unsigned int entity, float maxHeight, float jumpSpeed, float fallSpeed);
    void addDrawComponent(unsigned int entity, TextureAtlasLocation textureAtlasLocation);
    void addPositionComponent(unsigned int entity, float x, float y);
    void addDirectionComponent(unsigned int entity, float velocity, float angle);
    void addControllableComponent(unsigned int entity, float xSpeed, float ySpeed);
    void addMoveComponent(unsigned int entity, float xSpeed, float ySpeed);
    void addFollowerComponent(unsigned int entity, unsigned int owningEntity, float xOffset, float yOffset);
    void addLeaderComponent(unsigned int entity, std::vector<int> followers);
    void addBulletSpawnPointComponent(unsigned int entity, float rateOfFire, float bulletXSpeed, float bulletYSpeed, TextureAtlasLocation bullet, bool forPlayer);
    void addColliderComponent(unsigned int entity, float x, float y, float width, float height, int damage);
    void addHealthComponent(unsigned int entity, int initialHealth);

    void addXBoundaryEnforcement(unsigned int entity);
    void addYBoundaryEnforcement(unsigned int entity);

    void canCollideWithPlayer(unsigned int entity);
    void canCollideWithEnemy(unsigned int entity);
    
    unsigned int createControllable(float startX, float startY, float xSpeed, float ySpeed);
    unsigned int createMover(float startX, float startY, float xSpeed, float ySpeed);
    unsigned int createFollower(int owningEntity, float xOffset, float yOffset);
    unsigned int createBulletSpawnPoint(int owningEntity, float xOffset, float yOffset, float rateOfFire);
    unsigned int createPlayerBulletSpawnPoint(int owningEntity, float xOffset, float yOffset, float rateOfFire);
    unsigned int createProjectile(float x, float y, float velocity, float angle, TextureAtlasLocation textureAtlasLocation);
    unsigned int createPlayerBullet(int spawnPoint);
    unsigned int createEnemyBullet(int spawnPoint);
    unsigned int createEnemy(float startX, float startY, float xSpeed, float ySpeed, float rateOfFire);
};
#endif //UNTITLED_WORLD_H
