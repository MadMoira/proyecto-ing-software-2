#pragma once

#include "Player.h"

class MeerkatP2 : public Player
{
public:
	
	MeerkatP2::MeerkatP2();
	~MeerkatP2();

	void initializePlayer(IDSprites id, std::string filename, std::vector<GLfloat> speedX, GLfloat speedY, GLfloat posX,
				GLfloat posY, int initialFrame, std::vector < int > maxFrame, std::vector < int > returnFrame, 
				IDSpriteStates state, GLfloat widthSprite, GLfloat heightSprite);
	void walk();
	void run();
	void jump(){};
	void draw();
	void executeAction();
	
};

