#include "BR5Model.h"
#include "TextureLoader.h"

using namespace rendering;
using namespace std;

namespace loaders
{
	CBR5Model::CBR5Model(): 
		vbo(NULL),
		fileName(""), 
		vertexCount(0), 
		framesCount(0), 
		currentFrame(0),
		startFrame(0),
		endFrame(0),
		texture(0),
		actionsCount(0),
		textureCoordinates(NULL),
		vertexCoordinates(NULL),
		vertexCoordinatesAnim(NULL),
		animNextFrame(0.0f),
		animSpeed(1.0f),
		interpolate(false),
		update(true),
		connected(false),
		copy(false)
		//normalCoordinates(NULL)
	{
	}

	CBR5Model::CBR5Model(const CBR5Model &br5Model)
	{
		//vbo = br5Model.vbo

		fileName = br5Model.fileName;
		memcpy(textureName,br5Model.textureName,256);
		vertexCount = br5Model.vertexCount;
		framesCount = br5Model.framesCount;
		currentFrame = br5Model.currentFrame;
		startFrame = br5Model.startFrame;
		endFrame = br5Model.endFrame;
		texture = br5Model.texture;
		boundingBox = br5Model.boundingBox;

		actionsCount = br5Model.actionsCount;
		for (int ac=0; ac<actionsCount; ac++)
		{
			actions[ac] = br5Model.actions[ac];
		}

		textureCoordinates = br5Model.textureCoordinates;
		//vertexCoordinates = br5Model.vertexCoordinates;
		
		vertexCoordinates = new float[vertexCount*3];
		memcpy(vertexCoordinates,br5Model.vertexCoordinates,sizeof(float)*vertexCount*3);

		vertexCoordinatesAnim = br5Model.vertexCoordinatesAnim;
		animNextFrame = br5Model.animNextFrame;
		animSpeed = br5Model.animSpeed;
		interpolate = br5Model.interpolate;
		update = br5Model.update;
		connected = br5Model.connected;

		vbo = new CVBO(framesCount==1?CVBO::BT_STATIC_DRAW:CVBO::BT_STREAM_DRAW,false);
		vbo->setVertexData(vertexCount,3,sizeof(GLfloat),vertexCoordinates,CVBO::DT_FLOAT);
		vbo->setTextureData(CVBO::TU_UNIT0,texture,vertexCount,2,sizeof(GLfloat),textureCoordinates,CVBO::DT_FLOAT);
		vbo->setEnumMode(CVBO::EM_TRIANGLES);

		copy = true;
	}

	CBR5Model::~CBR5Model()
	{
		delete vbo;
		delete [] vertexCoordinates;

		if (!copy)
		{
			delete [] textureCoordinates;		

			if (framesCount>1)
			{
				for (unsigned int i=0; i<framesCount; i++)
				{
					delete [] vertexCoordinatesAnim[i];
				}

				delete [] vertexCoordinatesAnim;
			}
		}
	}

	bool CBR5Model::loadFromFile(std::string fileName)
	{
		/* ***************** 1. load the data ***************** */

		FILE *br5 = fopen(fileName.c_str(),"rb");

		if (!br5)
		{
			return false;
		}

		long fileSize = getFileSize(br5);

		byte *data = new byte[fileSize];
		byte *tmp = data;

		// read the full model data
		size_t read = fread(data,sizeof(byte),fileSize,br5);

		if (read!=fileSize)
		{
			return false;
		}

		// read the mark
		char mark[3];
		memcpy(mark,data,3);
		data+=3;

		if (mark[0]!='B' || mark[1]!='R' || mark[2]!='5')
		{
			// file is not a valid br5 model
			fclose(br5);
			return false;
		}

		// read texture name
		memcpy(textureName,data,256);
		data+=256;

		// read number of vertices
		vertexCount = *(unsigned int*)data;
		data+=4;

		textureCoordinates = new float[vertexCount*2];

		// read texture coordinates
		memcpy(textureCoordinates,data,sizeof(float)*vertexCount*2);
		data+=sizeof(float)*vertexCount*2;

		// read number of frames
		framesCount = *(unsigned int*)data;
		data+=4;

		if (framesCount==1)
		{
			// non animated version

			// read vertices
			vertexCoordinates = new float[vertexCount*3];
			memcpy(vertexCoordinates,data,sizeof(float)*vertexCount*3);
			data+=sizeof(float)*vertexCount*3;
		}
		else
		{
			// read vertices
			/*vertexCoordinates = new float[vertexCount*3];
			memcpy(vertexCoordinates,data,sizeof(float)*vertexCount*3);
			data+=sizeof(float)*vertexCount*3;*/

			// read other anims
			vertexCoordinatesAnim = new float*[framesCount];

			for (unsigned int i=0; i<framesCount; i++)
			{
				vertexCoordinatesAnim[i] = new float[vertexCount*3];
				memcpy(vertexCoordinatesAnim[i],data,sizeof(float)*vertexCount*3);
				data+=sizeof(float)*vertexCount*3;
			}

			vertexCoordinates = new float[vertexCount*3];

			memcpy(vertexCoordinates,vertexCoordinatesAnim[0],sizeof(float)*vertexCount*3);
		}

		fclose(br5);

		data = tmp;
		delete [] data;

		startFrame = 0;
		endFrame = framesCount-1;


		/* ***************** 3. setup the VBO ***************** */

		vbo = new CVBO(framesCount==1?CVBO::BT_STATIC_DRAW:CVBO::BT_STREAM_DRAW,false);
		vbo->setVertexData(vertexCount,3,sizeof(GLfloat),vertexCoordinates,CVBO::DT_FLOAT);
		vbo->setEnumMode(CVBO::EM_TRIANGLES);

		setupTexture();

		// calculate the bounding box
		boundingBox.reset();
		for (unsigned int v=0; v<vertexCount*3; v+=3)
		{
			boundingBox.update(vertexCoordinates+v);
		}
		boundingBox.calculateExtents();

		return true;
	}

