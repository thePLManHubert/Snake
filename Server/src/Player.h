#pragma once
#include "packets.h"

using namespace Datagram;


struct Player {
	static const char MAX_SEGMENT_COUNT = 20;

	sf::IpAddress ip;
	unsigned short port;
	int id;
	Direction direction;
	unsigned short score;
	unsigned short * position[MAX_SEGMENT_COUNT] = {nullptr};
	

	Player(sf::IpAddress ip, unsigned short port, int id);
	~Player();

	void move();

};

