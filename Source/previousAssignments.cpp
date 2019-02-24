//
//  previousAssignments.cpp
//  CSE_168
//
//  Created by Clayton Anderson on 4/23/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#include "previousAssignments.h"

void project4() {
    // Create scene
    Scene scn;
    scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));
    
    // Materials
    const int nummtls=4;
    AshikhminMaterial mtl[nummtls];
    
    // Diffuse
    mtl[0].SetSpecularLevel(0.0f);
    mtl[0].SetDiffuseLevel(1.0f);
    mtl[0].SetDiffuseColor(Color(0.7f,0.7f,0.7f));
    
    // Roughened copper
    mtl[1].SetDiffuseLevel(0.0f);
    mtl[1].SetSpecularLevel(1.0f);
    mtl[1].SetSpecularColor(Color(0.9f,0.6f,0.5f));
    mtl[1].SetRoughness(100.0f,100.0f);
    
    // Anisotropic gold
    mtl[2].SetDiffuseLevel(0.0f);
    mtl[2].SetSpecularLevel(1.0f);
    mtl[2].SetSpecularColor(Color(0.95f,0.7f,0.3f));
    mtl[2].SetRoughness(1.0f,1000.0f);
    
    // Red plastic
    mtl[3].SetDiffuseColor(Color(1.0f,0.1f,0.1f));
    mtl[3].SetDiffuseLevel(0.8f);
    mtl[3].SetSpecularLevel(0.2f);
    mtl[3].SetSpecularColor(Color(1.0f,1.0f,1.0f));
    mtl[3].SetRoughness(1000.0f,1000.0f);
    
    // Load dragon mesh
    MeshObject dragon;
    dragon.LoadPLY("dragon.ply");
    dragon.Smooth();
    
    
    // Create box tree
    BoxTreeObject tree;
    tree.Construct(dragon);
    
    // Create dragon instances
    glm::mat4 mtx;
    for(int i=0;i<nummtls;i++) {
        InstanceObject *inst=new InstanceObject(&tree);
        mtx[3]=glm::vec4(0.0f,0.0f,-0.1f*float(i),1.0f);
        inst->SetMatrix(mtx);
        inst->SetMaterial(&mtl[i]);
        scn.AddObject(*inst);
    }
    
    // Create ground
    LambertMaterial lambert;
    lambert.SetColor(Color(0.3f,0.3f,0.35f));
    MeshObject ground;
    ground.MakeBox(2.0f,0.11f,2.0f,&lambert);
    scn.AddObject(ground);
    
    // Create lights
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
    sunlgt.SetIntensity(1.0f);
    sunlgt.SetDirection(glm::vec3 (2.0f, -3.0f, -2.0f));
    scn.AddLight(sunlgt);
    
    // Create camera
    Camera cam;
    cam.SetResolution(800,600);
    cam.LookAt(glm::vec3(-0.5f,0.25f,-0.2f), glm::vec3(0.0f,0.15f,-0.15f),glm::vec3(0,1,0));
    SHOWVEC(cam.pos);
    SHOWVEC(cam.look_at);
    cam.SetFOV(40.0f);
    cam.SetAspect(1.33f);
    cam.SetSuperSample(4,4);
    cam.SetJitter(true);
    cam.SetShirley(true);
    //    cam.ShootSingleRay(425, 262, scn);
    // Render image
    cam.Render(scn);
    cam.SaveBitmap("project4.bmp");
}



