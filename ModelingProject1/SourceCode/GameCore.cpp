#include "GameCore.h"


GameCore::GameCore(void)
{
	currentGameState = 0;
	configuration = new GameConfiguration();
	saves = new GameSaves();
	screen = new GameScreen();
	timer = new GameTimer();
	currentStateID = STATE_NULL;
}


GameCore::~GameCore(void)
{
	delete currentGameState;
	delete configuration;
	delete saves;
	delete screen;
	delete timer;
}