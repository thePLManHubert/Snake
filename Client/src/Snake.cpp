#include "Snake.h"
#include <iostream>


// u¿ywany do gry singleplayer
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

// u¿ywany do gry multiplayer
Snake::Snake(unsigned short headPosition, int id, sf::Color color, int limit, bool collision)
	: m_head(headPosition, color),
	m_color(color),
	m_direction(STOP),
	m_prevDirection(STOP),
	m_fruits(0),
	m_limit(limit),
	m_collisionEnabled(collision),
	m_wait(false),
	m_id(id),
	m_mappedPtr(nullptr)
{
}

Snake::~Snake() {
	m_body.deleteAllSegments();
	if (m_mappedPtr) {
		delete m_mappedPtr;
		m_mappedPtr = nullptr;
	}
}

/*------------------------------------------------------------------------------------*/
//		Wykrywa kolizjê wê¿a z samym sob¹.
/*------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------*/
//		Porównuje pozycjê wê¿a (ka¿dego segmentu wê¿a), z danym segmentem wejœciowym.
//		Zwraca true gdy któryœ segment wê¿a znajdzie siê na tej samej pozycji
//		co segment wejœciowy.
/*------------------------------------------------------------------------------------*/
bool Snake::comparePosition(Field * segment) {
	if (m_head.getPosition() == segment->getPosition())
		return true;

	if (!m_body.tail) return false;

	Body::Segment* temp = m_body.tail;
	for (temp; temp->next; temp = temp->next) {
		if (temp->getPosition() == segment->getPosition())
			return true;
	}
	if (temp->getPosition() == segment->getPosition())
		return true;

	return false;
}

/*------------------------------------------------------------------------------------*/
//		Przek³ada segmenty wê¿a na ³atw¹ do przes³ania tablicê.
/*------------------------------------------------------------------------------------*/
unsigned short * Snake::map() {
	m_mappedPtr = new unsigned short[m_fruits + 2];
	m_mappedPtr[0] = m_fruits + 1; // liczba wszystkich segmentów
	m_mappedPtr[1] = m_head.getPosition().y / FIELD_HEIGHT * MAP_X + m_head.getPosition().x / FIELD_WIDTH; // pozycja g³owy

	if (m_body.tail) {
		Body::Segment * temp = m_body.tail;
		for (int i = m_fruits + 1; temp->next; temp = temp->next, i--) { // reszta cia³a wê¿a
			m_mappedPtr[i] = temp->getPosition().y / FIELD_HEIGHT * MAP_X + temp->getPosition().x / FIELD_WIDTH;
		}
		m_mappedPtr[2] = temp->getPosition().y / FIELD_HEIGHT * MAP_X + temp->getPosition().x / FIELD_WIDTH;
	}
	return m_mappedPtr;
}

void Snake::unmap()
{
}

