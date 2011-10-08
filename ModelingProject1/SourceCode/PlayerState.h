#pragma once

#include "State.h"

#include "GameInputContext.h"

namespace GameCoreStates
{
  class PlayerState : public State
  {
   public:
	PlayerState(int id);
	PlayerState(){};
	~PlayerState(){};

	int checkMovementRestrictions(int keyPreviouslyPressed, int previousState, 
		                          int currentState, std::list<InputMapping::Key> keys);
   protected:
	virtual int checkMovement(int keyPreviouslyPressed, int previousState, 
                              int currentState, std::list<InputMapping::Key> keys);
  };
}