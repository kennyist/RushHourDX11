#include "Soft351Hud.h"

// Usefull links:
// CDXUT dialog options
// https://github.com/Microsoft/DXUT/blob/master/Optional/DXUTgui.h

// Constructor with value setup
Soft351Hud::Soft351Hud(
	CDXUTDialog &dialog,	// CDXUT dialog for hud setup and rendering
	int width,				// Screen width
	int height,				// Screen height
	LPCWSTR gameTitle		// Game name
)
{
	// Set CDXUT dialog by reference
	m_hud = &dialog;

	// -- IDs
	// string
	str_title = 5;
	str_lives = 6;
	str_jumps = 7;
	str_score = 8;
	str_shield = 9;

	// Buttons
	btn_start = 11;
	btn_quit = 12;

	// Combo
	combo_laneCount = 21;
	combo_difficulty = 22;
	combo_controlType = 23;
	combo_sensitivity = 24;

	// -- Vars
	m_width = width;
	m_height = height;
	str_title_content = gameTitle;

	// Dialog
	m_hud->SetSize(m_width, m_height);
}

// Deconstructions
Soft351Hud::~Soft351Hud() {

}

// Update screen sizing
void Soft351Hud::Update(
	int width,
	int height
) 
{
	// Vars
	m_width = width;
	m_height = height;

	// --- Dialog
	// Set position for drawing beginning 
	m_hud->SetLocation(0, 0);
	// Set the size of the drawing space
	m_hud->SetSize(m_width, m_height);
}

// --- Button IDs methods
// Return start button id
int Soft351Hud::StartButtonID() {
	return btn_start;
}
// return Quit button ID
int Soft351Hud::QuitButtonID() {
	return btn_quit;
}
// return returnToMenu button ID
int Soft351Hud::ReturnToMenuButtonID() {
	return btn_returnToMenu;
}

// --- Get setting values
// return the selected number of lanes
int Soft351Hud::GetNumberLanes() {
	// Add 3 as index 0 is 3 lanes
	return m_hud->GetComboBox(combo_laneCount)->GetSelectedIndex() + 3;
}
// Return the selected difficulty 
int Soft351Hud::GetDifficulty() {
	return m_hud->GetComboBox(combo_difficulty)->GetSelectedIndex();
}
// Return the selected control type
int Soft351Hud::GetControlType() {
	return m_hud->GetComboBox(combo_controlType)->GetSelectedIndex();
}
// Return the selected sensitivity as float
float Soft351Hud::GetSensitivity() {
	// Multiply by 10 for 0-100
	return m_hud->GetComboBox(combo_sensitivity)->GetSelectedIndex() * 10;
}

