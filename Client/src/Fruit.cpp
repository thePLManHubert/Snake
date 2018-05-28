#include "Fruit.h"

Fruit::Fruit(sf::Vector2i position) {
	seed = time(NULL);
	m_type = FruitBlock;
	setPosition();
	setTexture();
}

Fruit::~Fruit() {}

sf::Vector2i Fruit::setPosition()
{
	sf::Vector2i position;
	position.x = rand();
	position.y = rand();
	return position;
}

void Fruit::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_sprite);
}
