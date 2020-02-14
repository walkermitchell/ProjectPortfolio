//===============================================================================
// @ Game.h
// ------------------------------------------------------------------------------
// Game core
//
// Copyright (C) 2008-2015 by James M. Van Verth and Lars M. Bishop.
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
//===============================================================================

#ifndef __GameDefs__
#define __GameDefs__

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#include <IvGame.h>

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------

class Player;

class Game : public IvGame
{
public:
    Game();
    ~Game();
    bool PostRendererInitialize();

    Player* mPlayer;

protected:
    virtual void UpdateObjects( float dt );
	void SetCamera(const IvVector3& viewDir, const IvVector3& viewSide, const IvVector3& viewPos);
	void Orthographic(float left, float right, float bottom, float top, float nearZ, float farZ);
	void Perspective(float fov, float aspect, float nearZ, float farZ);
	void Cabinet(float left, float right, float bottom, float top, float nearZ, float farZ);
	virtual void Render();
    
	IvVector3 mCameraPosition;
	IvVector3 mViewDirection;

	bool mOrthographic;
	bool mZoom;

private:
    Game( const Game& other );
    Game& operator=( const Game& other );

};

#endif
