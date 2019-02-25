//
//  DirectLight.cpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/6/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "DirectLight.h"

DirectLight::DirectLight()
{
    
}

float DirectLight::Illuminate(const glm::vec3 &pos, Color &col, glm::vec3 &toLight, glm::vec3 &ItPos)
{
    toLight = -Direction;
    col = BaseColor;
    ItPos = pos - 1000000.0f * Direction; // Create virtual position
    return Intensity;
}
