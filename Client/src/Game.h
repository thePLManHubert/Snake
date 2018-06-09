#pragma once
#include "TileMap.h"
#include "Snake.h"
#include "Scoreboard.h"
#include "Client.h"


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
	Fruit * m_fruitPtr;
	Snake ** m_snakesPtr;
	Scoreboard * m_scoreboardPtr;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	bool m_textureUpdated;

public:
	Client * m_clientPtr;

public:
	Game(int maxnPlayers = 4, int gameTime = 180, bool collision = true);
	~Game();

	void loadMenu();
	void control(sf::Event& event, sf::RenderWindow& window);
	void update();

	// Singleplayer game
	void startSingleplayer();
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

