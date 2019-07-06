//
// Created by kevin on 06/07/19.
//

#include <cstdio>
#include "EntityPool.h"

void EntityPool::usePooledObject() {
    if (!available.empty()) {
        inUse.push_back(available.back());
        available.pop_back();
    }
    else {
        printf("EntityPool: No free objects!\n");
    }
}

void EntityPool::releaseObject(int index) {
    if (!inUse.empty()) {
        available.push_back(inUse.at(index));
        inUse.erase(inUse.begin() + index);
    }
    else {
        printf("EntityPool: Attempted to release object when none are in use!\n");
    }
}