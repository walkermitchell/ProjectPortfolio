//===============================================================================
// @ Game.cpp
// ------------------------------------------------------------------------------
// Game core routines
//
// Copyright (C) 2008-2015 by James M. Van Verth and Lars M. Bishop.
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
//===============================================================================

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#include <IvRenderer.h>
#include <IvEventHandler.h>
#include <IvMath.h>
#include <IvVector3.h>
#include <IvMatrix33.h>
#include <IvVector4.h>
#include <IvMatrix44.h>
#include <IvRendererHelp.h>

#include "Game.h"
#include "Player.h"


//-------------------------------------------------------------------------------
//-- Static Members -------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//-- Methods --------------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// @ IvGame::Create()
//-------------------------------------------------------------------------------
// Static constructor
//-------------------------------------------------------------------------------
bool
IvGame::Create()
{
    IvGame::mGame = new Game();
    return ( IvGame::mGame != 0 );

}   // End of IvGame::Create()


//-------------------------------------------------------------------------------
// @ Game::Game()
//-------------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------------
Game::Game() : IvGame()
{
    // init variables
    mPlayer = 0;            // main player object

	//Set camera location and view direction
	mCameraPosition.Set(0.0f, 0.0f, 1.0f);
	mViewDirection.Set(1.0f, 0.0f, 0.0f);

	//Toggle for switching between orthographic and perspective
	mOrthographic = false;

	//Toggle for zoom function
	mZoom = false;

}   // End of Game::Game()


//-------------------------------------------------------------------------------
// @ Game::~Game()
//-------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------
Game::~Game()
{
    delete mPlayer; 
    mPlayer = 0;    
}   // End of Game::~Game()


//-------------------------------------------------------------------------------
// @ Game::PostRendererInitialize()
//-------------------------------------------------------------------------------
// Set up internal subsystems
//-------------------------------------------------------------------------------
bool 
Game::PostRendererInitialize()
{
    // Set up base class 
    if ( !IvGame::PostRendererInitialize() )
        return false;
    
    mPlayer = new Player();
    if (!mPlayer)
        return false;

    // Set some lights
    ::IvSetDefaultLighting();

    return true;
}   // End of Game::PostRendererInitialize()


//-------------------------------------------------------------------------------
// @ Game::Update()
//-------------------------------------------------------------------------------
// Main update loop
//-------------------------------------------------------------------------------
void
Game::UpdateObjects( float dt )
{
	//From src/Examples/Ch07-Viewing

	// change camera position
	float delta = 0.0f;
	if (mEventHandler->IsKeyDown('w'))
	{
		delta += dt;
	}
	if (mEventHandler->IsKeyDown('s'))
	{
		delta -= dt;
	}
	// move along view direction
	mCameraPosition.x += 2.0f * mViewDirection.x * delta;
	mCameraPosition.y += 2.0f * mViewDirection.y * delta;

	// change camera heading
	float headingChange = 0.0f;
	if (mEventHandler->IsKeyDown('a'))
	{
		headingChange += 0.25f * kPI * dt;
	}
	if (mEventHandler->IsKeyDown('d'))
	{
		headingChange -= 0.25f * kPI * dt;
	}
	float sintheta, costheta;
	IvSinCos(headingChange, sintheta, costheta);

	float x = mViewDirection.x * costheta - mViewDirection.y * sintheta;
	mViewDirection.y = mViewDirection.x * sintheta
		+ mViewDirection.y * costheta;
	mViewDirection.x = x;
	// shouldn't need this, but it keeps floating precision issues under control
	mViewDirection.Normalize();
    
	// Toggle projection setting
	if (mEventHandler->IsKeyPressed('v')) {
		mOrthographic = !mOrthographic;
	}

	// Toggle zoom
	if (mEventHandler->IsKeyPressed('c')) {
		mZoom = !mZoom;
	}

	// reset
	if (mEventHandler->IsKeyDown(' '))
	{
		mCameraPosition.Set(0.0f, 0.0f, 1.0f);
		mViewDirection.Set(1.0f, 0.0f, 0.0f);
		IvRenderer::mRenderer->SetFOV(45.0f);
		IvRenderer::mRenderer->SetNearPlane(0.1f);
		IvRenderer::mRenderer->SetFarPlane(27.5f);
		mOrthographic = false;
		mZoom = false;
	}
}   // End of Game::Update()


