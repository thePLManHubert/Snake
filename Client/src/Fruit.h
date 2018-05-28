#pragma once
#include "Field.h"
#include <random>

class Fruit : public Field {
private:
	unsigned int seed;

public:
	Fruit() = delete;
	Fruit(sf::Vector2i position);
	~Fruit();

	sf::Vector2i setPosition();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

