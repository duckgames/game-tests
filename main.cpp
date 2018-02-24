#include <SFML/Graphics.hpp>
#include <iostream>
#include <x86intrin.h>

#include "background.h"

#define CONTROLLER_AXIS_DEADZONE 20.0f

int main() {
    float stickAverageX = 0.0f;
    float stickAverageY = 0.0f;
    float rotation = 0.0f;
    float speed = 1000.0f;

    sf::Texture specialBrew;
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
    window.setVerticalSyncEnabled(true);

    specialBrew.loadFromFile("specialbrew.png");

    sf::RectangleShape spritealBrew(sf::Vector2f(32, 32));
    spritealBrew.setTexture(&specialBrew, true);

    sf::Vector2f position = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);

    sf::Clock tickClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    sf::Time duration = sf::Time::Zero;

	sf::Texture vertTexture;
	sf::Texture horizTexture;
	vertTexture.loadFromFile("../assets/backgrounds/background.png");
	horizTexture.loadFromFile("../assets/backgrounds/background-horiz.png");

    Background background = Background(
			&window,
			&vertTexture,
            sf::Vector2f(window.getSize().x / 2, 0.0f),
            -1000.0f,
			Y_AXIS);

	Background backgroundHoriz = Background(
			&window,
			&horizTexture,
			sf::Vector2f(0.0f, window.getSize().y / 2),
			-1000.0f,
			X_AXIS);

	ulong total = 0;
	ulong loops = 0;
	ulong result = 0;
    while (window.isOpen() && loops < 7000)
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                    break;

                case sf::Event::JoystickConnected:
                    break;

                case sf::Event::JoystickDisconnected:
                    break;

                case sf::Event::JoystickMoved:
                    if (event.joystickMove.axis == sf::Joystick::Axis::X) {
                        stickAverageX = event.joystickMove.position;
                    }
                    else if (event.joystickMove.axis == sf::Joystick::Axis::Y) {
                        stickAverageY = event.joystickMove.position;
                    }
                    else if (event.joystickMove.axis == sf::Joystick::Axis::Z) {
                        rotation -= (100 + event.joystickMove.position) / 200;
                    }
                    else if (event.joystickMove.axis == sf::Joystick::Axis::R) {
                        rotation += (100 + event.joystickMove.position) / 200;
                    }
                    break;
            }
        }

        timeSinceLastUpdate += tickClock.restart();
        while (timeSinceLastUpdate > timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;

            if (stickAverageX > CONTROLLER_AXIS_DEADZONE || stickAverageX < -CONTROLLER_AXIS_DEADZONE) {
                if (stickAverageX < 0) {
                    position.x += -speed * timePerFrame.asSeconds();
                }
                else {
                    position.x += speed * timePerFrame.asSeconds();
                }
            }

            if (stickAverageY > CONTROLLER_AXIS_DEADZONE || stickAverageY < -CONTROLLER_AXIS_DEADZONE) {
                if (stickAverageY < 0) {
                    position.y += -speed * timePerFrame.asSeconds();
                }
                else {
                    position.y += speed * timePerFrame.asSeconds();
                }
            }

			ulong t1 = __rdtsc();
			ulong t2 = __rdtsc();
            background.testUpdate2(timePerFrame.asSeconds());
			ulong t3 = __rdtsc();
			result = (t3 - t2) - (t2 - t1);

			total += result;
			loops++;

			backgroundHoriz.update(timePerFrame.asSeconds());
            spritealBrew.setPosition(position);
        }

        window.clear();
        background.draw();
		backgroundHoriz.draw();
        window.draw(spritealBrew);
        window.display();

		std::cout << total / loops << std::endl;
	}

    return 0;
}