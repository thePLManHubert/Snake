#include "Server.h"
#include <iostream>


void Server::start() {
	if (m_listeningSocket.bind(m_listeningPort) != sf::Socket::Done) {
		return;
	}
	m_listeningSocket.setBlocking(false);
	m_listening = true;
	m_listenThread = std::thread(&Server::listen, this);
}

void Server::shutdown() {
	if (m_listening) {
		m_listening = false;
		m_listenThread.join();
	}
	if (m_playing) {
		m_playing = false;
		m_gameThread.join();
	}
}

void Server::listen() {
	while (m_listening) {
		if (m_listeningSocket.receive(m_receivedData, MAX_DATA_SIZE, m_receivedSize, m_senderAddress, m_senderPort) == sf::Socket::Done) {
			process(m_receivedData);
		}
	}
}

void Server::play() {
	while (m_playing) {	
		if (m_gameSocket.receive(m_receivedGameData, MAX_DATA_SIZE, m_receivedGameSize, m_playerAddress, m_playerPort) == sf::Socket::Done) {
			process(m_receivedGameData);
		}
	}
}

void Server::process(void* packet) {
	using namespace Datagram;

	TypeOfPacket type = checkType(packet);

	switch (type) {
	case RequestPacket:
		process((Request*)packet);
		break;

	case DataPacket:
		process((Data*)packet);
		break;

	case QueuePacket:
		process((Queue*)packet);
		break;

	case DCPacket:
		process((DC*)packet);
		break;
	}
}

void Server::process(Datagram::Request * request) {
	using namespace Datagram;

#ifdef DEBUG
	std::cout << "Otrzymano RequestPacket z " << m_senderAddress << ":" << m_senderPort << std::endl;
#endif

	if (m_gamePort == m_listeningPort) {
		m_gamePort = 5050;
		m_gameSocket.bind(m_gamePort);
		m_gameSocket.setBlocking(false);
		m_playing = true;
		m_gameThread = std::thread(&Server::play, this);
	}
	m_nPlayers++;
	for (int i = 0; i < 4; i++) {
		if (!m_players[i]) {
			m_players[i] = new Player(m_senderAddress, m_senderPort, m_nPlayers);
#ifdef DEBUG
			std::cout << "Przydzielono id: " << m_nPlayers << std::endl;
			std::cout << "Dodano nowego gracza." << std::endl;
#endif
			m_playerAdded = true;
			break;
		}
	}
	
	if (m_playerAdded && request->playerID == -1) {
		m_playerAdded = false;
		Reply reply;
		reply.playerID = m_nPlayers;
		reply.newServerPort = m_gamePort;
		send(&reply, sizeof(Reply), m_senderAddress, m_senderPort);
	}
	else {
		send(request, sizeof(Reply), m_senderAddress, m_senderPort);
#ifdef DEBUG
		std::cout << "Brak miejsca na serwerze." << std::endl;
#endif
	}
}

void Server::process(Datagram::Queue * queue) {

}

void Server::process(Datagram::Data * data) {
	using namespace Datagram;
	for (int i = 0; i < 4; i++) {
		if (m_players[i] && (m_players[i]->id != data->playerID))
			broadcast(data, sizeof(Data), m_players[i]->ip, m_players[i]->port);
	}
}

void Server::process(Datagram::DC * dc) {
	for (int i = 0; i < 4; i++) {
		if (m_players[i] && (m_players[i]->id == dc->playerID)) {
			m_players[i] = nullptr;
#ifdef DEBUG
			std::cout << "Rozlaczono gracza o id: " << dc->playerID << std::endl;
#endif
		}
	}
}

void Server::send(const void * data, size_t size, const sf::IpAddress address, unsigned short port) {
	m_listeningSocket.send(data, size, address, port);
}

void Server::broadcast(const void * data, size_t size, const sf::IpAddress address, unsigned short port) {
	m_gameSocket.send(data, size, address, port);
}
