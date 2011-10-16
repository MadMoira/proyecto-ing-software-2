#pragma once

#include "RawInputConstants.h"
#include "PlayerSpriteStates.h"
#include "GameInputContext.h"

#include <map>
#include <set>
#include <list>
#include <string>
#include <vector>

#include <SDL\SDL.h>

class Player;

namespace InputMapping 
{
  struct MappedInput
  {
    std::set<GameCoreStates::Action> actions;
    std::vector<GameCoreStates::SpriteState> states;

	int directionKeyPressed;
    int buttonPreviouslyPressed;
	int jumpKeyPreviouslyPressed;
	int fastAttackKeyPreviouslyPressed;

    void eatAction(GameCoreStates::Action action);
    void eatStates();
  };

  typedef void (*inputCallback)(MappedInput& inputs, Player& playe, std::list<Key> keys);
  
  class GameInputContext;
  
  class GameInputMapper
  {
    public:
	 GameInputMapper(void);
	 ~GameInputMapper(void);

     void clearCurrentMappedInput(GameCoreStates::SpriteState activeState);
     void addCallback(inputCallback callback, int priorityInMultimap);
	 void dispatchInput(Player& player) const;

     void pushContext(const std::string& name);
     void popContext();

	 void convertRawSDLToRawButtons(Key& key);
	 void processNewInput();
     void setRawButtonState(Key key);

	 bool checkIfCanCleanStateVector();
	 void countAndClearStates();
	 int countStatesInMapper(int state);
	 bool verifyDoubleTappingForJumping(GameCoreStates::SpriteState state);
	 void pushBackNewState(int state, int valueButton);

	 void returnKeyForMappedInput(Uint8* keystate);
	 bool checkKeyState(Uint8 key);

	 std::list<Key> getListKeys() const; 

	 void pushBackStateOnMappedInput(GameCoreStates::SpriteState newState);

    private:
     std::map<std::string, GameInputContext*> inputContexts;
     std::list<GameInputContext*> activeContexts;
     std::multimap<int, inputCallback> callbackTable;
     MappedInput currentMappedInput;

     bool mapButtonToAction(RawInputButton button, GameCoreStates::Action& action) const;
     bool mapButtonToState(RawInputButton button, GameCoreStates::SpriteState& state) const;
  };
}
