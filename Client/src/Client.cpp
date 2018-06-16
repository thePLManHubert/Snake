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
//		Nawi�zuje po��czenie z serwerem
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

	// utworzenie i wys�anie do serwera pakietu inicjalizacyjnego
	Request packet;
	m_socket.send(&packet, sizeof(Request), m_serverIP, 5000);

	//ustawienie czasu na odpowied�
	sf::Clock requestClock;

	// sprawszenie czy odpowied� nadesz�a + odbi�r nowego portu serwera i przypisanego id
	while (requestClock.getElapsedTime().asSeconds() < 5) {
		if (m_socket.receive(m_data, 100, m_dataSize, m_serverIP, m_serverPort) == sf::Socket::Done) {
			// sprawdzenie czy na serwerze pozosta�o jakie� wolne miejsce
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

	// sprawdzenie czy po��czenie si� powiod�o
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
//		Wysy�a do serwera dane na odebrany z serwera port komunikacyjny
/*------------------------------------------------------------------------------------*/
void Client::send(const void * data, size_t size) {
	m_socket.send(data, size, m_serverIP, m_serverPort);
}

/*------------------------------------------------------------------------------------*/
//		Nas�uchuje czy serwer wys�a� jakie� dane.
//		Nast�pnie przetwarza odebrane dane.
/*------------------------------------------------------------------------------------*/
void Client::listen() {
	while (isConnected()){
		if(m_socket.receive(m_data, 100, m_dataSize, m_processIP, m_processPort) == sf::Socket::Done)
			process(m_data);
	}
}

/*------------------------------------------------------------------------------------*/
//		Funkcja zleca zadania podrz�dnym funkcjom. W zale�no�ci od typu odebranego
//		pakietu, dane zawarte w pakiecie s� przetwarzane przez dedykowan� funkcj�.
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
//		Je�eli klient jest po��czony z serwerem, funkcja zwraca true.
//		W przeciwnym razie false.
/*------------------------------------------------------------------------------------*/
bool Client::isConnected() const {
	return m_currentStage == Connected;
}

/*------------------------------------------------------------------------------------*/
//		Podaje informacj� w jakim stanie znajduje si� klient.
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


