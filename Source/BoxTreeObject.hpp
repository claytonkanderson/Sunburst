//
//  BoxTreeObject.hpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/23/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#ifndef BoxTreeObject_hpp
#define BoxTreeObject_hpp

#include "Object.h"
#include "MeshObject.h"
#include "BoxTreeNode.hpp"

class BoxTreeObject:public Object {
public:
    BoxTreeObject() {RootNode = new BoxTreeNode;}
    ~BoxTreeObject() {delete RootNode;}
    void Construct(MeshObject &mesh);
    bool Intersect(const Ray &ray, Intersection &hit);
private:
    BoxTreeNode *RootNode;
};

#endif /* BoxTreeObject_hpp */
