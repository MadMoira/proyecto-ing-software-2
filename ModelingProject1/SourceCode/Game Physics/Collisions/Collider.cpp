
#include <algorithm>
#include "PlayerSpriteStates.h"

#include "Collider.h"

#include "PhysicModes.h"
#include <MathFunctions.h>
#include <MathConstants.h>

Collider::Collider()
{
}

bool Collider::instanceFlag = false;
Collider* Collider::collider = NULL;

Collider* Collider::getInstance()
{
  if( !instanceFlag )
  {
    collider = new Collider();
    instanceFlag = true;
  }
  return collider;
}

void Collider::initializeColliderSprites(boost::ptr_vector< Characters::Enemy >* enemiesList, 
                                         boost::ptr_vector< Characters::Player >* playersList)
{
  enemies = enemiesList;
  players = playersList;
}

void Collider::addLayerTilemap(std::vector< std::vector < Tile > > layer)
{
  layers.push_back(layer);
}

void Collider::cleanUpResources()
{
  for (std::string::size_type i = 0; i < layers.size(); i++)
  {
    layers.at(i).clear();
  }
}

bool Collider::checkCollision(CollisionSystem::CollisionBox& A, CollisionSystem::CollisionBox& B, float directionX)
{
  if( ( ( ((A.getY() + A.getHeight()) > B.getY()) && 
        ( (A.getX() + A.getWidth()) > (B.getX()) ) && 
        ( A.getX() < (B.getX() + B.getWidth()))) || 
        (( (A.getX() + A.getWidth()) > (B.getX()) ) && 
        ( A.getX() < (B.getX() + B.getWidth())) && directionX > 0.0f) || 
        (( (A.getX() + A.getWidth()) > (B.getX()) ) && 
        ( A.getX() < (B.getX() + B.getWidth())) && directionX < 0.0f)))
  {
    return true;
  }

  return false;
}

void Collider::checkTileCollisionX(CollisionSystem::CollisionBox& A, GLfloat* speedX, int directionX, 
                                   CollisionSystem::DirectionsMove& directionsMove)
{
  directionsMove.setCanMoveRight(true);
  directionsMove.setCanMoveLeft(true);
  directionsMove.setCanMoveUp(true);
  directionsMove.setCanMoveDown(true);

  int sign = Math::LESS_OR_EQUAL;
  int positionToCheck, leftCondition, rightCondition;
  int axisX = 1;
  GLfloat newSpeed;

  int initialY = (int)A.getY();
  int height = (int)A.getHeight();

  if ( directionX == SpriteData::LEFT )
  {
	axisX = -1;
  }

  for (std::string::size_type indexLayer = 0; indexLayer < layers.size(); indexLayer++)
  {
    newSpeed = CollisionSystem::initialCheckingBox;
    positionToCheck = (int)A.getX();
    leftCondition = positionToCheck;
    sign = Math::LESS_OR_EQUAL;
	rightCondition = 0;

	rightCondition = positionCollisionBoxAxisX(A, directionX, newSpeed, rightCondition);

	for ( positionToCheck; comparatorValuesInt(sign, leftCondition, rightCondition); positionToCheck += 1*(axisX) )
	{
	  int i = positionToCheck;
	  int newRightCondition = i;

	  newRightCondition = positionCollisionBoxAxisX(A, directionX, newSpeed, newRightCondition);

	  for ( i; comparatorValuesInt(0, i, newRightCondition); i += 1 )
	  {
        int previousTileX = 0;
	    int previousTileY = 0;

		for ( int j = initialY; j < initialY + height; j += 2 )
	    {
          int x = (int)i/32;
		  int y = (int)j/32;

		  if ( x == previousTileX && y == previousTileY )
		  {
			continue;
	      }

		  previousTileX = x;
		  previousTileY = y;

		  if ( x > 6400.0f/32 || y > 720.0f/32 )
		  {
			return;
		  }

		  Tile foundTile = layers.at(indexLayer)[y][x];

	      if ( foundTile.getID() == 0 )
		  {
			continue;
		  }
 
		  if ( foundTile.getHasCollision() )
		  {
			CollisionSystem::CollisionBox* temporalBox = new CollisionSystem::CollisionBox();
			*temporalBox = A;

			if ( newSpeed != CollisionSystem::initialCheckingBox )
		    {
			  temporalBox = new CollisionSystem::CollisionBox((float)leftCondition, A.getY(), A.getWidth(), 
				                                               A.getHeight(), A.getOffset());
		    }
			
			checkBottomBoxCollision(*temporalBox, directionsMove, directionX, x, y);
			checkBodyBoxCollision(*temporalBox, directionsMove, directionX, y);

            delete temporalBox;

			if ( (newSpeed == CollisionSystem::initialCheckingBox) && 
				  ( (!directionsMove.canMoveXRight && !directionsMove.canMoveXLeft) ||
					(!directionsMove.canMoveYUp && !directionsMove.canMoveYDown) ) )
			{
			  *speedX = newSpeed;
			  return;
		    }

			if ( directionsMove.canMoveXRight && directionsMove.canMoveXLeft &&
				 directionsMove.canMoveYUp && directionsMove.canMoveYDown )
			{
			  if ( newSpeed != CollisionSystem::initialCheckingBox )
			  {
				*speedX = newSpeed;
			  }
              return;
			}
           
			if ( directionX == SpriteData::RIGHT )
			{
			  axisX = 0;
			}

			*speedX = newSpeed + -1*(axisX);
            return;
          }
        } 
      }

	  if (newSpeed == CollisionSystem::initialCheckingBox)
	  {
	    switch(directionX)
		{
		  case SpriteData::RIGHT:
		  {
		    positionToCheck = (int)A.getX() + (int)A.getWidth();
			break;
		  }
		  case SpriteData::LEFT:
		  {
			positionToCheck = (int)A.getX();
			sign = Math::GREATER_OR_EQUAL;
			break;
		  }
		}
		rightCondition = positionToCheck + (int)(*speedX);
	  }

	  newSpeed += 1*(axisX);
	  leftCondition = positionToCheck;
	}
  }
}

