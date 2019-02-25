//
//  BoxTreeObject.cpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/23/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "BoxTreeObject.hpp"

void BoxTreeObject::Construct(MeshObject &mesh)
{
    Triangle ** tri = mesh.GetTriangleHead();

    unsigned int numTris = mesh.GetNumTriangles();
//    std::vector<Triangle *> triangles;
    std::vector<Triangle *> triangles;
    triangles.reserve(numTris);
    triangles.resize(numTris);
    
    for (unsigned int i = 0 ; i < numTris; i++)
    {
        triangles[i] = (*tri) + i;
    }
    
    RootNode->Construct(mesh.GetNumTriangles(), triangles);
    triangles.clear();
}
bool BoxTreeObject::Intersect(const Ray &ray, Intersection &hit)
{
    bool success = false;
    success = RootNode->Intersect(ray, hit);
    return success;
}
