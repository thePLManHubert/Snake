#include "Client.h"
#include <iostream>


Client::Client(Game * game)
	: m_serverIP("192.168.1.108"),
	m_port(sf::Socket::AnyPort),
	m_serverPort(5000),
	m_id(-1),
	m_dataSize(0),
	m_game(game)
{
	start();
}

Client::Client(Game * game, sf::IpAddress serverIP, unsigned short serverPort, unsigned short socketPort) 
	: m_serverIP(serverIP),
	m_serverPort(serverPort),
	m_port(socketPort),
	m_id(-1),
	m_dataSize(0),
	m_game(game)
{
	start();
}

/*------------------------------------------------------------------------------------*/
//		Nawiπzuje po≥πczenie z serwerem
/*------------------------------------------------------------------------------------*/
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
			// sprawdzenie czy na serwerze pozosta≥o jakieú wolne miejsce
			if (checkType(m_data) != ReplyPacket) {
#ifdef DEBUG
				std::cout << "Brak miejsca na serwerze." << std::endl;
#endif
				m_currentStage = Disconnected;
				return isConnected();
			}
			m_id = ((Reply*)m_data)->playerID;
			m_serverPort = ((Reply*)m_data)->newServerPort;
#ifdef DEBUG
			std::cout << ((Reply*)m_data)->testInfo << " id: " << ((Reply*)m_data)->playerID << std::endl;
#endif
			break;
		}
	}

	// sprawdzenie czy po≥πczenie siÍ powiod≥o
	if (m_id != -1) {
		m_currentStage = Connected;
		m_thread = std::thread(&Client::listen, this);	
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

/*------------------------------------------------------------------------------------*/
//		Wysy≥a do serwera dane na odebrany z serwera port komunikacyjny
/*------------------------------------------------------------------------------------*/
void Client::send(const void * data, size_t size) {
	m_socket.send(data, size, m_serverIP, m_serverPort);
}

/*------------------------------------------------------------------------------------*/
//		Nas≥uchuje czy serwer wys≥a≥ jakieú dane.
//		NastÍpnie przetwarza odebrane dane.
/*------------------------------------------------------------------------------------*/
void Client::listen() {
	while (isConnected()){
		if(m_socket.receive(m_data, 100, m_dataSize, m_processIP, m_processPort) == sf::Socket::Done)
			process(m_data);
	}
}

/*------------------------------------------------------------------------------------*/
//		Funkcja zleca zadania podrzÍdnym funkcjom. W zaleønoúci od typu odebranego
//		pakietu, dane zawarte w pakiecie sπ przetwarzane przez dedykowanπ funkcjÍ.
/*------------------------------------------------------------------------------------*/
void Client::process(void * packet) {
	using namespace Datagram;

	TypeOfPacket type = *(TypeOfPacket*)packet;
	switch (type) {
	case Datagram::DataPacket:
		process((Data*)packet);
		break;

	case Datagram::StartPacket:
		process((Start*)packet);
		break;

	case Datagram::QuitPacket:
		process((Quit*)packet);
		break;

	case Datagram::SyncPacket:
		process((Sync*)packet);
		break;
	}
}

/*------------------------------------------------------------------------------------*/
//		Jeøeli klient jest po≥πczony z serwerem, funkcja zwraca true.
//		W przeciwnym razie false.
/*------------------------------------------------------------------------------------*/
bool Client::isConnected() const {
	return m_currentStage == Connected;
}

/*------------------------------------------------------------------------------------*/
//		Podaje informacjÍ w jakim stanie znajduje siÍ klient.
/*------------------------------------------------------------------------------------*/
Client::Stage Client::getCurrentStage() const {
	return m_currentStage;
}

/*------------------------------------------------------------------------------------*/
//		Zwraca id klienta.
/*------------------------------------------------------------------------------------*/
int Client::getID() const {
	return m_id;
}


