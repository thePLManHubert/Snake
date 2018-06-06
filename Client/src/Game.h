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
	sf::Clock m_clock;
	Fruit * m_fruitPtr;
	Snake ** m_snakesPtr;
	Scoreboard * m_scoreboardPtr;
	

public:
	Game(int maxnPlayers = 4);
	~Game();

	void start();
	void control(sf::Event& event, sf::RenderWindow& window);
	void reset();
	void update();

public:
	float getSpeed() const;
	Fruit& getFruit();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

