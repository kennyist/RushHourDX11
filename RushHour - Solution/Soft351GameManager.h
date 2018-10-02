#ifndef _SOFT351GAMEMANAGER_H
#define _SOFT351GAMEMANAGER_H

class Soft351GameManager
{
public:
	// --- Public Enums
	// Game state enum, what state the game is currently in
	enum GameState
	{
		MainMenu,
		Playing,
		EndScreen
	};

	// --- Constructor & Deconstructions 
	Soft351GameManager();
	~Soft351GameManager();

	// --- Public methods
	// Set the current game state
	void SetState(GameState state);
	// Get the currect game state
	GameState GetState();

private:
	// --- Private Vars
	// Current game state storage
	GameState m_gameState;
};

#endif // !_SOFT351GAMEMANAGER_H