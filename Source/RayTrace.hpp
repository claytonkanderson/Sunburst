//
//  RayTrace.hpp
//  CSE_168
//
//  Created by Clayton Anderson on 5/4/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#ifndef RayTrace_hpp
#define RayTrace_hpp

#include "Scene.h"
#include "Ray.h"
#include "Intersection.h"
#include "Material.h"
#include "glm/glm.hpp"

using namespace glm;

class RayTrace
{
public:
    RayTrace(Scene &s) {this->Scn = &s; MaxDepth = 10;}
    bool TraceRay(const Ray &ray, Intersection &hit, int depth=1);
    ~RayTrace() {
        std::cout << std::endl;
        std::cout << "RayTrace Destructor" << std::endl;
        std::cout << "Primary Rays: " << PrimaryRays << std::endl;
        std::cout << "Secondary Rays: " << SecondaryRays << std::endl;
        std::cout << "Shadow Rays: " << ShadowRays << std::endl;
    }
    
    // Public so Camera can increment it
    int PrimaryRays = 0;
    
private:
    Scene *Scn;
    
    // Settings
    int MaxDepth;
    
    // Statistics
    int SecondaryRays = 0;
    int ShadowRays = 0;
};
#endif /* RayTrace_hpp */
