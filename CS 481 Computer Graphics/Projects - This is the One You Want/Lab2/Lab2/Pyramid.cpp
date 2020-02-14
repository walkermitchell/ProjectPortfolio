

//Source code for Pyramid class

#include "Pyramid.h"

#include <IvVertexBuffer.h>
#include <IvIndexBuffer.h>
#include <IvResourceManager.h>
#include <IvRenderer.h>
#include <IvVector3.h>
#include <IvColor.h>
#include <IvStackAllocator.h>

#define NUM_VERTS 5
#define NUM_INDICES (6*3)

Pyramid::Pyramid()
	: mDataPtr(0), mIndexPtr(0), mVertices(0), mIndices(0), mVerticesNormals(0)
{
	mDataPtr = new IvCPVertex[NUM_VERTS];
	//Red
	mDataPtr[0].position.Set(0.5f, 0.5f, 0.0f);
	mDataPtr[0].color.Set(255, 0, 0, 255);
	//Yellow
	mDataPtr[1].position.Set(0.5f, -0.5f, 0.0f);
	mDataPtr[1].color.Set(255, 255, 0, 255);
	//Green
	mDataPtr[2].position.Set(-0.5f, -0.5f, 0.0f);
	mDataPtr[2].color.Set(0, 255, 0, 255);
	//Blue
	mDataPtr[3].position.Set(-0.5f, 0.5f, 0.0f);
	mDataPtr[3].color.Set(0, 0, 255, 255);
	//White
	mDataPtr[4].position.Set(0.0f, 0.0f, 2.0f);
	mDataPtr[4].color.Set(255, 255, 255, 255);

	mIndexPtr = new UInt32[NUM_INDICES]{
		
		//Pyramid base
		0,1,2,
		0,2,3,

		//Pyramid sides
		4,0,3,
		4,1,0,
		4,2,1,
		4,3,2
	};

	mVertices = IvRenderer::mRenderer->GetResourceManager()->CreateVertexBuffer(kCPFormat, NUM_VERTS, mDataPtr, kDefaultUsage);
	mIndices = IvRenderer::mRenderer->GetResourceManager()->CreateIndexBuffer(NUM_INDICES, mIndexPtr, kDefaultUsage);
}

Pyramid::~Pyramid()
{
	delete[] mDataPtr;
	delete[] mIndexPtr;
}

void Pyramid::Draw()
{
	IvRenderer::mRenderer->Draw(kTriangleListPrim, mVertices, mIndices);
}


void Pyramid::CalculateNormals()
{
	// Use preallocated memory from this "stack"
	size_t currentOffset = IvStackAllocator::mScratchAllocator->GetCurrentOffset();
	IvCPVertex* dataPtr = (IvCPVertex*)IvStackAllocator::mScratchAllocator->Allocate(kIvVFSize[kCPFormat] * 2 * NUM_INDICES / 3);

	if (nullptr == dataPtr)
	{
		return;
	}

	// Iterate over each component triangle in this pyramid
	for (int i = 0; i < NUM_INDICES / 3; ++i)
	{
		// Grab the 3 points of this triangle
		IvVector3 p0 = mDataPtr[mIndexPtr[3 * i + 0]].position;
		IvVector3 p1 = mDataPtr[mIndexPtr[3 * i + 1]].position;
		IvVector3 p2 = mDataPtr[mIndexPtr[3 * i + 2]].position;
		// Calculate starting point of the line, at the centroid
		IvVector3 pc = (p0 + p1 + p2) / 3.0f;
		// Need the normal vector to calculate the end-point
		IvVector3 vn = (p1 - p0).Cross(p2 - p0);
		vn.Normalize();
		IvVector3 pend = pc + 1 * vn;

		// Put this line segment into our data array
		dataPtr[2 * i + 0].position = pc;
		dataPtr[2 * i + 0].color.Set(0, 0, 0, 255);

		dataPtr[2 * i + 1].position = pend;
		dataPtr[2 * i + 1].color.Set(0, 0, 0, 255);
	}

	// Send it to the GPU in a Vertex Buffer Object
	mVerticesNormals = IvRenderer::mRenderer->GetResourceManager()->CreateVertexBuffer(kCPFormat, 2 * NUM_INDICES / 3, dataPtr, kImmutableUsage);

	// pop allocation off the stack
	IvStackAllocator::mScratchAllocator->Reset(currentOffset);
}