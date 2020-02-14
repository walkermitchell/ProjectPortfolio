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

#include <iostream>
#include <IvEventHandler.h>
#include <IvFragmentShader.h>
#include <IvIndexBuffer.h>
#include <IvMath.h>
#include <IvRenderer.h>
#include <IvRendererHelp.h>
#include <IvResourceManager.h>
#include <IvShaderProgram.h>
#include <IvVector2.h>
#include <IvVector3.h>
#include <IvVector4.h>
#include <IvVertexBuffer.h>
#include <IvVertexShader.h>
#include <IvUniform.h>

#include "Player.h"
#include "Game.h"
#include "Cylinder.h"

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
Player::Player()        // This constructor is based on the DonutShadingPhong example in the class repo 
{                       // and SurfaceOfRevolutionWithTexture.
    cyl = new Cylinder();
    mShader = IvRenderer::mRenderer->GetResourceManager()->CreateShaderProgram(
        IvRenderer::mRenderer->GetResourceManager()->CreateVertexShaderFromFile("PointPhong"),
        IvRenderer::mRenderer->GetResourceManager()->CreateFragmentShaderFromFile("PointPhong"));

    if (mShader == NULL)
    {
        // shader failed to compile
        std::cout << "Shader failed to compile" << std::endl;
        return;
    }

    IvRenderer::mRenderer->SetShaderProgram(mShader);

    // Set up point light source
    mLightPosition.Set(-10.0f, -10.0f, 10.0f);	// back up and to the right of the camera
    mLightRadiance.Set(0.6f, 0.8f, 1.0f);		// Slightly green-blue

    mLightPositionUniform = mShader->GetUniform("lightPosition");
    mLightRadianceUniform = mShader->GetUniform("lightRadiance");

    mLightPositionUniform->SetValue(mLightPosition, 0);
    mLightRadianceUniform->SetValue(mLightRadiance, 0);

    // send texture
    IvUniform* texUniform = mShader->GetUniform("textureLU");
    if (texUniform != NULL)
    {
        texUniform->SetValue(cyl->GetTexture());
    }


    mViewPosition.Set(-10.f, 2.0f, 10.0f);
    // sets standard lookat(0,0,0) from this position
    IvSetDefaultViewer(mViewPosition.x, mViewPosition.y, mViewPosition.z);

    mViewPositionUniform = mShader->GetUniform("viewPosition");
    mViewPositionUniform->SetValue(mViewPosition, 0);
}   // End of Player::Player()


//-------------------------------------------------------------------------------
// @ Player::~Player()
//-------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------
Player::~Player()
{
    IvRenderer::mRenderer->GetResourceManager()->Destroy(mShader);
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

    if (IvGame::mGame->mEventHandler->IsKeyDown('s'))
    {
        z -= 3.0f * dt;
    }
    if (IvGame::mGame->mEventHandler->IsKeyDown('w'))
    {
        z += 3.0f * dt;
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
    mShader->GetUniform("modelMatrix")->SetValue(mTransform, 0);

    // draw character
	cyl->Draw();
}
