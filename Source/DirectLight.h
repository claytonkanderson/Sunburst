//
//  DirectLight.h
//  CSE_168
//
//  Created by Clayton Anderson on 4/6/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#ifndef DirectLight_h
#define DirectLight_h

#include "Light.h"

class DirectLight:public Light
{
public:
    DirectLight();
    float Illuminate(const glm::vec3 &pos, Color &col, glm::vec3 &toLight, glm::vec3 &ItPos);
    void SetDirection(glm::vec3 dir) {Direction = glm::normalize(dir);}
private:
    glm::vec3 Direction;
};

#endif /* DirectLight_h */
