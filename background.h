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
	Background(sf::RenderWindow *window, sf::Texture *backgroundTexture, sf::Vector2f startPosition,
			   float scrollSpeed, ScrollAxis scrollAxis);

	~Background();

	void testUpdate2(float delta);

	void testUpdate(float delta);

	void update (float delta);

	void draw();
};

#endif //UNTITLED_BACKGROUND_H
