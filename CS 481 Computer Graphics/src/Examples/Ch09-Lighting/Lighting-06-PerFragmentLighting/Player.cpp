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
//
//===============================================================================

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#include <IvEventHandler.h>
#include <IvFragmentShader.h>
#include <IvImage.h>
#include <IvMath.h>
#include <IvRenderer.h>
#include <IvRendererHelp.h>
#include <IvResourceManager.h>
#include <IvShaderProgram.h>
#include <IvUniform.h>
#include <IvVector3.h>
#include <IvVertexShader.h>

#include <IvIndexBuffer.h>
#include <IvTexture.h>
#include <IvVertexBuffer.h>

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
    mRotate.Identity();
    mScale = 1.0f;
    mTranslate.Zero();

    mPlaneIndices = nullptr;

    mPlaneVerts = nullptr;

    mVertlitShader = IvRenderer::mRenderer->GetResourceManager()->CreateShaderProgram(
        IvRenderer::mRenderer->GetResourceManager()->CreateVertexShaderFromFile(
        "vertlitShader"),
        IvRenderer::mRenderer->GetResourceManager()->CreateFragmentShaderFromFile(
        "vertlitShader"));

    mFraglitShader = IvRenderer::mRenderer->GetResourceManager()->CreateShaderProgram(
        IvRenderer::mRenderer->GetResourceManager()->CreateVertexShaderFromFile(
        "fraglitShader"),
        IvRenderer::mRenderer->GetResourceManager()->CreateFragmentShaderFromFile(
        "fraglitShader"));

    mLightPos = IvVector3(0.0f, 0.0f, 1.0f);

    mShader = mFraglitShader;

    CreatePlane();
}   // End of Player::Player()


//-------------------------------------------------------------------------------
// @ Player::~Player()
//-------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------
Player::~Player()
{
    IvRenderer::mRenderer->GetResourceManager()->Destroy(mPlaneIndices);

    IvRenderer::mRenderer->GetResourceManager()->Destroy(mPlaneVerts);

    IvRenderer::mRenderer->GetResourceManager()->Destroy(mVertlitShader);
    IvRenderer::mRenderer->GetResourceManager()->Destroy(mFraglitShader);
}   // End of Player::~Player()