void Collider::checkTileCollisionY(CollisionSystem::CollisionBox& A, GLfloat* speedY, int directionY, 
                                   CollisionSystem::DirectionsMove& directionsMove)
{
  directionsMove.setCanMoveRight(true);
  directionsMove.setCanMoveLeft(true);
  directionsMove.setCanMoveUp(true);
  directionsMove.setCanMoveDown(true);

  int axisY = 1;
  int positionToCheck, leftCondition, rightCondition;
  int sign = Math::LESS_THAN;
  GLfloat newSpeed;

  int initialX = (int)A.getX();
  int width = (int)A.getWidth();

  if ( directionY == SpriteData::UP )
  {
    axisY = -1;
  }

  for (std::string::size_type indexLayer = 0; indexLayer < layers.size(); indexLayer++)
  {
    newSpeed = CollisionSystem::initialCheckingBox;
    positionToCheck = (int)A.getY();
	leftCondition = positionToCheck;
	sign = Math::LESS_THAN;
	rightCondition = 0;

	rightCondition = positionCollisionBoxAxisY(A, directionY, newSpeed, rightCondition);

	for ( positionToCheck; comparatorValuesInt(sign, leftCondition, rightCondition); positionToCheck += 1*(axisY) )
	{
	  int i = positionToCheck;
	  int newRightCondition = i;
      newRightCondition = positionCollisionBoxAxisY(A, directionY, newSpeed, newRightCondition);

	  for ( i; comparatorValuesInt(Math::LESS_OR_EQUAL, i, newRightCondition); i += 1 )
	  {
        int previousTileX = 0;
	    int previousTileY = 0;
		
		for ( int j = initialX; j < initialX + width; j += 2 )
	    {
          int x = (int)j/32;
		  int y = (int)i/32;

		  if ( x == previousTileX && y == previousTileY )
		  {
		    continue;
		  }

		  previousTileX = x;
		  previousTileY = y;

		  if ( x > 6400.0f/32 || y > 720.0f/32 )
		  {
			return;
		  }

		  Tile foundTile = layers.at(indexLayer)[y][x];

		  if ( foundTile.getID() == 0 )
		  {
		    continue;
		  }
 
		  if ( foundTile.getHasCollision() )
	      {
		    CollisionSystem::CollisionBox* temporalBox = new CollisionSystem::CollisionBox();
			*temporalBox = A;

		    if ( newSpeed != CollisionSystem::initialCheckingBox )
			{
			  if ( directionY == SpriteData::UP )
			  {
                temporalBox = new CollisionSystem::CollisionBox(A.getX(), (float)positionToCheck, A.getWidth(), 
				                                                A.getHeight(), A.getOffset());
			  }
			  else
			  {
			    temporalBox = new CollisionSystem::CollisionBox(A.getX(), (float)positionToCheck - A.getHeight(), A.getWidth(),
				                                              A.getHeight(), A.getOffset());
			  }
		    }

			if ( checkBottomBoxMovementY(directionsMove, newRightCondition, y, 
				                    (int)(temporalBox->getY() + temporalBox->getHeight())/32, foundTile.getIsWalkable()) )
			{
			  return;
			}
	        checkTopBoxCollision(directionsMove, (int)temporalBox->getY()/32, directionY, y);

		    delete temporalBox;

			if ( !directionsMove.canMoveYDown )
			{
			  if ( newSpeed == CollisionSystem::initialCheckingBox )
			  {
			    *speedY = newSpeed;
				return;
			  }

			  if ( directionY == SpriteData::DOWN )
			  {
				axisY = 0;
			  }
			  *speedY = newSpeed + -1*(axisY);
			}

			return;
	      }
	    }
	  }

	  if (newSpeed == CollisionSystem::initialCheckingBox)
	  {
	    switch(directionY)
		{
		  case SpriteData::DOWN:
		  {
		    positionToCheck = (int)A.getY() + (int)A.getHeight();
			break;
		  }
		  case SpriteData::UP:
		  {
			positionToCheck = (int)A.getY();
			sign = Math::GREATER_OR_EQUAL;
			break;
		  }
		}
		rightCondition = positionToCheck + (int)(*speedY);
	  }  

	  newSpeed += 1*(axisY);
	  leftCondition = positionToCheck;
	}
  }
}

