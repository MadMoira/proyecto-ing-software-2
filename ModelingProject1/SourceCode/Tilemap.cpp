
#include "Tilemap.h"
#include <math.h>
Tilemap::Tilemap(std::string name, int widthInTiles, int heightInTiles)
{
	nameLayer = name;
	widthLevelInTiles = widthInTiles;
	heightLevelInTiles = heightInTiles;
	offsetX = offsetY = 0.0f;
	sizeTiles = 32.f;

	layerMap.resize(heightLevelInTiles);
	
	for (int i = 0; i < heightLevelInTiles; i++)
	{
		layerMap[i].resize(widthLevelInTiles);
	}
}

Tilemap::~Tilemap(void)
{
	layerMap.clear();
}

bool Tilemap::drawTilemap(int indexTileset)
{
	GLfloat offsetXTemp = offsetX;
	
	offsetXTemp = transformOffsetXToIntervalValues(offsetX);
	
	GLfloat variableSizeTile = 32.f;

	GLfloat widthTilesetImage = tilesetList.at(indexTileset).getWidthImage();
	GLfloat heightTilesetImage = tilesetList.at(indexTileset).getHeightImage();

	int widthMap = (1280 / 32) + 1;
	int heigthMap = (int) ceil( 720.0f / 32.0f );

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );	
	
	glBindTexture( GL_TEXTURE_2D, tilesetList.at(indexTileset).getTexture() );

	GLfloat posXPrevious = 0.0f, posXPreviousOnTexture = 0.0f;

	for (int i = 0; i < heigthMap; i++)
	{
		int startX = (int)floor(offsetX/sizeTiles);
		posXPrevious = 0.0f;

		posXPrevious -= offsetXTemp;
		variableSizeTile = 32.f;

		for (int j = 0; j < widthMap; j++) 
		{
			if ( startX == widthLevelInTiles )
			{
				break;
			}
			
			int frameIndex = layerMap[i][startX];

			if ( frameIndex == 0 )
			{ 
				startX++;
				variableSizeTile = 32.f;
				posXPrevious = posXPrevious + variableSizeTile;
				
				continue; 
			}

			if ( j == 0 && offsetXTemp != sizeTiles)
			{
				posXPreviousOnTexture = offsetXTemp/widthTilesetImage;
				variableSizeTile -= offsetXTemp;
				posXPrevious = 0.0f;
			}

			else 
			{ 
				variableSizeTile = 32.f; 
				posXPreviousOnTexture = 0.0f;
			}

			if ( j == 40 )
			{
				variableSizeTile = offsetXTemp;
			}

			frameIndex -= 1;

			const GLfloat tileX = posXPrevious;
			const GLfloat tileY = sizeTiles * i;
			posXPrevious = tileX + variableSizeTile;

			const GLfloat verts[] = {
					tileX, tileY,
					tileX + variableSizeTile, tileY,
					tileX + variableSizeTile, tileY + sizeTiles,
					tileX, tileY + sizeTiles
			};

			const GLfloat textureWidth = variableSizeTile / (GLfloat)widthTilesetImage;
			const GLfloat textureHeight = sizeTiles / (GLfloat)heightTilesetImage;
			const int numFramePerRow = (int)widthTilesetImage / (int)sizeTiles;
			const GLfloat textureX = ( (frameIndex % numFramePerRow) * sizeTiles/(GLfloat)widthTilesetImage ) 
									+ posXPreviousOnTexture;
			const GLfloat textureY = ( frameIndex / numFramePerRow ) * textureHeight;

			const GLfloat texVerts[] = {
					textureX, textureY,
					textureX + textureWidth, textureY,
					textureX + textureWidth, textureY + textureHeight,
					textureX, textureY + textureHeight
			};
			
			glVertexPointer(2, GL_FLOAT, 0, verts);
			glTexCoordPointer(2, GL_FLOAT, 0, texVerts);
			glDrawArrays(GL_QUADS, 0, 4);

			startX++;
		}	
	}

	glDisableClientState( GL_VERTEX_ARRAY );			
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	return true;
}

GLfloat Tilemap::transformOffsetXToIntervalValues(GLfloat offX)
{
	int modForTextureCoordinates = sizeTiles* ( floor(offX/sizeTiles) );

	if (offX > sizeTiles*2 ) 
	{ 
		return offX - modForTextureCoordinates; 
	}

	else if ( offX > sizeTiles && offX <= sizeTiles*2 )
	{
		return offX - sizeTiles;
	} 

	return offX;
}

bool Tilemap::scrollTilemap()
{
	offsetX += speedX;
	offsetY += speedY;

	return checkScreenBoundaries();
}

bool Tilemap::checkScreenBoundaries()
{
	if (offsetX > widthLevelInTiles*sizeTiles - 1280.f)
	{
		offsetX = widthLevelInTiles*sizeTiles - 1280.f; 
		return true;
	}

	return false;
}

void Tilemap::addTileset(int id, std::string name, GLfloat widthTile, GLfloat heightTile, GLfloat imageWidth, GLfloat imageHeight, 
			int size)
{
	tilesetList.push_back( new Tileset(id, name, widthTile, heightTile, imageWidth, imageHeight, size, GameRender::loadTexture(name)) );
}