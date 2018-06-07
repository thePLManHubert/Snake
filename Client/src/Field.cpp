#include "Field.h"


Field::Field(sf::Vector2i position, Type type)
	: m_position(position), 
	m_rotation(0), 
	m_color(sf::Color::White), 
	m_type(type) 
{
	setTexture();
	m_sprite.setPosition((sf::Vector2f)m_position);
}

Field::Field(sf::Vector2i position, sf::Color color, Type type) 
	: m_position(position), 
	m_rotation(0), 
	m_color(color), 
	m_type(type) 
{
	setTexture();
	m_sprite.setColor(m_color);
	m_sprite.setPosition((sf::Vector2f)m_position);
}

Field::Field(sf::Vector2i position, int rotation, Type type)
	: m_position(position), 
	m_rotation(rotation), 
	m_color(sf::Color::White), 
	m_type(type) 
{
	setTexture();
	m_sprite.setPosition((sf::Vector2f)m_position);
	setRotation(m_rotation);
}

Field::Field(sf::Vector2i position, int rotation, sf::Color color, Type type)
	: m_position(position), 
	m_rotation(rotation), 
	m_color(color), 
	m_type(type) 
{
	setTexture();
	m_sprite.setColor(m_color);
	m_sprite.setPosition((sf::Vector2f)m_position);
	setRotation(m_rotation);
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

void Field::setRotation(int rotation) {
	m_rotation = rotation;
	switch (rotation) {
	case 0:
		m_sprite.setOrigin(0, 0);
		break;
	case 90:
		m_sprite.setOrigin(0, FIELD_HEIGHT*(32. / FIELD_HEIGHT));
		break;
	case 180:
		m_sprite.setOrigin(FIELD_WIDTH * (32. / FIELD_WIDTH), FIELD_HEIGHT * (32. / FIELD_HEIGHT));
		break;
	case 270:
		m_sprite.setOrigin(FIELD_WIDTH * (32. / FIELD_WIDTH), 0);
		break;
	}
	m_sprite.setRotation(m_rotation);
}

sf::Vector2i Field::getPosition() const {
	return m_position;
}

void Field::setTexture() {
	m_texture.loadFromFile("resources/TransparentWhite.png");
	m_sprite.setTexture(m_texture);

	switch(m_type) {
	case TailBlock:
		m_sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
		m_sprite.setScale(sf::Vector2f(FIELD_WIDTH / 32., FIELD_HEIGHT / 32.));
		break;
	case BodyBlock:
		m_sprite.setTextureRect(sf::IntRect(0, 32, 32, 32));
		m_sprite.setScale(sf::Vector2f(FIELD_WIDTH / 32., FIELD_HEIGHT / 32.));
		break;
	case FruitBlock:
		m_sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
		m_sprite.setScale(sf::Vector2f(FIELD_WIDTH / 32., FIELD_HEIGHT / 32.));
		break;
	case HeadBlock:
		m_sprite.setTextureRect(sf::IntRect(32, 32, 32, 32));
		m_sprite.setScale(sf::Vector2f(FIELD_WIDTH / 32., FIELD_HEIGHT / 32.));
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

int Field::getRotation() const {
	return m_rotation;
}

sf::Color Field::getColor() const {
	return m_color;
}