int Collider::positionCollisionBoxAxisX(CollisionSystem::CollisionBox& A, int directionX, 
	                                    GLfloat newSpeed, int rightCondition)
{
  if ( newSpeed == CollisionSystem::initialCheckingBox )
  {
    if ( directionX == SpriteData::RIGHT )
    {
      return (int)A.getX() + (int)A.getWidth();
    }
    else if ( directionX == SpriteData::LEFT )
    {
      return (int)A.getX() + (int)(A.getWidth()/2);
    }
  }

  return rightCondition;
}

int Collider::positionCollisionBoxAxisY(CollisionSystem::CollisionBox& A, int directionY, 
	                                    GLfloat newSpeed, int rightCondition)
{
  if ( newSpeed == CollisionSystem::initialCheckingBox )
  {
    if ( directionY == SpriteData::UP )
    {
      return (int)A.getY() + (int)A.getHeight()/2;
    }
    else if ( directionY == SpriteData::DOWN )
    {
      return (int)A.getY() + (int)A.getHeight();
    }
  }

  return rightCondition;
}

void Collider::checkBottomBoxCollision(CollisionSystem::CollisionBox& A, CollisionSystem::DirectionsMove& directionsMove, 
                                       int directionX, int currentPositionX, int currentPositionY)
{
  int bottomY = ( (int)A.getY() + (int)A.getHeight() ) / 32;

  if ( currentPositionY == bottomY )
  {
    switch(directionX)
    {
      case SpriteData::RIGHT:
      {
        if ( currentPositionX >= (int)(A.getX() + A.getWidth())/32 )
        {
          directionsMove.setCanMoveLeft(true);
          directionsMove.setCanMoveRight(false);
        }
        break;
      }
      case SpriteData::LEFT:
      {
        if ( currentPositionX <= (int)(A.getX())/32 )
        {
          directionsMove.setCanMoveLeft(false);
          directionsMove.setCanMoveRight(true);
        }
        break;
      }
    }
  }
}

bool Collider::checkBottomBoxMovementY(CollisionSystem::DirectionsMove& directionsMove, int newRightDirection, 
	                                   int currentY, int boxXtreme, bool tileIsWalkable)
{
  if ( (int)(newRightDirection/32) == currentY && currentY == boxXtreme )
  {
    if ( !tileIsWalkable ) 
	{
	  directionsMove.setCanMoveDown(true);
	  return true;
	}
	else
	{
      directionsMove.setCanMoveDown(false);
	}							
  }

  return false;
}

