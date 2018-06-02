#pragma once
#include "Field.h"
#include "Fruit.h"
#include <SFML/Graphics.hpp>

extern const int MAP_WIDTH;
extern const int MAP_HEIGHT;


class Snake : public sf::Drawable {
public:
	enum Direction { UP, DOWN, LEFT, RIGHT, STOP};

	class Head : public Field {
	private:
		sf::Vector2i m_prev_position;

	public:
		Head(sf::Vector2i position, sf::Color color = sf::Color::White);

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

		};

	public:
		Segment * tail;

	public:
		Body();

	public:
		void follow(const Head& head);
		void grow(const Head& head);
		void deleteAllSegments();

	};

public:
	Head m_head;
	Body m_body;
	Direction m_direction;
	int m_fruits;
	sf::Color m_color;
	int m_limit;

public:
	Snake(sf::Vector2i headPosition, sf::Color color, int limit = 1000);
	~Snake();

	void setDirection(Direction direction);
	void setPosition(sf::Vector2i position);

	void move(Fruit& fruit);
	void moveAutomatically(Fruit& fruit);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

