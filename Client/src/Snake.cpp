#include "Snake.h"
#include <iostream>


Snake::Snake(sf::Vector2i headPosition, sf::Color color, int limit, bool collision)
	: m_head(headPosition, color),
	m_color(color),
	m_direction(STOP),
	m_prevDirection(STOP),
	m_fruits(0),
	m_limit(limit),
	m_collisionEnabled(collision),
	m_wait(false)
{
}

Snake::~Snake() {
	m_body.deleteAllSegments();
}

bool Snake::selfCollision() {
	if (!m_body.tail) return false;

	Body::Segment * segment = m_body.tail;
	for (segment; segment->next; segment = segment->next) {
		if (m_head.getPosition() == segment->getPosition())
			return true;
	}
	if (m_head.getPosition() == segment->getPosition())
		return true;

	return false;
}

void Snake::setDirection(Direction direction) {
	if (m_direction == FREEZE) return;
	if (direction == FREEZE) {
		m_direction = direction;
		return;
	}
	if (!m_wait) {
		m_wait = true;
		if (m_direction == LEFT && direction == RIGHT) return;
		if (m_direction == RIGHT && direction == LEFT)	return;
		if (m_direction == UP && direction == DOWN)	return;
		if (m_direction == DOWN && direction == UP)	return;
		
		if (m_direction == STOP && m_fruits) {
			if (m_prevDirection == LEFT && direction == RIGHT) return;
			if (m_prevDirection == RIGHT && direction == LEFT)	return;
			if (m_prevDirection == UP && direction == DOWN)	return;
			if (m_prevDirection == DOWN && direction == UP)	return;
		}

		if(m_direction != STOP)
			m_prevDirection = m_direction;
		m_direction = direction;
	}
}

void Snake::setPosition(sf::Vector2i position) {
	if(!m_fruits) m_head.setPosition(position);
}

void Snake::move(Fruit& fruit) {
	if (m_collisionEnabled) 
		if (m_direction == FREEZE) return;

	m_head.move(m_direction);

	if (m_direction != STOP) {
		if (fruit.getPosition() == m_head.getPosition()) {
			if (m_fruits < m_limit - 1) m_body.grow(m_head);
			else m_body.follow(m_head);
			fruit.setPosition(fruit.preparePosition(*this));
			m_fruits++;
		}
		else
			m_body.follow(m_head);
	}
	if (m_collisionEnabled) {
		if (selfCollision()) {
			setDirection(FREEZE);
			return;
		}
	}

	m_wait = false;
}

void Snake::moveAutomatically(Fruit & fruit) {
	int random = rand() % 20;
	if (random >= 0 && random < 14) {
		if (m_head.getPosition().x < fruit.getPosition().x) this->setDirection(RIGHT);
		else if (m_head.getPosition().x > fruit.getPosition().x) this->setDirection(LEFT);
		else if (m_head.getPosition().y < fruit.getPosition().y) this->setDirection(DOWN);
		else if (m_head.getPosition().y > fruit.getPosition().y) this->setDirection(UP);
	}
	else if (random >= 14 && random < 18){
		if (m_head.getPosition().y < fruit.getPosition().y) this->setDirection(DOWN);
		else if (m_head.getPosition().y > fruit.getPosition().y) this->setDirection(UP);
		else if (m_head.getPosition().x < fruit.getPosition().x) this->setDirection(RIGHT);
		else if (m_head.getPosition().x > fruit.getPosition().x) this->setDirection(LEFT);
	}
	else
		setRandomDir();

	this->move(fruit);
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

void Snake::setRandomDir(){
	int random = rand() % 4;
	if (random == 0) setDirection(UP);
	if (random == 1) setDirection(DOWN);
	if (random == 2) setDirection(LEFT);
	if (random == 3) setDirection(RIGHT);
}

Snake::Head::Head(sf::Vector2i position, sf::Color color)
	: Field(position, color, HeadBlock), m_prev_position(0, 0) {}

void  Snake::Head::move(Direction direction) {
	if (direction != STOP)
		m_prev_position = m_position;

	switch (direction) {
	case UP:
		setRotation(0);
		setPosition(m_position.x, m_position.y - FIELD_HEIGHT);
		if (m_position.y < 0)
			setPosition(m_position.x, MAP_HEIGHT - FIELD_HEIGHT);
		break;
	case DOWN:
		setRotation(180);
		setPosition(m_position.x, m_position.y + FIELD_HEIGHT);
		if (m_position.y > MAP_HEIGHT - FIELD_HEIGHT)
			setPosition(m_position.x, 0);
		break;
	case LEFT:
		setRotation(270);
		setPosition(m_position.x - FIELD_WIDTH, m_position.y);
		if (m_position.x < 0)
			setPosition(MAP_WIDTH - FIELD_WIDTH, m_position.y);
		break;
	case RIGHT:
		setRotation(90);
		setPosition(m_position.x + FIELD_WIDTH, m_position.y);
		if (m_position.x > MAP_WIDTH - FIELD_WIDTH)
			setPosition(0, m_position.y);
		break;
	case STOP:
	case FREEZE:
		break;
	}
}

sf::Vector2i Snake::Head::getPrevPos() const {
	return m_prev_position;
}

Snake::Body::Segment::Segment(sf::Vector2i position, int rotation, sf::Color color, Type type)
	: Field(position, rotation, color, type) {
	next = nullptr;
}

Snake::Body::Body()
	: tail(nullptr) {}

void Snake::Body::follow(const Head & head) {
	if (!tail) return;

	if (!tail->next) {
		tail->setPosition(head.getPrevPos());
		tail->setRotation(head.getRotation());
	}
	else {
		Segment * segment = tail;
		while (segment->next) {
			segment->setPosition(segment->next->getPosition());
			segment->setRotation(segment->next->getRotation());
			segment = segment->next;
		}
		segment->setPosition(head.getPrevPos());
		segment->setRotation(head.getRotation());
	}
}

void Snake::Body::grow(const Head & head) {
	if (!tail)
		tail = new Segment(head.getPrevPos(), head.getRotation(), head.getColor(), Field::TailBlock);
	else {
		Segment * segment = tail;
		while (segment->next) segment = segment->next;
		segment->next = new Segment(head.getPrevPos(), head.getRotation(), head.getColor());
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
	tail = nullptr;
}

sf::Vector2i Fruit::preparePosition(const Snake& snake) {
	sf::Vector2i position;
	bool good = true;

#ifdef DEBUG
	int i = 0;
#endif

	do {
		good = true;

#ifdef DEBUG
		i++;
#endif
		position.x = (rand() % 20) * FIELD_WIDTH;
		position.y = (rand() % 15) * FIELD_HEIGHT;

		if (snake.m_head.getPosition() == position) good = false;
		auto segment = snake.m_body.tail;
		if (segment) {
			for (segment; segment->next; segment = segment->next)
				if (segment->getPosition() == position) good = false;
			if (segment->getPosition() == position) good = false;
		}

	} while (!good);
#ifdef DEBUG
	std::cout << "Generowano " << i << " razy." << std::endl;
#endif
	return position;
}