//
// Created by kevin on 10/02/18.
//

#ifndef UNTITLED_BACKGROUND_H
#define UNTITLED_BACKGROUND_H

#include <SFML/Graphics.hpp>

class Background {

private:
    sf::Texture backgroundTexture;
    sf::RectangleShape first;
    sf::RectangleShape second;
    sf::Vector2u textureSize;
    sf::Vector2f scrollSpeed;

public:
    Background(std::string texturePath, sf::Vector2f startPosition, sf::Vector2f scrollSpeed) {
        backgroundTexture.loadFromFile(texturePath);
        this->textureSize = backgroundTexture.getSize();
        this->scrollSpeed = scrollSpeed;


        first = sf::RectangleShape(sf::Vector2f(textureSize));
        first.setTexture(&backgroundTexture);
        first.setPosition(startPosition.x - (textureSize.x / 2), 0.0f);

        second = sf::RectangleShape(sf::Vector2f(textureSize));
        second.setTexture(&backgroundTexture);
        second.setPosition(startPosition.x - (textureSize.x / 2), startPosition.y -(float)textureSize.y);
    }

    ~Background() {}

    void update (float delta) {
        sf::Vector2f firstPos = first.getPosition() + (scrollSpeed * delta);
        sf::Vector2f secondPos = second.getPosition() + (scrollSpeed * delta);

        if (scrollSpeed.y > 0) {
            if (firstPos.y >= textureSize.y) {
                firstPos.y = secondPos.y - textureSize.y;
            }
            else if (secondPos.y >= textureSize.y) {
                secondPos.y = firstPos.y - textureSize.y;
            }

            if (firstPos.y > secondPos.y) {
                first.setPosition(firstPos);
                second.setPosition(secondPos.x, firstPos.y - textureSize.y);
            }
            else {
                second.setPosition(secondPos);
                first.setPosition(firstPos.x, secondPos.y - textureSize.y);
            }
        }
        else {
            if (firstPos.y <= 0.0f - textureSize.y) {
                firstPos.y = secondPos.y + textureSize.y;
            }
            else if (secondPos.y <= 0.0f - textureSize.y) {
                secondPos.y = firstPos.y + textureSize.y;
            }

            if (firstPos.y < secondPos.y) {
                first.setPosition(firstPos);
                second.setPosition(secondPos.x, firstPos.y + textureSize.y);
            }
            else {
                second.setPosition(secondPos);
                first.setPosition(firstPos.x, secondPos.y + textureSize.y);
            }
        }
    }

    sf::RectangleShape getFirst() {
        return first;
    }

    sf::RectangleShape getSecond() {
        return second;
    }
};

#endif //UNTITLED_BACKGROUND_H
