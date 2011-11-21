
#include <algorithm>
#include "ComparatorFunctions.h"

#include "Player.h"

void Characters::Player::executeAction()
{
  switch( characterSprite->getCurrentState() )
  {
    case GameCoreStates::STILL:
    {
      noAction();
      break;
    }
	case GameCoreStates::WALKING:
    {
      walk();
      break;
    }
	case GameCoreStates::RUNNING:
	{
      run();
      break;
    }
	case GameCoreStates::JUMPING:
	case GameCoreStates::DOUBLE_JUMP:
    {
      jump();
      break;
	}
	case GameCoreStates::FAST_ATTACK:
	{
	  fastAttack();
	  break;
	}
	case GameCoreStates::FALLING:
    {
      falling();
      break;
    }
	case GameCoreStates::STOPPING:
    {
      stopping();
      break;
    }
  }
}

void Characters::Player::stop()
{
  if ( !characterSprite->getPlayerMoveInX() && !characterSprite->getPlayerMoveInY() )
  {
    characterSprite->changeStateSprite(STILL_STATE, 0, getInputMapper()->getListKeys());
    getInputMapper()->pushBackStateOnMappedInput(GameCoreStates::STILL);
    characterSprite->changeCurrentFrame(GameCoreStates::STILL);
  }
}

void Characters::Player::returnToPreviousState()
{
  switch( characterSprite->getPreviousState() )
  {
    case GameCoreStates::STILL:
    {
	  characterSprite->changeStateSprite(STILL_STATE, 0, getInputMapper()->getListKeys());
	  break;
    }
    case GameCoreStates::WALKING:
    {
	  characterSprite->changeStateSprite(WALKING_STATE, 1, getInputMapper()->getListKeys());
	  break;
    }
	case GameCoreStates::RUNNING:
    {
      characterSprite->changeStateSprite(RUNNING_STATE, 1, getInputMapper()->getListKeys());
	  break;
    }
	case GameCoreStates::JUMPING:
    {
	  characterSprite->changeStateSprite(FALLING_STATE, 0, getInputMapper()->getListKeys());
	  break;
    }
  }

  getInputMapper()->pushBackStateOnMappedInput( GameCoreStates::SpriteState( characterSprite->getCurrentState() ) );
  characterSprite->changeCurrentFrame( characterSprite->getCurrentState() ); 
}

void Characters::Player::drawUIStats()
{
  stats->drawHealthBar();
  stats->drawHealth();
}

void Characters::Player::drawScore()
{
  score->drawDisplayPoints();
}

void Characters::Player::inputCallback(InputMapping::MappedInput& inputs, Player& player, std::list<InputMapping::Key> keys)
{
  Sprite* playerSprite = player.getCharacterSprite();

  playerSprite->setConstantSpeedX ( 
		        playerSprite->getHandlerAnimation()->changeAnimationDirection(inputs.directionKeyPressed) );

  bool findStillInStates = find(inputs.states.begin(), inputs.states.end(),GameCoreStates::STILL) 
		                   != inputs.states.end();
  bool findWalkingInStates = find(inputs.states.begin(), inputs.states.end(),GameCoreStates::WALKING) 
		                     != inputs.states.end();
  bool findJumpingInStates = find(inputs.states.begin(), inputs.states.end(),GameCoreStates::JUMPING) 
		                     != inputs.states.end();
  bool findRunningInStates = find(inputs.states.begin(), inputs.states.end(),GameCoreStates::RUNNING) 
		                     != inputs.states.end();
  bool findFastAttackWalkingInStates = find(inputs.states.begin(), inputs.states.end(), 
	                                   GameCoreStates::FAST_ATTACK) != inputs.states.end();

  InputMapping::Key checkKey = *std::find_if(keys.begin(), keys.end(), isJumpingKeyPressed);

  if ( findWalkingInStates && player.isReadyToPace() )
  {
    playerSprite->changeStateSprite(WALKING_STATE, inputs.buttonPreviouslyPressed, keys);
  }

  if ( findRunningInStates && player.isReadyToPace() )
  {
    playerSprite->changeStateSprite(RUNNING_STATE, inputs.buttonPreviouslyPressed, keys);
  }

  if ( findJumpingInStates )
  {
    playerSprite->changeStateSprite(JUMPING_STATE, checkKey.wasPreviouslyPressed, keys);

    if ( player.isReadyToDoubleJump() )
    {
      playerSprite->changeStateSprite(DOUBLE_JUMP_STATE, checkKey.wasPreviouslyPressed, keys);
    }

    if ( player.isFalling() )
    {
      playerSprite->changeStateSprite(FALLING_STATE, checkKey.wasPreviouslyPressed, keys);
    }
  }

  if ( player.isStoppingMovement(keys) )
  {
	playerSprite->changeStateSprite(STOPPING_STATE, inputs.buttonPreviouslyPressed, keys);
  }

  if ( findStillInStates )
  {
    playerSprite->changeStateSprite(STILL_STATE, inputs.buttonPreviouslyPressed, keys);
  }

  if ( findFastAttackWalkingInStates )
  {
    checkKey = *std::find_if(keys.begin(), keys.end(), isFastAttackKeyPressed);
    playerSprite->changeStateSprite(FAST_ATTACK_STATE, checkKey.wasPreviouslyPressed, keys);
  }
}
