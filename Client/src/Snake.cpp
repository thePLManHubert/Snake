#include "Snake.h"

Snake::Snake(sf::Vector2i headPosition) 
	: m_head(headPosition), m_direction(STOP), m_fruits(0) {}

Snake::~Snake() {
	m_body.deleteAllSegments();
}

void Snake::setDirection(Direction direction) {
	m_direction = direction;
}

void Snake::move(Fruit& fruit) {
	move(m_direction, fruit);
}

void Snake::move(Direction direction, Fruit& fruit) {
	m_head.move(direction);
	if (fruit.getPosition() == m_head.getPosition()) {
		m_body.grow(m_head);
		fruit.setPosition(fruit.preparePosition());
		m_fruits++;
	}
	else
		m_body.follow(m_head);
}

void Snake::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	if (m_body.tail) {
		Body::Segment * segment = m_body.tail;
		while (segment->next) {
			target.draw(*segment);
			segment = segment->next;
		}
		target.draw(*segment);
	}
	target.draw(m_head);
}

Snake::Head::Head(sf::Vector2i position)
	: Field(position, SnakeBlock), m_prev_position(0, 0) {}

void  Snake::Head::move(Direction direction) {
	if (direction != STOP)
		m_prev_position = m_position;

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
}

sf::Vector2i Snake::Head::getPrevPos() const {
	return m_prev_position;
}

Snake::Body::Segment::Segment(sf::Vector2i position) 
	: Field(position, SnakeBlock) {
	setTexture();
	next = nullptr;
}

Snake::Body::Body()
	: tail(nullptr) {}

void Snake::Body::follow(const Head & head) {
	if (!tail) return;

	if (!tail->next)
		tail->setPosition(head.getPrevPos());
	else {
		Segment * segment = tail;
		while (segment->next) {
			segment->setPosition(segment->next->getPosition());
			segment = segment->next;
		}
		segment->setPosition(head.getPrevPos());
	}
}

void Snake::Body::grow(const Head & head) {
	if (!tail)
		tail = new Segment(head.getPrevPos());
	else {
		Segment * segment = tail;
		while (segment->next) segment = segment->next;
		segment->next = new Segment(head.getPrevPos());
	}
}

void Snake::Body::deleteAllSegments() {
	if (!tail) return;

	Segment *prev, *temp = tail;
	while (temp->next) {
		prev = temp;
		temp = temp->next;
		delete prev;
	}
	delete temp;
}

