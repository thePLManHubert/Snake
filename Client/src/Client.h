#pragma once
#include "packets.h"
#include "Game.h"
#include <SFML/Network.hpp>
#include <thread>


//class Game;

class Client {
public:
	enum Stage { Disconnected, Connecting, Connected };

private:
	Stage m_currentStage = Disconnected;
	sf::UdpSocket m_socket;
	sf::IpAddress m_serverIP;
	sf::IpAddress m_processIP;
	unsigned short m_port;
	unsigned short m_serverPort;
	unsigned short m_processPort;
	int m_id;
	char m_data[100];
	std::size_t m_dataSize;
	std::thread m_thread;
	Game * m_game;

public:
	Client(Game * game);
	Client(Game * game, sf::IpAddress serverIP, unsigned short serverPort = 5000);
	~Client();

public:
	bool start();
	void disconnect();
	void send(const void * data, size_t size);

private:
	void listen();
	void process(void * packet);

private:
	void process(Datagram::Data * data);
	void process(Datagram::Start * start);
	void process(Datagram::Quit * quit);
	void process(Datagram::Sync * sync);
	void process(Datagram::Reset * reset);

public: // getters
	bool isConnected() const;
	Stage getCurrentStage() const;
	int getID() const;
};