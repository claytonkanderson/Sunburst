//
//  BoxTreeNode.hpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/23/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#ifndef BoxTreeNode_hpp
#define BoxTreeNode_hpp

#include "Triangle.h"
#include <vector>
#define MaxTrianglesPerBox 7

class BoxTreeNode {
public:
    BoxTreeNode() {Tri.reserve(MaxTrianglesPerBox); Tri.resize(MaxTrianglesPerBox);}
    ~BoxTreeNode() {}
    bool Intersect(const Ray &ray, Intersection &hit);
    bool IntersectVolume(const Ray &ray, Intersection &hit);
    bool IntersectChildren(const Ray &ray, Intersection &hit);
    void Construct(int count, std::vector<Triangle *> &tri);
    
private:
    glm::vec3 BoxMin, BoxMax;
    int NumChildren = 2;
    int NumTriangles;
    bool IntersectTriangles(const Ray &ray, Intersection &hit);
    BoxTreeNode *Child1, *Child2;
    std::vector<Triangle *> Tri;
};

#endif /* BoxTreeNode_hpp */
