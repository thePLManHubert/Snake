#include "Player.h"


Player::Player(sf::IpAddress ip, unsigned short port, int id)
	:ip(ip),
	port(port),
	id(id),
	direction(STOP),
	score(0),
	canMove(true)
{
}

Player::~Player() {
	for (int i = 0; i < MAX_SEGMENT_COUNT; i++) {
		if (position[i])
			delete position[i];
	}
}

void getCoordinates(unsigned short val, char & out_i, char & out_j);
unsigned short giveMutual(unsigned short i, unsigned short j);

bool Player::move(unsigned short fruitPos) {
	if(canMove){
		unsigned short temp = *position[0];
		char i, j;

		// przejœcie na zapis ze wspó³rzêdnymi
		getCoordinates(temp, i, j);

		switch (direction) {
		case UP:
			// sprawdzenie czy g³owa nie wychodzi za mapê
			(j == 0) ? j = MAP_Y - 1 : j--;
		case DOWN:
			j = (j + 1) % MAP_Y;
			break;
		case LEFT:
			(i == 0) ? i = MAP_X - 1 : i--;
			break;
		case RIGHT:
			i = (i + 1) % MAP_X;
			break;
		case STOP:
			return false;
		}
		// powrót do zunifikowanej postaci
		temp = giveMutual(i, j);
		// sprawdzenie czy w¹¿ trafi³ na owoc
		if (temp == fruitPos) {
			eat(fruitPos); return true;
		}
		else {
			follow(temp); return false;
		}
	}
	return false;
}

void Player::eat(unsigned short fruitPos) {
	score++;
	position[score] = new unsigned short;
	for (int i = score; i > 0; i--) {
		*position[i] = *position[i - 1];
	}
	*position[0] = fruitPos;
}

void Player::follow(unsigned short newPos) {
	for (int i = score; i > 0; i--) {
		*position[i] = *position[i - 1];
	}
	*position[0] = newPos;
}

void getCoordinates(unsigned short val, char & out_i, char & out_j) {
	out_i = val % MAP_X;
	out_j = val / MAP_X;
}

unsigned short giveMutual(unsigned short i, unsigned short j) {
	return j * MAP_X + i;
}