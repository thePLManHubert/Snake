#include "Game.h"

Game::Game(int maxnPlayers)
	: m_stage(InMenu),
	m_playingLength(300),
	MAX_PLAYERS(maxnPlayers),
	m_nPlayers(0),
	m_speed(10),
	m_snakesPtr(nullptr),
	m_scoreboardPtr(nullptr),
	m_fruitPtr(nullptr)
{
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
			m_snakesPtr[0]->m_body.deleteAllSegments();
			m_snakesPtr[0]->m_fruits = 0;
			m_snakesPtr[1]->m_body.deleteAllSegments();
			m_snakesPtr[1]->m_fruits = 0;
			m_clock.restart();
			m_scoreboardPtr->setTime(300);
			m_snakesPtr[0]->m_direction = Snake::Direction::STOP;
			m_snakesPtr[0]->m_direction = Snake::Direction::STOP;
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

	m_snakesPtr[0] = new Snake({ 5 * FIELD_WIDTH, 5 * FIELD_HEIGHT }, sf::Color(15, 150, 0, 200), 20, true);
	m_snakesPtr[1] = new Snake({ 15 * FIELD_WIDTH, 5 * FIELD_HEIGHT }, sf::Color(240, 0, 0, 200), 20, true);
	m_nPlayers = 2;
	m_scoreboardPtr = new Scoreboard(m_snakesPtr, 1, &m_clock, m_playingLength);

	m_fruitPtr = new Fruit(sf::Vector2i(32, 160));
}

void Game::reset() {

}

void Game::update() {
	if (!m_snakesPtr) return;

	(*m_snakesPtr[0]).move(*m_fruitPtr);

	for (int i = 1; i < m_nPlayers; i++) {
		(*m_snakesPtr[i]).moveAutomatically(*m_fruitPtr);
	}
	m_scoreboardPtr->update();
}

float Game::getSpeed() const {
	return m_speed;
}

Fruit & Game::getFruit() {
	return *m_fruitPtr;
}

void Game::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(*m_fruitPtr);
	if (!m_snakesPtr) return;

	for (int i = 0; i < m_nPlayers; i++) {
		target.draw(*m_snakesPtr[i]);
	}
	target.draw(*m_scoreboardPtr);
}
