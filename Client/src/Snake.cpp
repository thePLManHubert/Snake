#include "Snake.h"

Snake::Snake(sf::Vector2i headPosition) 
	: m_head(headPosition), m_direction(STOP), m_fruits(0), m_nSegments(0) 
{
}

Snake::~Snake() {
	for (int i = 0; i < m_nSegments; ++i) {
		delete m_segments[i];
	}
}

void Snake::setDirection(Direction direction) {
	m_direction = direction;
}

void Snake::move() {
	move(m_direction);
}

void Snake::move(Direction direction) {
	
	if (m_nSegments) {
		for (int i = m_nSegments - 1; i >= 0; --i) {
			m_segments[i]->follow(*m_segments[i - 1]);
		}
		m_segments[0]->follow(m_head);
	}

	m_head.move(direction);
}

void Snake::eatAndMove() {
	eatAndMove(m_direction);
}

void Snake::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	if (m_nSegments)
		for ( int i = m_nSegments - 1; i >= 0; --i)
			target.draw(*m_segments[m_nSegments]);
	target.draw(m_head);
}

void Snake::eatAndMove(Direction direction) {

	m_fruits++;

	if (m_nSegments < 100) {
		if (!m_nSegments)
			m_segments[m_nSegments] = new Segment(m_head.getPosition());
		else
			m_segments[m_nSegments] = new Segment(m_segments[m_nSegments - 1]->getPosition());
		m_nSegments++;
	}
	move(direction);
}

Snake::Head::Head(sf::Vector2i position) {
	m_type = SnakeBlock;
	setPosition(position);
	setTexture();
}

sf::Vector2i Snake::Head::move(Direction direction)
{
	switch (direction) {
	case UP:
		setPosition(m_position.x, m_position.y - FIELD_HEIGHT);
		if (m_position.y < 0)
			setPosition(m_position.x, MAP_HEIGHT - FIELD_HEIGHT);
		break;
	case DOWN:
		setPosition(m_position.x, m_position.y + FIELD_HEIGHT);
		if (m_position.y > MAP_HEIGHT - FIELD_HEIGHT)
			setPosition(m_position.x, 0);
		break;
	case LEFT:
		setPosition(m_position.x - FIELD_WIDTH, m_position.y);
		if (m_position.x < 0)
			setPosition(MAP_WIDTH - FIELD_WIDTH, m_position.y);
		break;
	case RIGHT:
		setPosition(m_position.x + FIELD_WIDTH, m_position.y);
		if (m_position.x > MAP_WIDTH - FIELD_WIDTH)
			setPosition(0, m_position.y);
		break;
	case STOP:
		break;
	}
	return m_position;
}

Snake::Segment::Segment(sf::Vector2i position) {
	m_type = SnakeBlock;
	setPosition(position);
	setTexture();
}

sf::Vector2i Snake::Segment::follow(const Field& field) {
	setPosition(field);

	return m_position;
}
