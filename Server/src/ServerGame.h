#pragma once
#include "Player.h"


struct ServerGame {
public:
	static const int MAX_PLAYER_COUNT = 2;

public:
	ServerGame();
	~ServerGame();

public:
	Player * m_players[MAX_PLAYER_COUNT] = { nullptr };
	int m_nPlayers;
	unsigned short m_fruitPos;
	bool m_fruitHit;

	bool isFull();

};