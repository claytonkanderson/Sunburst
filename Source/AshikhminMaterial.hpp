//
//  AshikhminMaterial.hpp
//  CSE_168
//
//  Created by Clayton Anderson on 5/23/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#ifndef AshikhminMaterial_hpp
#define AshikhminMaterial_hpp

#include "Material.h"
#include "Color.h"
#include <iostream>

#define SHOWVAR(v) std::cout << #v << ": " << v << std::endl
#define SHOWVEC(v) std::cout << #v << ": " << v.x << " " << v.y << " " << v.z << std::endl

using namespace glm;

class AshikhminMaterial : public Material
{
public:
    AshikhminMaterial() {randomGenerator = new RandomGen();
        DiffuseColor = Color::BLACK;
        SpecularColor = Color::BLACK;
        Nu = 0.0f;
        Nv = 0.0f;
        SpecularLevel = 0.0f;
        DiffuseLevel = 0.0f;
    }
    void ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit);
    void GenerateSample(const Intersection &isect,const glm::vec3 &inDir,glm::vec3 &outDir,Color &outColor);
    float ComputeIntensity(const glm::vec3 &lightDir, const glm::vec3 &normal);
    float ComputeDirectIntensity(const glm::vec3 &lightDir, const glm::vec3 &normal);
    
    void SetSpecularColor(Color col) {SpecularColor = col;}
    void SetDiffuseColor(Color col) {DiffuseColor = col;}
    void SetSpecularLevel(float specLevel) {SpecularLevel = specLevel;}
    void SetDiffuseLevel(float diffLevel) {DiffuseLevel = diffLevel;}
    void SetRoughness(float nu, float nv) {Nu = nu; Nv = nv;}
    
    
    Color DiffuseColor;
    Color SpecularColor;
    
    float SpecularLevel;
    float DiffuseLevel;
    float Nu; // Roughness 1
    float Nv; // Roughness 2
private:
    RandomGen * randomGenerator;    
};

#endif /* AshikhminMaterial_hpp */
