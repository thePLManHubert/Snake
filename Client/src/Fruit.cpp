#include "Fruit.h"


Fruit::Fruit(sf::Vector2i position)
	: Field(position, FruitBlock), seed(time(NULL)) {
	setTexture();
	srand(seed);
}

void Fruit::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_sprite);
}