//Bringing in camera setter function from src/Examples/Ch07-Viewing/Viewing-04-Stereo
void Game::SetCamera(const IvVector3& viewDir,
					 const IvVector3& viewSide,
					 const IvVector3& viewPos)
{
	// get the third view vector
	IvVector3 viewUp = viewSide.Cross(viewDir);

	// this effectively multiplies the view position by the
	// transpose of the view-to-world rotation and negates it
	IvVector3 invViewPos(-viewSide.Dot(viewPos),
		-viewUp.Dot(viewPos),
		viewDir.Dot(viewPos));

	// build the matrix
	// the rows of the 3x3 are the view vectors
	// the y column is the view position rotated into view space
	IvMatrix44 viewMatrix;
	viewMatrix.Identity();
	viewMatrix(0, 0) = viewSide.x;
	viewMatrix(0, 1) = viewSide.y;
	viewMatrix(0, 2) = viewSide.z;
	viewMatrix(0, 3) = invViewPos.x;
	viewMatrix(1, 0) = viewUp.x;
	viewMatrix(1, 1) = viewUp.y;
	viewMatrix(1, 2) = viewUp.z;
	viewMatrix(1, 3) = invViewPos.y;
	viewMatrix(2, 0) = -viewDir.x;
	viewMatrix(2, 1) = -viewDir.y;
	viewMatrix(2, 2) = -viewDir.z;
	viewMatrix(2, 3) = invViewPos.z;

	::IvSetViewMatrix(viewMatrix);

}   // End of Game::SetCamera()


// Orthographic projection logic from src\Examples\Ch07-Viewing\Viewing-05-Orthographic
void Game::Orthographic(float left, float right,
						float bottom, float top,
						float nearZ, float farZ)
{
	IvMatrix44 ortho;
	if (IvRenderer::mRenderer->GetAPI() == kOpenGL)
	{
		float recipX = 1.0f / (right - left);
		float recipY = 1.0f / (top - bottom);
		float recipZ = 1.0f / (nearZ - farZ);

		ortho(0, 0) = 2.0f * recipX;
		ortho(0, 3) = -(right + left) * recipX;

		ortho(1, 1) = 2.0f * recipY;
		ortho(1, 3) = -(top + bottom) * recipY;

		ortho(2, 2) = 2.0f * recipZ;
		ortho(2, 3) = (nearZ + farZ) * recipZ;
	}
	else
	{
		float recipX = 1.0f / (right - left);
		float recipY = 1.0f / (top - bottom);
		float recipZ = 1.0f / (farZ - nearZ);

		ortho(0, 0) = 2.0f * recipX;
		ortho(0, 3) = -(right + left) * recipX;

		ortho(1, 1) = 2.0f * recipY;
		ortho(1, 3) = -(top + bottom) * recipY;

		ortho(2, 2) = recipZ;
		ortho(2, 3) = -nearZ * recipZ;
	}

	// send to renderer
	::IvSetProjectionMatrix(ortho);
}

//Perspective matrix logic from src\Examples\Ch07-Viewing\Viewing-03-Perspective
void Game::Perspective(float fov, float aspect, float nearZ, float farZ)
{
	IvMatrix44 perspective;
	if (IvRenderer::mRenderer->GetAPI() == kOpenGL)
	{
		float d = 1.0f / IvTan(fov / 180.0f * kPI * 0.5f);
		float recip = 1.0f / (nearZ - farZ);

		perspective(0, 0) = d / aspect;
		perspective(1, 1) = d;
		perspective(2, 2) = (nearZ + farZ) * recip;
		perspective(2, 3) = 2.0f * nearZ * farZ * recip;
		perspective(3, 2) = -1.0f;
		perspective(3, 3) = 0.0f;
	}
	else
	{
		float d = 1.0f / IvTan(fov / 180.0f * kPI * 0.5f);
		float Q = farZ / (farZ - nearZ);

		perspective(0, 0) = d / aspect;
		perspective(1, 1) = d;
		perspective(2, 2) = Q;
		perspective(2, 3) = -nearZ * Q;
		perspective(3, 2) = 1.0f;
		perspective(3, 3) = 0.0f;
	}

	// send to Renderer
	IvSetProjectionMatrix(perspective);
}

