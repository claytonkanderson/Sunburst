////////////////////////////////////////
// MeshObject.h
////////////////////////////////////////

#pragma once

#include "Object.h"
#include "Triangle.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;


class MeshObject:public Object {
public:
	MeshObject();
	~MeshObject();

	bool Intersect(const Ray &ray,Intersection &hit);

	void MakeBox(float x, float y, float z, Material *mtl=nullptr);
    int GetNumTriangles() {return NumTriangles;}
    Triangle ** GetTriangleHead(){return &Triangles;}

	bool LoadPLY(const char *filename, Material *mtl = 0);
	bool LoadOFF(const char *filename, Material *mtl = 0);
	bool LoadOBJ(const char *filename, Material *mtl = 0);
    void Smooth();
    void PrintObj();
    void Translate(vec3 vec);
    void SetMaterial(Material * material) {mtl = material;}
	Material * GetMaterial() { return mtl; }
private:

	size_t NumVertexes;
	size_t NumTriangles;
	Vertex *Vertexes = nullptr;
	std::vector<Vertex> Vertices;
	std::vector<Triangle> Triangles_;
	Triangle *Triangles = nullptr;
	Material * mtl = nullptr;
};

////////////////////////////////////////////////////////////////////////////////
