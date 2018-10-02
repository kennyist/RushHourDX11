#include "Soft351GameManager.h"

// Constructor & setting default game state to Main Menu
Soft351GameManager::Soft351GameManager()
{
	m_gameState = GameState::MainMenu;
}

// Descontructor
Soft351GameManager::~Soft351GameManager()
{
}

// Return the current game state
Soft351GameManager::GameState Soft351GameManager::GetState() {
	return m_gameState;
}

// Set the current game state
void Soft351GameManager::SetState(Soft351GameManager::GameState state) {
	m_gameState = state;
}