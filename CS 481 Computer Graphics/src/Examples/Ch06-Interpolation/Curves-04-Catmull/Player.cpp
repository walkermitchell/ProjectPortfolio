//===============================================================================
// @ Player.cpp
// ------------------------------------------------------------------------------
// Player
//
// Copyright (C) 2008-2015 by James M. Van Verth and Lars M. Bishop.
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
//
//
//===============================================================================

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#include <IvRenderer.h>
#include <IvEventHandler.h>
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
// @ Player::Player()
//-------------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------------
Player::Player()
{
    mTranslate.Zero();
    
    IvVector3* samplePositions = new IvVector3[4];
    samplePositions[0].Set( -6.0f, 3.0f, 0.0f );
    samplePositions[1].Set( 0.0f, 0.0f, 0.0f );
    samplePositions[2].Set( -3.0f, -3.0f, 0.0f );
    samplePositions[3].Set( 6.0f, 0.0f, 0.0f );

    float* sampleTimes = new float[4];
    sampleTimes[0] = 0.0f;
    sampleTimes[1] = 2.0f;
    sampleTimes[2] = 6.0f;
    sampleTimes[3] = 9.0f;

    mCurve.Initialize( samplePositions, sampleTimes, 4 );

    delete [] samplePositions;
    delete [] sampleTimes;

    mTime = 0.0f;
    mRun = true;

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
    if (IvGame::mGame->mEventHandler->IsKeyPressed(' '))
    {
        if (mRun)
        {
            mTime = 0.0f;
            mRun = false;
        }
        else
        {
            mRun = true;
        }
    }

    if (mRun)
    {
        mTime += dt;
        mTranslate = mCurve.Evaluate( mTime );
    }

}   // End of Player::Update()


//-------------------------------------------------------------------------------
// @ Player::Render()
//-------------------------------------------------------------------------------
// Render stuff
//-------------------------------------------------------------------------------
void 
Player::Render()                                    
{   
    // draw the curve
    mCurve.Render();

    // build 4x4 matrix
    IvMatrix44 transform;
    transform.Translation( mTranslate );
    IvSetWorldMatrix( transform );

    IvDrawTeapot();

}


