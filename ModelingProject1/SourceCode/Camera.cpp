
#include "Camera.h"

Camera::Camera()
{
  posx = 0;
  speedx = 0;	
  midPoint = 0;
  onePlayer = true;	
}

Camera::~Camera()
{
}

void Camera::initCamera()
{
  defaultResolution = SDL_GetVideoInfo();
  midPoint = (GLfloat)defaultResolution->current_w/2;
}

void Camera::moveCamera(GLfloat posx1)
{
  if ( checkCam(posx1) )
  {
    posx += speedx;
    midPoint += speedx;
  }

  else
  {
    speedx = 0;
  }
}

void Camera::moveCamera(GLfloat posx1, GLfloat posx2)
{
  if(checkCam(posx1, posx2))
  {
    posx += speedx;
    midPoint += speedx;
  }

  else
  {
    speedx = 0;
  }
}

void Camera::renderCamera()
{
  glTranslatef(-posx, 0, 0);
}

void Camera::setCameraSpeed(GLfloat newSpeedx)
{
  if( onePlayer )
  {
    speedx += newSpeedx;
  }

  else
  {
    speedx += newSpeedx/2;
  }
}

void Camera::restartCameraSpeed()
{
  speedx = 0;
}

bool Camera::checkCam(GLfloat posx1)
{
  if( (posx1 - midPoint > 0 && posx < 4600) || posx - posx1 > -75)
  {
    return true;
  }	

  return false;
}

bool Camera::checkCam(GLfloat posx1, GLfloat posx2)
{
  if( ((posx1- midPoint > 0 || posx2 - midPoint > 0) && posx < 4600) || (posx - posx1 > -75 || posx - posx2 > -75) )
  {
    return true;
  }	

  return false;
}
