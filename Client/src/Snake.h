#pragma once
#include "Field.h"
#include "Fruit.h"
#include "packets.h"
#include <SFML/Graphics.hpp>

extern const int MAP_WIDTH;
extern const int MAP_HEIGHT;


class Snake : public sf::Drawable {
public:
	enum Direction { UP, DOWN, LEFT, RIGHT, STOP, FREEZE };

	class Head : public Field {
	private:
		sf::Vector2i m_prev_position;

	public:
		Head(sf::Vector2i position, sf::Color color = sf::Color::White);
		Head(unsigned short position, sf::Color color = sf::Color::White);

	public:
		void move(Direction direction);
		sf::Vector2i getPrevPos() const;

	};

	class Body {
	public:

		class Segment : public Field {
		public:
			Segment * next;

		public:
			Segment(sf::Vector2i position, int rotation = 0, sf::Color color = sf::Color::White, Type type = BodyBlock);
			Segment(unsigned short position, int rotation = 0, sf::Color color = sf::Color::White, Type type = BodyBlock);
		};

	public:
		Segment * tail;

	public:
		Body();

	public:
		void follow(const Head& head);
		void grow(const Head& head);
		void deleteAllSegments();

		// multiplayer
		void follow(const Head& head, Datagram::Data data);
		void grow(const Head& head, Datagram::Data data);
	};

public:
	Head m_head;
	Body m_body;
	Direction m_direction;
	Direction m_prevDirection;
	int m_fruits;
	sf::Color m_color;
	int m_limit;
	bool m_collisionEnabled;
	bool m_wait;
	int m_id;
	unsigned short * m_mappedPtr;

public:
	Snake(sf::Vector2i headPosition, sf::Color color, int limit = 1000, bool collision = false);
	Snake(unsigned short headPosition, int id, sf::Color color, int limit = 1000, bool collision = false);
	~Snake();

	void setDirection(Direction direction);
	void setPosition(sf::Vector2i position);

	bool move(Fruit& fruit);
	bool moveAutomatically(Fruit& fruit);

	bool comparePosition(Field* segment);

	// metody multiplayera
	unsigned short * map();
	void move(Datagram::Data * data);
	void unmap();


private:
	bool selfCollision();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private: // zabawa i testy
	void setRandomDir();

};

