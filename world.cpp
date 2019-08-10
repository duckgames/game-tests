//
// Created by kevin on 11/11/18.
//

#include <cstdio>
#include <SFML/Graphics/RectangleShape.hpp>
#include "world.h"

World::World(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    
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
    jumpersMap.erase(entity);
    drawablesMap.erase(entity);
    positionsMap.erase(entity);
    controllablesMap.erase(entity);
    moversMap.erase(entity);
    followersMap.erase(entity);
    bulletSpawnPointsMap.erase(entity);
    playerBulletSpawnPointsMap.erase(entity);
    collidersMap.erase(entity);
    healthMap.erase(entity);
    collideWithPlayer.erase(entity);
    collideWithEnemy.erase(entity);
    
    auto leader = leadersMap.find(entity);
    if (leader != leadersMap.end()) {
        for (auto follower: leader->second.followers) {
            jumpersMap.erase(follower);
            drawablesMap.erase(follower);
            positionsMap.erase(follower);
            controllablesMap.erase(follower);
            moversMap.erase(follower);
            followersMap.erase(follower);
            bulletSpawnPointsMap.erase(follower);
            playerBulletSpawnPointsMap.erase(follower);
            entities[follower] = COMPONENT_NONE;
        }
        leadersMap.erase(entity);
    }
    
    entities[entity] = COMPONENT_NONE;
}

void World::addJumpComponent(unsigned int entity, float maxHeight, float jumpSpeed, float fallSpeed) {
    Jump jump;
    jump.maxHeight = maxHeight;
    jump.jumpSpeed = jumpSpeed;
    jump.fallSpeed = fallSpeed;
    jump.isJumping = false;
    jump.isFalling = false;
    jumpersMap.insert(std::pair<int, Jump>(entity, jump));
}

void World::addDrawComponent(unsigned int entity, sf::RectangleShape rectangleShape) {
    Draw draw;
    draw.rectangleShape = rectangleShape;
    drawablesMap.insert(std::pair<int, Draw>(entity, draw));
}

void World::addPositionComponent(unsigned int entity, float x, float y) {
    Position position;
    position.x = x;
    position.y = y;
    positionsMap.insert(std::pair<int, Position>(entity, position));
}

void World::addControllableComponent(unsigned int entity, float xSpeed, float ySpeed) {
    Controllable controllable;
    controllable.xSpeed = xSpeed;
    controllable.ySpeed = ySpeed;
    controllablesMap.insert(std::pair<int, Controllable>(entity, controllable));
}

void World::addMoveComponent(unsigned int entity, float xSpeed, float ySpeed) {
    Move move;
    move.xSpeed = xSpeed;
    move.ySpeed = ySpeed;
    moversMap.insert(std::pair<int, Move>(entity, move));
}

void World::addFollowerComponent(unsigned int entity, unsigned int owningEntity, float xOffset, float yOffset) {
    Follower follower;
    follower.owningEntity = owningEntity;
    follower.xOffset = xOffset;
    follower.yOffset = yOffset;
    followersMap.insert(std::pair<int, Follower>(entity, follower));
}

void World::addLeaderComponent(unsigned int entity, std::vector<int> followers) {
    Leader leader;
    for (auto follower: followers) {
        leader.followers.push_back(follower);
    }
    leadersMap.insert(std::pair<int, Leader>(entity, leader));
}

void World::addBulletSpawnPointComponent(unsigned int entity, float rateOfFire, float bulletXSpeed, float bulletYSpeed, sf::RectangleShape bullet, bool forPlayer) {
    BulletSpawnPoint bulletSpawnPoint;
    bulletSpawnPoint.rateOfFire = rateOfFire;
    bulletSpawnPoint.timeElapsed = 0.0f;
    bulletSpawnPoint.bulletXSpeed = bulletXSpeed;
    bulletSpawnPoint.bulletYSpeed = bulletYSpeed;
    bulletSpawnPoint.bullet = bullet;
    
    std::pair<int, BulletSpawnPoint> pair = std::pair<int, BulletSpawnPoint>(entity, bulletSpawnPoint);
    
    if (forPlayer) {
        playerBulletSpawnPointsMap.insert(pair);
    }
    else {
        bulletSpawnPointsMap.insert(pair);
    }
}

void World::addColliderComponent(unsigned int entity, float x, float y, float width, float height, int damage) {
    Collider collider;
    collider.x = x;
    collider.y = y;
    collider.width = width;
    collider.height = height;
    collider.damage = damage;

    collidersMap.insert(std::pair<int, Collider>(entity, collider));
}

void World::addHealthComponent(unsigned int entity, int initialHealth) {
    Health health;
    health.currentHealth = initialHealth;
    health.initialHealth = initialHealth;

    healthMap.insert(std::pair<int, Health>(entity, health));
}

void World::addXBoundaryEnforcement(unsigned int entity) {
    enforceScreenXBoundaries.insert(entity);
}

void World::addYBoundaryEnforcement(unsigned int entity) {
    enforceScreenYBoundaries.insert(entity);
}

void World::canCollideWithPlayer(unsigned int entity) {
    collideWithPlayer.insert(entity);
}

void World::canCollideWithEnemy(unsigned int entity) {
    collideWithEnemy.insert(entity);
}

