////////////////////////////////////////
// Material.h
////////////////////////////////////////

#pragma once

#include "Intersection.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "RandomGen.hpp"


////////////////////////////////////////////////////////////////////////////////

class Material {
public:
	virtual void ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit)=0;
    virtual void GenerateSample(const Intersection &isect,const glm::vec3 &inDir,glm::vec3 &outDir,Color &outColor)=0;
    virtual float ComputeIntensity(const glm::vec3 &lightDir, const glm::vec3 &normal)=0;
    virtual float ComputeDirectIntensity(const glm::vec3 &lightDir, const glm::vec3 &normal)=0;
};

////////////////////////////////////////////////////////////////////////////////
