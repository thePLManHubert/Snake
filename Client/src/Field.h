#pragma once
#include "Globals.h"
#include <SFML/Graphics.hpp>

extern const int FIELD_WIDTH;
extern const int FIELD_HEIGHT;
#define FW FIELD_WIDTH
#define FH FIELD_HEIGHT


class Field : public sf::Drawable {
public:
	enum Type { TailBlock, BodyBlock, FruitBlock, HeadBlock };

protected:
	sf::Vector2i m_position;
	float m_rotation;
	sf::Color m_color;
	Type m_type;
	sf::Texture m_texture;
	sf::Sprite m_sprite;

public:
	Field(unsigned short position, Type type = TailBlock);
	Field(unsigned short position, sf::Color color = sf::Color::White, Type type = TailBlock);
	Field(unsigned short position, int rotation = 0, Type type = TailBlock);
	Field(unsigned short position, int rotation = 0, sf::Color color = sf::Color::White, Type type = TailBlock);
	Field(sf::Vector2i position, Type type = TailBlock);
	Field(sf::Vector2i position, sf::Color color = sf::Color::White, Type type = TailBlock);
	Field(sf::Vector2i position, int rotation = 0, Type type = TailBlock);
	Field(sf::Vector2i position, int rotation = 0, sf::Color color = sf::Color::White, Type type = TailBlock);

public:
	sf::Vector2i getPosition() const;
	Type getType() const;
	sf::Sprite& getSprite();
	int getRotation() const;
	sf::Color getColor() const;

	void setPosition(unsigned short position);
	void setPosition(int x, int y);
	void setPosition(const sf::Vector2i position);
	void setPosition(const Field& field);
	void setRotation(int rotation);

	static unsigned short convertPosition(sf::Vector2f position);

protected:
	void setTexture();

private:
	void convertPosition(unsigned short position);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
};
