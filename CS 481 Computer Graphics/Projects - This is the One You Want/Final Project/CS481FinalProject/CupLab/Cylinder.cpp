

//Source code for Cup class

#include "Cylinder.h"

#include <cmath>
#include <IvVertexBuffer.h>
#include <IvIndexBuffer.h>
#include <IvResourceManager.h>
#include <IvRenderer.h>
#include <IvVector3.h>
#include <IvColor.h>
#include <IvTexture.h>
#include <IvImage.h>
#include <IvStackAllocator.h>
#include <IvFileReader.h>
#include <IvMatrix33.h>
#include <IvMath.h>
#include <iostream>

#define NUM_SIDES 20
#define NUM_BASE_INDICES ((NUM_SIDES + 1)*3)
#define NUM_SIDE_INDICES (NUM_SIDES*6)

Cylinder::Cylinder(float rad, float h)
	: mBaseDataPtr(0), mBaseIndexPtr(0), mSideDataPtr(0), mSideIndexPtr(0), mBaseVertices(0), mBaseIndices(0), mSideVertices(0), mSideIndices(0), mVerticesNormals(0)
{

	//This pi value came from https://www.quantstart.com/articles/Mathematical-Constants-in-C
	const double pi = 3.14159265358979323846;

	this->radius = rad;
	this->height = h;

	// Load texture image
	IvImage* image = IvImage::CreateFromFile("image.tga");
	if (image)
	{
		mTexture = IvRenderer::mRenderer->GetResourceManager()->CreateTexture(
			(image->GetBytesPerPixel() == 4) ? kRGBA32TexFmt : kRGB24TexFmt,
			image->GetWidth(), image->GetHeight(), image->GetPixels(), kImmutableUsage);

		delete image;
		image = 0;
	}
	else
	{
		std::cout << "Failed to load texture image" << std::endl;
	}

	mBaseDataPtr = new IvTNPVertex[NUM_SIDES + 1];
	mLidDataPtr = new IvTNPVertex[NUM_SIDES + 1];
	mSideDataPtr = new IvTNPVertex[NUM_SIDES * 2];

	float baseTexU = 0.0f;
	float baseTexV = 1.0f;
	float sideTexU = 0.0f;
	float sideLowerTexV = 0.0f;
	float sideUpperTexV = 1.0f;

	//Preload center point as vertex 0
	mBaseDataPtr[0].position.Set(0.0f, 0.0f, 0.0f);
	mBaseDataPtr[0].texturecoord.Set(0, 0);
	//mBaseDataPtr[0].color.Set(10, 10, 210, 255);
	mLidDataPtr[0].position.Set(0.0f, 0.0f, 0.0f + height);
	mLidDataPtr[0].texturecoord.Set(0, 0);
	//mLidDataPtr[0].color.Set(10, 10, 210, 255);

	//Generate vertices
	for (int i = 0; i < NUM_SIDES; i++) {

		//Set texture coords
		baseTexU = i / (float)(NUM_SIDES - 1);
		sideTexU = i / (float)(NUM_SIDES - 1);

		//Cylinder base
		mBaseDataPtr[i + 1].position.Set(cos((double(i) / NUM_SIDES) * 2 * pi) * radius,
										 sin((double(i) / NUM_SIDES) * 2 * pi) * radius,
										 mBaseDataPtr[0].position[2]);
		//mBaseDataPtr[i + 1].color.Set(0, 200, 50, 255);
		mBaseDataPtr[i + 1].texturecoord.Set(baseTexU, baseTexV);

		mSideDataPtr[2 * i].position.Set(cos((double(i) / NUM_SIDES) * 2 * pi) * radius,
									 sin((double(i) / NUM_SIDES) * 2 * pi) * radius,
									 mBaseDataPtr[0].position[2]);
		//mSideDataPtr[2 * i].color.Set(0, 200, 50, 255);
		mSideDataPtr[2 * i].texturecoord.Set(sideTexU, sideLowerTexV);

		//Upper Cylinder rim
		mSideDataPtr[(2 * i) + 1].position.Set(cos((double(i) / NUM_SIDES) * 2 * pi) * radius,
												 sin((double(i) / NUM_SIDES) * 2 * pi) * radius,
												 mBaseDataPtr[0].position[2] + height);
		//mSideDataPtr[(2 * i) + 1].color.Set(150, 50, 50, 255);
		mSideDataPtr[(2 * i) + 1].texturecoord.Set(sideTexU, sideUpperTexV);

		// Cylinder lid - theta needs to be turned negative here so the normals look up out of the surface
		mLidDataPtr[i + 1].position.Set(cos((double(-i) / NUM_SIDES) * 2 * pi) * radius,
										sin((double(-i) / NUM_SIDES) * 2 * pi) * radius,
										mBaseDataPtr[0].position[2] + height);
		//mLidDataPtr[i + 1].color.Set(150, 50, 50, 255);
		mLidDataPtr[i + 1].texturecoord.Set(baseTexU, baseTexV);
	}

	mBaseIndexPtr = new UInt32[NUM_BASE_INDICES];
	mSideIndexPtr = new UInt32[NUM_SIDE_INDICES];

	for (int i = 0; i < NUM_SIDES; i++) {
		//Generate indices for bottom of Cylinder - should also work for lid, so we're not doing an mLidIndexPtr.
		mBaseIndexPtr[3 * i] = 0;
		mBaseIndexPtr[(3 * i) + 1] = i + 2;
		mBaseIndexPtr[(3 * i) + 2] = i + 1;

		//Fill in cup sides:
		//Indices for triangles with 2 points on Cylinder base
		mSideIndexPtr[6 * i] = (2 * i) % (2 * NUM_SIDES);
		mSideIndexPtr[(6 * i) + 1] = ((2* i) + 2) % (2 * NUM_SIDES);
		mSideIndexPtr[(6 * i) + 2] = ((2 * i) + 1) % (2 * NUM_SIDES);

		//Indices for triangles with 2 points on Cylinder rim
		mSideIndexPtr[(6 * i) + 3] = ((2 * i) + 1) % (2 * NUM_SIDES);
		mSideIndexPtr[(6 * i) + 4] = ((2 * i) + 2) % (2 * NUM_SIDES);
		mSideIndexPtr[(6 * i) + 5] = ((2 * i) + 3) % (2 * NUM_SIDES);
	}

	//Fixing hole in bottom of Cylinder
	mBaseIndexPtr[3 * NUM_SIDES] = 0;
	mBaseIndexPtr[(3 * NUM_SIDES) + 1] = 1;
	mBaseIndexPtr[(3 * NUM_SIDES) + 2] = NUM_SIDES;

	//Asign per-vertex normals as in the DonutShadingPhong example solution
	for (int i = 0; i < NUM_SIDES; i++) {

		//Initialize variables and calculate normals for base 
		IvVector3 P = mBaseDataPtr[mBaseIndexPtr[3 * i]].position;
		IvVector3 Q = mBaseDataPtr[mBaseIndexPtr[(3 * i) + 1]].position;
		IvVector3 R = mBaseDataPtr[mBaseIndexPtr[(3 * i) + 2]].position;
		IvVector3 vNorm = (Q - P).Cross(R - P);
		vNorm.Normalize();
		mBaseDataPtr[mBaseIndexPtr[3 * i]].normal += vNorm;
		mBaseDataPtr[mBaseIndexPtr[(3 * i) + 1]].normal += vNorm;
		mBaseDataPtr[mBaseIndexPtr[(3 * i) + 2]].normal += vNorm;

		//Calculate normals for lid
		P = mLidDataPtr[mBaseIndexPtr[3 * i]].position;
		Q = mLidDataPtr[mBaseIndexPtr[(3 * i) + 1]].position;
		R = mLidDataPtr[mBaseIndexPtr[(3 * i) + 2]].position;
		vNorm = (Q - P).Cross(R - P);
		vNorm.Normalize();
		mLidDataPtr[mBaseIndexPtr[3 * i]].normal += vNorm;
		mLidDataPtr[mBaseIndexPtr[(3 * i) + 1]].normal += vNorm;
		mLidDataPtr[mBaseIndexPtr[(3 * i) + 2]].normal += vNorm;

		//Calculate normals for triangles with 2 points on cup base
		P = mSideDataPtr[mSideIndexPtr[6 * i]].position;
		Q = mSideDataPtr[mSideIndexPtr[(6 * i) + 1]].position;
		R = mSideDataPtr[mSideIndexPtr[(6 * i) + 2]].position;
		vNorm = (Q - P).Cross(R - P);
		vNorm.Normalize();
		mSideDataPtr[mSideIndexPtr[6 * i]].normal += vNorm;
		mSideDataPtr[mSideIndexPtr[(6 * i) + 1]].normal += vNorm;
		mSideDataPtr[mSideIndexPtr[(6 * i) + 2]].normal += vNorm;

		//Calculate normals for triangles with 2 points on cup rim
		P = mSideDataPtr[mSideIndexPtr[(6 * i) + 3]].position;
		Q = mSideDataPtr[mSideIndexPtr[(6 * i) + 4]].position;
		R = mSideDataPtr[mSideIndexPtr[(6 * i) + 5]].position;
		vNorm = (Q - P).Cross(R - P);
		vNorm.Normalize();
		mSideDataPtr[mSideIndexPtr[(6 * i) + 3]].normal += vNorm;
		mSideDataPtr[mSideIndexPtr[(6 * i) + 4]].normal += vNorm;
		mSideDataPtr[mSideIndexPtr[(6 * i) + 5]].normal += vNorm;
	}

	//Normalize all the vertex normals
	for (int i = 0; i < NUM_SIDES; i++) {

		mBaseDataPtr[i].normal.Normalize();
		mLidDataPtr[i].normal.Normalize();

		mSideDataPtr[i].normal.Normalize();
		mSideDataPtr[2 * i].normal.Normalize();
	}

	//Finish off the last two in the base and lid
	mBaseDataPtr[NUM_SIDES].normal.Normalize();
	mLidDataPtr[NUM_SIDES].normal.Normalize();

	mBaseVertices = IvRenderer::mRenderer->GetResourceManager()->CreateVertexBuffer(kTNPFormat, NUM_SIDES + 1, mBaseDataPtr, kDefaultUsage);
	mBaseIndices = IvRenderer::mRenderer->GetResourceManager()->CreateIndexBuffer(NUM_BASE_INDICES, mBaseIndexPtr, kDefaultUsage);

	mSideVertices = IvRenderer::mRenderer->GetResourceManager()->CreateVertexBuffer(kTNPFormat, NUM_SIDES*2, mSideDataPtr, kDefaultUsage);
	mSideIndices = IvRenderer::mRenderer->GetResourceManager()->CreateIndexBuffer(NUM_SIDE_INDICES, mSideIndexPtr, kDefaultUsage);

	mLidVertices = IvRenderer::mRenderer->GetResourceManager()->CreateVertexBuffer(kTNPFormat, NUM_SIDES + 1, mLidDataPtr, kDefaultUsage);
	//mLidIndices = IvRenderer::mRenderer->GetResourceManager()->CreateIndexBuffer(NUM_BASE_INDICES, mBaseIndexPtr, kDefaultUsage);

	//CalculateNormals();
}

Cylinder::~Cylinder()
{
	delete[] mBaseDataPtr;
	delete[] mBaseIndexPtr;
	delete[] mSideDataPtr; 
	delete[] mSideIndexPtr;
}

IvTexture* Cylinder::GetTexture() {
	return mTexture;
}

void Cylinder::Draw()
{

	IvRenderer::mRenderer->Draw(kTriangleListPrim, mBaseVertices, mBaseIndices);
	IvRenderer::mRenderer->Draw(kTriangleListPrim, mSideVertices, mSideIndices);
	IvRenderer::mRenderer->Draw(kTriangleListPrim, mLidVertices, mBaseIndices);

	IvRenderer::mRenderer->Draw(kPointListPrim, mSideVertices);
	//IvRenderer::mRenderer->Draw(kLineListPrim, mVerticesNormals);
}


void Cylinder::CalculateNormals()
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