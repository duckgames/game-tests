//
// Created by kevin on 11/11/18.
//

#include <cstdio>
#include <cmath>
#include <fstream>
#include <jsoncpp/json/json.h>
#include "world.h"

World::World(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    unsigned int entity;

    for(entity = 0; entity < MAX_ENTITIES; ++entity) {
        entities[entity] = COMPONENT_NONE;
    }

    std::ifstream ifs("../assets/texture-atlas.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);

    const Json::Value &frames = obj["frames"];

    for (auto tex: frames) {
        TextureAtlasLocation location;
        location.x = tex["frame"]["x"].asInt();
        location.y = tex["frame"]["y"].asInt();
        location.w = tex["frame"]["w"].asInt();
        location.h = tex["frame"]["h"].asInt();

        textureAtlasLocationMap.insert(std::pair<std::string, TextureAtlasLocation>(tex["filename"].asString(), location));
    }
}

void World::clear() {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        entities[i] = COMPONENT_NONE;
    }

    jumpersMap.clear();
    drawablesMap.clear();
    positionsMap.clear();
    directionsMap.clear();
    controllablesMap.clear();
    followersMap.clear();
    leadersMap.clear();
    bulletSpawnPointsMap.clear();
    playerBulletSpawnPointsMap.clear();
    collidersMap.clear();
    healthMap.clear();
    infiniteBackgroundsMap.clear();
    scoresMap.clear();
    dropItemsMap.clear();
    attractorsMap.clear();
    attractablesMap.clear();
    animationsMap.clear();
    pendingCollisions.clear();
    enemies.clear();
    collideWithPlayer.clear();
    collideWithEnemy.clear();
    waitingToFire.clear();
    playerWaitingToFire.clear();
    waitingForDeath.clear();
    enforceScreenXBoundaries.clear();
    enforceScreenYBoundaries.clear();
    scoresToAdd.clear();
    beingAttracted.clear();

    score = 0;
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
    auto dropItem = dropItemsMap.find(entity);
    if (dropItem != dropItemsMap.end()) {
        Droppable droppable = droppableItems.at(dropItem->second.itemName);
        droppable.xPosition = positionsMap.at(entity).x;
        droppable.yPosition = positionsMap.at(entity).y;
        createDroppableItem(player, droppable);
    }

    jumpersMap.erase(entity);
    drawablesMap.erase(entity);
    positionsMap.erase(entity);
    directionsMap.erase(entity);
    controllablesMap.erase(entity);
    followersMap.erase(entity);
    bulletSpawnPointsMap.erase(entity);
    playerBulletSpawnPointsMap.erase(entity);
    collidersMap.erase(entity);
    healthMap.erase(entity);
    dropItemsMap.erase(entity);
    attractorsMap.erase(entity);
    attractablesMap.erase(entity);
    enemies.erase(entity);
    collideWithPlayer.erase(entity);
    collideWithEnemy.erase(entity);
    beingAttracted.erase(entity);

    auto iterator = scoresMap.find(entity);
    if (iterator != scoresMap.end()) {
        score += iterator->second.points;
        scoresMap.erase(iterator);
    }

    auto leader = leadersMap.find(entity);
    if (leader != leadersMap.end()) {
        for (auto follower: leader->second.followers) {
            jumpersMap.erase(follower);
            drawablesMap.erase(follower);
            positionsMap.erase(follower);
            controllablesMap.erase(follower);
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

void World::addDrawComponent(unsigned int entity, TextureAtlasLocation textureAtlasLocation) {
    Draw draw;

    draw.x = textureAtlasLocation.x;
    draw.y = textureAtlasLocation.y;
    draw.width = textureAtlasLocation.w;
    draw.height = textureAtlasLocation.h;

    drawablesMap.insert(std::pair<int, Draw>(entity, draw));
}

void World::addPositionComponent(unsigned int entity, float x, float y) {
    Position position;
    position.x = x;
    position.y = y;
    positionsMap.insert(std::pair<int, Position>(entity, position));
}

void World::addDirectionComponent(unsigned int entity, float velocity, float angle) {
    Direction direction;
    double radians = angle * M_PI / 180;
    direction.xMove = cos(radians) * velocity;
    direction.yMove = sin(radians) * velocity;
    directionsMap.insert(std::pair<int, Direction>(entity, direction));
}

void World::addControllableComponent(unsigned int entity, float xSpeed, float ySpeed) {
    Controllable controllable;
    controllable.xSpeed = xSpeed;
    controllable.ySpeed = ySpeed;
    controllablesMap.insert(std::pair<int, Controllable>(entity, controllable));
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

void World::addBulletSpawnPointComponent(unsigned int entity, float rateOfFire, float angle, bool forPlayer, std::vector<BulletDefinition> bullets) {
    BulletSpawnPoint bulletSpawnPoint;
    bulletSpawnPoint.rateOfFire = rateOfFire;
    bulletSpawnPoint.angle = angle;
    bulletSpawnPoint.timeElapsed = 0.0f;
    bulletSpawnPoint.bullets = bullets;

    std::pair<int, BulletSpawnPoint> pair = std::pair<int, BulletSpawnPoint>(entity, bulletSpawnPoint);

    if (forPlayer) {
        playerBulletSpawnPointsMap.insert(pair);
    }
    else {
        bulletSpawnPointsMap.insert(pair);
    }
}

void World::addColliderComponent(unsigned int entity, float xOffset, float yOffset, float width, float height, int damage) {
    Collider collider;
    collider.xOffset = xOffset;
    collider.yOffset = yOffset;
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

void World::addInfiniteBackgroundComponent(unsigned int entity, float startY, float xSpeed, float ySpeed) {
    InfiniteBackground infiniteBackground;
    infiniteBackground.startY = startY;
    infiniteBackground.xSpeed = xSpeed;
    infiniteBackground.ySpeed = ySpeed;

    infiniteBackgroundsMap.insert(std::pair<int, InfiniteBackground>(entity, infiniteBackground));
}

void World::addScoreComponent(unsigned int entity, long points) {
    Score score;
    score.points = points;

    scoresMap.insert(std::pair<int, Score>(entity, score));
}

void World::addDropItemComponent(unsigned int entity, std::string itemName) {
    DropItem dropItem;
    dropItem.itemName = itemName;

    dropItemsMap.insert(std::pair<int, DropItem>(entity, dropItem));
}

void World::addAttractorComponent(unsigned int entity, float radius, float speed) {
    Attractor attractor;
    attractor.radius = radius;
    attractor.speed = speed;

    attractorsMap.insert(std::pair<int, Attractor>(entity, attractor));
}

void World::addAttractableComponent(unsigned int entity, unsigned int attractorEntity) {
    Attractable attractable;
    attractable.attractorEntity = attractorEntity;

    attractablesMap.insert(std::pair<int, Attractable>(entity, attractable));
}

void World::addAnimationComponent(unsigned int entity, int numFrames, int startFrame, float frameDuration, bool loop, TextureAtlasLocation *textureAtlasLocations) {
    Animation animation;
    animation.totalFrames = numFrames;
    animation.currentFrame = startFrame;
    animation.frameDuration = frameDuration;
    animation.loop = loop;
    animation.timeElapsed = 0.0f;

    for (int i = 0; i < numFrames; i++) {
        Draw draw;
        draw.x = textureAtlasLocations[i].x;
        draw.y = textureAtlasLocations[i].y;
        draw.width = textureAtlasLocations[i].w;
        draw.height = textureAtlasLocations[i].h;
        animation.drawables.push_back(draw);
    }

    animationsMap.insert(std::pair<int, Animation>(entity, animation));
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

unsigned int World::createInfiniteBackground(float startX, float startY, float xSpeed, float ySpeed, TextureAtlasLocation textureAtlasLocation) {
    unsigned int entity = createEntity();

    addDrawComponent(entity, textureAtlasLocation);
    addPositionComponent(entity, startX, startY);
    addInfiniteBackgroundComponent(entity, startY, xSpeed, ySpeed);

    int follower = createEntity();
    addPositionComponent(follower, positionsMap[entity].x + 0.0f, positionsMap[entity].y + -textureAtlasLocation.h);
    addFollowerComponent(follower, entity, 0.0f, -textureAtlasLocation.h);
    addDrawComponent(follower, textureAtlasLocation);
    infiniteBackgroundsMap[entity].follower = follower;

    return entity;
}

unsigned int World::createProjectile(float x, float y, float velocity, float angle, TextureAtlasLocation textureAtlasLocation) {
    unsigned int entity = createEntity();

    addDrawComponent(entity, textureAtlasLocation);
    addPositionComponent(entity, x, y);
    addDirectionComponent(entity, velocity, angle);

    return entity;
}

void World::createPlayerBullet(int spawnPoint) {
    Position *spawnPointPosition = &positionsMap[spawnPoint];
    BulletSpawnPoint *bulletSpawnPoint = &playerBulletSpawnPointsMap[spawnPoint];

    for (BulletDefinition bullet : bulletSpawnPoint->bullets) {
        unsigned int entity = createProjectile(spawnPointPosition->x + bullet.xOffset,
                                               spawnPointPosition->y + bullet.yOffset,
                                               bullet.velocity,
                                               bulletSpawnPoint->angle + bullet.angle,
                                               bullet.bullet);

        addColliderComponent(entity, 0, 0, bullet.bullet.w, bullet.bullet.h, 1);
        canCollideWithEnemy(entity);

        addHealthComponent(entity, 1);
    }
}

void World::createEnemyBullet(int spawnPoint) {
    Position *spawnPointPosition = &positionsMap[spawnPoint];
    BulletSpawnPoint *bulletSpawnPoint = &bulletSpawnPointsMap[spawnPoint];


    for (BulletDefinition bullet : bulletSpawnPoint->bullets) {
        unsigned int entity = createProjectile(spawnPointPosition->x + bullet.xOffset,
                                               spawnPointPosition->y + bullet.yOffset,
                                               bullet.velocity,
                                               bullet.angle,
                                               bullet.bullet);

        addColliderComponent(entity, 0.0, 0.0, bullet.bullet.w, bullet.bullet.h, 1);
        canCollideWithPlayer(entity);

        addHealthComponent(entity, 1);
    }
}

void World::createDroppableItem(unsigned int attractorEntity, Droppable droppable) {
    TextureAtlasLocation textureAtlasLocation = textureAtlasLocationMap.at(droppable.textureAtlasLocation);

    unsigned int entity = createProjectile(droppable.xPosition,
                                      droppable.yPosition,
                                      droppable.xSpeed,
                                      droppable.ySpeed,
                                      textureAtlasLocation);

    addColliderComponent(entity, 0, 0, textureAtlasLocation.w, textureAtlasLocation.h, 1);
    canCollideWithPlayer(entity);
    addScoreComponent(entity, droppable.points);
    addAttractableComponent(entity, attractorEntity);
}