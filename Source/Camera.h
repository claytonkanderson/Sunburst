//
//  Camera.h
//  CSE_168
//
//  Created by Clayton Anderson on 4/6/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Bitmap.h"
#include "LambertMaterial.h"
#include "RayTrace.hpp"
#include "RandomGen.hpp"
#include <iostream>
#include <thread>
#define SHOWVEC(v) std::cout << #v << ": " << v.x << " " << v.y << " " << v.z << std::endl
#define LOOP_COUNTER(str,i, max) \
if ((int)i % ((int)max/10) == 0) std::cout << str << ": " << (float)(i)/max << std::endl;

using namespace glm;

class Camera
{
public:
    Camera();
    
    vec3 pos;
    vec3 look_at;
    vec3 up;
    void LookAt(vec3 pos, vec3 targetPos, vec3 up);
    void SetResolution(int x, int y);
    void SetFOV(float fov);
    void SetAspect(float aspect);
    void ShootSingleRay(int xPix, int yPix, Scene &scn);
    void SetShirley(bool shirley) {ShirleyFlag = shirley;}
    void SetJitter(bool jitter) {JitterFlag = jitter;}
    void SetSuperSample(int x, int y) {SuperSampleX = x; SuperSampleY = y;}
    float GenerateRandom(float min, float max);
    
    // Render image
    void Render(Scene &scn);
    void RenderMultiThread(Scene &scn, int numThreads);
    static void ThreadFunc(Scene &scn, int minX, int maxX, int minY, int maxY);
    void SaveBitmap(const char * filename);
private:
    static int XRes,YRes;
    static mat4 WorldMatrix;
    static float VerticalFOV;
    static float HorziontalFOV;
    static float Aspect;
    static Bitmap * BMP;
    static bool ShirleyFlag;
    static bool JitterFlag;
    static int SuperSampleX;
    static int SuperSampleY;
};

#endif /* Camera_h */
