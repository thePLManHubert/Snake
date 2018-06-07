#pragma once
#include "packets.h"
#include <SFML/Network.hpp>
#include <thread>

class Server {
	static const int MAX_DATA_SIZE = 100;

private:
	int port;
	std::thread listenThread;
	volatile bool listening;
	sf::UdpSocket socket;
	size_t receivedSize;
	char receivedData[MAX_DATA_SIZE];
	sf::IpAddress senderAddress;
	unsigned short senderPort;
	Datagram::State state;


public:
	Server(int port) : port(port), listening(false) {}

	void start();
	void shutdown();
	void send(const void* data, size_t size, const sf::IpAddress address, unsigned short port);
	
private:
	void listen();
	void process(void* packet);
	void process(Datagram::Request & request);
	void process(Datagram::Data & data);
	void broadcast(Datagram::State & state);

};