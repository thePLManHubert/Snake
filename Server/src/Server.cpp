#include "Server.h"
#include <iostream>


void Server::start() {
	if (m_socket.bind(m_port) != sf::Socket::Done) {
		return;
	}
	m_socket.setBlocking(false);
	m_listening = true;
	m_listenThread = std::thread(&Server::listen, this);
}

void Server::shutdown() {
	if (m_listening) {
		m_listening = false;
		m_listenThread.join();
	}
}

void Server::listen() {
	while (m_listening) {
		if (m_socket.receive(m_receivedData, MAX_DATA_SIZE, m_receivedSize, m_senderAddress, m_senderPort) == sf::Socket::Done) {
			process(m_receivedData);
		}
	}
}

void Server::process(void* packet) {
	using namespace Datagram;

	TypeOfPacket type = *(TypeOfPacket*)packet;

	switch (type) {
	case RequestPacket:
		process(*(Request*)packet);
		break;

	case DataPacket:
		process(*(Data*)packet);
		break;
	}
}

void Server::process(Datagram::Request & request) {
	using namespace Datagram;
	m_nPlayers++;
#ifdef DEBUG
	std::cout << "Otrzymano RequestPacket z " << m_senderAddress << ":" << m_senderPort << std::endl
			  << "Przydzielono id: " << m_nPlayers << std::endl;
#endif	
	if (request.playerID == -1) {
		Reply reply;
		reply.playerID = m_nPlayers;
		send(&reply, sizeof(Reply), m_senderAddress, m_senderPort);
	}
}

void Server::process(Datagram::Data & data) {

}

void Server::send(const void * data, size_t size, const sf::IpAddress address, unsigned short port) {
	m_socket.send(data, size, address, port);
}
