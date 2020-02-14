

//Source code for Cup class

#include "Cup.h"

#include <cmath>
#include <IvVertexBuffer.h>
#include <IvIndexBuffer.h>
#include <IvResourceManager.h>
#include <IvRenderer.h>
#include <IvVector3.h>
#include <IvColor.h>
#include <IvStackAllocator.h>

#define NUM_SIDES 20
#define NUM_BASE_INDICES ((NUM_SIDES + 1)*3)
#define NUM_SIDE_INDICES (NUM_SIDES*6)

Cup::Cup(float rad, float h)
	: mBaseDataPtr(0), mBaseIndexPtr(0), mSideDataPtr(0), mSideIndexPtr(0), mBaseVertices(0), mBaseIndices(0), mSideVertices(0), mSideIndices(0), mVerticesNormals(0)
{

	//This pi value came from https://www.quantstart.com/articles/Mathematical-Constants-in-C
	const double pi = 3.14159265358979323846;

	this->radius = rad;
	this->height = h;

	mBaseDataPtr = new IvCPVertex[NUM_SIDES + 1];
	mSideDataPtr = new IvCPVertex[NUM_SIDES * 2];

	//Preload center point as vertex 0
	mBaseDataPtr[0].position.Set(0.0f, 0.0f, 0.0f);
	mBaseDataPtr[0].color.Set(10, 10, 210, 255);

	//Generate vertices
	for (int i = 0; i < NUM_SIDES; i++) {
		//Cup base
		mBaseDataPtr[i + 1].position.Set(cos((double(i) / NUM_SIDES) * 2 * pi) * radius,
										 sin((double(i) / NUM_SIDES) * 2 * pi) * radius,
										 mBaseDataPtr[0].position[2]);
		mBaseDataPtr[i + 1].color.Set(0, 200, 50, 255);

		mSideDataPtr[2 * i].position.Set(cos((double(i) / NUM_SIDES) * 2 * pi) * radius,
									 sin((double(i) / NUM_SIDES) * 2 * pi) * radius,
									 mBaseDataPtr[0].position[2]);
		mSideDataPtr[2 * i].color.Set(0, 200, 50, 255);

		//Upper cup rim
		mSideDataPtr[(2 * i) + 1].position.Set(cos((double(i) / NUM_SIDES) * 2 * pi) * radius * 1.2,
												 sin((double(i) / NUM_SIDES) * 2 * pi) * radius * 1.2,
												 mBaseDataPtr[0].position[2] + height);
		mSideDataPtr[(2 * i) + 1].color.Set(150, 50, 50, 255);
	}

	mBaseIndexPtr = new UInt32[NUM_BASE_INDICES];
	mSideIndexPtr = new UInt32[NUM_SIDE_INDICES];

	for (int i = 0; i < NUM_SIDES; i++) {
		//Generate indices for bottom of cup
		mBaseIndexPtr[3 * i] = 0;
		mBaseIndexPtr[(3 * i) + 1] = i + 2;
		mBaseIndexPtr[(3 * i) + 2] = i + 1;

		//Fill in cup sides:
		//Indices for triangles with 2 points on cup base
		mSideIndexPtr[6 * i] = (2 * i) % (2 * NUM_SIDES);
		mSideIndexPtr[(6 * i) + 1] = ((2* i) + 2) % (2 * NUM_SIDES);
		mSideIndexPtr[(6 * i) + 2] = ((2 * i) + 1) % (2 * NUM_SIDES);

		//Indices for triangles with 2 points on cup rim
		mSideIndexPtr[(6 * i) + 3] = ((2 * i) + 1) % (2 * NUM_SIDES);
		mSideIndexPtr[(6 * i) + 4] = ((2 * i) + 2) % (2 * NUM_SIDES);
		mSideIndexPtr[(6 * i) + 5] = ((2 * i) + 3) % (2 * NUM_SIDES);
	}

	//Fixing hole in bottom of cup
	mBaseIndexPtr[3 * NUM_SIDES] = 0;
	mBaseIndexPtr[(3 * NUM_SIDES) + 1] = 1;
	mBaseIndexPtr[(3 * NUM_SIDES) + 2] = NUM_SIDES;

	mBaseVertices = IvRenderer::mRenderer->GetResourceManager()->CreateVertexBuffer(kCPFormat, NUM_SIDES + 1, mBaseDataPtr, kDefaultUsage);
	mBaseIndices = IvRenderer::mRenderer->GetResourceManager()->CreateIndexBuffer(NUM_BASE_INDICES, mBaseIndexPtr, kDefaultUsage);

	mSideVertices = IvRenderer::mRenderer->GetResourceManager()->CreateVertexBuffer(kCPFormat, NUM_SIDES*2, mSideDataPtr, kDefaultUsage);
	mSideIndices = IvRenderer::mRenderer->GetResourceManager()->CreateIndexBuffer(NUM_SIDE_INDICES, mSideIndexPtr, kDefaultUsage);

	CalculateNormals();
}

