#include "Game.h"
#include "Snake.h"
#include "Fruit.h"
#include "Scoreboard.h"
#include <SFML/Graphics.hpp>
#include <iostream>


extern const int SCOREBOARD_HEIGHT;
sf::Mutex mutex;

int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(MAP_WIDTH, MAP_HEIGHT + SCOREBOARD_HEIGHT), "My window");

	// vertical sync
	window.setVerticalSyncEnabled(true);
	window.setTitle("Snake");

	// create game
	//Game game(2);
	Game game(2, "192.168.0.5");

#ifdef DEBUG
	window.setVerticalSyncEnabled(true);
	sf::Clock clk;
	sf::Clock fpsClock;
#endif

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event)) {
			game.control(event, window);
		}
		
#ifdef DEBUG
		std::string fps = "Snake fps: " + std::to_string((int)(1 / fpsClock.restart().asSeconds()));
		if (clk.getElapsedTime().asSeconds() > 1/10.) {
			clk.restart();
			window.setTitle(fps);
		}
#endif
		game.update();

		window.clear(sf::Color(240, 240, 240));
		
		mutex.lock();
		window.draw(game);

		window.display();
		mutex.unlock();
	}

	
	return 0;
}