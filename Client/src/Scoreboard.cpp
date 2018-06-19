#include "Scoreboard.h"


extern sf::Mutex mutex;

Scoreboard::Scoreboard(Snake ** snakes, int nPlayers, sf::Clock * clock, int startTime, int height, const std::string & tileset, const std::string & fontName)
	: m_nPlayers(nPlayers),
	m_snakes(snakes),
	m_clockPtr(clock),
	m_prevTime(0),
	m_height(height), 
	m_position(sf::Vector2i(0, MAP_HEIGHT)),
	m_score(nullptr),
	m_timeText(nullptr)
{
	m_min = (startTime) / 60;
	m_sec = (startTime) % 60;
	load(tileset, fontName);
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

/*------------------------------------------------------------------------------------*/
//		Jednorazowo ³aduje do pamiêci potrzebne dane o tablicy wyników.
/*------------------------------------------------------------------------------------*/
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
		m_score[player]->setFillColor(sf::Color::Black);
		m_score[player]->setPosition(sf::Vector2f(player*MAP_WIDTH / m_nPlayers + m_position.x + MAP_WIDTH / m_nPlayers / 2, m_position.y + 16));

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

/*------------------------------------------------------------------------------------*/
//		Zwraca true gdy up³ynê³a sekunda.
/*------------------------------------------------------------------------------------*/
bool Scoreboard::tick() {
	if (m_prevTime != (int)m_clockPtr->getElapsedTime().asSeconds()) {
		m_prevTime++;
		return true;
	}
	else return false;
}

/*------------------------------------------------------------------------------------*/
//		Odœwie¿a wynik gry.
/*------------------------------------------------------------------------------------*/
void Scoreboard::updateScore() {
	for (int player = 0; player < m_nPlayers; player++) {
		auto str = std::to_string(m_snakes[player]->m_fruits);
		m_score[player]->setString(str);
	}
}

/*------------------------------------------------------------------------------------*/
//		Odœwie¿a czas do koñca gry.
/*------------------------------------------------------------------------------------*/
void Scoreboard::updateTime() {

	if (tick()) {
		m_sec--;
		if (m_sec < 0) {
			m_sec = 59; m_min--;
		}
	}
	std::string zero_str = "0";
	if (m_sec > 9) zero_str = "";
	std::string time_str = std::to_string(m_min) + ":" + zero_str + std::to_string(m_sec);

	m_timeText->setString(time_str);
}

/*------------------------------------------------------------------------------------*/
//		Odœwie¿a czas do koñca gry (multiplayer).
/*------------------------------------------------------------------------------------*/
void Scoreboard::updateTime(int time) {
	m_min = time / 60;
	m_sec = time % 60;

	std::string zero_str = "0";
	if (m_sec > 9) zero_str = "";
	std::string time_str = std::to_string(m_min) + ":" + zero_str + std::to_string(m_sec);

	m_timeText->setString(time_str);
}

/*------------------------------------------------------------------------------------*/
//		Odœwie¿a czas do koñca gry. (multiplayer)
/*------------------------------------------------------------------------------------*/
void Scoreboard::updateTime(unsigned short time) {

	if (tick()) {
		m_sec--;
		if (m_sec < 0) {
			m_sec = 59; m_min--;
		}
	}
	std::string zero_str = "0";
	if (m_sec > 9) zero_str = "";
	std::string time_str = std::to_string(m_min) + ":" + zero_str + std::to_string(m_sec);

	m_timeText->setString(time_str);
}

/*------------------------------------------------------------------------------------*/
//		Zbiorcza funkcja odœwie¿aj¹ca dane o grze.
/*------------------------------------------------------------------------------------*/
void Scoreboard::update() {
	updateScore();
	updateTime();
}

/*------------------------------------------------------------------------------------*/
//		Zbiorcza funkcja odœwie¿aj¹ca dane o grze.
/*------------------------------------------------------------------------------------*/
void Scoreboard::update(int time) {
	updateScore();
	mutex.lock();
	updateTime(time);
	mutex.unlock();
}

/*------------------------------------------------------------------------------------*/
//		Ustawia czas w grze.
/*------------------------------------------------------------------------------------*/
void Scoreboard::setTime(int time) {
	m_min = time / 60;
	m_sec = time % 60;
	m_prevTime = 0;
}

/*------------------------------------------------------------------------------------*/
//		Zwraca true gdy gra siê zakoñczy.
/*------------------------------------------------------------------------------------*/
bool Scoreboard::timeUp() {
	return (m_min == 0 && m_sec == 0);
}

/*------------------------------------------------------------------------------------*/
//		Rysuje tablicê wyników.
/*------------------------------------------------------------------------------------*/
void Scoreboard::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	
	states.texture = &m_texture;
	target.draw(m_vertices, states);

	for (int player = 0; player < m_nPlayers; player++) {
		target.draw(*m_score[player]);
	}

	target.draw(*m_timeText);
}
