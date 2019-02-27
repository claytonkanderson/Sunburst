//
//  MetalMaterial.h
//  CSE_168
//
//  Created by Clayton Anderson on 5/4/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#ifndef MetalMaterial_h
#define MetalMaterial_h

#include "Material.h"

#include <iostream>

class MetalMaterial:public Material {
public:
    void ComputeReflectance(Color &col, const glm::vec3 &lightDir, const glm::vec3 &out, const Intersection &hit) {
        col.Multiply(DiffuseColor);
    }
    
    void GenerateSample(const Intersection &isect,const glm::vec3 &inDir,glm::vec3 &outDir,Color &outColor)
    {
        outDir = normalize(-inDir+2.0f*dot(isect.Normal,inDir)*isect.Normal);
        outColor = DiffuseColor;
    }
    
    float ComputeIntensity(const glm::vec3 &lightDir, const glm::vec3 &normal)
    {
        return 1.0f;
    }
    
    float ComputeDirectIntensity(const glm::vec3 &lightDir, const glm::vec3 &normal)
    {
        return 0.0f;
    }
    
    void SetColor(Color color) {DiffuseColor = color;}
private:
    Color DiffuseColor;

};

#endif /* MetalMaterial_h */
