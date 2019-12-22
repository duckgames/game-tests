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
#include "lib/raylib/raygui.h"

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

void game() {
 //   update(GetFrameTime());

    BeginDrawing();

    ClearBackground(BLACK);

 //   DrawTextureV(backgroundTexture, (Vector2) { background.first.x, background.first.y }, WHITE);
 //   DrawTextureV(backgroundTexture, (Vector2) { background.second.x, background.second.y }, WHITE);

    if (GuiButton((Rectangle){ 500, 673, 125, 30 }, "DIE")) DieButton();

    EndDrawing();
}

int main()
{
    // Initialization
    int screenWidth = 1024;
    int screenHeight = 768;

    InitWindow(screenWidth, screenHeight, "layout_name");
    SetTargetFPS(60);

    backgroundTexture = LoadTexture("../assets/background.png");

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (gameState == STATE_PLAYING) {
            game();
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