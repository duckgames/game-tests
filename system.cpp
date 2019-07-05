//
// Created by kevin on 11/11/18.
//

#include <cstdio>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "math.h"
#include "components.h"
#include "system.h"
#include "controller.h"

void System::jumpers(float delta) {
    /*
    unsigned int entity;
    Jump *jump;
    Draw *draw;

    for(entity = 0; entity < MAX_ENTITIES; ++entity) {
        if((world->mask[entity] & JUMP_MASK) == JUMP_MASK) {
            jump = &(world->jump[entity]);
            draw = &(world->draw[entity]);

            if (jump->isJumping) {
                float toAdd = jump->jumpSpeed * delta;

                if (jump->currentJumpHeight + toAdd < jump->maxHeight) {
                    jump->currentJumpHeight += toAdd;
                }
                else {
                    jump->currentJumpHeight = jump->maxHeight;
                    jump->isJumping = false;
                    jump->isFalling = true;
                    continue;
                }
            }
            if (jump->isFalling) {
                float toSubtract = jump->fallSpeed * delta;

                if (jump->currentJumpHeight - toSubtract > 0.0f) {
                    jump->currentJumpHeight -= toSubtract;
                }
                else {
                    jump->currentJumpHeight = 0.0f;
                    jump->isFalling = false;
                }
            }

            draw->rectangleShape.setPosition(draw->rectangleShape.getPosition().x, jump->currentJumpHeight);
            draw = &(world->draw[entity]);
            window->draw(draw->rectangleShape);
        }
    }
     */
}

// Note there is a nullptr check on the keyboard input - this is so keyboard input can be disabled by passing a nullptr
// for the keyboardInput parameter.
void System::updateControllables(float delta, GameControllerInput *padInput, GameControllerInput *keyboardInput) {
    Draw *draw;
    Position *position;

    for (auto controllable: world->controllable) {
        draw = &world->draw[controllable.first];
        position = &world->position[controllable.first];

        if (padInput->stickAverageX != 0 || padInput->stickAverageY != 0) {
            position->x += (padInput->stickAverageX * controllable.second.xSpeed) * delta;
            position->y += (padInput->stickAverageY * controllable.second.ySpeed) * delta;
        }
        else if (padInput->povX != 0 || padInput->povY != 0) {
            position->x += (padInput->povX * controllable.second.xSpeed) * delta;
            position->y += (padInput->povY * controllable.second.ySpeed) * delta;
        }
        else if (keyboardInput != nullptr) {
            position->x += (keyboardInput->stickAverageX * controllable.second.xSpeed) * delta;
            position->y += (keyboardInput->stickAverageY * controllable.second.ySpeed) * delta;
        }

        draw->rectangleShape.setPosition(position->x, position->y);
    }
}

void System::renderDrawables(sf::RenderWindow *window) {
    for(auto drawable : world->draw) {
        window->draw(drawable.second.rectangleShape);
    }
}