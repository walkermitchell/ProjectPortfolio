//===============================================================================
// @ Barrel.h
// ------------------------------------------------------------------------------
// Barrel of player
//
// Copyright (C) 2008-2015 by James M. Van Verth and Lars M. Bishop.
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
//===============================================================================

#ifndef __BarrelDefs__
#define __BarrelDefs__

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#include <IvMatrix33.h>
#include <IvVector3.h>

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------

class Barrel
{
public:
    Barrel();
    ~Barrel();

    void Update( float dt );
    void Render( float parentScale, 
                 const IvMatrix33& parentRotate, 
                 const IvVector3& parentTranslation );

    bool ReadData();

private:
    void CleanData();
    void DrawBox();

    // transformation
    float       mScale;
    IvMatrix33  mRotate;
    IvVector3   mTranslate;

    // geometry
    IvVertexBuffer* mVertices;
    IvIndexBuffer*  mIndices;
    
};

#endif
