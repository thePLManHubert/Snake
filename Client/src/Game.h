#pragma once
#include "TileMap.h"
#include "Snake.h"
#include "Scoreboard.h"


class Game : public sf::Drawable {
public:
	enum Stage { InMenu, InQueue, InGame };

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
	bool ** m_collisionMatrix;
	

public:
	Game(int maxnPlayers = 4, int gameTime = 180, bool collision = true);
	~Game();

	void control(sf::Event& event, sf::RenderWindow& window);
	void start();
	void reset();
	void update();

public:
	void detectCollision();

public:
	int getnPlayers() const;
	float getSpeed() const;
	Snake** getSnakes() const;
	Fruit& getFruit();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

