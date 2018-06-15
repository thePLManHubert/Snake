#include "Game.h"
#include "Client.h"
#include <iostream>


/*------------------------------------------------------------------------------------*/
//		Funkcja s�u��ca do przetwarzania danych.
/*------------------------------------------------------------------------------------*/
void Client::process(Datagram::Data * data) {
	std::cout << "Przetwarzanie..." << std::endl;
	std::cout << data->data << std::endl;
}

/*------------------------------------------------------------------------------------*/
//		Funkcja s�u��ca do reagowania na sygna� startu.
/*------------------------------------------------------------------------------------*/
void Client::process(Datagram::Start * data) {
	std::cout << "Start." << std::endl;
	m_game->startMultiplayer();
	m_game->setStage(Game::InMultiplayer);
}

/*------------------------------------------------------------------------------------*/
//		Funkcja s�u��ca do reagowania na sygna� roz��czenia.
/*------------------------------------------------------------------------------------*/
void Client::process(Datagram::Quit * data) {
	m_game->setStage(Game::InQueue);
}



/*------------------------------------------------------------------------------------*/
//		Ustawia gr� w odpowiednie stan.
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
	}
}

/*------------------------------------------------------------------------------------*/
//		Usuwa klienta gry.
/*------------------------------------------------------------------------------------*/
void Game::deleteClient() {
	delete m_clientPtr;
	m_clientPtr = nullptr;
}

/*------------------------------------------------------------------------------------*/
//		Wy��cza gr�.
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