#pragma once
#include "Field.h"
#include "Fruit.h"
#include <SFML/Graphics.hpp>
#include <vector>

extern const int MAP_WIDTH;
extern const int MAP_HEIGHT;

class Snake : public sf::Drawable {
public:
	enum Direction { UP, DOWN, LEFT, RIGHT, STOP};

	class Head : public Field {
	public:
		Head(sf::Vector2i position);
		sf::Vector2i move(Direction direction);

	};

	class Segment : public Field {
	public:
		Segment(sf::Vector2i position);
		sf::Vector2i follow(const Field& field);

	};

public:
	Head m_head;
	Segment* m_segments[100];
	int m_nSegments;
	Direction m_direction;
	int m_fruits;

public:
	Snake(sf::Vector2i headPosition);
	~Snake();

	void setDirection(Direction direction);

	void move();
	void move(Direction direction);
	void eatAndMove();
	void eatAndMove(Direction direction);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

