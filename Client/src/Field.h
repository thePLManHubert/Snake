#pragma once
#include "Globals.h"
#include <SFML/Graphics.hpp>

#define FIELD_WIDTH 32
#define FIELD_HEIGHT 32

class Field : public sf::Drawable {
public:
	enum Type { EmptyBlock, SnakeBlock, FruitBlock, WallBlock };

protected:
	sf::Vector2i m_position;
	Type m_type;
	sf::Texture m_texture;
	sf::Sprite m_sprite;

public:
	Field();
	Field(sf::Vector2i position, Type type = EmptyBlock);
	~Field();

	sf::Vector2i getPosition() const;
	Type getType() const;
	sf::Sprite& getSprite();

	void setPosition(int x, int y);
	void setPosition(sf::Vector2i position);
	void setPosition(const Field& field);

protected:
	void setTexture();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
};
