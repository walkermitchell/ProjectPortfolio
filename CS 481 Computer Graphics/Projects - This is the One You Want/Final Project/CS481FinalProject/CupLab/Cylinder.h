#pragma once


#include <IvVertexFormats.h>

// Forward Declaration
class IvVertexBuffer;
class IvIndexBuffer;
class IvTexture;

class Cylinder
{
public:
	Cylinder(float rad= 1.0f, float h = 2.0f);
	~Cylinder();

	//void BuildSurface();   //Not sure if this is actually necessary


	IvTexture* GetTexture();


	void Draw();


private:
	void CalculateNormals();

	IvTNPVertex* mBaseDataPtr;
	IvTNPVertex* mLidDataPtr;
	UInt32* mBaseIndexPtr;
	IvTNPVertex* mSideDataPtr;
	UInt32* mSideIndexPtr;
	IvVertexBuffer* mBaseVertices;
	IvIndexBuffer* mBaseIndices;
	IvVertexBuffer* mSideVertices;
	IvIndexBuffer* mSideIndices;
	IvVertexBuffer* mVerticesNormals;
	IvVertexBuffer* mLidVertices; //Base indices should also work for lid so we dont need an index buffer for it

	IvTexture* mTexture;

	//member variables
	float radius, height;
};
