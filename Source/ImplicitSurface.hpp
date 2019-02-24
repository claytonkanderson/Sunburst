//
//  ImplicitSurface.hpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/11/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#ifndef ImplicitSurface_hpp
#define ImplicitSurface_hpp

#include "Object.h"
#include <glm/mat4x4.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <boost/math/tools/roots.hpp>
#include <vector>
#include <algorithm>
#include "glm/gtc/random.hpp"
#define SHOWVEC(v) std::cout << #v << ": " << v.x << " " << v.y << " " << v.z << std::endl

using namespace glm;
using namespace boost::math::tools;

class ImplicitSurface:public Object
{
public:
    ImplicitSurface() {}
    ImplicitSurface( float (*f)(float, float ,float) ) {fun = f;}
    bool Intersect(const Ray &ray, Intersection &hit);
    
    void SetF(float (*f)(float, float ,float) ){fun = f;}
    void SetGrad(std::tuple<float, float, float> (*f)(float, float, float)) {grad = f;}
    float EvaluateF(float x, float y, float z) {return fun(x,y,z); }
    std::tuple<float,float,float> EvaluateGrad(float x, float y, float z) {return grad(x,y,z); }
    std::vector<float> freqs;
    void SetFandG();
    
private:
    float (*fun)(float, float, float);
    
    std::tuple<float, float, float> (*grad)(float, float, float);
};


#endif /* ImplicitSurface_hpp */
