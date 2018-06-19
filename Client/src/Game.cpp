#include "Game.h"
#include <iostream>


extern sf::Mutex mutex;

Game::Game(int maxnPlayers, sf::IpAddress ip, int gameTime, bool collision)
	: m_stage(InMenu),
	m_playingLength(gameTime),
	MAX_PLAYERS(maxnPlayers),
	m_nPlayers(0),
	m_speed(10),
	m_gameServerIP(ip),
	m_collisionEnabled(collision),
	m_snakesPtr(nullptr),
	m_scoreboardPtr(nullptr),
	m_fruitPtr(nullptr),
	m_textureUpdated(false),
	m_updateGame(false),
	m_clientPtr(nullptr)
{
	srand(time(NULL));
	loadMenu();
}

Game::~Game() {
	closeGame();
	if (m_clientPtr) 
		deleteClient();
}

/*------------------------------------------------------------------------------------*/
//		W zale¿noœci od tego w jakim stanie znajduje siê gra, poddaje kontrolê
//		nad gr¹ pod dany zestaw przycisków.
/*------------------------------------------------------------------------------------*/
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
	case End:
		controlEnd(event, window);
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
		if (m_renderClock.getElapsedTime().asSeconds() > 1 / m_speed) {
			updateSingleplayer();
			m_renderClock.restart();
		}
		break;
	case InMultiplayer:
		updateMultiplayer();
		break;
	case End:
		updateEnd();
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
	case End:
		drawEnd(target, states);
		break;
	}
}

/*------------------------------------------------------------------------------------*/
//		Dokonuje jednorazowego za³adowania tekstury menu.
/*------------------------------------------------------------------------------------*/
void Game::loadMenu() {
	if (!m_texture.loadFromFile("resources/SnakeMenu.png")) return;
}

/*------------------------------------------------------------------------------------*/
//		Dokonuje jednorazowego za³adowania tekstury menu.
/*------------------------------------------------------------------------------------*/
void Game::loadStats(Datagram::EndGame * endGame) {
	if (endGame->playerStatus == Datagram::Status::Win) {
		if (!m_texture.loadFromFile("resources/Win.png")) return;
	}
	else if (endGame->playerStatus == Datagram::Status::Loss) {
		if (!m_texture.loadFromFile("resources/Loss.png")) return;
	}
	else {
		if (!m_texture.loadFromFile("resources/Draw.png")) return;
	}
}

/*------------------------------------------------------------------------------------*/
//		Dokonuje jednorazowego za³adowania tekstury kolejki.
/*------------------------------------------------------------------------------------*/
void Game::loadQueue() {
	if (!m_texture.loadFromFile("resources/SnakeQueue.png")) return;
}

/*------------------------------------------------------------------------------------*/
//		Dokonuje jednorazowego za³adowania danych do gry singleplayer.
/*------------------------------------------------------------------------------------*/
void Game::startSingleplayer() {
	m_snakesPtr = new Snake*[MAX_PLAYERS];

	m_snakesPtr[0] = new Snake({ 4 * FIELD_WIDTH, 5 * FIELD_HEIGHT }, sf::Color(15, 150, 0, 200), 20, false);
	m_snakesPtr[1] = new Snake({ MAP_WIDTH - 5 * FIELD_WIDTH, 5 * FIELD_HEIGHT }, sf::Color(240, 0, 0, 200), 20, false);

	m_nPlayers = 2;
	m_scoreboardPtr = new Scoreboard(m_snakesPtr, m_nPlayers, &m_clock, m_playingLength);

	m_fruitPtr = new Fruit(m_fruitPtr->preparePosition(*this));
	m_clock.restart();
}

/*------------------------------------------------------------------------------------*/
//		Dokonuje jednorazowego za³adowania danych do gry multiplayer.
/*------------------------------------------------------------------------------------*/
void Game::startMultiplayer(Datagram::Start * start) {
	m_snakesPtr = new Snake*[MAX_PLAYERS];

	if (start->id == start->id1) {
		m_snakesPtr[0] = new Snake(start->position1, start->id1, sf::Color(15, 150, 0, 200), 20, false);
		m_snakesPtr[1] = new Snake(start->position2, start->id2, sf::Color(240, 0, 0, 200), 20, false);
	}
	else {
		m_snakesPtr[0] = new Snake(start->position2, start->id2, sf::Color(15, 150, 0, 200), 20, false);
		m_snakesPtr[1] = new Snake(start->position1, start->id1, sf::Color(240, 0, 0, 200), 20, false);
	}

	m_nPlayers = 2;
	m_scoreboardPtr = new Scoreboard(m_snakesPtr, m_nPlayers, &m_clock, m_playingLength);

	m_fruitPtr = new Fruit(start->fruit);
	m_clock.restart();
}

/*------------------------------------------------------------------------------------*/
//		Przyjmuje dane z serwera i stosuje je do gry.
/*------------------------------------------------------------------------------------*/
void Game::approveChanges(Datagram::Data * data) {
	m_fruitPtr->setPosition(data->fruit);
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (m_snakesPtr[i]->m_id == data->playerID) {
			m_snakesPtr[i]->m_fruits = data->score;
			m_snakesPtr[i]->move(data);
		}
	}
}

/*------------------------------------------------------------------------------------*/
//		Resetuje grê singleplayer.
/*------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------*/
//		Resetuje dane o graczu w grze singleplayer.
/*------------------------------------------------------------------------------------*/
void Game::resetPlayerStatus() {
	m_snakesPtr[0]->m_body.deleteAllSegments();
	m_snakesPtr[0]->m_fruits = 0;
	m_snakesPtr[0]->m_direction = Snake::Direction::STOP;
}

/*------------------------------------------------------------------------------------*/
//		Resetuje dane o graczu w grze multiplayer.
/*------------------------------------------------------------------------------------*/
void Game::resetPlayerStatus(int id) {
	mutex.lock();
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (m_snakesPtr[i]->m_id == id){
			m_snakesPtr[i]->m_body.deleteAllSegments();
			m_snakesPtr[i]->m_fruits = 0;
			m_snakesPtr[i]->m_direction = Snake::Direction::STOP;
		}
	}
	mutex.unlock();
}

/*------------------------------------------------------------------------------------*/
//		Sprawdza czy nast¹pi³a kolizja dwóch wê¿ów.
/*------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------*/
//		Przygotowuje pozycjê owocu tak aby nie nak³ada³ siê on na cia³o wê¿a.
/*------------------------------------------------------------------------------------*/
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
