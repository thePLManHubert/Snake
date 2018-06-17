#include "Fruit.h"


Fruit::Fruit(sf::Vector2i position)
	: Field(position, FruitBlock)
{
}

Fruit::Fruit(unsigned short position) 
	: Field(position, FruitBlock) 
{
}

void Fruit::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_sprite);
}
