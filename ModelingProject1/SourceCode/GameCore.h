#pragma once

#include "GameState.h"
#include "GameConfiguration.h"
#include "GameSaves.h"
#include "GameScreen.h"
#include "GameTimer.h"

class GameCore
{
public:
	GameCore(void);
	~GameCore(void);

	void changeState();
	void close();
	void initGame();
	void initPlayers();
	void setNewState();
	void setup();
	void setupFPS();

private:
	GameState *currentGameState;
	GameConfiguration *configuration;
	GameSaves *saves;
	GameScreen *screen;
	GameTimer *timer;
	int currentStateID;
};

