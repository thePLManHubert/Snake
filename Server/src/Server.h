#pragma once
#include "packets.h"
#include "ServerGame.h"
#include <SFML/Network.hpp>
#include <thread>


class Server {
	static const int MAX_DATA_SIZE = 100;

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
	bool m_playerAdded;
	ServerGame m_game;


public:
	Server(unsigned short port, unsigned short gamePort) :
		m_listeningPort(port),
		m_gamePort(gamePort),
		m_listening(false),
		m_playing(false),
		m_playerAdded(false),
		m_game(ServerGame())
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
	void process(Datagram::Data * data);
	void process(Datagram::DC * dc);

};
