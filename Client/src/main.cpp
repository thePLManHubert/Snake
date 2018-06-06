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

	// vertical sync
	window.setVerticalSyncEnabled(true);

	// create game
	Game game = Game(4);
	sf::Clock renderClock;

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			game.control(event, window);
		}
		
		if (renderClock.getElapsedTime().asSeconds() > 1. / game.getSpeed()) {
			game.update();
			renderClock.restart();
		}

		window.clear(sf::Color(240, 240, 240));

		window.draw(game);

		window.display();
	}

	
	return 0;
}