//Cabinet zoom function from src\Examples\Ch07-Viewing\Viewing-06-Oblique
void Game::Cabinet(float left, float right,
				   float bottom, float top,
				   float nearZ, float farZ)
{
	IvMatrix44 cabinet;
	cabinet.Identity();
	if (IvRenderer::mRenderer->GetAPI() == kOpenGL)
	{
		float recipX = 1.0f / (right - left);
		float recipY = 1.0f / (top - bottom);
		float recipZ = 1.0f / (nearZ - farZ);

		float halfsqrt2 = ::IvSqrt(2.0f) * 0.5f;

		cabinet(0, 0) = 2.0f * recipX;
		cabinet(0, 2) = -halfsqrt2 * recipX;
		cabinet(0, 3) = (-halfsqrt2 * nearZ - right - left) * recipX;

		cabinet(1, 1) = 2.0f * recipY;
		cabinet(1, 2) = -halfsqrt2 * recipY;
		cabinet(1, 3) = (-halfsqrt2 * nearZ - top - bottom) * recipY;

		cabinet(2, 2) = 2.0f * recipZ;
		cabinet(2, 3) = (nearZ + farZ) * recipZ;
	}
	else
	{
		float recipX = 1.0f / (right - left);
		float recipY = 1.0f / (top - bottom);
		float recipZ = 1.0f / (farZ - nearZ);

		float halfsqrt2 = ::IvSqrt(2.0f) * 0.5f;

		cabinet(0, 0) = 2.0f * recipX;
		cabinet(0, 2) = halfsqrt2 * recipX;
		cabinet(0, 3) = (-halfsqrt2 * nearZ - right - left) * recipX;

		cabinet(1, 1) = 2.0f * recipY;
		cabinet(1, 2) = halfsqrt2 * recipY;
		cabinet(1, 3) = (-halfsqrt2 * nearZ - top - bottom) * recipY;

		cabinet(2, 2) = recipZ;
		cabinet(2, 3) = -nearZ * recipZ;
	}

	// send to renderer
	IvSetProjectionMatrix(cabinet);
}

//-------------------------------------------------------------------------------
// @ Game::Render()
//-------------------------------------------------------------------------------
// Render stuff
//-------------------------------------------------------------------------------
void Game::Render()                                  // Here's Where We Do All The Drawing
{   
	const float viewerDistance = 6.0f;
	const float halfHeight = 3.0f;
	float quartersqrt2 = ::IvSqrt(2.0f) * 0.25f;
	float aspect = (float)IvRenderer::mRenderer->GetWidth() / (float)IvRenderer::mRenderer->GetHeight();

	// Perspective matrix toggle logic from src\Examples\Ch07-Viewing\Viewing-05-Orthographic 
	// Zoom toggle logic from Cabinet() in src\Examples\Ch07-Viewing\Viewing-06-Oblique
	if (mZoom) {
		Cabinet(-aspect * 0.5f + quartersqrt2,
			aspect * 0.5f + quartersqrt2,
			-0.5f + quartersqrt2,
			0.5f + quartersqrt2,
			IvRenderer::mRenderer->GetNearPlane(),
			IvRenderer::mRenderer->GetFarPlane());
	}
	else if (mOrthographic)
	{
		Orthographic(-aspect, aspect, -1.0f, 1.0f,
			IvRenderer::mRenderer->GetNearPlane(),
			IvRenderer::mRenderer->GetFarPlane());
	}
	else
	{
		Perspective(IvRenderer::mRenderer->GetFOV(),
			(float)IvRenderer::mRenderer->GetWidth() / (float)IvRenderer::mRenderer->GetHeight(),
			IvRenderer::mRenderer->GetNearPlane(),
			IvRenderer::mRenderer->GetFarPlane());
	}

	

    // set up viewer using example code in src/Examples/Ch07-Viewing

	// get view side vector so we can separate the eye positions
	IvVector3 viewSide = mViewDirection.Cross(IvVector3::zAxis);
	viewSide.Normalize();

	// set view transform 
	SetCamera(mViewDirection, viewSide, mCameraPosition);
	
    // draw coordinate axes
    IvDrawAxes();
    
    // draw the world objects
    mPlayer->Render();
}