/*------------------------------------------------------------------------------------*/
//		Ustawia kierunek, w którym w¹¿ bêdzie siê porusza³.
/*------------------------------------------------------------------------------------*/
void Snake::setDirection(Direction direction) {
	if (m_direction == FREEZE) return;
	if (direction == FREEZE) {
		m_direction = direction;
		return;
	}
	if (!m_wait) {
		m_wait = true;
		if (m_fruits) {
			if (m_direction == LEFT && direction == RIGHT) return;
			if (m_direction == RIGHT && direction == LEFT)	return;
			if (m_direction == UP && direction == DOWN)	return;
			if (m_direction == DOWN && direction == UP)	return;
		}
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

/*------------------------------------------------------------------------------------*/
//		Ustawia pocz¹tkow¹ pozycjê wê¿a, gdy nie uzbiera³ jeszcze ¿adnych owoców.
/*------------------------------------------------------------------------------------*/
void Snake::setPosition(sf::Vector2i position) {
	if(!m_fruits) m_head.setPosition(position);
}

/*------------------------------------------------------------------------------------*/
//		Powoduje poruszanie siê wê¿a w wybranym kierunku. (multiplayer)
/*------------------------------------------------------------------------------------*/
void Snake::move(Datagram::Data * data) {

	unsigned short last;
	last = (data->score < 20) ? data->score : 19;

	m_head.setPosition(data->position[0]); // mo¿na to poprawiæ
	if (data->grow) {
		if (!m_body.tail)
			m_body.tail = new Body::Segment(data->position[1], 0, m_head.getColor(), Field::TailBlock);
		else {
			Body::Segment * segment = m_body.tail;
			while (segment->next) segment = segment->next;
			segment->next = new Body::Segment(data->position[1], m_head.getRotation(), m_head.getColor());
		}
	}
	else {
		if (m_body.tail) {
			Body::Segment * segment = m_body.tail;
			for (int i = last; i > 0; i--) {
				segment->setPosition(data->position[i]);
				segment = segment->next;
			}
		}
	}
}

/*------------------------------------------------------------------------------------*/
//		Powoduje poruszanie siê wê¿a w wybranym kierunku.
/*------------------------------------------------------------------------------------*/
bool Snake::move(Fruit & fruit) {
	if (m_direction == FREEZE) return false;

	bool fruitHit = false;
	m_head.move(m_direction);

	if (m_direction != STOP) {
		if (fruit.getPosition() == m_head.getPosition()) {
			if (m_fruits < m_limit - 1) m_body.grow(m_head);
			else m_body.follow(m_head);
			m_fruits++;
			fruitHit = true;
		}
		else
			m_body.follow(m_head);
	}

	m_wait = false;
	return fruitHit;
}

/*------------------------------------------------------------------------------------*/
//		Sterowanie wê¿em przez komputer.
/*------------------------------------------------------------------------------------*/
bool Snake::moveAutomatically(Fruit & fruit) {
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

	return this->move(fruit);
}

/*------------------------------------------------------------------------------------*/
//		Rysuje elementy gry.
/*------------------------------------------------------------------------------------*/
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
	: Field(position, color, HeadBlock), 
	m_prev_position(0, 0) 
{
}

Snake::Head::Head(unsigned short position, sf::Color color) 
	: Field(position, color, HeadBlock),
	m_prev_position(0, 0)
{
}

/*------------------------------------------------------------------------------------*/
//		Poruszanie g³ow¹ wê¿a.
/*------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------*/
//		Podaje popzredni¹ pozycjê g³owy.
/*------------------------------------------------------------------------------------*/
sf::Vector2i Snake::Head::getPrevPos() const {
	return m_prev_position;
}

Snake::Body::Segment::Segment(sf::Vector2i position, int rotation, sf::Color color, Type type)
	: Field(position, rotation, color, type) {
	next = nullptr;
}

Snake::Body::Segment::Segment(unsigned short position, int rotation, sf::Color color, Type type) 
	: Field(position, rotation, color, type) {
	next = nullptr;
}

Snake::Body::Body()
	: tail(nullptr) {}

/*------------------------------------------------------------------------------------*/
//		Odpowiednio porusza segmentami wê¿a, tak aby pod¹¿a³y za g³ow¹.
/*------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------*/
//		Implementuje zachowanie wê¿a podczas wzrostu.
/*------------------------------------------------------------------------------------*/
void Snake::Body::grow(const Head & head) {
	if (!tail)
		tail = new Segment(head.getPrevPos(), head.getRotation(), head.getColor(), Field::TailBlock);
	else {
		Segment * segment = tail;
		while (segment->next) segment = segment->next;
		segment->next = new Segment(head.getPrevPos(), head.getRotation(), head.getColor());
	}
}

/*------------------------------------------------------------------------------------*/
//		Usuwa wszystkie segmenty wê¿a.
/*------------------------------------------------------------------------------------*/
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

void Snake::Body::follow(const Head& head, Datagram::Data data) {

}

void Snake::Body::grow(const Head& head, Datagram::Data data) {
	if (!tail) {
		
	}
}

/*------------------------------------------------------------------------------------*/
//		Przygotowuje pozycjê owocu.
/*------------------------------------------------------------------------------------*/
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