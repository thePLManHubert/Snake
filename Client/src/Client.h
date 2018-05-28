#pragma once
#include <SFML/Network.hpp>

class Client {
public:
	enum Stage { Disconnected, Connecting, InQueue, InGame };

private:
	Stage m_currentStage = Disconnected;
	sf::UdpSocket m_socket;

public:
	Client();


public: // getters
	Stage getCurrentStage();
	sf::UdpSocket& getSocket();
};