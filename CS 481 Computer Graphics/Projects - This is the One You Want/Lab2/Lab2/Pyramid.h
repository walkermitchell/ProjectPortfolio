#pragma once


#include <IvVertexFormats.h>

// Forward Declaration
class IvVertexBuffer;
class IvIndexBuffer;

class Pyramid
{
public:
	Pyramid();
	~Pyramid();

	void Draw();


private:
	void CalculateNormals();

	IvCPVertex* mDataPtr;
	UInt32* mIndexPtr;
	IvVertexBuffer* mVertices;
	IvIndexBuffer* mIndices;
	IvVertexBuffer* mVerticesNormals;
};
