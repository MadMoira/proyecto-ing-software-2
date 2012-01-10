
#include "MeerkatP2.h"

#include <Keyboard.h>
#include <Gamepad.h>

Characters::MeerkatP2::~MeerkatP2()
{
  delete characterSprite;
  delete inputMapper;
  delete controller;
  delete stats;
  delete score;
}

void Characters::MeerkatP2::initializeCharacter(SpriteData::IDSprites id, std::string filename,
                Vector2f pos, int initialFrame, std::vector < int > maxFrame, 
                std::vector < int > returnFrame, GLfloat widthSprite, GLfloat heightSprite,
                std::vector < int > framerateAnimations, std::vector< Vector2f> delayMovement)
{
  characterSprite = new PlayerSprite(id, filename, pos, initialFrame, maxFrame, returnFrame,
                            widthSprite, heightSprite, framerateAnimations, delayMovement);

  stats = new PlayerStats::Stats();
  score = new PlayerScore::Score();

  /*inputMapper = new InputMapping::GameInputMapper("Resources/Input/KeyboardContextList.txt");
  //inputMapper = new InputMapping::GameInputMapper("Resources/Input/GamepadContextList.txt");

  //inputMapper->pushContext("gamepadcontext");
  inputMapper->pushContext("keyboardcontext");
  inputMapper->addCallback(Player::inputCallback, 0);

  //controller = new InputMapping::Gamepad(0);
  controller = new InputMapping::Keyboard(0);
  controller->initializeKeys(inputMapper->getListKeys(), inputMapper->getStateMap(), inputMapper->getActionMap());*/
}

void Characters::MeerkatP2::noAction()
{
  characterSprite->setPlayerMoveInX(false);
  characterSprite->setPlayerMoveInY(false);
  characterSprite->setPlayerMoveInXCurrentFrame(false);
  stop();	
}

void Characters::MeerkatP2::walk()
{
  characterSprite->movePosXWithSpeed();
  characterSprite->getHandlerAnimation()->animate();
  characterSprite->setPlayerMoveInY(false);
  stop();
}

void Characters::MeerkatP2::run()
{
  characterSprite->movePosXWithSpeed();
  characterSprite->getHandlerAnimation()->animate();
  characterSprite->setPlayerMoveInY(false);
  stop();
}

void Characters::MeerkatP2::jump()
{
  characterSprite->setPlayerMoveInY(true);
  characterSprite->movePosXWithSpeed();
  characterSprite->movePosYWithSpeed();
  characterSprite->getHandlerAnimation()->animate();
  stop();
}

void Characters::MeerkatP2::fastAttack()
{
  characterSprite->setPlayerMoveInY(true);
  characterSprite->movePosXWithSpeed();
  characterSprite->movePosYWithSpeed();

  if ( characterSprite->getHandlerAnimation()->getAnimationAlreadyEnd() )
  {
    returnToPreviousState();
  }
  else
  {
    characterSprite->getHandlerAnimation()->animate(); 
  }
}

void Characters::MeerkatP2::falling()
{
  characterSprite->setPlayerMoveInY(true);
  characterSprite->movePosXWithSpeed();
  characterSprite->movePosYWithSpeed();
  characterSprite->getHandlerAnimation()->animate();

  if ( !isFalling() )
  {
    stop();
  }
}

void Characters::MeerkatP2::draw()
{
  characterSprite->drawTexture();
}