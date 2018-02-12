//
// Created by kevin on 10/02/18.
//

#ifndef UNTITLED_BACKGROUND_H
#define UNTITLED_BACKGROUND_H

#include <SFML/Graphics.hpp>

class Background {

private:
    sf::RectangleShape first;
    sf::RectangleShape second;
    sf::Vector2u textureSize;
    sf::Vector2f scrollSpeed;

public:
    Background(sf::Vector2u textureSize, sf::Vector2f scrollSpeed) {
        first = sf::RectangleShape(sf::Vector2f(textureSize));
        first.setFillColor(sf::Color::Green);
        first.setPosition(0.0f, 0.0f);

        second = sf::RectangleShape(sf::Vector2f(textureSize));
        second.setFillColor(sf::Color::Red);
        second.setPosition(0.0f, -(float)textureSize.y);

        this->textureSize = textureSize;
        this->scrollSpeed = scrollSpeed;
    }

    ~Background() {}

    void update (float delta) {
        if (first.getPosition().y >= textureSize.y) {
            first.setPosition(0.0f, second.getPosition().y - textureSize.y);
        }
        else if (second.getPosition().y >= textureSize.y) {
            second.setPosition(0.0f, first.getPosition().y - textureSize.y);
        }

        sf::Vector2f firstPos = first.getPosition() + (scrollSpeed * delta);
        sf::Vector2f secondPos = second.getPosition() + (scrollSpeed * delta);

        first.setPosition(firstPos);
        second.setPosition(secondPos);
    }

    sf::RectangleShape getFirst() {
        return first;
    }

    sf::RectangleShape getSecond() {
        return second;
    }
};

#endif //UNTITLED_BACKGROUND_H
