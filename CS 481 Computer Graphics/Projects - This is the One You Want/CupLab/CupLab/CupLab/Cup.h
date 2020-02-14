#pragma once


#include <IvVertexFormats.h>

// Forward Declaration
class IvVertexBuffer;
class IvIndexBuffer;

class Cup
{
public:
	Cup(float rad= 1.0f, float h = 2.0f);
	~Cup();

	void Draw();


private:
	void CalculateNormals();

	IvCPVertex* mBaseDataPtr;
	UInt32* mBaseIndexPtr;
	IvCPVertex* mSideDataPtr;
	UInt32* mSideIndexPtr;
	IvVertexBuffer* mBaseVertices;
	IvIndexBuffer* mBaseIndices;
	IvVertexBuffer* mSideVertices;
	IvIndexBuffer* mSideIndices;
	IvVertexBuffer* mVerticesNormals;

	//member variables
	float radius, height;
};
