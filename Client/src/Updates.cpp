#include "Game.h"


void Game::updateMenu() {
	m_sprite.setTexture(m_texture);
}

/*------------------------------------------------------------------------------------------*/

void Game::updateQueue() {
	m_sprite.setTexture(m_texture);
}

/*------------------------------------------------------------------------------------------*/

void Game::updateSingleplayer() {
	if (!m_snakesPtr) return;

	bool hitFruit = false;
	hitFruit = m_snakesPtr[0]->move(*m_fruitPtr);

	for (int i = 1; i < m_nPlayers; i++) {
		hitFruit = m_snakesPtr[i]->moveAutomatically(*m_fruitPtr) || hitFruit;
	}
	if (hitFruit)
		m_fruitPtr->setPosition(m_fruitPtr->preparePosition(*this));
	if (m_collisionEnabled)
		detectCollision();

	m_scoreboardPtr->update();
}

/*------------------------------------------------------------------------------------------*/

void Game::updateMultiplayer() {
	if (m_updateGame) {
		m_updateGame = false;
		if (!m_snakesPtr) return;

		bool hitFruit = false;
		hitFruit = m_snakesPtr[0]->move(*m_fruitPtr);

		for (int i = 1; i < m_nPlayers; i++) {
			hitFruit = m_snakesPtr[i]->move(*m_fruitPtr) || hitFruit;
		}
		if (hitFruit)
			m_fruitPtr->setPosition(m_fruitPtr->preparePosition(*this));
		if (m_collisionEnabled)
			detectCollision();

		m_scoreboardPtr->update();
	}
}
