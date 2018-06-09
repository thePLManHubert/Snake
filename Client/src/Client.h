#pragma once
#include "packets.h"
#include <SFML/Network.hpp>
#include <thread>


class Game;

class Client {
public:
	enum Stage { Disconnected, Connecting, Connected };

private:
	Stage m_currentStage = Disconnected;
	sf::UdpSocket m_socket;
	sf::IpAddress m_serverIP;
	unsigned short m_port;
	unsigned short m_serverPort;
	int m_id;
	char m_data[100];
	std::size_t m_dataSize;
	std::thread m_thread;
	Game * game;

public:
	Client(Game * game);
	Client(Game * game, sf::IpAddress serverIP, int serverPort = 5000, int socketPort = sf::Socket::AnyPort);
	~Client();

public:
	// zwraca informacjê o tym czy po³¹czenie siê powiod³o
	bool start();
	void disconnect();

private:
	void send(const void * data, size_t size, const sf::IpAddress address, unsigned short port);
	void listen();
	void process(void * packet);

private:
	void process(Datagram::Data & data);
	void * preparePacket();

public: // getters
	bool isConnected();
	Stage getCurrentStage();
};