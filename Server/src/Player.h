#pragma once
#include <SFML/Network.hpp>

struct Player {
	sf::IpAddress ip;
	unsigned short port;
	int id;

	Player(sf::IpAddress ip, unsigned short port, int id)
		:ip(ip),
		port(port),
		id(id)
	{
	}
};