unsigned int World::createJumper(float maxHeight, float jumpSpeed, float fallSpeed, sf::RectangleShape rectangleShape) {
    unsigned int entity = createEntity();
    
    addJumpComponent(entity, maxHeight, jumpSpeed, fallSpeed);
    addDrawComponent(entity, rectangleShape);
    
    return entity;
}

unsigned int World::createControllable(float startX, float startY, float xSpeed, float ySpeed, sf::RectangleShape rectangleShape) {
    unsigned int entity = createEntity();
    
    rectangleShape.setPosition(startX, startY);
    
    addDrawComponent(entity, rectangleShape);
    addPositionComponent(entity, startX, startY);
    addControllableComponent(entity, xSpeed, ySpeed);
    addColliderComponent(entity, startX, startY, rectangleShape.getSize().x, rectangleShape.getSize().y, 1);
    addHealthComponent(entity, 10);

    addXBoundaryEnforcement(entity);
    addYBoundaryEnforcement(entity);
    
    return entity;
}

unsigned int World::createMover(float startX, float startY, float xSpeed, float ySpeed, sf::RectangleShape rectangleShape) {
    unsigned int entity = createEntity();
    
    rectangleShape.setPosition(startX, startY);
    
    addDrawComponent(entity, rectangleShape);
    addPositionComponent(entity, startX, startY);
    addMoveComponent(entity, xSpeed, ySpeed);
    
    return entity;
}

unsigned int World::createFollower(int owningEntity, float xOffset, float yOffset) {
    unsigned int entity = createEntity();
    
    addPositionComponent(entity, positionsMap[owningEntity].x + xOffset, positionsMap[owningEntity].y + yOffset);
    addFollowerComponent(entity, owningEntity, xOffset, yOffset);
    
    return entity;
}

unsigned int World::createBulletSpawnPoint(int owningEntity, float xOffset, float yOffset, float rateOfFire, sf::RectangleShape spawnPoint, sf::RectangleShape bullet) {
    unsigned int entity = createFollower(owningEntity, xOffset, yOffset);
    
    addDrawComponent(entity, spawnPoint);
    addBulletSpawnPointComponent(entity, rateOfFire, 0.0f, 100.0f, bullet, false);
    
    return entity;
}

unsigned int World::createPlayerBulletSpawnPoint(int owningEntity, float xOffset, float yOffset, float rateOfFire, sf::RectangleShape spawnPoint, sf::RectangleShape bullet) {
    unsigned int entity = createFollower(owningEntity, xOffset, yOffset);
    
    addDrawComponent(entity, spawnPoint);
    addBulletSpawnPointComponent(entity, rateOfFire, 0.0f, -500.0f, bullet, true);
    
    return entity;
}

unsigned int World::createPlayerBullet(int spawnPoint) {
    Position *spawnPointPosition = &positionsMap[spawnPoint];
    BulletSpawnPoint *bulletSpawnPoint = &playerBulletSpawnPointsMap[spawnPoint];
    
    unsigned int entity = createMover(spawnPointPosition->x,
                                      spawnPointPosition->y,
                                      bulletSpawnPoint->bulletXSpeed,
                                      bulletSpawnPoint->bulletYSpeed,
                                      bulletSpawnPoint->bullet);

    addColliderComponent(entity, spawnPointPosition->x, spawnPointPosition->y, bulletSpawnPoint->bullet.getSize().x, bulletSpawnPoint->bullet.getSize().y, 1);
    canCollideWithEnemy(entity);

    addHealthComponent(entity, 1);

    return entity;
}

unsigned int World::createEnemyBullet(int spawnPoint) {
    Position *spawnPointPosition = &positionsMap[spawnPoint];
    BulletSpawnPoint *bulletSpawnPoint = &bulletSpawnPointsMap[spawnPoint];
    
    unsigned int entity = createMover(spawnPointPosition->x,
                                      spawnPointPosition->y,
                                      bulletSpawnPoint->bulletXSpeed,
                                      bulletSpawnPoint->bulletYSpeed,
                                      bulletSpawnPoint->bullet);

    addColliderComponent(entity, spawnPointPosition->x, spawnPointPosition->y, bulletSpawnPoint->bullet.getSize().x, bulletSpawnPoint->bullet.getSize().y, 1);
    canCollideWithPlayer(entity);

    addHealthComponent(entity, 1);

    return entity;
}

unsigned int World::createEnemy(float startX, float startY, float xSpeed, float ySpeed, float rateOfFire, sf::RectangleShape enemy, sf::RectangleShape spawnPoint, sf::RectangleShape bullet) {
    unsigned int entity = createMover(startX, startY, xSpeed, ySpeed, enemy);
    
    int bulletSpawnPoint = createBulletSpawnPoint(entity, enemy.getSize().x / 2, 0.0f, rateOfFire, spawnPoint, bullet);
    std::vector<int> followers;
    followers.push_back(bulletSpawnPoint);
    addLeaderComponent(entity, followers);

    addColliderComponent(entity, startX, startY, enemy.getSize().x, enemy.getSize().y, 1);
    canCollideWithPlayer(entity);

    addHealthComponent(entity, 5);

    return entity;
}
