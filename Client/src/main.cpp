#include "Game.h"
#include "Snake.h"
#include "Fruit.h"
#include "Scoreboard.h"
#include <SFML/Graphics.hpp>
#include <iostream>


extern const int SCOREBOARD_HEIGHT;

int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(MAP_WIDTH, MAP_HEIGHT + SCOREBOARD_HEIGHT), "My window");

	// game clock
	float SPEED = 10;
	int gameTime = 120;
	sf::Clock clock;

	sf::Clock gameClock;
	// creating objects
	int snakeLength = 20;
	Snake snake({ 5 * FIELD_WIDTH, 5 * FIELD_HEIGHT }, sf::Color(15, 150, 0, 200), snakeLength, true);
	Snake snake2({ 15 * FIELD_WIDTH, 5 * FIELD_HEIGHT }, sf::Color(240, 0, 0, 200), snakeLength, true);

	Snake *players[1] = { &snake };
	Scoreboard scoreboard(players, 1, &gameClock, gameTime);

	Fruit fruit(sf::Vector2i(32, 160));
	

	// vertical sync
	window.setVerticalSyncEnabled(true);

#ifdef DEBUG

	std::cout << "Sizeof(sf::Color): " << sizeof(sf::Color) << std::endl
		<< "Sizeof(sf::Vector2f): " << sizeof(sf::Vector2f) << std::endl
		<< "Sizeof(sf::Vector2i): " << sizeof(sf::Vector2i) << std::endl
		<< "Sizeof(sf::Sprite): " << sizeof(sf::Sprite) << std::endl;

#endif

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
				case sf::Keyboard::W:
					snake.setDirection(Snake::Direction::UP);
					break;
				case sf::Keyboard::S:
					snake.setDirection(Snake::Direction::DOWN);
					break;
				case sf::Keyboard::A:
					snake.setDirection(Snake::Direction::LEFT);
					break;
				case sf::Keyboard::D:
					snake.setDirection(Snake::Direction::RIGHT);
					break;
				case sf::Keyboard::Space:
					snake.setDirection(Snake::Direction::STOP);
					break;
				case sf::Keyboard::Subtract:
					if (SPEED > 2) SPEED -= 0.5;
					break;
				case sf::Keyboard::Add:
					if (SPEED < 60) SPEED += 0.5;
					break;
				case sf::Keyboard::R:
					snake.m_body.deleteAllSegments();
					snake.m_fruits = 0;
					snake2.m_body.deleteAllSegments();
					snake2.m_fruits = 0;
					gameClock.restart();
					scoreboard.setTime(gameTime);
					snake.m_direction = Snake::Direction::STOP;
					snake2.m_direction = Snake::Direction::STOP;
					break;
				case sf::Keyboard::Escape:
					window.close();
					break;
				}
				break;
			}
		}

		// update everything here...
		scoreboard.update();

		if (scoreboard.timeUp()) {
			sf::sleep(sf::seconds(3));
			window.close();
		}

		// delay
		if (clock.getElapsedTime() > sf::seconds(1 / SPEED)) {
			snake.move(fruit);
			//snake2.moveAutomatically(fruit);
			clock.restart();
		}

		// clear the window with black color
		window.clear(sf::Color(240, 240, 240));


		// draw everything here...
		window.draw(fruit);
		//window.draw(snake2);
		window.draw(snake);	
		window.draw(scoreboard);

		// end the current frame
		window.display();
	}

	return 0;
}