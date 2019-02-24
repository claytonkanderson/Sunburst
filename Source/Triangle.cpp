//
//  Triangle.cpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/9/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#include "Triangle.h"

Triangle::Triangle() {}

void Triangle::Init(Vertex *v0,Vertex *v1,Vertex *v2,Material *m)
{
    Vtx[0]=v0; Vtx[1]=v1; Vtx[2]=v2; Mtl=m;
//        Normal = normalize(cross(Vtx[1]->Position - Vtx[0]->Position, Vtx[2]->Position - Vtx[0]->Position));
    Normal = cross(Vtx[1]->Position - Vtx[0]->Position, Vtx[2]->Position - Vtx[0]->Position);    
}

void Triangle::UpdateNormal()
{
    Normal = cross(Vtx[1]->Position - Vtx[0]->Position, Vtx[2]->Position - Vtx[0]->Position);
}

float Triangle::GetMinCoord(int i)
{
    if (i == 0)
    {
        float x = Vtx[0]->Position.x;
        if (Vtx[1]->Position.x < x) x = Vtx[1]->Position.x;
        if (Vtx[2]->Position.x < x) x = Vtx[2]->Position.x;
        return x;
    }
    
    else if (i == 1)
    {
        float y = Vtx[0]->Position.y;
        if (Vtx[1]->Position.y < y) y = Vtx[1]->Position.y;
        if (Vtx[2]->Position.y < y) y = Vtx[2]->Position.y;
        return y;
    }
    
    else if (i == 2)
    {
        float z = Vtx[0]->Position.z;
        if (Vtx[1]->Position.z < z) z = Vtx[1]->Position.z;
        if (Vtx[2]->Position.z < z) z = Vtx[2]->Position.z;
        return z;
    }
    
    else return 0.0f;
}

float Triangle::GetMaxCoord(int i)
{
    if (i == 0)
    {
        float x = Vtx[0]->Position.x;
        if (x < Vtx[1]->Position.x) x = Vtx[1]->Position.x;
        if (x < Vtx[2]->Position.x) x = Vtx[2]->Position.x;
        return x;
    }
    
    else if (i == 1)
    {
        float y = Vtx[0]->Position.y;
        if (y < Vtx[1]->Position.y) y = Vtx[1]->Position.y;
        if (y < Vtx[2]->Position.y) y = Vtx[2]->Position.y;
        return y;
    }
    
    else if (i == 2)
    {
        float z = Vtx[0]->Position.z;
        if (z < Vtx[1]->Position.z) z = Vtx[1]->Position.z;
        if (z < Vtx[2]->Position.z) z = Vtx[2]->Position.z;
        return z;
    }
    
    else return 0.0f;
}

bool Triangle::Intersect(const Ray &ray, Intersection &hit) const
{
    vec3 d = ray.Direction;
    vec3 p = ray.Origin;
    vec3 a = Vtx[0]->Position;
    vec3 b = Vtx[1]->Position;
    vec3 c = Vtx[2]->Position;
    
    float detM = -dot(d, Normal);
    
    detM = 1.0 / detM;
    if (!isfinite(detM)) return false;
    
    float beta = -dot(d, cross(b-a,p-a)) * detM;
    if (beta < 0) return false;
    
    float alpha = -dot(d, cross(p-a,c-a)) * detM;
    if (alpha < 0) return false;
    
    if (alpha + beta > 1) return false;
    
    float t = dot(p-a, Normal) * detM;
    if (t < 0) return false;
    
    //    vec3 hitPos = a + alpha * (b-a) + beta * (c-a);
    //    float hitDist = l2Norm(hitPos - p);
    vec3 hitPos = p + d * t;
    float hitDist = t;
    
    // Ignore hits that are too close
    if (hitDist < 0.1) return false;
    
    // Only accept if the new hit is closer than the previous hit
    if (hitDist < hit.HitDistance)
    {
        hit.Normal = normalize((1 - alpha - beta) * Vtx[0]->Normal + alpha * Vtx[1]->Normal + beta * Vtx[2]->Normal);
        if (dot(hit.Normal, d) > 0) hit.Normal *= -1.0f;
//        if (hit.Normal.y < 0) hit.Normal *= -1.0f;
        hit.Mtl = Mtl;
        hit.Position = hitPos;
        hit.HitDistance = hitDist;
        
        hit.TangentU = cross(vec3(0,1,0), hit.Normal);
        if (length(hit.TangentU) < 0.0001)
            hit.TangentU = cross(vec3(1,0,0), hit.Normal);
        hit.TangentU = normalize(hit.TangentU);
        hit.TangentV = cross(hit.Normal,hit.TangentU);
    }
    
    return true;
}
