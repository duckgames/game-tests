//
// Created by kevin on 16/06/19.
//

#ifndef UNTITLED_CONTROLLER_H
#define UNTITLED_CONTROLLER_H

#include <string>

const int NUM_BUTTONS = 10;

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

    float stickAverageU;
    float stickAverageV;

    float stickAverageR;
    float stickAverageZ;

    union
    {
        GameButtonState buttons[NUM_BUTTONS];
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

            // NOTE: All buttons must be added above this line

            GameButtonState terminator;
        };
    };
};

typedef struct GameInput
{
    GameButtonState *buttons[4][NUM_BUTTONS];
    GameControllerInput controllers[4];

    GameControllerInput keyboard;

    GameButtonState mouseButtons[3];
    int mouseX, mouseY, mouseZ;

    float dtForFrame;
};

inline GameControllerInput *getController(GameInput *input, uint controllerIndex) {
    return &input->controllers[controllerIndex];
}

static void requestButtonPress(sf::RenderWindow *window, int controllerNumber, GameInput *input, GameInput *oldInput, GameButtonState *gameButtonState, GameButtonState *oldGameButtonState, const char *buttonName) {
    sf::Event event;

    bool waiting = true;
    printf("Press Controller %d %s\n ", controllerNumber, buttonName);
    while (waiting) {
        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::JoystickButtonPressed:
                    if (event.joystickButton.joystickId == controllerNumber) {
                        input->buttons[controllerNumber][event.joystickButton.button] = oldGameButtonState;
                        oldInput->buttons[controllerNumber][event.joystickButton.button] = gameButtonState;
                        waiting = false;
                    }
                    break;
            }
        }
    }
}

#endif //UNTITLED_CONTROLLER_H
