#pragma once
#include "Snake.h"
#include "SFML/Main.hpp"

const int SCOREBOARD_HEIGHT = 64;

class Scoreboard : public sf::Drawable {
private:
	int m_nPlayers;
	int m_height;
	int m_min, m_sec;
	int m_prevTime;
	Snake ** m_snakes;
	sf::Clock * m_clockPtr; // from game
	sf::Text * m_timeText;
	sf::VertexArray m_vertices;
	sf::Texture m_texture;
	sf::Font m_font;
	sf::Text ** m_score;
	sf::Vector2i m_position;

public:
	Scoreboard(Snake ** snakes, int nPlayers, sf::Clock * clock, int startTime = 180, int height = 64, const std::string& tileset = "resources/Scoreboard.png", const std::string& fontName = "resources/consola.ttf");
	~Scoreboard();

private:
	bool load(const std::string& tileset, const std::string& fontName);
	bool tick();
	void updateScore();
	void updateTime();
	void updateTime(int time);
	void updateTime(unsigned short time); // multiplayer

public:
	void update();
	void update(int time);
	void setTime(int time);
	bool timeUp();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

