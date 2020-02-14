//===============================================================================
// @ IvClock.cpp
// 
// Time management class
// ------------------------------------------------------------------------------
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
#include <stdlib.h>

#include "IvClock.h"

//-------------------------------------------------------------------------------
//-- Static Members -------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//-- Methods --------------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// @ IvClock::IvClock()
//-------------------------------------------------------------------------------
// Default constructor
//-------------------------------------------------------------------------------
IvClock::IvClock() :
    mCurrentTime( 0.0f )
    ,mElapsedTime( 0.0f )
{
}   // End of IvClock::IvClock()

//-------------------------------------------------------------------------------
// @ IvClock::~IvClock()
//-------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------
IvClock::~IvClock()
{
}   // End of IvClock::~IvClock()
    

//-------------------------------------------------------------------------------
// @ operator<<()
//-------------------------------------------------------------------------------
// Text output for debugging
//-------------------------------------------------------------------------------
IvWriter& 
operator<<(IvWriter& out, const IvClock& source)
{
    out << source.mCurrentTime << ' ' << source.mElapsedTime << eol;

    return out;
    
}   // End of operator<<()


//-------------------------------------------------------------------------------
// @ IvClock::Update()
//-------------------------------------------------------------------------------
// Update the game time
//-------------------------------------------------------------------------------
void
IvClock::Update()
{
    float newTime = ((float)GetTime())*0.001f;
    mElapsedTime = newTime - mCurrentTime;
    mCurrentTime = newTime;
 
}   // End of IvClock::Update()

