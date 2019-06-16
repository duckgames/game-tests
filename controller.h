//
// Created by kevin on 16/06/19.
//

#ifndef UNTITLED_CONTROLLER_H
#define UNTITLED_CONTROLLER_H

#include <string>

static int BUTTON_ACTION_UP = 0;
static int BUTTON_ACTION_DOWN = 1;
static int BUTTON_ACTION_LEFT = 2;
static int BUTTON_ACTION_RIGHT = 3;

static int BUTTON_MOVE_UP = 4;
static int BUTTON_MOVE_DOWN = 5;
static int BUTTON_MOVE_LEFT = 6;
static int BUTTON_MOVE_RIGHT = 7;

static int BUTTON_LEFT_SHOULDER = 8;
static int BUTTON_RIGHT_SHOULDER = 9;

static int BUTTON_BACK = 10;
static int BUTTON_START = 11;

struct GameButtonState
{
    int halfTransitionCount;
    bool endedDown;
};

struct GameControllerInput
{
    bool isConnected;
    bool isAnalog;
    float stickAverageX;
    float stickAverageY;

    union
    {
        GameButtonState buttons[12];
        struct
        {
            GameButtonState actionUp;
            GameButtonState actionDown;
            GameButtonState actionLeft;
            GameButtonState actionRight;

            GameButtonState moveUp;
            GameButtonState moveDown;
            GameButtonState moveLeft;
            GameButtonState moveRight;

            GameButtonState leftShoulder;
            GameButtonState rightShoulder;

            GameButtonState back;
            GameButtonState start;

            // NOTE(casey): All buttons must be added above this line

            GameButtonState terminator;
        };
    };
};

typedef struct GameInput
{
    GameButtonState mouseButtons[3];
    int mouseX, mouseY, mouseZ;

    GameControllerInput controller;

    float dtForFrame;
};

static int requestButtonPress(sf::RenderWindow *window, char *buttonName) {
    sf::Event event;

    bool waiting = true;
    printf("Press %s\n ", buttonName);
    while (waiting) {
        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::JoystickButtonPressed:
                    return event.joystickButton.button;
            }
        }
    }
}

static void calibrate(sf::RenderWindow *window) {
    BUTTON_ACTION_UP = requestButtonPress(window, "Action Up");
    BUTTON_ACTION_DOWN = requestButtonPress(window, "Action Down");
    BUTTON_ACTION_LEFT = requestButtonPress(window, "Action Left");
    BUTTON_ACTION_RIGHT = requestButtonPress(window, "Action Right");
    
    BUTTON_MOVE_UP = requestButtonPress(window, "Move Up");
    BUTTON_MOVE_DOWN = requestButtonPress(window, "Move Down");
    BUTTON_MOVE_LEFT = requestButtonPress(window, "Move Left");
    BUTTON_MOVE_RIGHT = requestButtonPress(window, "Move Right");

    BUTTON_LEFT_SHOULDER = requestButtonPress(window, "Left Shoulder");
    BUTTON_RIGHT_SHOULDER = requestButtonPress(window, "Right Shoulder");

    BUTTON_BACK = requestButtonPress(window, "Back");
    BUTTON_START = requestButtonPress(window, "Start");
}

#endif //UNTITLED_CONTROLLER_H
