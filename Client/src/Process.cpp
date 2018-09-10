#include "Game.h"
#include "Client.h"
#include <iostream>


/*------------------------------------------------------------------------------------*/
//		Funkcja s³u¿¹ca do przetwarzania danych.
/*------------------------------------------------------------------------------------*/
void Client::process(Datagram::Data * data) {	
#ifdef DEBUG
	if (data->playerID == this->getID()) {
		unsigned short limit;
		std::cout << "---------------------------------------------"
			<< std::endl << "id: " << data->playerID << " " << data->directionToString() << std::endl;
		std::cout << "fruit: " << data->fruit << std::endl;

		limit = (data->score < 20) ? data->score : 19;
		for (int i = 0; i < limit; i++) {
			std::cout << data->position[i] << " | ";
		}
		std::cout << data->position[limit];
		std::cout << std::endl;
	}
#endif

	m_game->approveChanges(data);
}

/*------------------------------------------------------------------------------------*/
//		Funkcja s³u¿¹ca do reagowania na sygna³ startu.
/*------------------------------------------------------------------------------------*/
void Client::process(Datagram::Start * start) {
	std::cout << "Start." << std::endl;
	m_serverPort = start->gamePort;
	m_game->startMultiplayer(start);
	m_game->setStage(Game::InMultiplayer);
}

/*------------------------------------------------------------------------------------*/
//		Funkcja s³u¿¹ca do reagowania na sygna³ roz³¹czenia.
/*------------------------------------------------------------------------------------*/
void Client::process(Datagram::Quit * quit) {
	m_game->setStage(Game::InQueue);
}

/*------------------------------------------------------------------------------------*/
//		Funkcja s³u¿¹ca do synchronizacji klientów.
/*------------------------------------------------------------------------------------*/
void Client::process(Datagram::Sync * sync) {
	m_game->m_updateGame = true;
	m_game->m_syncPacket = *sync;
}

void Client::process(Datagram::Reset * reset){
	m_game->resetPlayerStatus(reset->playerID);
}

void Client::process(Datagram::EndGame * endGame) {
	m_game->loadStats(endGame);
	m_currentStage = Disconnected;
}

/*------------------------------------------------------------------------------------*/
//		Ustawia grê w odpowiednie stan.
/*------------------------------------------------------------------------------------*/
void Game::setStage(Stage stage) {
	m_stage = stage;

	switch (m_stage) {
	case InMenu:
		loadMenu();
		closeGame();
		break;
	case InQueue:
		loadQueue();
		break;
	case InSingleplayer:
		startSingleplayer();
		break;
	case InMultiplayer:
		break;
	case End:
		closeGame();
		break;
	}
}

Client::~Client() {
	disconnect();
}

/*------------------------------------------------------------------------------------*/
//		Od³¹cza klienta.
/*------------------------------------------------------------------------------------*/
void Client::disconnect() {
	if (isConnected()) {
		m_currentStage = Disconnected;
		m_thread.join();
		Datagram::DC packet;
		packet.playerID = m_id;
		m_socket.send(&packet, sizeof(Datagram::DC), m_serverIP, 5000);
#ifdef DEBUG
		std::cout << "Rozlaczono z serwerem." << std::endl;
#endif
	}
}

/*------------------------------------------------------------------------------------*/
//		Usuwa klienta gry.
/*------------------------------------------------------------------------------------*/
void Game::deleteClient() {
	delete m_clientPtr;
	m_clientPtr = nullptr;
}

unsigned short * Game::mapSnake() {
	if (m_snakesPtr)
		if (m_snakesPtr[0])
			return m_snakesPtr[0]->map();
	return nullptr;
}

/*------------------------------------------------------------------------------------*/
//		Wy³¹cza grê.
/*------------------------------------------------------------------------------------*/
void Game::closeGame() {
	if (m_clientPtr) {
		delete m_clientPtr;
		m_clientPtr = nullptr;
	}
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