// --- UI drawing methods:
// Draw main menu
void Soft351Hud::DrawMainMenu() {
	// Clear current objects
	ClearHud();

	// Initial position setup
	int posY = 200;
	int posX = CenterCoordsX(400);
	int height = 40;

	// Add game title
	m_hud->AddStatic(
		str_title,
		str_title_content,
		posX,
		posY,
		400,
		50
	);

	posY += 50;

	// --- Difficulty select:
	// Draw difficulty title
	m_hud->AddStatic(
		0,
		L"Difficulty: ",
		posX,
		posY,
		200,
		height
	);

	posX += 200;

	// Draw difficulty combo box and add settings
	m_hud->AddComboBox(combo_difficulty, posX, posY, 200, height);
	m_hud->GetComboBox(combo_difficulty)->AddItem(L"Easy", NULL);
	m_hud->GetComboBox(combo_difficulty)->AddItem(L"Medium", NULL);
	m_hud->GetComboBox(combo_difficulty)->AddItem(L"Hard", NULL);
	m_hud->GetComboBox(combo_difficulty)->SetDropHeight(100);
	posX -= 200;

	// --- Control select:
	posY += height + 10;
	// Draw control type title
	m_hud->AddStatic(
		0,
		L"Control Type: ",
		posX,
		posY,
		200,
		height
	);

	posX += 200;
	// Draw control type combo box and add settings
	m_hud->AddComboBox(combo_controlType, posX, posY, 200, height);
	m_hud->GetComboBox(combo_controlType)->AddItem(L"Quick", NULL);
	m_hud->GetComboBox(combo_controlType)->AddItem(L"Smooth", NULL);
	m_hud->GetComboBox(combo_controlType)->AddItem(L"Mouse", NULL);
	m_hud->GetComboBox(combo_controlType)->SetDropHeight(100);
	posX -= 200;

	// --- Mouse sensitivity settings:
	posY += height + 10;
	// Draw sensitivity title
	m_hud->AddStatic(
		0,
		L"Mouse Sensitivity: ",
		posX,
		posY,
		200,
		height
	);

	posX += 200;
	// Draw combo box and add settings
	// Tried slider instead of combo but caused program to crash with no found fix
	m_hud->AddComboBox(combo_sensitivity, posX, posY, 200, height);
	m_hud->GetComboBox(combo_sensitivity)->AddItem(L"1", NULL);
	m_hud->GetComboBox(combo_sensitivity)->AddItem(L"2", NULL);
	m_hud->GetComboBox(combo_sensitivity)->AddItem(L"3", NULL);
	m_hud->GetComboBox(combo_sensitivity)->AddItem(L"4", NULL);
	m_hud->GetComboBox(combo_sensitivity)->AddItem(L"5", NULL);
	m_hud->GetComboBox(combo_sensitivity)->AddItem(L"6", NULL);
	m_hud->GetComboBox(combo_sensitivity)->AddItem(L"7", NULL);
	m_hud->GetComboBox(combo_sensitivity)->AddItem(L"8", NULL);
	m_hud->GetComboBox(combo_sensitivity)->AddItem(L"9", NULL);
	m_hud->GetComboBox(combo_sensitivity)->AddItem(L"10", NULL);
	m_hud->GetComboBox(combo_sensitivity)->SetDropHeight(100);
	m_hud->GetComboBox(combo_sensitivity)->SetSelectedByIndex(4);
	posX -= 200;

	// --- Lane settings
	posY += height + 10;
	// Draw lane select title
	m_hud->AddStatic(
		0,
		L"No. of lanes: ",
		posX,
		posY,
		200,
		height
	);

	posX += 200;
	// Draw lane select combo box and add settings
	m_hud->AddComboBox(combo_laneCount, posX, posY, 200, height);
	m_hud->GetComboBox(combo_laneCount)->AddItem(L"3", NULL);
	m_hud->GetComboBox(combo_laneCount)->AddItem(L"4", NULL);
	m_hud->GetComboBox(combo_laneCount)->AddItem(L"5", NULL);
	m_hud->GetComboBox(combo_laneCount)->AddItem(L"6", NULL);
	m_hud->GetComboBox(combo_laneCount)->AddItem(L"7", NULL);
	m_hud->GetComboBox(combo_laneCount)->AddItem(L"8", NULL);
	m_hud->GetComboBox(combo_laneCount)->AddItem(L"9", NULL);
	m_hud->GetComboBox(combo_laneCount)->SetDropHeight(100);
	posX -= 100;

	// --- Start game
	posY += height + 10;
	// Draw start game button
	m_hud->AddButton(
		btn_start,
		L"Start Game!",
		posX,
		posY,
		200,
		height
	);

	posY += height + 10;
	// --- Quit game
	// Draw quit game button
	m_hud->AddButton(btn_quit, L"Quit game (Esc)", posX, posY, 200, height, VK_ESCAPE);
}

