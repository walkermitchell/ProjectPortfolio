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
#include <IvRendererHelp.h>

#include "Player.h"
#include "Game.h"
#include "Cup.h"

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
    // get change in transform for this frame
    IvMatrix44 scale, rotate, vertRotate, xlate, unxform, rexform;
    scale.Identity();
    rotate.Identity();
    float s = 1.0f;
	float r = 0.0f;
	float t = 0.0f;
    float x = 0.0f, y = 0.0f, z = 0.0f; 
    
    // set up scaling
    if (IvGame::mGame->mEventHandler->IsKeyDown(';'))
    {
        s -= 0.25f*dt;
    }
    if (IvGame::mGame->mEventHandler->IsKeyDown('p'))
    {
        s += 0.25f*dt;
    }
    scale.Scaling( IvVector3(s, s, s) );
    
    // set up rotate
    if (IvGame::mGame->mEventHandler->IsKeyDown('o'))
    {
        r -= kPI*0.25f*dt;
    }
    if (IvGame::mGame->mEventHandler->IsKeyDown('u'))
    {
        r += kPI*0.25f*dt;
    }
    rotate.RotationZ( r );

	// set up vertical rotate
	if (IvGame::mGame->mEventHandler->IsKeyDown('e'))
	{
		t -= kPI * 0.25f * dt;
	}
	if (IvGame::mGame->mEventHandler->IsKeyDown('d'))
	{
		t += kPI * 0.25f * dt;
	}
	vertRotate.RotationY(t);
    
    // set up translation
    if (IvGame::mGame->mEventHandler->IsKeyDown('k'))
    {
        x -= 3.0f*dt;
    }
    if (IvGame::mGame->mEventHandler->IsKeyDown('i'))
    {
        x += 3.0f*dt;
    }
    if (IvGame::mGame->mEventHandler->IsKeyDown('l'))
    {
        y -= 3.0f*dt;
    }
    if (IvGame::mGame->mEventHandler->IsKeyDown('j'))
    {
        y += 3.0f*dt;
    }
    IvVector3 xlatevector(x,y,z);
    xlate.Translation( xlatevector );
    
    // clear transform
    if (IvGame::mGame->mEventHandler->IsKeyPressed(' '))
    {
        mTransform.Identity();
    }

	// append transforms for this frame to current transform
	// note order: 
	// since scale is uniform in this example, we can apply first in transform chain
	// then we apply old transform
	// then translate center to origin, rotate around origin,
	// then retranslate to old position
	// then finally, apply new translation
	IvVector3 originalXlate(mTransform(0, 3), mTransform(1, 3), mTransform(2, 3));
	unxform.Translation(-originalXlate);
	rexform.Translation(originalXlate);
    mTransform = xlate*rexform*rotate*vertRotate*unxform*scale*mTransform;
    
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

    // draw character
	Cup().Draw();
}
