#include "Player.h"


Player::Player(sf::IpAddress ip, unsigned short port, int id)
	:ip(ip),
	port(port),
	id(id),
	direction(STOP),
	score(0)
{
}

Player::~Player() {
	for (int i = 0; i < MAX_SEGMENT_COUNT; i++) {
		if (position[i])
			delete position[i];
	}
}

void Player::move() {
	switch (direction) {
	case UP:
		break;
	case DOWN:
		break;
	case LEFT:
		break;
	case RIGHT:
		break;
	}
}
