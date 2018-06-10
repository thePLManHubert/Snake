#pragma once
#include "packets.h"
#include <SFML/Network.hpp>
#include <thread>


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

class Server {
	static const int MAX_DATA_SIZE = 100;
	static const int MAX_PLAYER_NUMBER = 10;

private:
	unsigned short m_listeningPort;
	unsigned short m_gamePort;
	std::thread m_listenThread;
	std::thread m_gameThread;
	volatile bool m_playing;
	volatile bool m_listening;
	sf::UdpSocket m_listeningSocket;
	sf::UdpSocket m_gameSocket;
	size_t m_receivedSize;
	size_t m_receivedGameSize;
	char m_receivedData[MAX_DATA_SIZE];
	char m_receivedGameData[MAX_DATA_SIZE];
	sf::IpAddress m_senderAddress;
	sf::IpAddress m_playerAddress;
	unsigned short m_playerPort;
	unsigned short m_senderPort;
	int m_nPlayers;
	bool m_playerAdded;
	Player * m_players[4] = {nullptr};


public:
	Server(unsigned short port) : 
		m_listeningPort(port), 
		m_gamePort(port),
		m_listening(false),
		m_playing(false),
		m_nPlayers(0),
		m_playerAdded(false)
	{
	}

	void start();
	void shutdown();
	void send(const void* data, size_t size, const sf::IpAddress address, unsigned short port);
	void broadcast(const void * data, size_t size, const sf::IpAddress address, unsigned short port);
	
private:
	void listen();
	void play();
	void process(void* packet);
	void process(Datagram::Request * request);
	void process(Datagram::Queue * queue);
	void process(Datagram::Data * data);
	void process(Datagram::DC * dc);

};