	bool CBR5Model::loadFromMD3(std::string fileName)
	{
	

		return true;
	}

	char *CBR5Model::getTextureName()
	{
		return textureName;
	}

	void CBR5Model::setTextureName(std::string textureName)
	{
		strcpy(this->textureName,textureName.c_str());

		setupTexture();
	}

	void CBR5Model::setupTexture()
	{
		if (textureName[0]!='0')
		{
			CTextureLoader::buildTexture(textureName,texture,true);
		}

		if (texture!=0)
		{
			vbo->setTextureData(CVBO::TU_UNIT0,texture,vertexCount,2,sizeof(GLfloat),textureCoordinates,CVBO::DT_FLOAT);
		}
	}

	long CBR5Model::getFileSize(FILE *file)
	{
		fseek(file,0,SEEK_END);
		long pos = ftell(file);
		fseek(file,0,SEEK_SET);
		return pos;
	}

	void CBR5Model::setAnimSpeed(float animSpeed)
	{
		this->animSpeed = animSpeed;
	}

	void CBR5Model::draw(GLfloat deltaTime)
	{
		if (framesCount>1)
		{
			if(update)
			{
				animNextFrame+=animSpeed*deltaTime;

				if (animNextFrame>=1.0f)
				{
					animNextFrame=1.0;
				}

				if (interpolate)
				{
					if (currentFrame<endFrame+(connected?1:0))
					{
						// interpolate between 2 frames
						for (unsigned int v=0; v<vertexCount*3; v+=3)
						{
							// newPos = nextFrame*animNextFrame+currentFrame*(1.0f-animNextFrame)

							GLint nextFrameIndex = currentFrame+1;

							if (connected && currentFrame == endFrame)
							{
								nextFrameIndex = startFrame;
							}

							vertexCoordinates[v+0] = vertexCoordinatesAnim[nextFrameIndex][v+0]*animNextFrame+vertexCoordinatesAnim[currentFrame][v+0]*(1.0f-animNextFrame);
							vertexCoordinates[v+1] = vertexCoordinatesAnim[nextFrameIndex][v+1]*animNextFrame+vertexCoordinatesAnim[currentFrame][v+1]*(1.0f-animNextFrame);
							vertexCoordinates[v+2] = vertexCoordinatesAnim[nextFrameIndex][v+2]*animNextFrame+vertexCoordinatesAnim[currentFrame][v+2]*(1.0f-animNextFrame);
						}
					}
				}

				if (animNextFrame>=1.0f)
				{
					animNextFrame = 0.0f;

					currentFrame++;

					if (currentFrame>=endFrame)
					{
						currentFrame = startFrame;
					}

					if (!interpolate)
					{
						memcpy(vertexCoordinates,vertexCoordinatesAnim[currentFrame],sizeof(GLfloat)*vertexCount*3);
					}
				}		
			}
		}

		vbo->draw();
	}

	void CBR5Model::setInterpolate(bool interpolate)
	{
		this->interpolate = interpolate;
	}

	void CBR5Model::setConnected(bool connected)
	{
		this->connected = connected;
	}

	void CBR5Model::setUpdate(bool update)
	{
		this->update = update;
	}

	int CBR5Model::registerAction(int startFrame, int endFrame)
	{
		actions[actionsCount++] = pair<int, int>(startFrame,endFrame);
		return actionsCount-1;
	}

	void CBR5Model::doAction(int actionID)
	{
		animNextFrame = 0;

		pair<int,int> framePair = actions[actionID];

		currentFrame = startFrame = framePair.first;

		endFrame = framePair.second;
	}

	void CBR5Model::scaleToMaxY(float maxY)
	{
		float dx = maxY/boundingBox.height;

		boundingBox.reset();
		for (unsigned int v=0; v<vertexCount*3; v+=3)
		{
			vertexCoordinates[v+0]*=dx;
			vertexCoordinates[v+1]*=dx;
			vertexCoordinates[v+2]*=dx;
			boundingBox.update(vertexCoordinates+v);
		}
		boundingBox.calculateExtents();

		// update the anim part
		if (framesCount>1)
		{
			for (unsigned int i=0; i<framesCount; i++)
			{
				for (unsigned int v=0; v<vertexCount*3; v+=3)
				{
					vertexCoordinatesAnim[i][v+0]*=dx;
					vertexCoordinatesAnim[i][v+1]*=dx;
					vertexCoordinatesAnim[i][v+2]*=dx;
				}
			}
		}
		else
		{
			// we need to reupload the modified vertices
			vbo->setVertexData(vertexCount,3,sizeof(GLfloat),vertexCoordinates,CVBO::DT_FLOAT);
		}
	}

	sBoundingBox *CBR5Model::getBoundingBox()
	{
		return &boundingBox;
	}

	CVBO *CBR5Model::getVBO()
	{
		return vbo;
	}

	unsigned int CBR5Model::getVertexCount()
	{
		return vertexCount;
	}

	float *CBR5Model::getVertexCoordinates()
	{
		return vertexCoordinates;
	}

	float CBR5Model::getAnimSpeed()
	{
		return animSpeed;
	}
};