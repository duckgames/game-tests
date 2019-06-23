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
                    input->buttons[controllerNumber][event.joystickButton.button] = oldGameButtonState;
                    oldInput->buttons[controllerNumber][event.joystickButton.button] = gameButtonState;
                    waiting = false;
                    break;
            }
        }
    }
}

static void setButtons(sf::RenderWindow *window, int controllerNumber, GameInput *input, GameInput *oldInput) {
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].actionUp, &oldInput->controllers[controllerNumber].actionUp, "Action Up");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].actionDown, &oldInput->controllers[controllerNumber].actionDown, "Action Down");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].actionLeft, &oldInput->controllers[controllerNumber].actionLeft, "Action Left");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].actionRight, &oldInput->controllers[controllerNumber].actionRight, "Action Right");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].leftShoulder, &oldInput->controllers[controllerNumber].leftShoulder, "Left Shoulder");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].rightShoulder, &oldInput->controllers[controllerNumber].rightShoulder, "Right Shoulder");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].back, &oldInput->controllers[controllerNumber].back, "Back");
    requestButtonPress(window, controllerNumber, input, oldInput, &input->controllers[controllerNumber].start, &oldInput->controllers[controllerNumber].start, "Start");
}

#endif //UNTITLED_CONTROLLER_H
