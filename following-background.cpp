//
// Created by kevin on 10/11/18.
//

#include "following-background.h"

FollowingBackground::FollowingBackground(sf::RenderWindow *window, sf::Texture *backgroundTexture, float scrollSpeed, unsigned int screenHeight) {
    FollowingBackground::window = window;
    this->scrollSpeed = scrollSpeed;
    
    background = sf::RectangleShape(sf::Vector2f(backgroundTexture->getSize()));
    background.setTexture(backgroundTexture);
    background.setOrigin(0.0f, 0.0f);
    background.setPosition(0.0f, screenHeight - backgroundTexture->getSize().y);
}

FollowingBackground::~FollowingBackground() {}

void FollowingBackground::moveX(float delta, bool positive) {
    float change = scrollSpeed * delta;
    
    positive ? background.setPosition(background.getPosition().x + change, background.getPosition().y) : background.setPosition(background.getPosition().x - change, background.getPosition().y);
}

void FollowingBackground::moveY(float delta, bool positive) {
    float change = scrollSpeed * delta;
    positive ? background.setPosition(background.getPosition().x, background.getPosition().y + change) : background.setPosition(background.getPosition().x, background.getPosition().y - change);
}

void FollowingBackground::draw() {
    window->draw(background);
}