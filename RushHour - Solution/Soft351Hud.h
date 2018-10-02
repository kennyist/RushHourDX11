#ifndef _SOFT351HUD_H
#define _SOFT351HUD_H

#include "DXUT.h"
#include "DXUTgui.h"

#include <string>

class Soft351Hud
{
public:
	// --- Constructor and Deconstructions
	// Constructor with value setup
	Soft351Hud(
		CDXUTDialog &dialog,	// CDXUT dialog for hud setup and rendering
		int width,				// Screen width
		int height,				// Screen height
		LPCWSTR gameTitle		// Game name
	);
	~Soft351Hud();
	
	// --- Public methods
	// Update screen sizing
	void Update(
		int width,
		int height
	);
	// Draw the main menu to the hud
	void DrawMainMenu();
	// Draw the game menu to the hud
	void DrawGameMenu(int controlType);
	// Draw the end menu to the hud
	void DrawEndGame(int score);
	// Update the text on the game menu 
	void UpdateGameUI(
		int lives,
		int jumps,
		int score
	);
	// Update shield text
	void UpdateShieldUI(
		bool active,
		float time
	);
	// Get the ID for the start button
	int StartButtonID();
	// Get the ID for the quit button
	int QuitButtonID();
	// Get the ID for the return to menu button
	int ReturnToMenuButtonID();
	// Get the selected number of lanes
	int GetNumberLanes();
	// Get the selected difficulty 
	int GetDifficulty();
	// Get the selected control type
	int GetControlType();
	// Get the selected mouse sensitivity 
	float GetSensitivity();
private:
	// --- Private variables
	// CDXUT dialog for hud rendering
	CDXUTDialog* m_hud;
	// Screen width and height storage
	int m_width, m_height;
	// Game name storage
	LPCWSTR str_title_content;
	// ID storage for each string
	int str_title, str_lives, str_jumps, str_score, str_shield;
	// ID storage for each button 
	int btn_start, btn_quit, btn_returnToMenu;
	// ID storage for each combo select box
	int combo_laneCount, combo_difficulty, combo_controlType, combo_sensitivity;

	// --- Private 
	// Clear everything from the hud before drawing new menu
	void ClearHud();
	// Get screen center with content offset
	int CenterCoordsX(int contentsWidth);
	// Convert String to Wchar
	std::wstring StringToWchar(const std::string& value);
};

#endif // !_SOFT351HUD_H