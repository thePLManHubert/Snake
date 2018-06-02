#include "Scoreboard.h"


Scoreboard::Scoreboard(Snake ** snakes, int nPlayers, sf::Clock * clock, int height, const std::string & tileset, const std::string & fontName)
	: m_nPlayers(nPlayers), m_snakes(snakes), m_clockPtr(clock), m_height(height), m_position(sf::Vector2i(0, MAP_HEIGHT)) {
	load(tileset, fontName);
	createScore(snakes, m_nPlayers);
}

Scoreboard::~Scoreboard() {
	for (int player = 0; player < m_nPlayers; player++) {
		delete m_score[player];
	}
	delete[] m_score;
	delete m_timeText;
	m_score = nullptr;
	m_timeText = nullptr;
}

bool Scoreboard::load(const std::string & tileset, const std::string & fontName){
	if (!m_texture.loadFromFile(tileset))
		return false;
	if (!m_font.loadFromFile(fontName))
		return false;

	m_timeText = new sf::Text();
	m_timeText->setFont(m_font);
	m_timeText->setCharacterSize(24);
	m_timeText->setFillColor(sf::Color::Black);
	m_timeText->setPosition(sf::Vector2f(MAP_WIDTH / 2 - 24, MAP_HEIGHT + m_height - 26));

	m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
	m_vertices.resize(m_nPlayers * 4);

	m_score = new sf::Text*[m_nPlayers];

	for (int player = 0; player < m_nPlayers; player++) {

		m_score[player] = new sf::Text("", m_font, 19);

		sf::Vertex* quad = &m_vertices[player * 4];

		quad[0].position = sf::Vector2f(player*MAP_WIDTH / m_nPlayers + m_position.x, m_position.y);
		quad[1].position = sf::Vector2f((player + 1)*MAP_WIDTH / m_nPlayers + m_position.x, m_position.y);
		quad[2].position = sf::Vector2f((player + 1)*MAP_WIDTH / m_nPlayers + m_position.x, m_height + m_position.y);
		quad[3].position = sf::Vector2f(player*MAP_WIDTH / m_nPlayers + m_position.x, m_height + m_position.y);

		quad[0].texCoords = sf::Vector2f(0, 0);
		quad[1].texCoords = sf::Vector2f(64, 0);
		quad[2].texCoords = sf::Vector2f(64, 64);
		quad[3].texCoords = sf::Vector2f(0, 64);

		quad[0].color =
		quad[1].color = m_snakes[player]->m_color;
		quad[2].color =
		quad[3].color = sf::Color::White;
	}

	return true;
}

bool Scoreboard::createScore(Snake ** snake, int nPlayers)
{
	return false;
}

void Scoreboard::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	
	states.texture = &m_texture;
	target.draw(m_vertices, states);

	for (int player = 0; player < m_nPlayers; player++) {
		auto str = std::to_string(m_snakes[player]->m_fruits);
		m_score[player]->setString(str);
		m_score[player]->setFillColor(sf::Color::Black);
		m_score[player]->setPosition(sf::Vector2f(player*MAP_WIDTH / m_nPlayers + m_position.x + MAP_WIDTH / m_nPlayers / 2, m_position.y + 16));

		target.draw(*m_score[player]);
	}

	int time = (int)m_clockPtr->getElapsedTime().asSeconds();
	int min = 2 -  time / 60;
	int sec = 59 - time % 60;
	std::string zero_str;
	(sec > 9) ? zero_str = "" : zero_str = "0";
	std::string time_str = std::to_string(min) + ":" + zero_str + std::to_string(sec);

	m_timeText->setString(time_str);
	target.draw(*m_timeText);
}
