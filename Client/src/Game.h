#pragma once
#include "packets.h"
#include "TileMap.h"
#include "Snake.h"
#include "Scoreboard.h"

class Client;

class Game : public sf::Drawable {
public:
	enum Stage { InMenu, InQueue, InSingleplayer, InMultiplayer };

private:
	Stage m_stage;
	int m_playingLength;
	const int MAX_PLAYERS;
	int m_nPlayers;
	float m_speed;
	bool m_collisionEnabled;
	sf::Clock m_clock;
	sf::Clock m_renderClock;
	Fruit * m_fruitPtr;
	Snake ** m_snakesPtr;
	Scoreboard * m_scoreboardPtr;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	bool m_textureUpdated;
	sf::Mutex mutex;

public:
	bool m_updateGame;
	Client * m_clientPtr;

public:
	Game(int maxnPlayers, int gameTime = 180, bool collision = true);
	~Game();

	void loadMenu();
	void loadQueue();
	void control(sf::Event& event, sf::RenderWindow& window);
	void update();

	// Singleplayer game
	void startSingleplayer();
	void startMultiplayer(Datagram::Start * start);
	void approveChanges(Datagram::Data * data);
	void resetSingleplayer();
	void resetPlayerStatus();

	void closeGame();
	void deleteClient();

private:
	void controlMenu(sf::Event event, sf::RenderWindow& window);
	void controlQueue(sf::Event event, sf::RenderWindow& window);
	void controlSingleplayer(sf::Event event, sf::RenderWindow& window);
	void controlMultiplayer(sf::Event event, sf::RenderWindow& window);

	void updateMenu();
	void updateQueue();
	void updateSingleplayer();
	void updateMultiplayer();

	void drawMenu(sf::RenderTarget & target, sf::RenderStates states) const;
	void drawQueue(sf::RenderTarget & target, sf::RenderStates states) const;
	void drawGame(sf::RenderTarget & target, sf::RenderStates states) const;

public:
	void detectCollision();

public:
	void setStage(Stage stage);

	int getnPlayers() const;
	float getSpeed() const;
	Snake** getSnakes() const;
	Fruit& getFruit();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

