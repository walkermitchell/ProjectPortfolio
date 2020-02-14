//===============================================================================
// @ Player.h
// ------------------------------------------------------------------------------
// Main player
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

#ifndef __PlayerDefs__
#define __PlayerDefs__

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#include <IvQuat.h>
#include <IvVector3.h>

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------

class Player
{
public:
    Player();
    ~Player();

    void Update( float dt );
    void Render();

private:
    // run parameters
    float       mTime;
    bool        mRun;
    bool        mCorrection;

    // current lerped and slerped orientations
    IvQuat      mLerpRotate;
    IvQuat      mSlerpRotate;

    // start and end orientations to be interpolated
    IvQuat      mStartRotate;
    IvQuat      mEndRotate;
};

#endif
