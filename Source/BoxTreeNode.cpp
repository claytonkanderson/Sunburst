//
//  BoxTreeNode.cpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/23/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#include "BoxTreeNode.hpp"


// Want intersect to only return true if a triangle was intersected
bool BoxTreeNode::Intersect(const Ray &ray, Intersection &hit)
{
    //    std::cout << "BoxTreeNode intersect" << std::endl;
    
    // If we use hit intersection and there is a closer hit then tempHit.HitDistance will equal
    //     hit.HitDistance, so use a tempHit to do the intersection
    Intersection tempHit;
    IntersectVolume(ray, tempHit);
    bool success = false;
    if (tempHit.HitDistance < hit.HitDistance)
    {
        success = IntersectChildren(ray, hit);
    }
    
    // Or just do this, yeah no, don't want to save volume hits in hit
    // Doing this saves the hit against largest box and all subsequent hits are farther away
    // DOES NOT WORK
    //    if (IntersectVolume(ray, hit))
    //    {
    ////        std::cout << "Intersected a volume with dist: " << hit.HitDistance << std::endl;
    //        return IntersectChildren(ray, hit);
    //    }
    //    std::cout << "BoxTreeNode intersect returning... " << success << std::endl;
    return success;
}

bool BoxTreeNode::IntersectVolume(const Ray &ray, Intersection &hit)
{
    //    std::cout << "Calling BoxTreeNode IntersectVolume" << std::endl;
    
    vec3 a = BoxMin;
    vec3 b = BoxMax;
    vec3 p = ray.Origin;
    vec3 d = ray.Direction;
    
    //    if (a.x < p.x && p.x < b.x)
    //        if (a.y < p.y && p.y < b.y)
    //            if (a.z < p.z && p.z < b.z)
    //            {
    //                std::cout << "Ray started in box" << std::endl;
    //                return true;
    //            }
    
    float tx1, tx2, ty1, ty2, tz1, tz2;
    
    tx1 = (a.x - p.x) / d.x;
    ty1 = (a.y - p.y) / d.y;
    tz1 = (a.z - p.z) / d.z;
    
    tx2 = (b.x - p.x) / d.x;
    ty2 = (b.y - p.y) / d.y;
    tz2 = (b.z - p.z) / d.z;
    
    float tmin = max(min(tx1,tx2),min(ty1,ty2));
    tmin = max(tmin, min(tz1,tz2));
    
    float tmax = min(max(tx1,tx2),max(ty1,ty2));
    tmax = min(tmax, max(tz1, tz2));
    
    if (tmax < 0) return false;
    
    if (tmin > tmax) return false;
    
    //    float hitDist = tmin;
    //    if (hitDist < hit.HitDistance)
    //    {
    //        hit.HitDistance = hitDist;
    //        hit.Position = p + d * hitDist;
    //        return true;
    //    }
    
    if (tmin > 0) {
        float hitDist = tmin;
        if (hitDist < hit.HitDistance)
        {
            hit.HitDistance = hitDist;
            hit.Position = p + d * hitDist;
            return true;
        }
    }
    
    else {
        float hitDist = tmax;
        if (hitDist < hit.HitDistance)
        {
            hit.HitDistance = hitDist;
            hit.Position = p + d * hitDist;
            return true;
        }
    }
    
    return false;
}

bool BoxTreeNode::IntersectTriangles(const Ray &ray, Intersection &hit)
{
    bool success=false;
    for (int i = 0; i < NumTriangles; i++) {
        if (Tri[i]->Intersect(ray, hit)) success = true;
    }
    return success;
}

bool BoxTreeNode::IntersectChildren(const Ray &ray, Intersection &hit)
{
    //    std::cout << "Calling BoxTreeNode IntersectChildren" << std::endl;
    // If this is a leaf node, test against triangles
    if(NumChildren==0)
    {
        return IntersectTriangles(ray,hit);
    }
    
    
    // Test all child volumes
    Intersection volhit[2];
    bool hits[2];
    hits[0] = false;
    hits[1] = false;
    
    volhit[0].HitDistance=hit.HitDistance;
    hits[0] = Child1->IntersectVolume(ray,volhit[0]);
    
    
    volhit[1].HitDistance=hit.HitDistance;
    hits[1] = Child2->IntersectVolume(ray,volhit[1]);
    
    
    // Full recursive test on children, sorted in order of distance
    bool success=false;
    
    if (hits[0] && hits[1])
    {
        if (volhit[0].HitDistance < volhit[1].HitDistance)
        {
            if (volhit[0].HitDistance < hit.HitDistance)
            {
                if (Child1->IntersectChildren(ray, hit)) success = true;
            }
            
            if (volhit[1].HitDistance < hit.HitDistance)
            {
                if (Child2->IntersectChildren(ray, hit)) success = true;
            }
        }
        
        else
        {
            if (volhit[1].HitDistance < hit.HitDistance)
            {
                if (Child2->IntersectChildren(ray, hit)) success = true;
            }
            
            if (volhit[0].HitDistance < hit.HitDistance)
            {
                if (Child1->IntersectChildren(ray, hit)) success = true;
            }
        }
    }
    else
    {
        
        // Want to search whichever hit was closer first
        if (hits[0])
            if (volhit[0].HitDistance < hit.HitDistance)
                if (Child1->IntersectChildren(ray, hit)) success = true;
        
        if (hits[1])
            if (volhit[1].HitDistance < hit.HitDistance)
                if (Child2->IntersectChildren(ray, hit)) success = true;
    }
    return success;
}

