//===============================================================================
// @ Player.cpp
// ------------------------------------------------------------------------------
// Player object
//
// Copyright (C) 2008-2015 by James M. Van Verth and Lars M. Bishop.
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// This example demonstrates the three basic transforms and how they interact
// with each other.   At each frame we append transforms to the current matrix,
// which is stored for the next frame.  The key commands are:
//
// I, K - translate in x
// J, L - translate in y
// U, O - rotate around z axis
// P, : - uniformly scale
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
#include <IvRendererHelp.h>

#include "Player.h"
#include "Game.h"

//-------------------------------------------------------------------------------
//-- Static Members -------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//-- Methods --------------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// @ Player::Player()
//-------------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------------
Player::Player()
{
}   // End of Player::Player()


//-------------------------------------------------------------------------------
// @ Player::~Player()
//-------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------
Player::~Player()
{
}   // End of Player::~Player()


//-------------------------------------------------------------------------------
// @ Player::Update()
//-------------------------------------------------------------------------------
// Main update loop
//-------------------------------------------------------------------------------
void
Player::Update( float dt )
{
	
    
}   // End of Player::Update()


//-------------------------------------------------------------------------------
// @ Player::Render()
//-------------------------------------------------------------------------------
// Render stuff
//-------------------------------------------------------------------------------
void Player::Render()                                    
{   
	//Wireframes
	//IvRenderer::mRenderer->SetFillMode(IvFillMode::kWireframeFill);

	//set up scene object data
	IvVector3 boxMin = IvVector3(4.0f, 4.0f, 0.0f);
	IvVector3 boxMax = IvVector3(5.0f, 5.0f, 1.0f);

	IvMatrix44 worldMatrix = IvRenderer::mRenderer->GetWorldMatrix();


	//draw scene geometry
	IvDrawFloor();
	IvDrawAxes();
	IvDrawBox(boxMin, boxMax, kRed);

	//Shift the world coordinates so that the next couple objects aren't on top of the others
	IvRenderer::mRenderer->SetWorldMatrix(worldMatrix.Translation(IvVector3(0.0f, -4.0f, 0.0f)));
	IvDrawTeapot();
	IvDrawBox(boxMin, boxMax, kYellow);

	//Shift again for last object
	IvRenderer::mRenderer->SetWorldMatrix(worldMatrix.Translation(IvVector3(-2.0f, 4.0f, 1.0f)));
	IvDrawSphere(0.5f);

	//Restore world coords
	IvRenderer::mRenderer->SetWorldMatrix(worldMatrix);

    // store current character modification matrix
    IvRenderer::mRenderer->SetWorldMatrix( mTransform );

	
}
