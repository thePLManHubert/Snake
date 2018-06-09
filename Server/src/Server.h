#pragma once
#include "packets.h"
#include <SFML/Network.hpp>
#include <thread>

class Server {
	static const int MAX_DATA_SIZE = 100;
	static const int MAX_PLAYER_NUMBER = 10;

private:
	unsigned short m_port;
	std::thread m_listenThread;
	volatile bool m_listening;
	sf::UdpSocket m_socket;
	size_t m_receivedSize;
	char m_receivedData[MAX_DATA_SIZE];
	sf::IpAddress m_senderAddress;
	unsigned short m_senderPort;
	int m_nPlayers;


public:
	Server(unsigned short port) : 
		m_port(port), 
		m_listening(false),
		m_nPlayers(0)
	{
	}

	void start();
	void shutdown();
	void send(const void* data, size_t size, const sf::IpAddress address, unsigned short port);
	
private:
	void listen();
	void process(void* packet);
	void process(Datagram::Request & request);
	void process(Datagram::Data & data);

};