Cup::~Cup()
{
	delete[] mBaseDataPtr;
	delete[] mBaseIndexPtr;
	delete[] mSideDataPtr; 
	delete[] mSideIndexPtr;
}

void Cup::Draw()
{

	IvRenderer::mRenderer->Draw(kTriangleListPrim, mBaseVertices, mBaseIndices);
	IvRenderer::mRenderer->Draw(kTriangleListPrim, mSideVertices, mSideIndices);
	IvRenderer::mRenderer->Draw(kPointListPrim, mSideVertices);
	IvRenderer::mRenderer->Draw(kLineListPrim, mVerticesNormals);
}


void Cup::CalculateNormals()
{
	// Use preallocated memory from this "stack"
	size_t currentOffset = IvStackAllocator::mScratchAllocator->GetCurrentOffset();
	IvCPVertex* dataPtr = (IvCPVertex*)IvStackAllocator::mScratchAllocator->Allocate(kIvVFSize[kCPFormat] * 2 * NUM_SIDES * 3);

	if (nullptr == dataPtr)
	{
		return;
	}

	// Iterate over each component triangle in the base of the cup
	for (int i = 0; i < NUM_BASE_INDICES / 3; ++i)
	{
		// Grab the 3 points of this triangle
		IvVector3 p0 = mBaseDataPtr[mBaseIndexPtr[3 * i + 0]].position;
		IvVector3 p1 = mBaseDataPtr[mBaseIndexPtr[3 * i + 1]].position;
		IvVector3 p2 = mBaseDataPtr[mBaseIndexPtr[3 * i + 2]].position;
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

	// Iterate over each component triangle in the side of the cup
	for (int i = 0; i < NUM_SIDE_INDICES / 3; ++i)
	{
		// Grab the 3 points of this triangle
		IvVector3 p0 = mSideDataPtr[mSideIndexPtr[3 * i + 0]].position;
		IvVector3 p1 = mSideDataPtr[mSideIndexPtr[3 * i + 1]].position;
		IvVector3 p2 = mSideDataPtr[mSideIndexPtr[3 * i + 2]].position;
		// Calculate starting point of the line, at the centroid
		IvVector3 pc = (p0 + p1 + p2) / 3.0f;
		// Need the normal vector to calculate the end-point
		IvVector3 vn = (p1 - p0).Cross(p2 - p0);
		vn.Normalize();
		IvVector3 pend = pc + 1 * vn;

		// Put this line segment into our data array
		dataPtr[2 * (i + NUM_SIDES) + 0].position = pc;
		dataPtr[2 * (i + NUM_SIDES) + 0].color.Set(0, 0, 0, 255);

		dataPtr[2 * (i + NUM_SIDES) + 1].position = pend;
		dataPtr[2 * (i + NUM_SIDES) + 1].color.Set(0, 0, 0, 255);
	}

	// Send it to the GPU in a Vertex Buffer Object
	mVerticesNormals = IvRenderer::mRenderer->GetResourceManager()->CreateVertexBuffer(kCPFormat, 2 * NUM_SIDES * 3, dataPtr, kImmutableUsage);

	// pop allocation off the stack
	IvStackAllocator::mScratchAllocator->Reset(currentOffset);
}