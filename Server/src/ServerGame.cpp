#include "ServerGame.h"


ServerGame::ServerGame() 
	: m_nPlayers(0),
	m_fruitHit(false)
{
}

ServerGame::~ServerGame()
{
	for (int i = 0; i < MAX_PLAYER_COUNT; i++) {
		if (m_players[i]) {
			delete m_players[i];
			m_players[i] = nullptr;
		}
	}
}

bool ServerGame::isFull() {
	for (int i = 0; i < MAX_PLAYER_COUNT; i++) {
		if (m_players[i] == nullptr)
			return false;
	}
	return true;
}
