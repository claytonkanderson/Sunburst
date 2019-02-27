//
//  Camera.cpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/6/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "Camera.h"

int Camera::XRes, Camera::YRes;
mat4 Camera::WorldMatrix;
float Camera::VerticalFOV;
float Camera::HorziontalFOV;
float Camera::Aspect;
bool Camera::JitterFlag;
bool Camera::ShirleyFlag;
int Camera::SuperSampleX;
int Camera::SuperSampleY;
Bitmap * Camera::BMP;

Camera::Camera() {}
void Camera::LookAt(vec3 position, vec3 targetPos, vec3 up)
{
    vec3 d = position;
    vec3 c = normalize(d - targetPos);
    vec3 a = normalize(cross(up, c));
    vec3 b = cross(c, a);
    
    this->pos = d;
    this->up = b;
    this->look_at = -c;
    
    WorldMatrix = mat4(vec4(a,0), vec4(b,0), vec4(c,0), vec4(d,1));
    
    //    WorldMatrix = inverse(lookAt(position, targetPos, up));
}
void Camera::SetResolution(int x, int y) {
    delete BMP;
    XRes = x; YRes = y;
    
    BMP = new Bitmap(XRes, YRes);
}

// Assume fov is in degrees
void Camera::SetFOV(float fov) {
    VerticalFOV = fov * 3.1415 / 180.0f;
}

// XRes / YRes
void Camera::SetAspect(float aspect) {
    Aspect = aspect;
}

void Camera::SaveBitmap(const char * filename) { BMP->SaveBMP(filename); }

// Render image
void Camera::Render(Scene &scn)
{
    HorziontalFOV = 2 * atan(Aspect*tan(VerticalFOV / 2.0));
    
    int i, j, kx, ky;
    float fx = 0, fy = 0;
    float scaleX = 2 * tan(HorziontalFOV / 2.0);
    float scaleY = 2 * tan(VerticalFOV / 2.0);
    Color color;
    Color color2;
    
    vec3 a, b, c, d;
    
    a = vec3(WorldMatrix[0]);
    b = vec3(WorldMatrix[1]);
    c = vec3(WorldMatrix[2]);
    d = vec3(WorldMatrix[3]);
    
    Ray ray;
    Intersection intersection;
    
    RayTrace rayTracer(scn);
    
    RandomGen randomGenerator;
    
    // Width, Height of sub pixel squares (rectangles)
    float jitterX = 1.0 / SuperSampleX;
    float jitterY = 1.0 / SuperSampleY;
    
    for (i = 0; i < YRes; i++)
    {
        LOOP_COUNTER("Camera loop", i, YRes);
        for (j = 0; j < XRes; j++)
        {
            
            rayTracer.PrimaryRays++;
            
            ray.Origin = d;
            ray.type = 0; // Primary Ray
            Color colorAccumulator(0,0,0);
            
            // Assuming uniform sampling with jittered and or shirley
            for (kx = 0; kx < SuperSampleX; kx++)
            {
                
                for (ky = 0; ky < SuperSampleY; ky++)
                {
                    intersection.HitDistance = 1e10;
                    intersection.Shade.Set(0, 0, 0);
                    
                    float xShift, yShift;
                    
                    // Centers of sub-pixels
                    xShift = kx * jitterX + 0.5f;
                    yShift = ky * jitterY + 0.5f;
                    
                    if (JitterFlag)
                    {
                        xShift += randomGenerator.GenerateRandom(-jitterX, jitterX);
                        yShift += randomGenerator.GenerateRandom(-jitterY, jitterY);
                        fx = (float(j) + xShift) / float(XRes) - 0.5f;
                        fy = (float(i) + yShift) / float(YRes) - 0.5f;
                    }
                    
                    
                    if (ShirleyFlag)
                    {
                        float rand1 = randomGenerator.GenerateRandom(0, 1);
                        float rand2 = randomGenerator.GenerateRandom(0, 1);
                        
                        // Shirley Transformation
                        rand1 = rand1 < 0.5f ? -0.5 + sqrt(2.0*rand1) : 1.5 - sqrt(2.0 - 2.0*rand1);
                        rand2 = rand2 < 0.5f ? -0.5 + sqrt(2.0*rand2) : 1.5 - sqrt(2.0 - 2.0*rand2);
                        
                        fx = (float(j) + rand1) / float(XRes) - 0.5f;
                        fy = (float(i) + rand2) / float(YRes) - 0.5f;
                    }
                    
                    if (!JitterFlag && ! ShirleyFlag) {
                        fx = (float(j) + xShift) / float(XRes) - 0.5f;
                        fy = (float(i) + yShift) / float(YRes) - 0.5f;
                    }
                    
                    
                    ray.Direction = normalize(fx*scaleX*a + fy*scaleY*b - c);
                    
                    rayTracer.TraceRay(ray, intersection);
					
                    colorAccumulator.Add(intersection.Shade);                    
                }
            }
            
            colorAccumulator.Scale(1.0f / (SuperSampleY*SuperSampleX));
           
            BMP->SetPixel(j, i, colorAccumulator.ToInt());
        }
    }
}

