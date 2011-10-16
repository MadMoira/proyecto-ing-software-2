#pragma once

#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>

class Camera{

public:

	Camera();
	~Camera();

	void initCamera();

	void moveCamera(GLfloat posx1);
	void moveCamera(GLfloat posx1, GLfloat posx2);
	
	void renderCamera();

	bool checkCam(GLfloat posx1);
	bool checkCam(GLfloat posx1, GLfloat posx2);

	void setCameraSpeed(GLfloat newSpeedx);
	void restartCameraSpeed();

	GLfloat getCameraPosition() { return posx; }

	GLfloat getCameraSpeed() {return speedx;};
	bool getOnePlayer() {return onePlayer;};

private:
	GLfloat posx;	
	GLfloat speedx;
	GLfloat midPoint;
	bool onePlayer;
	const SDL_VideoInfo* defaultResolution;
};