void Soft351Hud::DrawGameMenu(int controlType)
{
	// Clear current UI contents
	ClearHud();
	// Draw quit button in top cornor
	m_hud->AddButton(btn_quit, L"Quit game (Esc)", 20, 20, 200, 40, VK_ESCAPE);

	// Set initial positioning
	int posY = 100;
	int posX = 50;
	int height = 20;
	
	// --- Help text:
	// Draw help text to screen based on control type
	switch (controlType)
	{
	default:
	case 0:
	case 1:	// if default, quick or smooth draw keyboard help
		m_hud->AddStatic(-1, L"Move left: Left Arrow / A", posX, posY, 200, height);
		posY += height + 10;
		m_hud->AddStatic(-2, L"Move right: Right Arrow / D", posX, posY, 200, height);
		posY += height + 10;
		m_hud->AddStatic(-3, L"Jump: Up Arrow / W", posX, posY, 200, height);
		break;
	case 2: // if mouse control, draw mouse help
		m_hud->AddStatic(-1, L"Movement: Move mouse left/right", posX, posY, 200, height);
		posY += height + 10;
		m_hud->AddStatic(-2, L"Jump: Left Click", posX, posY, 200, height);
		break;

	}

	posY += height + 10;
	// Draw score count
	m_hud->AddStatic(str_score, L"Score: 0", posX, posY, 200, height);
	posY += height + 10;
	// Draw life count
	m_hud->AddStatic(str_lives, L"Lives: 0", posX, posY, 200, height);
	posY += height + 10;
	// Draw Jump count
	m_hud->AddStatic(str_jumps, L"Jumps (Up arrow): 0", posX, posY, 200, height);
	posY += height + 10;
	// Shield time remaining
	m_hud->AddStatic(str_shield, L"", posX, posY, 200, height);
	// Disable until shield is active
	m_hud->GetStatic(str_shield)->SetVisible(false);

}

// Update game UI text
void Soft351Hud::UpdateGameUI(
	int lives,	// Life count
	int jumps,	// jump count
	int score	// score
)
{
	// Setup score string
	std::string string = "Score: ";
	// Add the score value to the string
	string += std::to_string(score);
	string += "!";
	// Update score UI element
	m_hud->GetStatic(str_score)->SetText(StringToWchar(string).c_str());
	// Setup Live string
	string = "Lives: ";
	// Add lives to string
	string += std::to_string(lives);
	// Update life UI element
	m_hud->GetStatic(str_lives)->SetText(StringToWchar(string).c_str());
	// Setup jump string
	string = "Jumps: ";
	// Add jumps to string
	string += std::to_string(jumps);
	// Update jump UI element
	m_hud->GetStatic(str_jumps)->SetText(StringToWchar(string).c_str());
}

// Update shield text
void Soft351Hud::UpdateShieldUI(
	bool active,
	float timer
)
{
	// if not active and ui is, disable ui
	if (!active && m_hud->GetStatic(str_shield)->GetVisible()) {
		m_hud->GetStatic(str_shield)->SetVisible(false);
		return;
	}
	// if shield active and ui not, enable ui
	if (active && !m_hud->GetStatic(str_shield)->GetVisible()) {
		m_hud->GetStatic(str_shield)->SetVisible(true);
	}
	// if not active, end
	if (!active) return;
	

	// Make string
	std::string string = "Shield active: ";
	string += std::to_string((int)timer);

	// update text
	m_hud->GetStatic(str_shield)->SetText(StringToWchar(string).c_str());
}

// Draw end game screen
void Soft351Hud::DrawEndGame(int score)
{
	// Clear the current UI
	ClearHud();
	// Setup initial ui positioning
	int posY = 100;
	int posX = CenterCoordsX(200);
	int height = 40;

	// Setup score string
	std::string string = "You Scored: ";
	string += std::to_string(score);
	string += "!";
	
	// Draw score UI 
	m_hud->AddStatic(0, StringToWchar(string).c_str(), posX, posY, 200, height);

	posY += height + 10;

	// Draw return to menu button
	m_hud->AddButton(btn_returnToMenu, L"Return to menu", posX, posY, 200, height);
}

// --- Private Methods:
// Clear everything from the hud
void Soft351Hud::ClearHud() {
	m_hud->RemoveAllControls();
}
// Convert string to wChar
std::wstring Soft351Hud::StringToWchar(const std::string& value) {
	return std::wstring(value.begin(), value.end());
}
// Get screen center with content offset
int Soft351Hud::CenterCoordsX(int contentsWidth) {
	// device screen in half for center
	// Take content width in half for offset
	return (m_width / 2) - (contentsWidth / 2);
}