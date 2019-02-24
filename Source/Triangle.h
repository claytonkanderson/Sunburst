////////////////////////////////////////
// Triangle.h
////////////////////////////////////////

#pragma once

#include "Vertex.h"
#include "Material.h"
#include "Ray.h"
#include <iostream>
#include <glm/gtx/norm.hpp>
#define SHOWVEC(v) std::cout << #v << ": " << v.x << " " << v.y << " " << v.z << std::endl


////////////////////////////////////////////////////////////////////////////////

using namespace glm;

class Triangle {
public:
	Triangle();
    void Init(Vertex *v0,Vertex *v1,Vertex *v2,Material *m);

	bool Intersect(const Ray &ray, Intersection &hit) const;
    float GetMinCoord(int i);
    float GetMaxCoord(int i);
    vec3 GetCenter() {return 0.33333f * (Vtx[0]->Position + Vtx[1]->Position + Vtx[2]->Position); }
    Vertex * GetVtx(int index) {return Vtx[index];}
    vec3 Normal;
    
    void UpdateNormal();
    void SetMaterial(Material * material) {Mtl = material;}
    
private:
	Vertex *Vtx[3];    
	Material *Mtl;
    float ep = 0.00001f;
};

////////////////////////////////////////////////////////////////////////////////
