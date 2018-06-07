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
	textureUpdated(false)
{
	srand(time(NULL));
	loadMenu();
}

Game::~Game() {
	closeGame();
}

void Game::control(sf::Event & event, sf::RenderWindow& window) {
	switch (m_stage) {
	case InMenu:
		controlMenu(event, window);
		break;
	case InQueue:
		controlQueue(event, window);
		break;
	case InSingleplayer:
		controlSingleplayer(event, window);
		break;
	case InMultiplayer:
		controlMultiplayer(event, window);
		break;
	}
}

void Game::update() {
	switch (m_stage) {
	case InMenu:
		updateMenu();
		break;
	case InQueue:
		updateQueue();
		break;
	case InSingleplayer:
		updateSingleplayer();
		break;
	case InMultiplayer:
		updateMultiplayer();
		break;
	}
}

void Game::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	switch (m_stage) {
	case InMenu:
		drawMenu(target, states);
		break;
	case InQueue:
		drawQueue(target, states);
		break;
	case InSingleplayer:
	case InMultiplayer:
		drawGame(target, states);
		break;
	}
}

void Game::loadMenu() {
	if (!m_texture.loadFromFile("resources/SnakeMenu.png")) return;
}

void Game::startSingleplayer() {
	m_snakesPtr = new Snake*[MAX_PLAYERS];

	m_snakesPtr[0] = new Snake({ 4 * FIELD_WIDTH, 5 * FIELD_HEIGHT }, sf::Color(15, 150, 0, 200), 20, false);
	m_snakesPtr[1] = new Snake({ MAP_WIDTH - 5 * FIELD_WIDTH, 5 * FIELD_HEIGHT }, sf::Color(240, 0, 0, 200), 20, false);

	m_nPlayers = 2;
	m_scoreboardPtr = new Scoreboard(m_snakesPtr, m_nPlayers, &m_clock, m_playingLength);

	m_fruitPtr = new Fruit(m_fruitPtr->preparePosition(*this));
	m_clock.restart();
}

void Game::resetSingleplayer() {
	if (m_snakesPtr)
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

void Game::closeGame() {
	if (m_snakesPtr) {
		Snake ** player = m_snakesPtr;
		for (int i = 0; i < m_nPlayers; i++)
			delete player[i];
		delete[] m_snakesPtr;
		m_snakesPtr = nullptr;
	}
	if (m_fruitPtr) {
		delete m_fruitPtr;
		m_fruitPtr = nullptr;
	}
	if (m_scoreboardPtr) {
		delete m_scoreboardPtr;
		m_scoreboardPtr = nullptr;
	}
}

void Game::detectCollision() {
	if (!m_snakesPtr || m_clock.getElapsedTime().asSeconds() < 1) return;

	for (int j = 0; j < m_nPlayers; j++) {
		for (int i = 0; i < m_nPlayers; i++) {
			if (j == i) continue;
			if (m_snakesPtr[i]->comparePosition(&m_snakesPtr[j]->m_head))
				m_snakesPtr[j]->setDirection(Snake::Direction::FREEZE);
		}
	}
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

void Game::setStage(Stage stage) {
	m_stage = stage;

	switch (m_stage) {
	case InMenu:
		loadMenu();
		closeGame();
		break;
	case InQueue:
		break;
	case InSingleplayer:
		startSingleplayer();
		break;
	case InMultiplayer:
		break;
	}
}

// gettery
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