//-------------------------------------------------------------------------------
// @ Player::Update()
//-------------------------------------------------------------------------------
// Main update loop
//-------------------------------------------------------------------------------
void
Player::Update( float dt )
{
    // get scale, rotate, translate for this frame
    float s = 1.0f;
    float r = 0.0f;
    
    // set up scaling
    if (IvGame::mGame->mEventHandler->IsKeyDown(';'))
    {
        s -= 0.25f*dt;
    }
    if (IvGame::mGame->mEventHandler->IsKeyDown('p'))
    {
        s += 0.25f*dt;
    }
    mScale *= s;
    
    // set up rotate
    if (IvGame::mGame->mEventHandler->IsKeyDown('o'))
    {
        r -= kPI*0.25f*dt;
    }
    if (IvGame::mGame->mEventHandler->IsKeyDown('u'))
    {
        r += kPI*0.25f*dt;
    }
    IvMatrix33 rotate;
    rotate.RotationZ(r);
    mRotate = rotate*mRotate;
    
    // set up translation
    IvVector3 xlate;
    xlate.Zero();
    if (IvGame::mGame->mEventHandler->IsKeyDown('k'))
    {
        xlate.x -= 3.0f*dt;
    }
    if (IvGame::mGame->mEventHandler->IsKeyDown('i'))
    {
        xlate.x += 3.0f*dt;
    }
    if (IvGame::mGame->mEventHandler->IsKeyDown('l'))
    {
        xlate.y -= 3.0f*dt;
    }
    if (IvGame::mGame->mEventHandler->IsKeyDown('j'))
    {
        xlate.y += 3.0f*dt;
    }
    mTranslate += xlate;
    
    // clear transform
    if (IvGame::mGame->mEventHandler->IsKeyDown(' '))
    {
        mRotate.Identity();
        mScale = 1.0f;
        mTranslate.Zero();
    }

    r = 0.0f;
    static bool lightDirChanged = true;

    // set up rotate
    if (IvGame::mGame->mEventHandler->IsKeyDown('a'))
    {
        r -= kPI*0.0625f*dt;
        lightDirChanged = true;
    }
    if (IvGame::mGame->mEventHandler->IsKeyDown('d'))
    {
        r += kPI*0.0625f*dt;
        lightDirChanged = true;
    }

    if (lightDirChanged)
    {
        IvMatrix33 rotate;
        rotate.RotationX(r);
       
        mLightPos = rotate * mLightPos;
        mLightPos.Normalize();

        lightDirChanged = false;
    }

    if (IvGame::mGame->mEventHandler->IsKeyDown('f'))
    {
        mShader = mFraglitShader;
    }
    if (IvGame::mGame->mEventHandler->IsKeyDown('v'))
    {
        mShader = mVertlitShader;
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
    // build 4x4 matrix
    IvMatrix44 transform(mRotate);

    transform(0,0) *= mScale;
    transform(1,0) *= mScale;
    transform(2,0) *= mScale;
    transform(0,1) *= mScale;
    transform(1,1) *= mScale;
    transform(2,1) *= mScale;
    transform(0,2) *= mScale;
    transform(1,2) *= mScale;
    transform(2,2) *= mScale;
    transform(0,3) = mTranslate.x;
    transform(1,3) = mTranslate.y;
    transform(2,3) = mTranslate.z;

    IvRenderer::mRenderer->SetShaderProgram(mShader);
    mShader->GetUniform("dirLightPosition")->SetValue(
        mLightPos, 0);

    IvSetWorldMatrix(transform);

    // draw geometry
    DrawPlane();

}   // End of Player::Render()


//-------------------------------------------------------------------------------
// @ Player::DrawPlane()
//-------------------------------------------------------------------------------
// Draw vertex arrays for a plane centered around the origin
//-------------------------------------------------------------------------------
void
Player::DrawPlane()
{
    IvRenderer::mRenderer->Draw(kTriangleStripPrim, mPlaneVerts, mPlaneIndices);
} // End of Player::DrawPlane()
    
//-------------------------------------------------------------------------------
// @ Player::CreatePlane()
//-------------------------------------------------------------------------------
// Create vertex arrays for a plane centered around the origin
//-------------------------------------------------------------------------------
void 
Player::CreatePlane()                                    
{
    mPlaneVerts = IvRenderer::mRenderer->GetResourceManager()->CreateVertexBuffer(
        kNPFormat, 4, nullptr, kDefaultUsage);

    IvNPVertex* tempVerts = (IvNPVertex*)(mPlaneVerts->BeginLoadData());

    tempVerts->position = IvVector3(-5.0f, -5.0f, 0.0f);
    tempVerts->normal = IvVector3(-1.0f, -1.0f, 0.15f);
    tempVerts->normal.Normalize();

    tempVerts++;

    tempVerts->position = IvVector3(5.0f, -5.0f, 0.0f);
    tempVerts->normal = IvVector3(1.0f, -1.0f, 0.15f);
    tempVerts->normal.Normalize();

    tempVerts++;

    tempVerts->position = IvVector3(-5.0f, 5.0f, 0.0f);
    tempVerts->normal = IvVector3(-1.0f, 1.0f, 0.15f);
    tempVerts->normal.Normalize();

    tempVerts++;

    tempVerts->position = IvVector3(5.0f, 5.0f, 0.0f);
    tempVerts->normal = IvVector3(1.0f, 1.0f, 0.15f);
    tempVerts->normal.Normalize();

    mPlaneVerts->EndLoadData();

    mPlaneIndices = IvRenderer::mRenderer->GetResourceManager()->CreateIndexBuffer(4, nullptr,
                                                                                   kDefaultUsage);

    UInt32* tempIndices = (UInt32*)mPlaneIndices->BeginLoadData();

    UInt32 i;
    for (i = 0; i < 4; i++)
        tempIndices[i] = i;


    mPlaneIndices->EndLoadData();

}   // End of Player::CreatePlane()



