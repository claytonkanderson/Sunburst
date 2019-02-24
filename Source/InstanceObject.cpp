//
//  InstanceObject.cpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/9/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#include "InstanceObject.h"
InstanceObject::InstanceObject() {}
InstanceObject::InstanceObject(Object * obj)
{
    Child = obj;
}
bool InstanceObject::Intersect(const Ray &ray, Intersection &hit)
{
    Ray ray2;
    Intersection hit2;
    
    // Transform to Material Frame
    ray2.Origin=glm::vec3( Inverse * glm::vec4(ray.Origin,1) );
    ray2.Direction=glm::vec3( Inverse * glm::vec4(ray.Direction,0));
    
    // Do intersection test in Material Frame
    if(Child->Intersect(ray2, hit2)==false) return false;
    
    
    // Convert back to World Frame
    glm::vec3 hitPos = glm::vec3( Matrix * glm::vec4(hit2.Position,1) );
    float hitDist = glm::distance(ray.Origin, hitPos);
    
    // Accept hit if it's closer than current hit
    if (hitDist < hit.HitDistance)
    {
        hit.Position = hitPos;
        hit.Normal=glm::vec3( Matrix * glm::vec4(hit2.Normal,0));
        hit.TangentU=glm::vec3( Matrix * glm::vec4(hit2.TangentU,0));
        hit.TangentV=glm::vec3( Matrix * glm::vec4(hit2.TangentV,0));
        hit.HitDistance = hitDist;
        hit.Mtl = mtl;
        
        if (MultiMaterialFlag)
        {
            if (BlendType == 0)
            {
                
                float lower = -60;
                float upper = 60;
                
                if (hit.Position.y < lower)
                    hit.Mtl = lowMtl;
                else if ( hit.Position.y > upper)
                    hit.Mtl = highMtl;
                else
                {
                    float t = (hit.Position.y - lower) / (upper - lower);
                    float dl = highMtl->DiffuseLevel + (1-t)*(lowMtl->DiffuseLevel - highMtl->DiffuseLevel);
                    float sl = highMtl->SpecularLevel + (1-t)*(lowMtl->SpecularLevel- highMtl->SpecularLevel);
                    float nu = highMtl->Nu + (1-t)*(lowMtl->Nu - highMtl->Nu);
                    float nv = highMtl->Nv + (1-t)*(lowMtl->Nv - highMtl->Nv);
                    Color diffColor = lowMtl->DiffuseColor;
                    diffColor.AddScaled(highMtl->DiffuseColor, -1);
                    diffColor.Scale(1-t);
                    diffColor.Add(highMtl->DiffuseColor);
                    Color specColor = lowMtl->SpecularColor;
                    specColor.AddScaled(highMtl->SpecularColor, -1);
                    specColor.Scale(1-t);
                    specColor.Add(highMtl->SpecularColor);
                    
                    blendedMaterial.SetDiffuseColor(diffColor);
                    blendedMaterial.SetDiffuseLevel(dl);
                    blendedMaterial.SetSpecularLevel(sl);
                    blendedMaterial.SetSpecularColor(specColor);
                    blendedMaterial.SetRoughness(nu,nv);
                    hit.Mtl = &blendedMaterial;
                    
                }
            }
            
            // Blending based on normal direction
            // highMtl = 'up' material
            // lowmtl = 'side' material
            // t = 1 gives highMtl
            else if (BlendType == 1)
            {
                float t = dot(glm::vec3(0,1,0), hit.Normal);
                t *= t;
                float dl = highMtl->DiffuseLevel + (1-t)*(lowMtl->DiffuseLevel - highMtl->DiffuseLevel);
                float sl = highMtl->SpecularLevel + (1-t)*(lowMtl->SpecularLevel- highMtl->SpecularLevel);
                float nu = highMtl->Nu + (1-t)*(lowMtl->Nu - highMtl->Nu);
                float nv = highMtl->Nv + (1-t)*(lowMtl->Nv - highMtl->Nv);
                Color diffColor = lowMtl->DiffuseColor;
                diffColor.AddScaled(highMtl->DiffuseColor, -1);
                diffColor.Scale(1-t);
                diffColor.Add(highMtl->DiffuseColor);
                Color specColor = lowMtl->SpecularColor;
                specColor.AddScaled(highMtl->SpecularColor, -1);
                specColor.Scale(1-t);
                specColor.Add(highMtl->SpecularColor);
                
                blendedMaterial.SetDiffuseColor(diffColor);
                blendedMaterial.SetDiffuseLevel(dl);
                blendedMaterial.SetSpecularLevel(sl);
                blendedMaterial.SetSpecularColor(specColor);
                blendedMaterial.SetRoughness(nu,nv);
                hit.Mtl = &blendedMaterial;
            }
        }
    }
    return true;
    
    // From slides
    //    Ray ray2;
    //    ray2.Origin=glm::vec3( Inverse * glm::vec4(ray.Origin,1) );
    //    ray2.Direction=glm::vec3(Inverse * glm::vec4(ray.Direction,0));
    //    if(Child->Intersect(ray2, hit)==false) return false;
    //    hit.Position=glm::vec3( Matrix * glm::vec4(hit.Position,1) );
    //    hit.Normal=glm::vec3( Matrix * glm::vec4(hit.Normal,0));
    //    hit.HitDistance=glm::distance(ray.Origin,hit.Position); // Correct for any scaling
    //    return true;
    
    
}
void InstanceObject::SetChild(Object &obj) {Child = &obj;}
void InstanceObject::SetMatrix(glm::mat4 &mtx) {Matrix = mtx; Inverse = glm::inverse(Matrix);}
