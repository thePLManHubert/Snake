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
		Head(sf::Vector2i position);

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
			Segment(sf::Vector2i position, int rotation = 0, Type type = SnakeBlock);

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

public:
	Snake(sf::Vector2i headPosition);
	~Snake();

	void setDirection(Direction direction);

	void move(Fruit& fruit);
	void move(Direction direction, Fruit& fruit);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