void BoxTreeNode::Construct(int count, std::vector<Triangle *> &tri) {
    // Compute BoxMin & BoxMax to fit around all tri’s
    // Check if this is a leaf node
    assert(count != 0);
    
    // Determine largest box dimension x, y, or z
    // Compute splitting plane halfway along largest dimension
    float minX, minY, minZ;
    minX = tri[0]->GetMinCoord(0);
    minY = tri[0]->GetMinCoord(1);
    minZ = tri[0]->GetMinCoord(2);
    float maxX, maxY, maxZ;
    maxX = tri[0]->GetMaxCoord(0);
    maxY = tri[0]->GetMaxCoord(1);
    maxZ = tri[0]->GetMaxCoord(2);
    
    for(unsigned int i = 0; i < count; i++) {
        if (tri[i]->GetMinCoord(0) < minX) minX = tri[i]->GetMinCoord(0);
        if (tri[i]->GetMinCoord(1) < minY) minY = tri[i]->GetMinCoord(1);
        if (tri[i]->GetMinCoord(2) < minZ) minZ = tri[i]->GetMinCoord(2);
        
        if (maxX < tri[i]->GetMaxCoord(0)) maxX = tri[i]->GetMaxCoord(0);
        if (maxY < tri[i]->GetMaxCoord(1)) maxY = tri[i]->GetMaxCoord(1);
        if (maxZ < tri[i]->GetMaxCoord(2)) maxZ = tri[i]->GetMaxCoord(2);
    }
    
    int axis = 0;
    float xDim = maxX - minX; float xCenter = (maxX + minX) / 2.0f;
    float yDim = maxY - minY; float yCenter = (maxY + minY) / 2.0f;
    float zDim = maxZ - minZ; float zCenter = (maxZ + minZ) / 2.0f;
    float maxDim = xDim;
    float center = xCenter;
    if (maxDim < yDim) {axis = 1; maxDim = yDim; center = yCenter;}
    if (maxDim < zDim) {axis = 2; maxDim = zDim; center = zCenter;}
    
    BoxMin = vec3(minX, minY, minZ);
    BoxMax = vec3(maxX, maxY, maxZ);
    
    
    if(count <= MaxTrianglesPerBox) {
        // Copy triangles to BoxTreeNode’s Tri array
        for (int i = 0 ; i < count; i++)
        {
            Tri[i] = tri[i];
        }
        
        NumTriangles = count;
        NumChildren = 0;
        return;
    }
    
    // Allocate two new temporary vectors
    std::vector<Triangle *> tri1;
    tri1.reserve(count);
    tri1.resize(count);
    
    std::vector<Triangle *> tri2;
    tri2.reserve(count);
    tri2.resize(count);
    int count1=0, count2=0;
    
    // Place triangles into group 1 or group 2
    // Compute center of triangle & determine which side of splitting plane
    // Add to appropriate group
    vec3 triCenter(0,0,0);
    for(unsigned int i = 0; i < count; i++) {
        triCenter = tri[i]->GetCenter();
        
        if (triCenter[axis] < center)
        {
            tri1[count1] = tri[i];
            count1++;
        }
        else {
            tri2[count2] = tri[i];
            count2++;
        }
    }
    
    // Check if either group is empty. If so, move (at least) 1 triangle into that group
    //    assert((count1 > 1) || count2 > 1);
    
    if (count1 == 0) {
        tri1[count1] = tri2[count2 - 1];
        count2--;
        count1++;
    }
    
    if (count2 == 0) {
        tri2[count2] = tri1[count1 - 1];
        count1--;
        count2++;
    }
    
    // Recursively build sub-trees
    Child1=new BoxTreeNode;
    Child2=new BoxTreeNode;
    Child1->NumTriangles = count1;
    Child1->Construct(count1,tri1);
    Child2->NumTriangles = count2;
    Child2->Construct(count2,tri2);
    
    tri1.clear();
    tri2.clear();
    
}
