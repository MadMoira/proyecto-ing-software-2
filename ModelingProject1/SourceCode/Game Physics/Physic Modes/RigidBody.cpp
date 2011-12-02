
#include "RigidBody.h"

#include "SpriteDataConstants.h"
#include "PlayerSpriteStates.h"

GamePhysics::RigidBody::RigidBody(int mode) : PhysicsCore( mode )
{
  nameMode = (GamePhysics::PhysicModes)mode;
}

GamePhysics::RigidBody::~RigidBody(void)
{
  maxSpeed.clear();
}

void GamePhysics::RigidBody::initializeNaturalPhysicsForces(float forceOne, float forceTwo)
{
  gravityValue = forceOne;
  accelerationXValue = forceTwo;

  accState = GamePhysics::NO_ACCELERATE;
}

void GamePhysics::RigidBody::initializeSpeedVectors(std::vector< Vector2f > maxSpeed)
{
  this->maxSpeed = maxSpeed;
}

void GamePhysics::RigidBody::applyNaturalPhysicForces(int currentMovement, GLfloat* speedX, GLfloat* speedY, 
                                                      int playerState, int direction, int previousState)
{
  switch(currentMovement)
  {
    case GamePhysics::X:
    {
	  acceleratePlayer(speedX, playerState, previousState, direction);
      break;
    }
    case GamePhysics::Y:
    {
      parabolicShot(speedY, playerState);
      break;
    }
  }
}

GLfloat GamePhysics::RigidBody::getMomentumForce(GLfloat speedX, int axisDirection)
{
  GLfloat forceMomentum = 9.0f;
  if ( speedX*(axisDirection) - forceMomentum <= 0.0f )
  {
	return speedX;
  }

  return speedX - (axisDirection)*forceMomentum;
}

void GamePhysics::RigidBody::parabolicShot(GLfloat* yVelocity, int playerState)
{
  if( playerState == GameCoreStates::JUMPING || playerState == GameCoreStates::DOUBLE_JUMP || 
      playerState == GameCoreStates::FALLING )
  {
    *yVelocity -= gravityValue;
    if ( *yVelocity >= 30.0f )
    {
      *yVelocity = 30.0f;
    }
    return;
  }
  *yVelocity = 0.0f;
}

void GamePhysics::RigidBody::acceleratePlayer(GLfloat* xVelocity, int playerState, int previousState, int directionX)
{
  int axisDirection = 1;
  int indexState = playerState;

  if ( (playerState == GameCoreStates::JUMPING || playerState == GameCoreStates::DOUBLE_JUMP) &&
	   (previousState == GameCoreStates::WALKING || previousState == GameCoreStates::RUNNING) )
  {
    indexState = previousState;
  }

  if ( directionX == SpriteData::LEFT )
  {
	axisDirection = -1;
  }

  bool noAccelerationState = indexState != GameCoreStates::FALLING && indexState != GameCoreStates::FAST_ATTACK &&
	                        (playerState != GameCoreStates::DOUBLE_JUMP && previousState != GameCoreStates::JUMPING); 

  switch( accState )
  {
    case ACCELERATE:
    {
      if ( noAccelerationState )
      {
        *xVelocity += accelerationXValue*(axisDirection);
        if ( *xVelocity*(axisDirection) >= maxSpeed.at(indexState).x )
        {
	      *xVelocity = maxSpeed.at(indexState).x * axisDirection;
        }
      }
      break;
    }
    case DECELERATE:
    {
      if ( noAccelerationState )
      {
        *xVelocity -= accelerationXValue*(axisDirection);
        if ( *xVelocity*(axisDirection) <= 0.0f )
        {
	      *xVelocity = 0.0f;
        }
	  }				  
	  break;
	}
  }
}