void project3() {
    // Create scene
    Scene scn;
    scn.SetSkyColor(Color(0.8f,0.9f,1.0f));
    //    scn.SetSkyColor(Color(0,0,0));
    
    // Create ground
    LambertMaterial groundMtl;
    groundMtl.SetColor(Color(0.25f,0.25f,0.25f));
    
    MeshObject ground;
    ground.MakeBox(2.0f,0.11f,2.0f,&groundMtl);
    scn.AddObject(ground);
    
    
    // Load dragon mesh
    MeshObject dragon;
    dragon.LoadPLY("dragon.ply");
    dragon.Smooth();
    
    // Create box tree
    BoxTreeObject tree;
    tree.Construct(dragon);
    
    
    // Materials
    LambertMaterial white;
    white.SetColor(Color(0.7f,0.7f,0.7f));
    
    LambertMaterial red;
    red.SetColor(Color(0.7f,0.1f,0.1f));
    
    MetalMaterial metal;
    metal.SetColor(Color(0.95f,0.64f,0.54f));
    
    const int numDragons=4;
    Material *mtl[numDragons]={&white,&metal,&red,&white};
    
    
    // Create dragon instances
    glm::mat4 mtx;
    for(int i=0;i<numDragons;i++) {
        InstanceObject *inst=new InstanceObject(&tree);
        mtx[3]=glm::vec4(0.0f,0.0f,0.3f*(float(i)/float(numDragons-1)-0.5f),1.0f);
        inst->SetMatrix(mtx);
        inst->SetMaterial(mtl[i]);
        scn.AddObject(*inst);
    }
    
    // Create lights
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
    sunlgt.SetIntensity(1.0f);
    sunlgt.SetDirection(glm::vec3(2.0f, -3.0f, -2.0f));
    scn.AddLight(sunlgt);
    
    // Create camera
    Camera cam;
    cam.SetResolution(640,480);
    cam.SetAspect(1.33f);
    cam.LookAt(glm::vec3(-0.5f,0.25f,-0.2f),glm::vec3(0.0f,0.15f,0.0f),glm::vec3(0,1.0f,0));
    cam.SetFOV(40.0f);
    cam.SetSuperSample(10,10);
    cam.SetJitter(true);
    cam.SetShirley(true);
    // Render image
//    cam.ShootSingleRay(264, 166, scn);
    cam.Render(scn);
    cam.SaveBitmap("project3.bmp");
}


void project2()
{
    Scene scn;
    scn.SetSkyColor(Color(0.8f, 0.8f, 1.0f));
    
    // Create ground
    MeshObject ground;
    ground.MakeBox(5.0f,0.1f,5.0f);
    scn.AddObject(ground);
    
    //    MeshObject box1;
    //    box1.MakeBox(1.0f,1.1f,1.0f);
    
    
    //    BoxTreeObject tree;
    //    tree.Construct(box1);
    //
    //    scn.AddObject(tree);
    //    scn.AddObject(box1);
    
    //    std::cout << "Printing ground verts" << std::endl;
    //    ground.PrintObj();
    //    std::cout <<  std::endl;
    //
    //    // Create dragon
    MeshObject dragon;
    dragon.LoadPLY("dragon.ply");
    dragon.Smooth();
    BoxTreeObject tree;
    
    
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    tree.Construct(dragon);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Tree Construction: " << time_span.count() << " seconds." << std::endl;
    
    scn.AddObject(tree);
    //    scn.AddObject(dragon);
    
    
    // Create instance
    InstanceObject inst(&tree);
    glm::mat4x4 mtx=glm::eulerAngleY(3.141593f);
    mtx[3]=glm::vec4(-0.05f,0.0f,-0.1f,1.0f);
    inst.SetMatrix(mtx);
    scn.AddObject(inst);
    
    // Create lights
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
    sunlgt.SetIntensity(1.0f);
    sunlgt.SetDirection(glm::vec3(2.0f, -3.0f, -2.0f));
    scn.AddLight(sunlgt);
    
    PointLight redlgt;
    redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f));
    redlgt.SetIntensity(0.02f);
    redlgt.SetPosition(glm::vec3(-0.2f, 0.2f, 0.2f));
    scn.AddLight(redlgt);
    //
    PointLight bluelgt;
    bluelgt.SetBaseColor(Color(0.2f, 0.2f, 1.0f));
    bluelgt.SetIntensity(0.02f);
    bluelgt.SetPosition(glm::vec3(0.1f, 0.1f, 0.3f));
    scn.AddLight(bluelgt);
    
    // Create camera
    Camera cam;
    cam.LookAt(glm::vec3(-0.1f,0.1f,0.2f),glm::vec3(-0.05f,0.12f,0.0f),
               glm::vec3(0,1.0f,0));
    //    cam.LookAt(glm::vec3(2.0f,2.0f,5.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0,1,0));
    cam.SetFOV(40.0f);
    cam.SetAspect(1.33f);
    cam.SetResolution(800,600);
    // Render image
    t1 = high_resolution_clock::now();
    cam.RenderMultiThread(scn, 8);
    //    cam.Render(scn);
    
    t2 = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(t2 - t1);
    
    std::cout << "Render Time: " << time_span.count() << " seconds." << std::endl;
    cam.SaveBitmap("project2.bmp");
}


