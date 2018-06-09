#include "Client.h"
#include <iostream>

Client::Client(Game * game)
	: m_serverIP("192.168.1.100"),
	m_port(sf::Socket::AnyPort),
	m_id(-1),
	m_dataSize(0),
	m_serverPort(5000)
{
	start();
}

Client::Client(Game * game, sf::IpAddress serverIP, int serverPort, int socketPort) 
	: m_serverIP(serverIP),
	m_port(socketPort),
	m_id(-1),
	m_dataSize(0),
	m_serverPort(m_serverPort)
{
	start();
}

Client::~Client() {
	disconnect();
}

bool Client::start() {
	using namespace Datagram;
	m_currentStage = Connecting;
	m_socket.setBlocking(false);

	// przypisanie do scoketu wolnego portu
	if (m_socket.bind(m_port) != sf::Socket::Done) {
		m_currentStage = Disconnected;
		return isConnected();
	}

	// utworzenie i wys≥anie do serwera pakietu inicjalizacyjnego
	Request packet;
	m_socket.send(&packet, sizeof(Request), m_serverIP, 5000);

	//ustawienie czasu na odpowiedü
	sf::Clock requestClock;

	// sprawszenie czy odpowiedü nadesz≥a + odbiÛr nowego portu serwera i przypisanego id
	while (requestClock.getElapsedTime().asSeconds() < 5) {
		if (m_socket.receive(m_data, 100, m_dataSize, m_serverIP, m_serverPort) == sf::Socket::Done) {
			m_id = ((Reply*)m_data)->playerID;
#ifdef DEBUG
			std::cout << ((Reply*)m_data)->testInfo << " id: " << ((Reply*)m_data)->playerID << std::endl;
#endif
			break;
		}
	}

	// sprawdzenie czy po≥πczenie siÍ powiod≥o
	if (m_id != -1) {
		m_thread = std::thread(&Client::listen, this);
		m_currentStage = Connected;
#ifdef DEBUG
		std::cout << "Polaczono." << std::endl;
#endif
	}
	else {
		m_currentStage = Disconnected;
#ifdef DEBUG
		std::cout << "Problem z polaczeniem." << std::endl;
#endif
	}

	return isConnected();
}

void Client::disconnect() {
	m_currentStage = Disconnected;
	m_thread.join();
}

void Client::send(const void * data, size_t size, const sf::IpAddress address, unsigned short port) {
	m_socket.send(data, size, address, port);
}

void Client::listen() {
	while (isConnected()){
		if(m_socket.receive(m_data, 100, m_dataSize, m_serverIP, m_serverPort) == sf::Socket::Done)
			process(m_data);
	}
}

void Client::process(void * packet) {
	using namespace Datagram;

	TypeOfPacket type = *(TypeOfPacket*)packet;
	switch (type) {
	case Datagram::DataPacket:
		process(*(Data*)packet);
		break;
	}
}

void Client::process(Datagram::Data & data) {
	std::cout << "Przetwarzanie..." << std::endl;
}

void * Client::preparePacket()
{
	return nullptr;
}

bool Client::isConnected() {
	return m_currentStage == Connected;
}

Client::Stage Client::getCurrentStage() {
	return m_currentStage;
}


