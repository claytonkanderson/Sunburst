//
//  SphereObj.hpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/11/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#ifndef SphereObject_hpp
#define SphereObject_hpp

#include "MeshObject.h"
#include <glm/mat4x4.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class SphereObject:public Object
{
public:
    SphereObject() {}
    SphereObject(vec3 center, float radius) {center = center; radius = radius;}
    bool Intersect(const Ray &ray, Intersection &hit);
    void SetCenter(vec3 vec) {center = vec;}
    void SetRadius(float R) {radius = R;}
    
private:
    vec3 center;
    float radius;
};


#endif /* SphereObject_hpp */