void Collider::checkTopBoxCollision(CollisionSystem::DirectionsMove& directionsMove, int topY, int directionY,
                                    int currentPositionY)
{
  if ( currentPositionY == topY )
  {
    if ( directionY == SpriteData::UP || directionY == SpriteData::DOWN )
    {
      directionsMove.setCanMoveUp(false);
      directionsMove.setCanMoveDown(true);
    }
    else 
    {
      directionsMove.setCanMoveUp(true);
      directionsMove.setCanMoveDown(false);
    }
  }
}

void Collider::checkBodyBoxCollision(CollisionSystem::CollisionBox& A, CollisionSystem::DirectionsMove& directionsMove, 
                                     int directionX, int currentPositionY)
{
  if ( directionX == SpriteData::RIGHT && 
       currentPositionY != ( (int)A.getY() + (int)A.getHeight() ) / 32 )
  {
    directionsMove.setCanMoveRight(false);
    directionsMove.setCanMoveLeft(true);
  }

  else if ( directionX == SpriteData::LEFT && 
            currentPositionY != ( (int)A.getY() + (int)A.getHeight() ) / 32 )
  {
    directionsMove.setCanMoveRight(true);
    directionsMove.setCanMoveLeft(false);
  }
}

void Collider::checkStateCollisionPlayer(Sprite& playerSprite)
{
  if ( !playerSprite.getDirectionsMove().canMoveYUp && !playerSprite.getIsOnGround() && 
        playerSprite.getCurrentState() != GameCoreStates::FALLING )
  {
    playerSprite.changeStateSprite(new GameCoreStates::FallingState(GameCoreStates::FALLING), 0, 
                                         std::list<InputMapping::Key>() );
    playerSprite.setSpeedX(0.0f);
    playerSprite.setSpeedY(4.0f);
    playerSprite.getHandlerAnimation()->changeDirectionY(4.0f);
    return;
  }

  if ( playerSprite.getIsOnGround() && ( playerSprite.getCurrentState() == GameCoreStates::FALLING || (
	                                     playerSprite.getCurrentState() == GameCoreStates::JUMPING && playerSprite.getSpeedY() >= -10.0f) ) )
  {
    checkStatePhysicsModes(playerSprite);
    
    playerSprite.setPlayerCanMoveYUp(true);
    playerSprite.setPlayerCanMoveYDown(false);
    playerSprite.setPlayerMoveInY(false);
    return;
  }

  if ( !playerSprite.getIsOnGround() && !playerSprite.isPlayerOnTheAir() )
  {
    playerSprite.changeStateSprite(new GameCoreStates::FallingState(GameCoreStates::FALLING), 0, 
                                         std::list<InputMapping::Key>() );
    return;
  }
}

void Collider::checkStateCollisionXAxis(Sprite& playerSprite)
{
  checkStateCollisionPlayer(playerSprite);

  if ( !playerSprite.getPlayerMoveBasedInDirection() && !playerSprite.getIsOnGround() && 
        playerSprite.getCurrentState() == GameCoreStates::FALLING )
  {                        
    playerSprite.changePreviousPlayerState(GameCoreStates::STILL);
    playerSprite.changeStateSprite(new GameCoreStates::FallingState(GameCoreStates::FALLING), 0, 
                                       std::list<InputMapping::Key>() );
  }

  if ( !playerSprite.getPlayerMoveBasedInDirection() && playerSprite.getIsOnGround() && 
        playerSprite.getCurrentState() != GameCoreStates::STILL )
  {
    playerSprite.changeStateSprite(new GameCoreStates::StillState(GameCoreStates::STILL), 0, 
                                         std::list<InputMapping::Key>() );

	playerSprite.setNormalPositionX( playerSprite.getSpeedX() );
	playerSprite.setSpeedX(0.0f);
	playerSprite.setPlayerMoveInX(false);
    playerSprite.setPlayerMoveInY(false);
  }

  int animationDirection = playerSprite.getHandlerAnimation()->getAnimationDirection();

  playerSprite.setNormalPositionX( playerSprite.getSpeedX() );
  playerSprite.getCollisionBox()->setX(playerSprite.getPosX() + 
	                                   playerSprite.getCollisionBox()->getOffsetXBasedOnDirection(animationDirection), 
		                               animationDirection);
}

