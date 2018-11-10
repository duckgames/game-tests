//
// Created by kevin on 10/11/18.
//

#ifndef UNTITLED_FOLLOWING_BACKGROUND_H
#define UNTITLED_FOLLOWING_BACKGROUND_H

#include <SFML/Graphics.hpp>
#include <cmath>

class FollowingBackground {

private:
    sf::RenderWindow *window;
    sf::RectangleShape background;
    float scrollSpeed;

public:
    FollowingBackground(sf::RenderWindow *window, sf::Texture *backgroundTexture, float scrollSpeed, unsigned int screenHeight);

    ~FollowingBackground();

    void moveX(float delta, bool positive);

    void moveY(float delta, bool positive);

    void draw();
};

#endif //UNTITLED_FOLLOWING_BACKGROUND_H
