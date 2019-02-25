//
//  RayTrace.cpp
//  CSE_168
//
//  Created by Clayton Anderson on 5/4/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//
#define GLM_ENABLE_EXPERIMENTAL

#include "RayTrace.hpp"

bool RayTrace::TraceRay(const Ray &ray, Intersection &hit, int depth)
{
    
    // Find Ray Intersection
    if(Scn->Intersect(ray,hit) == false)
    {
        hit.Shade = Scn->GetSkyColor();
        return false;
    }

    // Compute shade due to lighting
    Color colorLight;
    
    for (int i = 0; i < Scn->GetNumLights(); i++)
    {
        ShadowRays++;
        
        vec3 toLight;
        vec3 lightPos;
        float intensity;
        
        // Set colorLight's color and intensity to be that of the light source
        intensity = Scn->GetLight(i).Illuminate(hit.Position, colorLight, toLight, lightPos);
        colorLight.Scale(intensity);
        
        float lightDot = dot(toLight, hit.Normal);
        if (lightDot < 0) continue;
        
        Ray shadowRay;
        Intersection shadowIntersection;
        shadowRay.Origin = hit.Position;
        shadowRay.Direction = toLight;
        shadowRay.type = 1;
        
        shadowIntersection.HitDistance = 1e10;
        
        if (Scn->Intersect(shadowRay, shadowIntersection))
        {
            // Could optimize this a bit more
            float intersectionDist = l2Norm(shadowIntersection.Position -shadowRay.Origin);
            float lightDist = l2Norm(lightPos - shadowRay.Origin);
            if (intersectionDist < lightDist) continue;
        }
        
        Color materialColor;
        
        // toLight is the 'inDir' of ComputeReflectance
        // The outDir should be the negative ray direction (to go back towards the camera)
        hit.Mtl->ComputeReflectance(materialColor, toLight, -ray.Direction, hit);
        float directIllumCoeff = hit.Mtl->ComputeDirectIntensity(toLight, hit.Normal);
        
        materialColor.Scale(directIllumCoeff);
        
        // Combine light color & intensity with material color & reflectivity
        colorLight.Multiply(materialColor);
        hit.Shade.Add(colorLight);
    }

    if (depth == MaxDepth) return true;
    
    SecondaryRays++;
    vec3 reflectionDir;
    Color reflectionColor; // Really the reflection intensity
    hit.Mtl->GenerateSample(hit, -ray.Direction, reflectionDir, reflectionColor);
    
    Color materialColor;
    
    // Light comes in in the -ray.Direction direction and goes out in the reflection direction
//    hit.Mtl->ComputeReflectance(materialColor, -ray.Direction, reflectionDir, hit);
    
    Ray reflectionRay;
    reflectionRay.Origin = hit.Position;
    reflectionRay.Direction = reflectionDir;
    reflectionRay.type = 2;

    Intersection reflectionHit;
    reflectionHit.HitDistance = 1e10;
    reflectionHit.Shade = Color::BLACK;
    
    // Get light color from reflection hit
    TraceRay(reflectionRay, reflectionHit, depth+1);
    reflectionHit.Shade.Multiply(reflectionColor); // Ray intensity Weighting (Monte Carlo + Reflectance?)
//    materialColor.Multiply(reflectionHit.Shade);   // Material Reflectance Weighting
//    hit.Shade.Add(materialColor);
    hit.Shade.Add(reflectionHit.Shade);
    
    return true;
}
