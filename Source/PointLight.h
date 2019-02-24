//
//  PointLight.h
//  CSE_168
//
//  Created by Clayton Anderson on 4/6/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#ifndef PointLight_h
#define PointLight_h

#include "Light.h"

class PointLight:public Light
{
public:
    PointLight();
    float Illuminate(const glm::vec3 &pos, Color &col, glm::vec3 &toLight, glm::vec3 &ItPos);
    void SetPosition(glm::vec3 pos) {position = pos;}
private:
    glm::vec3 position;
};

#endif /* PointLight_h */
