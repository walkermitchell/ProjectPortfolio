//===============================================================================
// @ Player.h
// ------------------------------------------------------------------------------
// Player object
//
// Copyright (C) 2008-2015 by James M. Van Verth and Lars M. Bishop.
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
//===============================================================================

#ifndef __PlayerDefs__
#define __PlayerDefs__

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#include <IvMatrix44.h>
#include <IvShaderProgram.h>
#include <IvUniform.h>
#include "Cylinder.h"

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------

class Player		//This lighting and shading logic is based on the DonutShadingPhong example in the class repo.
{
public:
    Player();
    ~Player();

	void ComputeMaterialComponents();

    void Update( float dt );
    void Render();

private:
    IvMatrix44  mTransform;
	Cylinder* cyl;
	IvShaderProgram* mShader;

    /*enum { NUM_LIGHTS = 2 };      //This is to support multiple light sources
    unsigned int mCurrentLight;
    IvVector3 mLightPos[NUM_LIGHTS];

    float mSpecularPercentage;
    float mAmbientFactor;
    float mEmissiveFactor;*/

	IvUniform* mLightPositionUniform;
	IvUniform* mLightRadianceUniform;
	IvUniform* mViewPositionUniform;

	IvVector3 mLightPosition;		// position of point light source
	IvVector3 mLightRadiance;		// color of the light source
	IvVector3 mViewPosition;		// camera position in world coordinates
};

#endif
