//
//  SphereObj.cpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/11/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#include "SphereObject.hpp"

bool SphereObject::Intersect(const Ray &ray, Intersection &hit)
{
    vec3 p, d, q, c;
    p = ray.Origin;
    d = ray.Direction;
    c = center;
    
    q = p - dot(p-c, d) * d;
    
    float qcSquared = dot(q-c,q-c);
    
    if (qcSquared > radius*radius) return false;
    
    float a = sqrt(radius*radius - qcSquared);
    float t = -dot(p-c, d);
    
    if (t - a >= 0)
    {
        vec3 q1 = p + (t - a) * d;
        float hitDist = distance(q1, p);
        
        if (hitDist < minHitDist) return false;
        
        if (hitDist < hit.HitDistance)
        {
            hit.Position = q1;
            hit.Normal = (q1 - c) / radius;
            hit.HitDistance = hitDist;
        }
        
        return true;
    }
    
    else if (t + a >= 0)
    {
        
        vec3 q2 = p + (t + a) * d;
        float hitDist = distance(q2, p);
        
        if (hitDist < minHitDist) return false;
        
        if (hitDist < hit.HitDistance)
        {
            hit.Position = q2;
            hit.Normal = (q2 - c) / radius;
            hit.HitDistance = hitDist;
        }
        
        return true;
    }
    
    return false;
}
