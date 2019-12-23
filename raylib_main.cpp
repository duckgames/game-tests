/*******************************************************************************************
*
*   LayoutName v1.0.0 - Tool Description
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2019 raylib technologies. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_RICONS
#include "lib/lua/sol.hpp"
#include "lib/raylib/raygui.h"
#include "world.h"
#include "system.h"
#include <iostream>

static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;
static const double timePerFrame = 1.0f / 120.0f;

typedef enum GameState {
    STATE_MENU,
    STATE_PLAYING,
    STATE_DEAD
} GameState;

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------
static void PlayButton();
static void DieButton();
static void MenuButton();

GameState gameState = STATE_MENU;
Texture2D backgroundTexture;
int player;

// If this is being called to create a follower entity, the owning entity should be the ID of the owner (>= 0 && < MAX_ENTITIES).
// If it is not a follower entity, any negative value.
int createEntity(World *world, sol::table entityData, int owningEntity) {
    int entity = world->createEntity();

    // Add Draw Component
    sol::optional<sol::table> drawExists = entityData["components"]["draw"];
    if (drawExists != sol::nullopt) {
        std::string textureAtlasLocation = entityData["components"]["draw"]["textureAtlasLocation"];
        world->addDrawComponent(entity, world->textureAtlasLocationMap.at(textureAtlasLocation));
    }

    // Add Position Component
    sol::optional<sol::table> positionExists = entityData["components"]["position"];
    if (positionExists != sol::nullopt) {
        world->addPositionComponent(entity,
                                    static_cast<float>(entityData["components"]["position"]["x"]),
                                    static_cast<float>(entityData["components"]["position"]["y"])
        );
    }

    // Add Direction Component
    sol::optional<sol::table> directionExists = entityData["components"]["direction"];
    if (directionExists != sol::nullopt) {
        world->addDirectionComponent(entity,
                                     static_cast<float>(entityData["components"]["direction"]["xMove"]),
                                     static_cast<float>(entityData["components"]["direction"]["yMove"])
        );
    }

    // Add Controllable Component
    sol::optional<sol::table> controllableExists = entityData["components"]["controllable"];
    if (controllableExists != sol::nullopt) {
        player = entity;
        world->addControllableComponent(entity,
                                        static_cast<float>(entityData["components"]["controllable"]["xSpeed"]),
                                        static_cast<float>(entityData["components"]["controllable"]["ySpeed"])
        );
    }

    // Add Move Component
    sol::optional<sol::table> moveExists = entityData["components"]["move"];
    if (moveExists != sol::nullopt) {
        world->addMoveComponent(entity,
                                static_cast<float>(entityData["components"]["move"]["xSpeed"]),
                                static_cast<float>(entityData["components"]["move"]["ySpeed"])
        );
    }

    // Add Leader Component
    sol::optional<sol::table> leaderExists = entityData["components"]["leader"];
    if (leaderExists != sol::nullopt) {
        std::vector<int> followers;

        sol::table followerTable = entityData["components"]["leader"]["followers"];
        unsigned int followerIndex = 0;
        while (true) {
            sol::optional<sol::table> followerExists = followerTable[followerIndex];
            if (followerExists == sol::nullopt) {
                break;
            } else {
                sol::table follower = followerTable[followerIndex];
                // Recursively call createEntity function to create a follower of the current entity
                followers.emplace_back(createEntity(world, follower, entity));
            }
            followerIndex++;
        }

        world->addLeaderComponent(entity, followers);
    }

    // Add Follower Component
    if (owningEntity >= 0) {
        sol::optional<sol::table> followerExists = entityData["components"]["follower"];
        if (followerExists != sol::nullopt) {
            world->addFollowerComponent(entity,
                                        owningEntity,
                                        static_cast<float>(entityData["components"]["follower"]["xOffset"]),
                                        static_cast<float>(entityData["components"]["follower"]["yOffset"])
            );
        }
    }

    // Add BulletSpawnPoint Component
    sol::optional<sol::table> bulletSpawnPointExists = entityData["components"]["bulletSpawnPoint"];
    if (bulletSpawnPointExists != sol::nullopt) {
        std::string textureAtlasLocation = entityData["components"]["bulletSpawnPoint"]["textureAtlasLocation"];
        world->addBulletSpawnPointComponent(entity,
                                            static_cast<float>(entityData["components"]["bulletSpawnPoint"]["rateOfFire"]),
                                            static_cast<float>(entityData["components"]["bulletSpawnPoint"]["velocity"]),
                                            static_cast<float>(entityData["components"]["bulletSpawnPoint"]["angle"]),
                                            world->textureAtlasLocationMap.at(textureAtlasLocation),
                                            static_cast<bool>(entityData["components"]["bulletSpawnPoint"]["forPlayer"])
        );
    }

    // Add Collider Component
    sol::optional<sol::table> colliderExists = entityData["components"]["collider"];
    if (colliderExists != sol::nullopt) {
        world->addColliderComponent(entity,
                                    static_cast<float>(entityData["components"]["collider"]["xOffset"]),
                                    static_cast<float>(entityData["components"]["collider"]["yOffset"]),
                                    static_cast<float>(entityData["components"]["collider"]["width"]),
                                    static_cast<float>(entityData["components"]["collider"]["height"]),
                                    static_cast<int>(entityData["components"]["collider"]["damage"])
        );
    }

    // Add Health Component
    sol::optional<sol::table> healthExists = entityData["components"]["health"];
    if (healthExists != sol::nullopt) {
        world->addHealthComponent(entity, static_cast<int>(entityData["components"]["health"]["initialHealth"]));
    }

    // Add Score Component
    sol::optional<sol::table> scoreExists = entityData["components"]["score"];
    if (scoreExists != sol::nullopt) {
        world->addScoreComponent(entity, static_cast<int>(entityData["components"]["score"]["points"]));
    }

    // Add Animation Component
    sol::optional<sol::table> animationExists = entityData["components"]["animation"];
    if (animationExists != sol::nullopt) {
        sol::table frames = entityData["components"]["animation"]["frames"];
        int numFrames = static_cast<int>(entityData["components"]["animation"]["numFrames"]);
        TextureAtlasLocation textureAtlasLocations[numFrames];

        for (int i = 0; i < numFrames; i++) {
            sol::table frame = frames[i];
            std::string textureAtlasLocation = frame["frame"];
            textureAtlasLocations[i] = world->textureAtlasLocationMap.at(textureAtlasLocation);
        }

        world->addAnimationComponent(entity,
                                     static_cast<int>(entityData["components"]["animation"]["numFrames"]),
                                     static_cast<int>(entityData["components"]["animation"]["startFrame"]),
                                     static_cast<float>(entityData["components"]["animation"]["frameDuration"]),
                                     static_cast<bool>(entityData["components"]["animation"]["loop"]),
                                     textureAtlasLocations
        );
    }

    bool isEnemy = entityData["isEnemy"];
    bool enforceXBoundary = entityData["enforceXBoundary"];
    bool enforceYBoundary = entityData["enforceYBoundary"];

    if (isEnemy) {
        world->canCollideWithPlayer(entity);
        world->enemies.insert(entity);
    }

    if (enforceXBoundary) {
        world->addXBoundaryEnforcement(entity);
    }

    if (enforceYBoundary) {
        world->addYBoundaryEnforcement(entity);
    }

    return entity;
}

int loadLevel(int levelNumber, World *world) {
    sol::state lua;
    lua["SCREEN_WIDTH"] = SCREEN_WIDTH;
    lua["SCREEN_HEIGHT"] = SCREEN_HEIGHT;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);
    std::string levelName = "level" + std::to_string(levelNumber);
    lua.script_file("../scripts/" + levelName + ".lua");

    sol::table enemies = lua["entities"];
    unsigned int entityIndex = 0;
    while (true) {
        sol::optional<sol::table> entityExists = enemies[entityIndex];
        if (entityExists == sol::nullopt) {
            break;
        } else {
            sol::table enemy = enemies[entityIndex];
            createEntity(world, enemy, -1);
        }
        entityIndex++;
    }
}

void menu() {
    BeginDrawing();

    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    if (GuiButton((Rectangle){ 500, 673, 125, 30 }, "PLAY")) PlayButton();

    EndDrawing();
}

void dead() {
    BeginDrawing();

    ClearBackground(GREEN);
    if (GuiButton((Rectangle){ 400, 673, 125, 30 }, "PLAY")) PlayButton();
    if (GuiButton((Rectangle){ 600, 673, 125, 30 }, "MENU")) MenuButton();

    EndDrawing();
}

void game(World *world, System *system, Texture2D texMap) {
    system->clearDeadEntities();
    system->processWaitingToFire();
    system->updateInfiniteBackgrounds(timePerFrame);
    system->updateMovers(timePerFrame);
  //  system->updateControllables(GetFrameTime(), &newInput->controllers[0], &newInput->keyboard);
    system->updateProjectiles(timePerFrame);
    system->enforceScreenBoundaries();
    system->updateFollowers();
    system->updateBulletSpawnPoints(timePerFrame);
    system->updatePlayerCollisions(player);
    system->updateEnemyCollisions();
    system->processPendingCollisions();
    system->updateScore();
    system->updateAnimations(timePerFrame);

    BeginDrawing();

    ClearBackground(BLACK);
    Rectangle texRect;

    for (auto drawable : world->drawablesMap) {
        Position *position;
        position = &world->positionsMap[drawable.first];

        texRect = {
                (float) drawable.second.x,
                (float) drawable.second.y,
                (float) drawable.second.width,
                (float) drawable.second.height
        };

        DrawTextureRec(texMap, texRect, (Vector2) { position->x, position->y }, WHITE);
    }

    if (GuiButton((Rectangle){ 500, 673, 125, 30 }, "DIE")) DieButton();

    EndDrawing();
}

int main()
{
    // Initialization
    World world(SCREEN_WIDTH, SCREEN_HEIGHT);
    System system(&world);

    TextureAtlasLocation background = world.textureAtlasLocationMap.at("background");
    world.createInfiniteBackground((SCREEN_WIDTH / 2) - (background.w / 2), -background.h + SCREEN_HEIGHT,
                                   0.0f, 200.0f, background);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib");

    Texture2D texMap = LoadTexture("../assets/texture-atlas.png");
    loadLevel(1, &world);

    double frameStart = GetTime();
    double timeSinceLastUpdate = GetTime();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (gameState == STATE_PLAYING) {
            timeSinceLastUpdate += (GetTime() - frameStart);
            frameStart = GetTime();

            while (timeSinceLastUpdate >= timePerFrame) {
                timeSinceLastUpdate -= timePerFrame;
                game(&world, &system, texMap);
            }
        }
        else if (gameState == STATE_DEAD) {
            dead();
        }
        else if (gameState == STATE_MENU) {
            menu();
        }
    }

    // De-Initialization
    UnloadTexture(backgroundTexture);
    CloseWindow();

    return 0;
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------
static void PlayButton()
{
    gameState = STATE_PLAYING;
}

static void DieButton()
{
    gameState = STATE_DEAD;
}

static void MenuButton() {
    gameState = STATE_MENU;
}