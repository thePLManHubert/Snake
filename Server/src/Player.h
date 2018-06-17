#pragma once
#include "packets.h"
#include "Globals.h"

using namespace Datagram;


struct Player {
	static const char MAX_SEGMENT_COUNT = 20;

	sf::IpAddress ip;
	unsigned short port;
	int id;
	Direction direction;
	unsigned short score;
	unsigned short * position[MAX_SEGMENT_COUNT] = {nullptr};
	bool grow;
	bool canMove;
	

	Player(sf::IpAddress ip, unsigned short port, int id);
	~Player();

	bool move(unsigned short fruitPos); // true je¿eli owoc zosta³ zjedzony
	void eat(unsigned short fruitPos);
	void follow(unsigned short newPos);

};