void project1() {
    // Create scene
    Scene scn;
    scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));
    
    // Create boxes
    MeshObject box1;
    box1.MakeBox(5.0f,0.1f,5.0f);
    scn.AddObject(box1);
    
    MeshObject box2;
    box2.MakeBox(1.0f,1.0f,1.0f);
    
    InstanceObject inst1(&box2);
    glm::mat4x4 mtx=glm::rotate(glm::mat4x4(),0.5f,glm::vec3(1,0,0));
    mtx[3][1]=1.0f;
    inst1.SetMatrix(mtx);
    scn.AddObject(inst1);
    
    InstanceObject inst2(&box2);
    mtx=glm::rotate(glm::mat4x4(),1.0f,glm::vec3(0,1,0));
    mtx[3]=glm::vec4(-1,0,1,1);
    inst2.SetMatrix(mtx);
    scn.AddObject(inst2);
    
    // Create lights
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
    sunlgt.SetIntensity(0.5f);
    sunlgt.SetDirection(glm::vec3(-0.5f, -1.0f, -0.5f));
    scn.AddLight(sunlgt);
    
    PointLight redlgt;
    redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f));
    redlgt.SetIntensity(2.0f);
    redlgt.SetPosition(glm::vec3(2.0f, 2.0f, 0.0f));
    scn.AddLight(redlgt);
    
    //    // Create camera
    Camera cam;
    cam.LookAt(glm::vec3(2.0f,2.0f,5.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0,1,0));
    cam.SetResolution(800,600);
    //    cam.SetResolution(16,12);
    cam.SetFOV(40.0f);
    cam.SetAspect(1.33f);
    
    // Render image
    //    cam.Render(scn);
    cam.RenderMultiThread(scn, 8);
    cam.SaveBitmap("project1.bmp");
}
//
//////////////////////////////////////////////////////////////////////////////////

void project1EC()
{
    srand(time(0));
    
    Scene scn;
    scn.SetSkyColor(Color(0.8f, 0.8f, 1.0f));
    // Create ground plane
    PlaneObject ground;
    scn.AddObject(ground);
    //     Create spheres
    for(int i=0;i<20;i++) {
        SphereObject *sphere=new SphereObject;
        float rad=glm::linearRand(0.25f,0.5f);
        glm::vec3 pos(glm::linearRand(-5.0f,5.0f), rad, glm::linearRand(-5.0f,5.0f));
        sphere->SetRadius(rad);
        sphere->SetCenter(pos);
        scn.AddObject(*sphere);
    }
    // Create lights
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
    sunlgt.SetIntensity(1.0f);
    sunlgt.SetDirection(glm::vec3(2.0f, -3.0f, -2.0f));
    scn.AddLight(sunlgt);
    // Create camera
    Camera cam;
    cam.LookAt(glm::vec3(-0.75f,0.25f,5.0f), glm::vec3(0.0f,0.5f,0.0f), glm::vec3(0,1,0));
    cam.SetFOV(40.0f);
    cam.SetAspect(1.33f);
    cam.SetResolution(800,600);
    // Render image
    //    cam.Render(scn);
    cam.RenderMultiThread(scn, 8);
    cam.SaveBitmap("spheres.bmp");
}

////////////////////////////////////////////////////////////////////////////////

