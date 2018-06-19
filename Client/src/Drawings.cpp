#include "Game.h"


void Game::drawEnd(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_sprite);
}

void Game::drawMenu(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_sprite);
}

/*------------------------------------------------------------------------------------------*/

void Game::drawQueue(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_sprite);
}

/*------------------------------------------------------------------------------------------*/

void Game::drawGame(sf::RenderTarget & target, sf::RenderStates states) const {
	if (m_fruitPtr)
		target.draw(*m_fruitPtr);
	if (!m_snakesPtr) return;

	for (int i = m_nPlayers - 1; i > -1; i--) {
		target.draw(*m_snakesPtr[i]);
	}
	target.draw(*m_scoreboardPtr);
}