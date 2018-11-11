//
// Created by kevin on 11/11/18.
//

#include <cstdio>
#include "components.h"
#include "system.h"

void System::jump(float delta) {
    unsigned int entity;
    Jump *jump;

    for(entity = 0; entity < MAX_ENTITIES; ++entity) {
        if((world->mask[entity] & JUMP_MASK) == JUMP_MASK) {
            jump = &(world->jump[entity]);

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
                printf("Entity #%d is jumping...\n", entity);
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
                printf("Entity #%d is falling...\n", entity);
            }
        }
    }
}
