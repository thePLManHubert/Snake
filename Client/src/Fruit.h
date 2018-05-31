#pragma once
#include "Field.h"

class Snake;

class Fruit : public Field {
private:
	unsigned int seed;

public:
	Fruit(sf::Vector2i position);

public:
	sf::Vector2i preparePosition(const Snake& snake);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

