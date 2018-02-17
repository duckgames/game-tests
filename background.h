//
// Created by kevin on 10/02/18.
//

#ifndef UNTITLED_BACKGROUND_H
#define UNTITLED_BACKGROUND_H

#include <SFML/Graphics.hpp>
#include <cmath>

enum ScrollAxis { X_AXIS, Y_AXIS };

class Background {

private:
	sf::RenderWindow *window;
    sf::RectangleShape first;
    sf::RectangleShape second;
    sf::Vector2u textureSize;
    float scrollSpeed;
	ScrollAxis scrollAxis;

	float *toChangeFirst;
	float *toChangeSecond;
	float scrollAxisSize;
	sf::Vector2f newFirst;
	sf::Vector2f newSecond;
	float scrollDirectionAdjustment = 1.0f;

public:
    Background(sf::RenderWindow *window, sf::Texture *backgroundTexture, sf::Vector2f startPosition, float scrollSpeed, ScrollAxis scrollAxis) {
		this->window = window;
        this->textureSize = backgroundTexture->getSize();
        this->scrollSpeed = scrollSpeed;
		this->scrollAxis = scrollAxis;

        first = sf::RectangleShape(sf::Vector2f(textureSize));
        first.setTexture(backgroundTexture);

        second = sf::RectangleShape(sf::Vector2f(textureSize));
        second.setTexture(backgroundTexture);

		if (scrollAxis == X_AXIS) {
			first.setPosition(0.0f, startPosition.y - (textureSize.y / 2));
			second.setPosition(0.0f - (float)textureSize.x, startPosition.y - (textureSize.y / 2));
			newFirst.x = first.getPosition().x;
			newFirst.y = first.getPosition().y;
			newSecond.x = second.getPosition().x;
			newSecond.y = second.getPosition().y;
			toChangeFirst = &newFirst.x;
			toChangeSecond = &newSecond.x;
			scrollAxisSize = backgroundTexture->getSize().x;
		}
		else {
			first.setPosition(startPosition.x - (textureSize.x / 2), 0.0f);
			second.setPosition(startPosition.x - (textureSize.x / 2), startPosition.y -(float)textureSize.y);
			newFirst.x = first.getPosition().x;
			newFirst.y = first.getPosition().y;
			newSecond.x = second.getPosition().x;
			newSecond.y = second.getPosition().y;
			toChangeFirst = &newFirst.y;
			toChangeSecond = &newSecond.y;
			scrollAxisSize = backgroundTexture->getSize().y;
		}

		if (scrollSpeed < 0) {
			scrollDirectionAdjustment = -1.0f;
		}
    }

    ~Background() {}

	void testUpdate2(float delta) {
		float distanceToAdd = scrollSpeed * delta;
		*toChangeFirst += distanceToAdd;
		*toChangeSecond += distanceToAdd;

		if (*toChangeFirst >= scrollAxisSize || *toChangeFirst <= 0.0f - scrollAxisSize) {
			*toChangeFirst = *toChangeSecond - (scrollAxisSize * scrollDirectionAdjustment);
		}
		else if (*toChangeSecond >= scrollAxisSize || *toChangeSecond <= 0.0f - scrollAxisSize) {
			*toChangeSecond = *toChangeFirst - (scrollAxisSize * scrollDirectionAdjustment);
		}

		if (scrollSpeed > 0 && *toChangeFirst > *toChangeSecond ||
				scrollSpeed < 0  && *toChangeFirst < *toChangeSecond) {
			*toChangeSecond = *toChangeFirst - (scrollAxisSize * scrollDirectionAdjustment);
		}
		else {
			*toChangeFirst = *toChangeSecond - (scrollAxisSize * scrollDirectionAdjustment);
		}

		first.setPosition(newFirst);
		second.setPosition(newSecond);
	}

	void testUpdate(float delta) {
		float distanceToAdd = scrollSpeed * delta;

		*toChangeFirst += distanceToAdd;
		*toChangeSecond += distanceToAdd;

		if (scrollSpeed > 0) {
			if (*toChangeFirst >= scrollAxisSize) {
				*toChangeFirst = *toChangeSecond - scrollAxisSize;
			}
			else if (*toChangeSecond >= scrollAxisSize) {
				*toChangeSecond = *toChangeFirst - scrollAxisSize;
			}

			if (*toChangeFirst > *toChangeSecond) {
				*toChangeSecond = *toChangeFirst - scrollAxisSize;
			}
			else {
				*toChangeFirst = *toChangeSecond - scrollAxisSize;
			}
		}
		else if (scrollSpeed < 0) {
			if (*toChangeFirst <= 0.0f - scrollAxisSize) {
				*toChangeFirst = *toChangeSecond + scrollAxisSize;
			}
			else if (*toChangeSecond <= 0.0f - scrollAxisSize) {
				*toChangeSecond = *toChangeFirst + scrollAxisSize;
			}

			if (*toChangeFirst < *toChangeSecond) {
				*toChangeSecond = *toChangeFirst + scrollAxisSize;
			}
			else {
				*toChangeFirst = *toChangeSecond + scrollAxisSize;
			}
		}

		first.setPosition(newFirst);
		second.setPosition(newSecond);
	}

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
