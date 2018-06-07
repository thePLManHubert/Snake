#include "Server.h"


void Server::start() {
	if (socket.bind(port) != sf::Socket::Done) {
		return;
	}
	socket.setBlocking(false);
	listening = true;
	listenThread = std::thread(&Server::listen, this);
}

void Server::shutdown() {
	if (listening) {
		listening = false;
		listenThread.join();
	}
}

void Server::listen() {
	while (listening) {
		if (socket.receive(receivedData, MAX_DATA_SIZE, receivedSize, senderAddress, senderPort) == sf::Socket::Done) {
			process(receivedData);
			broadcast(state);
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
	
}

void Server::process(Datagram::Data & data) {

}

void Server::broadcast(Datagram::State & state) {

}


void Server::send(const void * data, size_t size, const sf::IpAddress address, unsigned short port) {
	socket.send(data, size, address, port);
}
