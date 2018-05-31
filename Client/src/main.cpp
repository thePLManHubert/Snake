#include "Game.h"
#include "Snake.h"
#include "Fruit.h"
#include <SFML/Graphics.hpp>

int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(MAP_WIDTH, MAP_HEIGHT), "My window");

	// game clock
	const float SPEED = 10;
	sf::Clock clock;

	// creating objects
	Snake snake({5*FIELD_WIDTH, 5*FIELD_HEIGHT});
	Fruit fruit(sf::Vector2i(32, 160));

	// vertical sync
	window.setVerticalSyncEnabled(true);

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::KeyPressed:

				switch (event.key.code) 
				{
				case sf::Keyboard::Up:
					snake.setDirection(Snake::Direction::UP);
					break;
				case sf::Keyboard::Down:
					snake.setDirection(Snake::Direction::DOWN);
					break;
				case sf::Keyboard::Left:
					snake.setDirection(Snake::Direction::LEFT);
					break;
				case sf::Keyboard::Right:
					snake.setDirection(Snake::Direction::RIGHT);
					break;
				}
				break;
			}
		}

		// clear the window with black color
		window.clear(sf::Color::White);

		// delay
		if (clock.getElapsedTime() > sf::seconds(1/SPEED)) {
			snake.move(fruit);
			clock.restart();
		}

		// draw everything here...
		window.draw(fruit);
		window.draw(snake);
		

		// end the current frame
		window.display();
	}

	return 0;
}