void Collider::checkStatePhysicsModes(Sprite& playerSprite)
{
  int directionAxis = playerSprite.getHandlerAnimation()->returnAnimationDirectionAxisValue();
  std::vector< Vector2f > tempMaxSpeed = playerSprite.getRigidBody().getMaxSpeed();

  if ( playerSprite.getSpeedX()*directionAxis > 0.0f && 
	   playerSprite.getSpeedX()*directionAxis <= tempMaxSpeed.at(GameCoreStates::WALKING).x )
  {
    playerSprite.changeStateSprite(new GameCoreStates::WalkingState(GameCoreStates::WALKING), 0, 
                                   std::list<InputMapping::Key>() );
    playerSprite.setPlayerMoveInX(true);
	return;
  }

  else if ( playerSprite.getSpeedX()*directionAxis > tempMaxSpeed.at(GameCoreStates::WALKING).x && 
	        playerSprite.getSpeedX()*directionAxis <= tempMaxSpeed.at(GameCoreStates::RUNNING).x )
  {
    playerSprite.changeStateSprite(new GameCoreStates::RunningState(GameCoreStates::RUNNING, playerSprite.getID()), 
		                           GamePhysics::TO_WALKING, 
                                   std::list<InputMapping::Key>() );
    playerSprite.setPlayerMoveInX(true);
	return;
  }

  playerSprite.changeStateSprite(new GameCoreStates::StillState(GameCoreStates::STILL), 0, 
                                     std::list<InputMapping::Key>() );
  playerSprite.setSpeedX(0.0f);
  playerSprite.setPlayerMoveInX(false);
}

bool Collider::checkEnemiesCollision(CollisionSystem::CollisionBox& A, float directionX)
{
 /* for(int i = 0; i == enemies->size(); i++)
  {
    if( checkCollision(A, *enemies->at(i).getSprite()->getCollisionBox(), directionX) )
    {
      return true;
    }
  }
  */
  return false;
}
/*
boost::ptr_vector< Characters::Enemy > Collider::checkAttackCollision(CollisionBox& A, float directionX)
{
  boost::ptr_vector< Characters::Enemy > enemiesCollided;

  for(int i = 0; i == enemies->size(); i++)
  {
    if( checkCollision( A, *enemies->at(i).getSprite()->getCollisionBox(), directionX) )
    {
      enemiesCollided.push_back(&enemies->at(i));
    }
  }
  
  return enemiesCollided;
}*/

/*boost::ptr_vector< Characters::Player > checkEnemiesAttackCollision(CollisionBox& A, float directionX)
{
  boost::ptr_vector< Characters::Player > playersCollided;
  for(int i = 0; i == players->size(); i++)
  {
    if( checkCollision(A, *players->at(i).getCharacterSprite()->getCollisionBox(), directionX) )
    {
      playersCollided.push_back(&players->at(i));
    }
  }
  return playersCollided;
}*/

bool Collider::onTheGround(CollisionSystem::CollisionBox& A, int directionX, int directionY)
{
  int directionPlayer = SpriteData::RIGHT;
  int initialX = 0;

  initialX = (int)A.getX();

  int positionY = ( (int)A.getY() + (int)A.getHeight() )/32;

  if ( initialX < 0 )
  {
    initialX = 0;
  }

  bool isOnGround = false;

  for (std::string::size_type indexLayer = 0; indexLayer < layers.size(); indexLayer++)
  {
    if ( isOnGround )
    {
      break;
    }

    isOnGround = false;

    for(int i = initialX; i < initialX + (int)A.getWidth(); i += 8)
    {
      int x = (int)i/32;
      if ( positionY > 720.0f/32 )
      {
        return false;
      }

      Tile groundTile = layers.at(indexLayer)[positionY][x];

      if( !groundTile.getHasCollision() || (groundTile.getHasCollision() && !groundTile.getIsWalkable()) )
      {
        isOnGround = false;
      }
      else
      {
        isOnGround = true;
        break;
      }
    }
  }

  return isOnGround;
}

bool Collider::checkPositionWithinLevelLength(CollisionSystem::CollisionBox& A, CollisionSystem::DirectionsMove& directionsMove, 
	                                   Vector2f speed, int directionX)
{
  if ( directionX == SpriteData::RIGHT )
  {
	if ( A.getX() + speed.x + A.getWidth() > levelLength )
	{
      directionsMove.setCanMoveRight(false);
      return false;
	}
  }
  else
  {
    if ( A.getX() + speed.x < 0.0f )
    {
	  directionsMove.setCanMoveLeft(false);
	  return false;
	}
  }
  return true;
}