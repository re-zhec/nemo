#pragma once

namespace fe
{

enum class GameState {
	Start,
	LoadSave,
	WorldMap,
	Cutscene,
	Battleground,
	Encounter,
	Pause
};

class Game
{
public:

	Game();
	void Pause();
	void Resume();
	GameState State() const;

private:
	GameState m_state;
	GameState m_old_state;

	float m_width;
	float m_height;
};

}