void project1Implicit()
{
    srand(0);
    Scene scn;
    scn.SetSkyColor(Color(0.8f, 0.8f, 1.0f));
    
    PlaneObject ground;
    scn.AddObject(ground);
    
    auto sumFun = [](float (*f)(float, float ,float), float (*g)(float,float,float)) {
        return [f, g](float x, float y, float z){return f(x,y,z) + g(x,y,z);};
    };
    //
    auto gauss1 = [](float x, float y, float z) { return (x-2)*(x-2) + (y-2)*(y-2) + (z+3)*(z+3) - 4; };
    //    auto guass1d = [](float x, float y, float z) { return std::make_tuple(2*(x-2),2*(y-2),2*(z+3));
    auto gauss2 = [](float x, float y, float z) { return (x+2)*(x+2) + (y-2)*(y-2) + (z+3)*(z+3) - 4; };
    //    auto gauss2d = [](float x, float y, float z) { return std::make_tuple(2*(x+2),2*(y-2),2*(z+3));
    auto sumGauss = sumFun(gauss1, gauss2);
    
    
    // Sphere
    //    ImplicitSurface surf( [](float x, float y, float z) { return (x-2)*(x-2) + (y-2)*(y-2) + (z+3)*(z+3) - 4; });
    //    surf.SetGrad( [](float x, float y, float z) { return std::make_tuple(2*(x-2),2*(y-2),2*(z+3)); });
    //    scn.AddObject(surf);
    
    
    // Gaussian
//    ImplicitSurface surf2( [](float x, float y, float z) { return y - 5*exp(-x*x - z*z); });
//    surf2.SetGrad( [](float x, float y, float z) {
//        return std::make_tuple(2*5*x*exp(-x*x-z*z),1.0f,2*5*z*exp(-x*x-z*z)); });
    
    //    surf2.SetF(sumGauss);
//    scn.AddObject(surf2);
    
    // Cube-Sphere
    //    ImplicitSurface surf3( [](float x, float y, float z) { return (x+4)*(x+4)*(x+4)*(x+4) + (y-2)*(y-2)*(y-2)*(y-2) + z*z*z*z - 4; });
    //    surf3.SetGrad( [](float x, float y, float z) { return std::make_tuple(4*(x+4)*(x+4)*(x+4),4*(y-2)*(y-2)*(y-2),4*z*z*z); });
    //    scn.AddObject(surf3);
    
        ImplicitSurface surf4( [](float x, float y, float z) { return y - 1 - sin(x)*cos(z); });
        surf4.SetGrad( [](float x, float y, float z) { return std::make_tuple(-cos(x)*cos(z),1.0f,sin(x)*sin(z)); });
        scn.AddObject(surf4);
    
    //    ImplicitSurface surf5( [](float x, float y, float z) { return y - 1 - sin(x)*cos(z) + sin(1.41f*x)*cos(0.707f*z); });
    //    surf5.SetGrad( [](float x, float y, float z) {
    //        return std::make_tuple(-cos(x)*cos(z)+1.41f*cos(1.41f*x)*cos(0.707f*z),
    //                               1.0f,
    //                               sin(x)*sin(z)-0.707f*sin(1.41f*x)*sin(0.707f*z)); });
    //    scn.AddObject(surf5);
    
    // Heart
    //        ImplicitSurface surf6( [](float x, float y, float z) {
    //            return pow(x*x+2.25f*z*z+(y-2)*(y-2)-1,3.0f) -(x*x + 0.1125f*z*z)*(y-2)*(y-2)*(y-2); });
    //        surf6.SetGrad( [](float x, float y, float z) {
    //            return std::make_tuple(6.0f*x*pow(x*x+2.25f*z*z+(y-2)*(y-2)-1,2.0f)-2.0f*x*(y-2)*(y-2)*(y-2),
    //                                   13.5f*z*pow(x*x+2.25f*z*z+(y-2)*(y-2)-1,2.0f)-0.225f*z*(y-2)*(y-2)*(y-2),
    //                                   6.0f*(y-2)*pow(x*x+2.25f*z*z+(y-2)*(y-2)-1,2.0f)-(3.0f*x*x+0.3375f*z*z)*(y-2)*(y-2)); });
    //        scn.AddObject(surf6);
    
    
    
    
    
    //    ImplicitSurface surf7;
    //    int numFreqs = 10;
    //    for (int i = 0; i < numFreqs; i++) surf7.freqs.push_back(glm::linearRand(0.1f, 10.f));
    //
    //    scn.AddObject(surf7);
    
    
    // Create lights
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(0.9f, 0.3f, 0.4f));
    sunlgt.SetIntensity(1.0f);
    sunlgt.SetDirection(glm::vec3(2.0f, -3.0f, -2.0f));
    scn.AddLight(sunlgt);
    
    PointLight bluelgt;
    bluelgt.SetBaseColor(Color(0.0f, 0.2f, 1.0f));
    bluelgt.SetIntensity(2.0f);
    bluelgt.SetPosition(glm::vec3(2.0f, 4.0f, 0.0f));
    scn.AddLight(bluelgt);
    
    Camera cam;
    cam.LookAt(glm::vec3(0.0f,7.0f,6.0f),glm::vec3(0.0f,1.5f,0.0f),glm::vec3(0,1,0));
    cam.SetFOV(40.0f);
    cam.SetAspect(1.33f);
    cam.SetResolution(800,600);
    // Render image
    cam.RenderMultiThread(scn, 8);
    //    cam.Render(scn);
    cam.SaveBitmap("implict.bmp");
}
