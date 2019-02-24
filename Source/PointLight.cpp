//
//  PointLight.cpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/6/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#include "PointLight.h"

PointLight::PointLight()
{
    
}


float PointLight::Illuminate(const glm::vec3 &pos, Color &col, glm::vec3 &toLight, glm::vec3 &ItPos)
{    
    toLight = position - pos;
    float bright = Intensity / glm::dot(toLight, toLight); // Inverse square falloff
    toLight = glm::normalize(toLight);
    col = BaseColor;
    ItPos = position;
    return bright;
}
