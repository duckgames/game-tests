//
// Created by kevin on 10/02/18.
//

#ifndef UNTITLED_BACKGROUND_H
#define UNTITLED_BACKGROUND_H

#include <SFML/Graphics.hpp>
enum ScrollAxis { X_AXIS, Y_AXIS };

class Background {

private:
	sf::RenderWindow *window;
    sf::Texture backgroundTexture;
    sf::RectangleShape first;
    sf::RectangleShape second;
    sf::Vector2u textureSize;
    float scrollSpeed;
	ScrollAxis scrollAxis;

public:
    Background(sf::RenderWindow *window, std::string texturePath, sf::Vector2f startPosition, float scrollSpeed, ScrollAxis scrollAxis) {
		this->window = window;
        backgroundTexture.loadFromFile(texturePath);
        this->textureSize = backgroundTexture.getSize();
        this->scrollSpeed = scrollSpeed;
		this->scrollAxis = scrollAxis;

        first = sf::RectangleShape(sf::Vector2f(textureSize));
        first.setTexture(&backgroundTexture);

        second = sf::RectangleShape(sf::Vector2f(textureSize));
        second.setTexture(&backgroundTexture);

		if (scrollAxis == X_AXIS) {
			first.setPosition(0.0f, startPosition.y - (textureSize.y / 2));
			second.setPosition(0.0f - (float)textureSize.x, startPosition.y - (textureSize.y / 2));
		}
		else {
			first.setPosition(startPosition.x - (textureSize.x / 2), 0.0f);
			second.setPosition(startPosition.x - (textureSize.x / 2), startPosition.y -(float)textureSize.y);
		}
    }

    ~Background() {}

    void update (float delta) {
        float firstPos;
        float secondPos;

		if (this->scrollAxis == X_AXIS) {
			firstPos = first.getPosition().x + (scrollSpeed * delta);
			secondPos = second.getPosition().x + (scrollSpeed * delta);
			float y = first.getPosition().y;

			if (scrollSpeed > 0) {
				if (firstPos >= textureSize.x) {
					firstPos = secondPos - textureSize.x;
				}
				else if (secondPos >= textureSize.x) {
					secondPos = firstPos - textureSize.x;
				}

				if (firstPos > secondPos) {
					first.setPosition(firstPos, y);
					second.setPosition(firstPos - textureSize.x, y);
				}
				else {
					second.setPosition(secondPos, y);
					first.setPosition(secondPos - textureSize.x, y);
				}
			}
			else if (scrollSpeed < 0) {
				if (firstPos <= 0.0f - textureSize.x) {
					firstPos = secondPos + textureSize.x;
				}
				else if (secondPos <= 0.0f - textureSize.x) {
					secondPos = firstPos + textureSize.x;
				}

				if (firstPos < secondPos) {
					first.setPosition(firstPos, y);
					second.setPosition(firstPos + textureSize.x, y);
				}
				else {
					second.setPosition(secondPos, y);
					first.setPosition(secondPos + textureSize.x, y);
				}
			}
		}
		else {
			firstPos = first.getPosition().y + (scrollSpeed * delta);
			secondPos = second.getPosition().y + (scrollSpeed * delta);
			float x = first.getPosition().x;

			if (scrollSpeed > 0) {
				if (firstPos >= textureSize.y) {
					firstPos = secondPos - textureSize.y;
				}
				else if (secondPos >= textureSize.y) {
					secondPos = firstPos - textureSize.y;
				}

				if (firstPos > secondPos) {
					first.setPosition(x, firstPos);
					second.setPosition(x, firstPos - textureSize.y);
				}
				else {
					second.setPosition(x, secondPos);
					first.setPosition(x, secondPos - textureSize.y);
				}
			}
			else if (scrollSpeed < 0) {
				if (firstPos <= 0.0f - textureSize.y) {
					firstPos = secondPos + textureSize.y;
				}
				else if (secondPos <= 0.0f - textureSize.y) {
					secondPos = firstPos + textureSize.y;
				}

				if (firstPos < secondPos) {
					first.setPosition(x, firstPos);
					second.setPosition(x, firstPos + textureSize.y);
				}
				else {
					second.setPosition(x, secondPos);
					first.setPosition(x, secondPos + textureSize.y);
				}
			}
		}
    }

	void draw() {
		window->draw(first);
		window->draw(second);
	}
};

#endif //UNTITLED_BACKGROUND_H
