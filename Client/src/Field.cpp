#include "Field.h"


Field::Field(sf::Vector2i position, Type type)
	: m_position(position), m_type(type) {
	setTexture();
	m_sprite.setPosition((sf::Vector2f)m_position);
}

void Field::setPosition(int x, int y) {
	m_position.x = x;
	m_position.y = y;
	m_sprite.setPosition((sf::Vector2f)m_position);
}

void Field::setPosition(const sf::Vector2i position) {
	m_position = position;
	m_sprite.setPosition((sf::Vector2f)m_position);
}

void Field::setPosition(const Field& field) {
	m_position = field.m_position;
	m_sprite.setPosition((sf::Vector2f)m_position);
}

sf::Vector2i Field::getPosition() const {
	return m_position;
}

void Field::setTexture() {
	m_texture.loadFromFile("resources/Textures.png");
	m_sprite.setTexture(m_texture);

	switch(m_type) {
	case EmptyBlock:
		m_sprite.setTextureRect(sf::IntRect(0, 0, FIELD_WIDTH, FIELD_HEIGHT));
		break;
	case SnakeBlock:
		m_sprite.setTextureRect(sf::IntRect(0, FIELD_HEIGHT, FIELD_WIDTH, FIELD_HEIGHT));
		break;
	case FruitBlock:
		m_sprite.setTextureRect(sf::IntRect(FIELD_WIDTH, 0, FIELD_WIDTH, FIELD_HEIGHT));
		break;
	case WallBlock:
		m_sprite.setTextureRect(sf::IntRect(FIELD_WIDTH, FIELD_HEIGHT, FIELD_WIDTH, FIELD_HEIGHT));
		break;
	}
}

void Field::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_sprite);
}

Field::Type Field::getType() const {
	return m_type;
}

sf::Sprite & Field::getSprite() {
	return m_sprite;
}
