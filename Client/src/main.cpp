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
	sf::Clock clock;

	sf::Clock gameClock;
	// creating objects
	int snakesLength = 20;
	Snake snake({ 5 * FIELD_WIDTH, 5 * FIELD_HEIGHT }, sf::Color(15, 150, 0, 200), snakesLength);
	Snake snakeOther({ 10 * FIELD_WIDTH, 8 * FIELD_HEIGHT }, sf::Color(240, 0, 0, 200), snakesLength);
	Snake snakeOther2({ 15 * FIELD_WIDTH, 5 * FIELD_HEIGHT }, sf::Color(0, 120, 255, 200), snakesLength);
	Snake snakeOther3({ 17 * FIELD_WIDTH, 9 * FIELD_HEIGHT }, sf::Color::Yellow, snakesLength);
	Snake snakeOther4({ 1 * FIELD_WIDTH, 14 * FIELD_HEIGHT }, sf::Color(160, 160, 160, 200), snakesLength);

	Snake *players[4] = { &snakeOther, &snakeOther2, &snakeOther3, &snakeOther4 };
	Scoreboard scoreboard(players, 4, &gameClock);

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
				case sf::Keyboard::Subtract:
					if (SPEED > 2) SPEED -= 0.5;
					break;
				case sf::Keyboard::Add:
					if (SPEED < 60) SPEED += 0.5;
					break;
				case sf::Keyboard::R:
					snake.m_body.deleteAllSegments();
					snakeOther.m_body.deleteAllSegments();
					snakeOther2.m_body.deleteAllSegments();
					snakeOther3.m_body.deleteAllSegments();
					snakeOther4.m_body.deleteAllSegments();
					snake.m_fruits = 0;
					snakeOther.m_fruits = 0;
					snakeOther2.m_fruits = 0;
					snakeOther3.m_fruits = 0;
					snakeOther4.m_fruits = 0;
					gameClock.restart();
					break;
				case sf::Keyboard::Escape:
					window.close();
					break;
				}
				break;
			}
		}

		// update everything here...
		if (gameClock.getElapsedTime().asSeconds() > 180) {
			sf::sleep(sf::seconds(5));
			window.close();
		}

		// delay
		if (clock.getElapsedTime() > sf::seconds(1 / SPEED)) {
			//snake.move(fruit);
			snakeOther.moveAutomatically(fruit);
			snakeOther2.moveAutomatically(fruit);
			snakeOther3.moveAutomatically(fruit);
			snakeOther4.moveAutomatically(fruit);
			clock.restart();
		}

		// clear the window with black color
		window.clear(sf::Color(240, 240, 240));


		// draw everything here...

		window.draw(fruit);
		
		window.draw(snakeOther4);
		window.draw(snakeOther3);
		window.draw(snakeOther2);
		window.draw(snakeOther);
		//window.draw(snake);
		
		window.draw(scoreboard);

		// end the current frame
		window.display();
	}

	return 0;
}