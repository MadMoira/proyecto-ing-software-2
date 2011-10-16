
#include "GameRender.h"

#include <math.h>

GameRender::GameRender(void)
{
}

GameRender::~GameRender(void)
{
}

GLuint GameRender::loadTexture(std::string name)
{
	SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load( name.c_str() );

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );

        SDL_FreeSurface( loadedImage );
    }
	
	GLuint texture;
	
	if( optimizedImage != NULL )
    {
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		glTexImage2D(   GL_TEXTURE_2D,
                                    0, 
                                    4, 
                    optimizedImage->w, 
                    optimizedImage->h,
                                    0, 
							  GL_BGRA,
                     GL_UNSIGNED_BYTE, 
               optimizedImage->pixels);

		
		SDL_FreeSurface(optimizedImage);

        return texture;
     }

     return NULL;
}

void GameRender::drawFullTexture(GLuint texture, Vector2f pos, GLfloat widthTexture, GLfloat heightTexture)
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );	
	
	glBindTexture( GL_TEXTURE_2D, texture );

	const GLfloat verts[] = {
				pos.x, pos.y,
				pos.x + widthTexture, pos.y,
				pos.x + widthTexture, pos.y + heightTexture,
				pos.x, pos.y + heightTexture
	};

	const GLfloat textureX = 0.0f;
	const GLfloat textureY = 0.0f;
	const GLfloat textureWidth = 1.0f;
	const GLfloat textureHeight = 1.0f;

	const GLfloat texVerts[] = {
			textureX, textureY,
			textureWidth, textureY,
			textureWidth, textureHeight,
			textureX, textureHeight
	};
			
	glVertexPointer(2, GL_FLOAT, 0, verts);
	glTexCoordPointer(2, GL_FLOAT, 0, texVerts);
	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState( GL_VERTEX_ARRAY );			
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

void GameRender::drawSpriteTexture(GLuint texture, Vector2f pos, int currentFrame, 
								GLfloat widthTexture, GLfloat heightTexture, GLfloat widthSprite, GLfloat heightSprite,
								int direction, int state)
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );	
	
	glBindTexture( GL_TEXTURE_2D, texture );

	GLfloat verts[] = {
				pos.x, pos.y,
				pos.x + widthSprite, pos.y,
				pos.x + widthSprite, pos.y + heightSprite,
				pos.x, pos.y + heightSprite
	};

	if ( direction == 1 )
	{
		verts[0] = verts[6] = pos.x + widthSprite;
		verts[2] = verts[4] = pos.x;
	}
	
	const GLfloat textureWidth = widthSprite / widthTexture;
	const GLfloat textureHeight = heightSprite / heightTexture;
	const int numFramePerRow = (int) widthTexture / (int) widthSprite;

	if ( state != 0 )
	{
		currentFrame += ( (state - 1) * numFramePerRow); 
	}

	const GLfloat textureX = ( currentFrame % numFramePerRow ) * textureWidth;
	const GLfloat textureY = ( currentFrame / numFramePerRow ) * textureHeight;

	GLfloat texVerts[] = {
			textureX, textureY,
			textureX + textureWidth, textureY,
			textureX + textureWidth, textureY + textureHeight,
			textureX, textureY + textureHeight
	};

	glVertexPointer(2, GL_FLOAT, 0, verts);
	glTexCoordPointer(2, GL_FLOAT, 0, texVerts);
	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState( GL_VERTEX_ARRAY );			
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

void GameRender::drawLayerTexture(GLuint texture, Vector2f offset, GLfloat widthScreen, 
								GLfloat heightScreen)
{
	GLfloat widthTexture, heightTexture;
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );	
	
	glBindTexture( GL_TEXTURE_2D, texture );

	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &widthTexture);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &heightTexture);

	const GLfloat vertX = 0.0f;
	const GLfloat vertY = 0.0f;
	
	const GLfloat verts[] = {
			vertX, vertY,
			vertX + widthScreen, vertY,
			vertX + widthScreen, vertY + heightScreen,
			vertX, vertY + heightScreen
	};

	const GLfloat textureX = offset.x/widthTexture;
	const GLfloat textureY = 0.0f;
	const GLfloat textureWidth = (widthScreen + offset.x) / widthTexture;
	const GLfloat textureHeight = (heightScreen + offset.y) / heightTexture;

	const GLfloat texVerts[] = {
			textureX, textureY,
			textureWidth, textureY,
			textureWidth, textureHeight,
			textureX, textureHeight
	};
			
	glVertexPointer(2, GL_FLOAT, 0, verts);
	glTexCoordPointer(2, GL_FLOAT, 0, texVerts);
	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState( GL_VERTEX_ARRAY );			
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );	
}

int round(double x)
{
	return (int)(x + 0.5);
}

int nextpoweroftwo(int x)
{
	double logbase2 = log(double(x)) / log(double(2));
	return round(pow(2,ceil(logbase2)));
}

void GameRender::drawText(Font::GameFont* font, Text::GameText text)
{
  SDL_Surface *initial;
  SDL_Surface *intermediary;
  SDL_Rect rect;
  int w, h;
  GLuint texture;
	
  rect.h = rect.w = rect.x = rect.y = 0;

	GLfloat x = text.getPosition().x;
	GLfloat y = text.getPosition().y;

	
	/* Use SDL_TTF to render our text */
	initial = TTF_RenderText_Blended(font->getFont(), 
	                                 text.getDataText().c_str(), 
									 font->getColor());
	
	w = nextpoweroftwo(initial->w);
	h = nextpoweroftwo(initial->h);

	/* Convert the rendered text to a known format */
	text.setOffset( Vector2f( GLfloat(nextpoweroftwo(initial->w)) ,
		GLfloat(nextpoweroftwo(initial->h)) ) );


	GLfloat offsetX = text.getOffset().x + x;
	GLfloat offsetY = text.getOffset().y + y;
	
	intermediary = SDL_CreateRGBSurface(0, w, h, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	SDL_SetAlpha(intermediary, 0, 0);
	SDL_BlitSurface(initial, 0, intermediary, 0);
	
	/* Tell GL about our new texture */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, intermediary->pixels );
	
	/* GL_NEAREST looks horrible, if scaled... */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

	glBlendFunc(GL_ONE, GL_ONE);

	glBindTexture( GL_TEXTURE_2D, texture );
	glBegin( GL_QUADS );
				//Top-left vertex (corner)
				glTexCoord2i( 0, 0 );
				glVertex3f( x, y, 0.f );

				//Bottom-left vertex (corner)
				glTexCoord2i( 1, 0 );
				glVertex3f( offsetX, y, 0.f );
			 
				//Bottom-right vertex (corner)
				glTexCoord2i( 1, 1 );
				glVertex3f( offsetX, offsetY, 0.f );
			 
				//Top-right vertex (corner)
				glTexCoord2i( 0, 1 );
				glVertex3f( x,  offsetY, 0.f );
	glEnd();

	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);
	glDeleteTextures(1, &texture);
}
