#pragma once
#include "Field.h"

class Snake;
class Game;

class Fruit : public Field {
public:
	Fruit(sf::Vector2i position);

public:
	sf::Vector2i preparePosition(const Snake& snake);
	sf::Vector2i preparePosition(Game& game);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

