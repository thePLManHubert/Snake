#include "Server.h"
#include "Globals.h"
#include <iostream>


/*------------------------------------------------------------------------------------*/
//		Uruchamia nowy serwer pod wskazanym portem i w��cza nas�uchiwanie.
/*------------------------------------------------------------------------------------*/
void Server::start() {
	if (m_listeningSocket.bind(m_listeningPort) != sf::Socket::Done) {
		return;
	}
	m_listeningSocket.setBlocking(false);
	m_listening = true;
	m_listenThread = std::thread(&Server::listen, this);
}

/*------------------------------------------------------------------------------------*/
//		Wy��cza serwer.
/*------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------*/
//		Nas�uchuje zg�osze� od klint�w, kt�rzy maj� zamiar si� pod��czy�.
/*------------------------------------------------------------------------------------*/
void Server::listen() {
	while (m_listening) {
		if (m_listeningSocket.receive(m_receivedData, MAX_DATA_SIZE, m_receivedSize, m_senderAddress, m_senderPort) == sf::Socket::Done) {
			process(m_receivedData);
		}
	}
}

/*------------------------------------------------------------------------------------*/
//		Nas�uchuje zg�osze� od pod��czonych klient�w.
/*------------------------------------------------------------------------------------*/
void Server::play() {
	sf::Clock clock;
	Datagram::Sync sync;

	while (m_playing) {
		if (clock.getElapsedTime().asSeconds() > 0.1) {
			for (int i = 0; i < m_game.MAX_PLAYER_COUNT; i++)
				broadcast(&sync, sizeof(Datagram::Sync), m_game.m_players[i]->ip, m_game.m_players[i]->port);
			clock.restart();
		}
		if (m_gameSocket.receive(m_receivedGameData, MAX_DATA_SIZE, m_receivedGameSize, m_playerAddress, m_playerPort) == sf::Socket::Done) {
			process(m_receivedGameData);
		}
	}
}

/*------------------------------------------------------------------------------------*/
//		Funkcja zleca zadania podrz�dnym funkcjom. W zale�no�ci od typu odebranego
//		pakietu, dane zawarte w pakiecie s� przetwarzane przez dedykowan� funkcj�.
/*------------------------------------------------------------------------------------*/
void Server::process(void* packet) {
	using namespace Datagram;

	TypeOfPacket type = checkType(packet);

	switch (type) {
	case RequestPacket:
		process((Request*)packet);
		break;

	case DCPacket:
		process((DC*)packet);
		break;

	case DataPacket:
		process((Data*)packet);
		break;
	}
}

/*------------------------------------------------------------------------------------*/
//		Przetwarza pakiet typu Request.
/*------------------------------------------------------------------------------------*/
void Server::process(Datagram::Request * request) {
	using namespace Datagram;

#ifdef DEBUG
	std::cout << "Otrzymano RequestPacket z " << m_senderAddress << ":" << m_senderPort << std::endl;
#endif
	// zapewnienie unikalnego id w ramach pojedynczego uruchomienia serwera
	m_game.m_nPlayers++;
	// sprawdzenie czy na serwerze znajduje si� jeszcze jaki� wolny slot
	for (int i = 0; i < m_game.MAX_PLAYER_COUNT; i++) {
		if (!m_game.m_players[i]) {
			// przydziel graczowi miejsce
			m_game.m_players[i] = new Player(m_senderAddress, m_senderPort, m_game.m_nPlayers);
#ifdef DEBUG
			std::cout << "Przydzielono id: " << m_game.m_nPlayers << std::endl;
			std::cout << "Dodano nowego gracza." << std::endl;
#endif
			// daj zna�, �e gracz zosta� dodany
			m_playerAdded = true;
			break;
		}
	}

	// je�eli gracz zosta� dodany, wy�lij informacj� do klienta, �e tak si� sta�o
	if (m_playerAdded && request->playerID == -1) {
		m_playerAdded = false;
		Reply reply;
		reply.playerID = m_game.m_nPlayers;
		reply.newServerPort = m_gamePort;
		send(&reply, sizeof(Reply), m_senderAddress, m_senderPort);
	}
	else {
		send(request, sizeof(Reply), m_senderAddress, m_senderPort);
#ifdef DEBUG
		std::cout << "Brak miejsca na serwerze." << std::endl;
#endif
	}

	// je�eli wszyscy gracze do��czyli do gry, stw�rz nowy w�tek z gr�
	if (m_game.isFull() && !m_playing) {
		while (m_gameSocket.bind(m_gamePort) != sf::Socket::Done)
			m_gamePort++;
		m_gameSocket.setBlocking(false);
#ifdef DEBUG
		std::cout << "Przydzielono do gry port " << m_gamePort << std::endl;
#endif
		m_playing = true;

		Start start;
		start.id1 = m_game.m_players[0]->id;
		start.id2 = m_game.m_players[1]->id;
		start.position1 = { 4 * FIELD_WIDTH, 5 * FIELD_HEIGHT };
		start.position2 = { MAP_WIDTH - 5 * FIELD_WIDTH, 5 * FIELD_HEIGHT };
		start.gamePort = m_gamePort;

		for (int i = 0; i < m_game.MAX_PLAYER_COUNT; i++) {
			start.id = m_game.m_players[i]->id;
			send(&start, sizeof(Start), m_game.m_players[i]->ip, m_game.m_players[i]->port);
		}

		m_gameThread = std::thread(&Server::play, this);
	}
}

/*------------------------------------------------------------------------------------*/
//		Przetwarza pakiet typu DC.
/*------------------------------------------------------------------------------------*/
void Server::process(Datagram::DC * dc) {
	// wyrzu� z gry wszystkich graczy i usu� z listy tych, kt�rzy si� roz��czyli
	Datagram::Quit quit;
	for (int i = 0; i < m_game.MAX_PLAYER_COUNT; i++) {
		if (m_game.m_players[i])
			send(&quit, sizeof(Datagram::Quit), m_game.m_players[i]->ip, m_game.m_players[i]->port);
		if (m_game.m_players[i] && (m_game.m_players[i]->id == dc->playerID)) {
			delete m_game.m_players[i];
			m_game.m_players[i] = nullptr;
#ifdef DEBUG
			std::cout << "Rozlaczono gracza o id: " << dc->playerID << std::endl;
#endif
		}
	}
	// usu� w�tek gry, od��cz socket
	if (m_playing) {
		m_gameSocket.unbind();
		m_playing = false;
		m_gameThread.join();
	}
}

/*------------------------------------------------------------------------------------*/
//		Przetwarza pakiet typu Data.
/*------------------------------------------------------------------------------------*/
void Server::process(Datagram::Data * data) {
	using namespace Datagram;
	for (int i = 0; i < m_game.MAX_PLAYER_COUNT; i++) {
		if (m_game.m_players[i]) //&& (m_players[i]->id != data->playerID))
			broadcast(data, sizeof(Data), m_game.m_players[i]->ip, m_game.m_players[i]->port);
	}
}

/*------------------------------------------------------------------------------------*/
//		Wysy�a dane do klient�w oczekuj�cych pod��czenia do serwera.
/*------------------------------------------------------------------------------------*/
void Server::send(const void * data, size_t size, const sf::IpAddress address, unsigned short port) {
	m_listeningSocket.send(data, size, address, port);
}

/*------------------------------------------------------------------------------------*/
//		Wysy�a dane do wszysktich uczestnik�w gry.
/*------------------------------------------------------------------------------------*/
void Server::broadcast(const void * data, size_t size, const sf::IpAddress address, unsigned short port) {
	m_gameSocket.send(data, size, address, port);
}