void Camera::RenderMultiThread(Scene &scn, int numThreads)
{
    HorziontalFOV = 2 * atan(Aspect*tan(VerticalFOV / 2.0));
    
    
    std::thread * threads = new std::thread[numThreads];
    
    int deltaX = XRes / numThreads;
    int deltaXExtra = XRes % numThreads;
    
    for(int i = 0; i < numThreads; i++)
    {
        if (i == numThreads - 1)
            threads[i] = std::thread(std::ref(Camera::ThreadFunc), std::ref(scn), i * deltaX, (i+1)*deltaX + deltaXExtra, 0, YRes);
        else
            threads[i] = std::thread(std::ref(Camera::ThreadFunc), std::ref(scn), i * deltaX, (i+1)*deltaX, 0, YRes);
    }
    
    for (int i = 0; i < numThreads; i++)
    {
        threads[i].join();
    }
}

void Camera::ThreadFunc(Scene &scn, int minX, int maxX, int minY, int maxY)
{
    
    float fx, fy;
    float scaleX = 2 * tan(HorziontalFOV / 2.0);
    float scaleY = 2 * tan(VerticalFOV / 2.0);
    
    float i, j, k;
    vec3 a, b, c, d;
    
    Ray ray;
    Intersection intersection;
    
    a = vec3(WorldMatrix[0]);
    b = vec3(WorldMatrix[1]);
    c = vec3(WorldMatrix[2]);
    d = vec3(WorldMatrix[3]);
    
    Color color;
    Color color2;
    
    Intersection intersection2;
	RandomGen randomGenerator;
	RayTrace rayTracer(scn);

	float jitterX = 1.0 / SuperSampleX;
	float jitterY = 1.0 / SuperSampleY;

    for (i = minY; i < maxY; i++)
    {
        for (j = minX; j < maxX; j++)
        {

			rayTracer.PrimaryRays++;

			ray.Origin = d;
			ray.type = 0; // Primary Ray
			Color colorAccumulator(0, 0, 0);

			// Assuming uniform sampling with jittered and or shirley
			for (int kx = 0; kx < SuperSampleX; kx++)
			{

				for (int ky = 0; ky < SuperSampleY; ky++)
				{
					intersection.HitDistance = 1e10;
					intersection.Shade.Set(0, 0, 0);

					float xShift, yShift;

					// Centers of sub-pixels
					xShift = kx * jitterX + 0.5f;
					yShift = ky * jitterY + 0.5f;

					if (JitterFlag)
					{
						xShift += randomGenerator.GenerateRandom(-jitterX, jitterX);
						yShift += randomGenerator.GenerateRandom(-jitterY, jitterY);
						fx = (float(j) + xShift) / float(XRes) - 0.5f;
						fy = (float(i) + yShift) / float(YRes) - 0.5f;
					}


					if (ShirleyFlag)
					{
						float rand1 = randomGenerator.GenerateRandom(0, 1);
						float rand2 = randomGenerator.GenerateRandom(0, 1);

						// Shirley Transformation
						rand1 = rand1 < 0.5f ? -0.5 + sqrt(2.0*rand1) : 1.5 - sqrt(2.0 - 2.0*rand1);
						rand2 = rand2 < 0.5f ? -0.5 + sqrt(2.0*rand2) : 1.5 - sqrt(2.0 - 2.0*rand2);

						fx = (float(j) + rand1) / float(XRes) - 0.5f;
						fy = (float(i) + rand2) / float(YRes) - 0.5f;
					}

					if (!JitterFlag && !ShirleyFlag) {
						fx = (float(j) + xShift) / float(XRes) - 0.5f;
						fy = (float(i) + yShift) / float(YRes) - 0.5f;
					}


					ray.Direction = normalize(fx*scaleX*a + fy * scaleY*b - c);

					rayTracer.TraceRay(ray, intersection);

					colorAccumulator.Add(intersection.Shade);
				}
			}

			colorAccumulator.Scale(1.0f / (SuperSampleY*SuperSampleX));

			BMP->SetPixel(j, i, colorAccumulator.ToInt());
        }
    }
}

void Camera::ShootSingleRay(int xPix, int yPix, Scene &scn)
{
    HorziontalFOV = 2 * atan(Aspect*tan(VerticalFOV / 2.0));
    
    int i, j, kx, ky;
    float fx = 0, fy = 0;
    float scaleX = 2 * tan(HorziontalFOV / 2.0);
    float scaleY = 2 * tan(VerticalFOV / 2.0);
    Color color;
    Color color2;
    
    vec3 a, b, c, d;
    
    a = vec3(WorldMatrix[0]);
    b = vec3(WorldMatrix[1]);
    c = vec3(WorldMatrix[2]);
    d = vec3(WorldMatrix[3]);
    
    Ray ray;
    Intersection intersection;
    
    RayTrace rayTracer(scn);
    
    RandomGen randomGenerator;
    
    i = yPix;
    j = xPix;
    
    ray.Origin = d;
    ray.type = 0; // Primary Ray
    Color colorAccumulator(0,0,0);
    
    // Assuming uniform sampling with jittered and or shirley
    
    intersection.HitDistance = 1e10;
    intersection.Shade.Set(0, 0, 0);
    
    fx = (float(j) + 0.5f) / float(XRes) - 0.5f;
    fy = (float(i) + 0.5f) / float(YRes) - 0.5f;
    
    ray.Direction = normalize(fx*scaleX*a + fy*scaleY*b - c);
    
    SHOWVEC(ray.Direction);
    
    rayTracer.TraceRay(ray, intersection);
    
    rayTracer.PrimaryRays++;
    
    colorAccumulator.Add(intersection.Shade);
    
    colorAccumulator.Scale(1.0f / (SuperSampleY*SuperSampleX));
    
    colorAccumulator.PrintColorRGB();
    
    BMP->SetPixel(j, i, colorAccumulator.ToInt());    
}
