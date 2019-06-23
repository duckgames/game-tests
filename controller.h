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
    GameButtonState *buttons[NUM_BUTTONS];
    GameControllerInput controller;

    GameButtonState mouseButtons[3];
    int mouseX, mouseY, mouseZ;

    float dtForFrame;
};

static void requestButtonPress(sf::RenderWindow *window, GameInput *input, GameInput *oldInput, GameButtonState *gameButtonState, GameButtonState *oldGameButtonState, const char *buttonName) {
    sf::Event event;

    bool waiting = true;
    printf("Press %s\n ", buttonName);
    while (waiting) {
        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::JoystickButtonPressed:
                    input->buttons[event.joystickButton.button] = oldGameButtonState;
                    oldInput->buttons[event.joystickButton.button] = gameButtonState;
                    waiting = false;
                    break;
            }
        }
    }
}

static void setButtons(sf::RenderWindow *window, GameInput *input, GameInput *oldInput) {
    requestButtonPress(window, input, oldInput, &input->controller.actionUp, &oldInput->controller.actionUp, "Action Up");
    requestButtonPress(window, input, oldInput, &input->controller.actionDown, &oldInput->controller.actionDown, "Action Down");
    requestButtonPress(window, input, oldInput, &input->controller.actionLeft, &oldInput->controller.actionLeft, "Action Left");
    requestButtonPress(window, input, oldInput, &input->controller.actionRight, &oldInput->controller.actionRight, "Action Right");
    requestButtonPress(window, input, oldInput, &input->controller.leftShoulder, &oldInput->controller.leftShoulder, "Left Shoulder");
    requestButtonPress(window, input, oldInput, &input->controller.rightShoulder, &oldInput->controller.rightShoulder, "Right Shoulder");
    requestButtonPress(window, input, oldInput, &input->controller.back, &oldInput->controller.back, "Back");
    requestButtonPress(window, input, oldInput, &input->controller.start, &oldInput->controller.start, "Start");
}

#endif //UNTITLED_CONTROLLER_H
