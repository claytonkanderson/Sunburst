//
//  AshikhminMaterial.cpp
//  CSE_168
//
//  Created by Clayton Anderson on 5/23/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "AshikhminMaterial.hpp"


void AshikhminMaterial::ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit)
{
    glm::vec3 k1 = in; // To Light
    glm::vec3 k2 = out; // To Viewer (negative of ray.Direction)
    glm::vec3 n = hit.Normal;
    glm::vec3 h = glm::normalize(k1 + k2); // Assuming in, out are normalized
    
    float hu = dot(h, hit.TangentU);
    float hv = dot(h, hit.TangentV);
    float hn = dot(h, n);
    float hk = dot(h, k1);
    float nk1 = dot(n, k1);
    float nk2 = dot(n, k2);
    
    // Normal opposes viewing direction
    if (nk2 < 0) {
        col.Set(0, 0, 0);
        return;
    }
    
    float specularReflectivity;
    float sqrtTerm = sqrt((Nu+1)*(Nv+1)) / (8.0f*3.14159);
    float exponentTerm = pow(hn, (Nu*hu*hu + Nv*hv*hv) / (1-hn*hn));
    float denom = hk * max(nk1, nk2);
    float schlickProduct = (1-hk)*(1-hk)*(1-hk)*(1-hk)*(1-hk);
    specularReflectivity = sqrtTerm * exponentTerm / denom;
        
	specularReflectivity *= SpecularLevel + (1 - SpecularLevel)*schlickProduct;

    Color specColor;
	specColor.Red = specularReflectivity * SpecularColor.Red;
	specColor.Green = specularReflectivity * SpecularColor.Green;
	specColor.Blue = specularReflectivity * SpecularColor.Blue;;
    
    float diffuseReflectivity;
    diffuseReflectivity = 28.0f/(23.0f*3.14159);
    
    Color diffColor;
    diffColor.Red = DiffuseLevel*DiffuseColor.Red * (1 - SpecularLevel * SpecularColor.Red);
    diffColor.Green = DiffuseLevel*DiffuseColor.Green * (1 - SpecularLevel * SpecularColor.Green);
    diffColor.Blue = DiffuseLevel*DiffuseColor.Blue * (1 - SpecularLevel * SpecularColor.Blue);

    float val1 = (1-0.5f*dot(n,k1));
    float term1 = 1 - val1*val1*val1*val1*val1;    
    float val2 = (1-0.5f*dot(n,k2));
    float term2 = 1 - val2*val2*val2*val2*val2;
    diffuseReflectivity *= term1*term2;
   
    diffColor.Scale(diffuseReflectivity);

    Color netColor;
    netColor = diffColor;
    netColor.Add(specColor);
    col.Multiply(netColor);
}

void AshikhminMaterial::GenerateSample(const Intersection &isect,const glm::vec3 &inDir, glm::vec3 &outDir,Color &outColor)
{
    glm::vec3 in = inDir;
    
    // Need to decide if specular or diffuse
    float p = randomGenerator->GenerateRandom(0.001f, SpecularLevel+DiffuseLevel);
    
    if (p <= SpecularLevel) {
        
        float z1 = randomGenerator->GenerateRandom(0, 1);
        float z2 = randomGenerator->GenerateRandom(0, 1);
        
        float phi = atanf(sqrt((Nu+1.0f)/(Nv+1.0f)) * tanf(3.14159 * 0.5f * z1));
        
        float z3 = randomGenerator->GenerateRandom(0, 4);
        
        if (z3 < 1){ } // Already in first quad
        else if (z3 < 2) { phi = 3.14159 - phi; } // Shift to second quadrant
        else if (z3 < 3) { phi = 3.14159 + phi; } // Third
        else if (z3 < 4) { phi = -phi; } // Fourth
        
        float cosPhi = cosf(phi);
        float sinPhi = sinf(phi);
        float cosTheta = pow((1 - z2), 1.0f / (Nu * cosPhi * cosPhi + Nv * sinPhi * sinPhi + 1));
        float sinTheta = sqrt(1 - cosTheta*cosTheta);
        
        vec3 a = isect.Normal;
        vec3 b = isect.TangentU;
        vec3 c = isect.TangentV;
        vec3 h = sinTheta*cosPhi*b + sinTheta*sinPhi*c + cosTheta*a;
        
        // Material Color
        outColor = SpecularColor;
        outColor.Scale(SpecularLevel);
        
        // Monte-Carlo scaling
        outColor.Scale((DiffuseLevel+SpecularLevel)/SpecularLevel);
        outDir = normalize(-in+2.0f*dot(in,h)*h);
    }
    
    else {
        
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
        
        // Material Color
        outColor = DiffuseColor;
        outColor.Scale(DiffuseLevel);
        
        // Monte-Carlo scaling
        outColor.Scale((DiffuseLevel+SpecularLevel)/DiffuseLevel);
    }
}

float AshikhminMaterial::ComputeIntensity(const glm::vec3 &lightDir, const glm::vec3 &normal)
{
    return 1.0f;
}

float AshikhminMaterial::ComputeDirectIntensity(const glm::vec3 &lightDir, const glm::vec3 &normal)
{
    float dotProduct = dot(lightDir, normal);
    float directIllumCoeff = dotProduct < 0 ? 0 : dotProduct;
    return directIllumCoeff;
}
