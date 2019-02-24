//
//  PlaneObj.hpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/11/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#ifndef PlaneObject_hpp
#define PlaneObject_hpp

#include "MeshObject.h"
#include <glm/mat4x4.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class PlaneObject:public Object
{
public:
    PlaneObject() {point = vec3(0,0,0); normal = vec3(0,1,0);}
    PlaneObject(vec3 point, vec3 normal) {point = point; normal = normal;}
    bool Intersect(const Ray &ray, Intersection &hit);
    void SetPoint(vec3 point) {point = point;}
    void SetNormal(vec3 normal) {normal = normal;}
    
private:
    vec3 point;
    vec3 normal;
};


#endif /* PlaneObject_hpp */
