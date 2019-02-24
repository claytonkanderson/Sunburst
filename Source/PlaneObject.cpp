//
//  PlaneObj.cpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/11/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#include "PlaneObject.hpp"


bool PlaneObject::Intersect(const Ray &ray, Intersection &hit)
{
    vec3 p, d, q;
    
    p = ray.Origin;
    d = ray.Direction;
    
    float dotDN = dot(d, normal);
    
    if (dotDN*dotDN < 0.000001) return false;
    
    float t = dot(q-p , normal) / dotDN;
    
    if (t < 0) return false;
    
    vec3 hitPos = p + t * d;
    float hitDist = distance(p, hitPos);
    
    if (hitDist < minHitDist) return false;
    
    if (hitDist < hit.HitDistance)
    {
        
        hit.Position = hitPos;
        hit.Normal = normal;
        hit.HitDistance = hitDist;
    }
    
    return true;
}
