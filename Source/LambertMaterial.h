////////////////////////////////////////
// LambertMaterial.h
////////////////////////////////////////

#pragma once

#include "Material.h"

////////////////////////////////////////////////////////////////////////////////

class LambertMaterial:public Material {
public:
    LambertMaterial() {randomGenerator = new RandomGen(); }
    
    void ComputeReflectance(Color &col, const glm::vec3 &lightDir, const glm::vec3 &out, const Intersection &hit) {
        col.Multiply(DiffuseColor);
    }
    
    // Need to rotate outDir so that it's correct for triangles whose normals aren't in the y-dir
    void GenerateSample(const Intersection &isect,const glm::vec3 &inDir,glm::vec3 &outDir,Color &outColor)
    {
        float s, t, u, v;
        s = randomGenerator->GenerateRandom(0,1);
        t = randomGenerator->GenerateRandom(0,1);
        
        u = 2*3.14159*s;
        v = sqrt(1-t);
        
        outDir = normalize(glm::vec3(v*cos(u),sqrt(t),v*sin(u)));
    
        if (l2Norm(isect.Normal - glm::vec3(0,1,0)) > 0.0001f)
        {
            glm::vec3 rotationDirection = glm::normalize(glm::cross(glm::vec3(0,1,0), isect.Normal));
            float angle = glm::angle(glm::vec3(0,1,0), isect.Normal);            
            outDir = glm::vec3(glm::rotate(angle, rotationDirection) * glm::vec4(outDir,0));
        }
        
        outColor = DiffuseColor;
    }
    
    float ComputeIntensity(const glm::vec3 &lightDir, const glm::vec3 &normal)
    {
        return 1.0f;
    }
    
    float ComputeDirectIntensity(const glm::vec3 &lightDir, const glm::vec3 &normal)
    {
        float dotProduct = dot(lightDir, normal);
        float directIllumCoeff = dotProduct < 0 ? 0 : dotProduct;
        return directIllumCoeff;
    }
    
    void SetColor(Color color) {DiffuseColor = color;};
    void SetRandom(unsigned long int seed_in) {
        randomGenerator = new RandomGen(seed_in);
        randomGenerator->GenerateRandom(0,1);
    };
private:
    Color DiffuseColor;
    RandomGen * randomGenerator;
};

////////////////////////////////////////////////////////////////////////////////
