//
//  InstanceObject.h
//  CSE_168
//
//  Created by Clayton Anderson on 4/6/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#ifndef InstanceObject_h
#define InstanceObject_h

#include "MeshObject.h"
#include "AshikhminMaterial.hpp"
#include <glm/mat4x4.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

class InstanceObject:public Object
{
public:
    InstanceObject();
    InstanceObject(Object *obj);
    bool Intersect(const Ray &ray, Intersection &hit);
    void SetChild(Object &obj);
    void SetMatrix(glm::mat4 &mtx);
    void SetMaterial(Material *material) {mtl = material;}
    
    AshikhminMaterial * lowMtl;
    AshikhminMaterial * highMtl;
    AshikhminMaterial blendedMaterial;
    bool MultiMaterialFlag = false;
    int BlendType = 0;
private:
    glm::mat4 Matrix;
    glm::mat4 Inverse;
    Object * Child;
    Material * mtl;
    
};

#endif /* InstanceObject_h */
