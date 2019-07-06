//
// Created by kevin on 06/07/19.
//

#ifndef UNTITLED_ENTITYPOOL_H
#define UNTITLED_ENTITYPOOL_H

#include <vector>

class EntityPool {
public:
    std::vector<int> inUse;
    std::vector<int> available;

    std::vector<int> entities;

    EntityPool(int maxEntities): entities(maxEntities) {
        available = entities;
    };
    ~EntityPool() = default;
    void usePooledObject();
    void releaseObject(int index);
};


#endif //UNTITLED_ENTITYPOOL_H
