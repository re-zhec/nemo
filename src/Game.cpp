#include "../include/Game.hpp"

namespace fe
{

Game::Game()
	: m_state(GameState::Start)
{
}

void Game::Pause()
{
	m_old_state = m_state;
	m_state = GameState::Pause;
}

void Game::Resume()
{
	m_state = m_old_state;
}

GameState Game::State() const
{
	return m_state;
}

}