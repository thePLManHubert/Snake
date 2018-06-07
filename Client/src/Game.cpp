#include "Game.h"
#include <iostream>


Game::Game(int maxnPlayers, int gameTime, bool collision)
	: m_stage(InMenu),
	m_playingLength(gameTime),
	MAX_PLAYERS(maxnPlayers),
	m_nPlayers(0),
	m_speed(10),
	m_collisionEnabled(collision),
	m_snakesPtr(nullptr),
	m_scoreboardPtr(nullptr),
	m_fruitPtr(nullptr),
	m_collisionMatrix(nullptr)
{
	srand(time(NULL));
	start();
}

Game::~Game() {
	Snake ** player = m_snakesPtr;
	for (int i = 0; i < m_nPlayers; i++)
		delete player[i];
	delete[] m_snakesPtr;
}

void Game::control(sf::Event & event, sf::RenderWindow& window) {
	switch (event.type)
	{
	case sf::Event::Closed:
		window.close();
		break;

	case sf::Event::KeyPressed:

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
			reset();
			break;
		case sf::Keyboard::S:
			resetPlayerStatus();
			break;
		case sf::Keyboard::Escape:
			window.close();
			break;
		}
		break;
	}
}

void Game::start() {
	m_snakesPtr = new Snake*[MAX_PLAYERS];

	m_snakesPtr[0] = new Snake({ 4 * FIELD_WIDTH, 5 * FIELD_HEIGHT }, sf::Color(15, 150, 0, 200), 20, false);
	m_snakesPtr[1] = new Snake({ MAP_WIDTH - 5* FIELD_WIDTH, 5 * FIELD_HEIGHT }, sf::Color(240, 0, 0, 200), 20, false);
	m_nPlayers = 2;
	m_scoreboardPtr = new Scoreboard(m_snakesPtr, m_nPlayers, &m_clock, m_playingLength);

	m_fruitPtr = new Fruit(m_fruitPtr->preparePosition(*this));
}

void Game::reset() {
	if(m_snakesPtr)
	for (int i = 0; i < m_nPlayers; i++) {
		m_snakesPtr[i]->m_body.deleteAllSegments();
		m_snakesPtr[i]->m_fruits = 0;
		m_snakesPtr[i]->m_direction = Snake::Direction::STOP;
	}
	m_scoreboardPtr->setTime(m_playingLength);
	m_clock.restart();
}

void Game::resetPlayerStatus() {
	m_snakesPtr[0]->m_body.deleteAllSegments();
	m_snakesPtr[0]->m_fruits = 0;
	m_snakesPtr[0]->m_direction = Snake::Direction::STOP;
}

void Game::update() {
	if (!m_snakesPtr) return;

	bool hitFruit = false;
	hitFruit = m_snakesPtr[0]->move(*m_fruitPtr);

	for (int i = 1; i < m_nPlayers; i++) {
		hitFruit = m_snakesPtr[i]->move(*m_fruitPtr) || hitFruit;
	}
	if (hitFruit)
		m_fruitPtr->setPosition(m_fruitPtr->preparePosition(*this));
	if(m_collisionEnabled)
		detectCollision();

	m_scoreboardPtr->update();
}

void Game::detectCollision() {
	if (!m_snakesPtr && m_clock.getElapsedTime().asSeconds() < 1) return;
	for (int j = 0; j < m_nPlayers; j++) {
		for (int i = 0; i < m_nPlayers; i++) {
			if (j == i) continue;
			if (m_snakesPtr[i]->comparePosition(&m_snakesPtr[j]->m_head))
				m_snakesPtr[j]->setDirection(Snake::Direction::FREEZE);
		}
	}
}

int Game::getnPlayers() const {
	return m_nPlayers;
}

float Game::getSpeed() const {
	return m_speed;
}

Snake ** Game::getSnakes() const {
	return m_snakesPtr;
}

Fruit & Game::getFruit() {
	return *m_fruitPtr;
}

void Game::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(*m_fruitPtr);
	if (!m_snakesPtr) return;

	for (int i = m_nPlayers - 1; i > -1; i--) {
		target.draw(*m_snakesPtr[i]);
	}
	target.draw(*m_scoreboardPtr);
}

sf::Vector2i Fruit::preparePosition(Game & game) {
	sf::Vector2i position;
	bool good = true;

#ifdef DEBUG
	int i = 0;
#endif

	do {
		good = true;

#ifdef DEBUG
		i++;
#endif

		position.x = (rand() % 20) * FIELD_WIDTH;
		position.y = (rand() % 15) * FIELD_HEIGHT;

		for (int i = 0; i < game.getnPlayers(); i++) {
			if (game.getSnakes()[i]->m_head.getPosition() == position) good = false;
			auto segment = game.getSnakes()[i]->m_body.tail;
			if (segment) {
				for (segment; segment->next; segment = segment->next)
					if (segment->getPosition() == position) good = false;
				if (segment->getPosition() == position) good = false;
			}
		}

	} while (!good);
#ifdef DEBUG
	std::cout << "Generowano " << i << " razy." << std::endl;
#endif
	return position;
}