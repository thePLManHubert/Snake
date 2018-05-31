#include "Fruit.h"


Fruit::Fruit(sf::Vector2i position)
	: Field(position, FruitBlock), seed(time(NULL)) {
	setTexture();
	srand(seed);
}

sf::Vector2i Fruit::preparePosition() {
	sf::Vector2i position;

	position.x = (rand() % 20) * 32;
	position.y = (rand() % 15) * 32;

	return position;
}



void Fruit::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_sprite);
}
