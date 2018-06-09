#include "Game.h"


void Game::controlMenu(sf::Event event, sf::RenderWindow& window) {
	switch (event.type)
	{
	case sf::Event::Closed:
		window.close();
		break;

	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Enter:
			setStage(InSingleplayer);
			break;
		case sf::Keyboard::M:
			setStage(InQueue);
			m_clientPtr = new Client(this);
			if (!m_clientPtr->isConnected()) {
				setStage(InMenu);
				deleteClient();
			}
			break;
		case sf::Keyboard::Escape:
			window.close();
			break;
		}
		break;
	}
}

/*------------------------------------------------------------------------------------------*/

void Game::controlQueue(sf::Event event, sf::RenderWindow& window) {
	switch (event.type)
	{
	case sf::Event::Closed:
		window.close();
		if (m_clientPtr)
			deleteClient();
		break;

	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Up:
			// do sth...
			break;
		case sf::Keyboard::Escape:
			setStage(InMenu);
			if (m_clientPtr)
				deleteClient();
			break;
		}
		break;
	}
}

/*------------------------------------------------------------------------------------------*/

void Game::controlSingleplayer(sf::Event event, sf::RenderWindow& window) {
	switch (event.type)
	{
	case sf::Event::Closed:
		window.close();
		break;

	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape)
			setStage(InMenu);

		if (!m_snakesPtr) return;
		switch (event.key.code)
		{
		case sf::Keyboard::Up:
			(*m_snakesPtr)->setDirection(Snake::Direction::UP);
			break;
		case sf::Keyboard::Down:
			(*m_snakesPtr)->setDirection(Snake::Direction::DOWN);
			break;
		case sf::Keyboard::Left:
			(*m_snakesPtr)->setDirection(Snake::Direction::LEFT);
			break;
		case sf::Keyboard::Right:
			(*m_snakesPtr)->setDirection(Snake::Direction::RIGHT);
			break;
		case sf::Keyboard::Space:
			(*m_snakesPtr)->setDirection(Snake::Direction::STOP);
			break;
		case sf::Keyboard::Subtract:
			if (m_speed > 2) m_speed -= 0.5;
			break;
		case sf::Keyboard::Add:
			if (m_speed < 60) m_speed += 0.5;
			break;
		case sf::Keyboard::R:
			resetSingleplayer();
			break;
		case sf::Keyboard::S:
			resetPlayerStatus();
			break;
		}
		break;
	}
}

/*------------------------------------------------------------------------------------------*/

void Game::controlMultiplayer(sf::Event event, sf::RenderWindow& window) {
	switch (event.type)
	{
	case sf::Event::Closed:
		window.close();
		break;

	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Up:
			// do sth...
			break;
		case sf::Keyboard::Escape:
			setStage(InQueue);
			break;
		}
		break;
	}
}

