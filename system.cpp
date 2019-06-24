//
// Created by kevin on 11/11/18.
//

#include <cstdio>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "components.h"
#include "system.h"
#include "controller.h"

void System::jumpers(float delta, sf::RenderWindow *window) {
    unsigned int entity;
    Jump *jump;
    Draw *draw;

    for(entity = 0; entity < MAX_ENTITIES; ++entity) {
        if((world->mask[entity] & JUMP_MASK | DRAW_MASK) == JUMP_MASK | DRAW_MASK) {
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
}

void System::updateControllables(float delta, GameControllerInput *input, sf::RenderWindow *window) {
    unsigned int entity;
    Draw *draw;
    Position *position;
    Controllable *controllable;

    for (entity = 0; entity < MAX_ENTITIES; ++entity) {
        if ((world->mask[entity] & DRAW_MASK | POSITION_MASK | CONTROLLABLE_MASK) == DRAW_MASK | POSITION_MASK | CONTROLLABLE_MASK) {
            draw = &(world->draw[entity]);
            position = &(world->position[entity]);
            controllable = &(world->controllable[entity]);

            position->x += (input->stickAverageX * controllable->xSpeed) * delta;
            position->y += (input->stickAverageY * controllable->ySpeed) * delta;

            draw->rectangleShape.setPosition(position->x, position->y);
            draw = &(world->draw[entity]);
            window->draw(draw->rectangleShape);
        }
    }
}