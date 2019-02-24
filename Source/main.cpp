
////////////////////////////////////////
// Main.cpp
////////////////////////////////////////

#include "main.h"

////////////////////////////////////////////////////////////////////////////////

int main(int argc,char **argv) {
    
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    finalProject();
//    project4();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    
    std::cout << "Runtime: " << time_span.count() << " seconds." << std::endl;
    
    return 0;
}

void finalProject() {
    // Create scene
    Scene scn;
    scn.SetSkyColor(Color(0.8f, 0.8f, 1.0f));
    
    // Materials
    const int nummtls=5;
    AshikhminMaterial mtl[nummtls];
    
    // Diffuse
    mtl[0].SetSpecularLevel(0.0f);
    mtl[0].SetDiffuseLevel(1.0f);
    mtl[0].SetDiffuseColor(Color(0.2f,1.0f,0.2f));
    
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
    
    // Water
    mtl[3].SetDiffuseColor(Color(0.0f,0.1f,0.9f));
    mtl[3].SetDiffuseLevel(0.1f);
    mtl[3].SetSpecularLevel(0.9f);
    mtl[3].SetSpecularColor(Color(0.0f,0.0f,1.0f));
    mtl[3].SetRoughness(1.0f,1.0f);
    
    // White diffuse-ish
//    mtl[4].SetDiffuseColor(Color(9.37,8.66f,4.35f));
    Color sand = Color(3.6f,3.33f,1.15f);
    sand.Scale(0.33f);
    mtl[4].SetDiffuseColor(sand);
    mtl[4].SetDiffuseLevel(0.7f);
    mtl[4].SetSpecularLevel(0.3f);
    mtl[4].SetSpecularColor(Color(1.0f,1.0f,1.0f));
    mtl[4].SetRoughness(10.0f,10.0f);

    
    // Load dragon mesh
    MeshObject terrain;
    terrain.LoadOFF("terrain_erode_1-3M.off");
    
    // Create box tree
    BoxTreeObject tree;
    tree.Construct(terrain);
    
    float WIDTH = 10*4*256;
    float HEIGHT = 10*4*256;
    // Create Terrain instances
    glm::mat4 mtx;
    InstanceObject *inst=new InstanceObject(&tree);
    mtx = glm::mat4(1.0f);
//    mtx = glm::translate(glm::mat4(1.0f), glm::vec3(WIDTH,0.0f,HEIGHT));
    inst->SetMatrix(mtx);
    inst->SetMaterial(&mtl[0]);
    inst->MultiMaterialFlag = true;
    inst->BlendType = 0;
    inst->highMtl = &mtl[0];
    inst->lowMtl = &mtl[4];
    scn.AddObject(*inst);
    
    
//    InstanceObject *inst2=new InstanceObject(&tree);
//    glm::mat4 mtx2;
//    mtx2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,HEIGHT));
//    inst2->SetMatrix(mtx2);
//    inst2->SetMaterial(&mtl[0]);
//    inst2->MultiMaterialFlag = true;
//    inst2->highMtl = &mtl[0];
//    inst2->lowMtl = &mtl[4];
//    scn.AddObject(*inst2);
//
//    glm::mat4 mtx3;
//    InstanceObject *inst3=new InstanceObject(&tree);
//    mtx3 = glm::translate(glm::mat4(1.0f), glm::vec3(WIDTH,0.0f,0.0f));
//    inst3->SetMatrix(mtx3);
//    inst3->SetMaterial(&mtl[0]);
//    inst3->MultiMaterialFlag = true;
//    inst3->highMtl = &mtl[0];
//    inst3->lowMtl = &mtl[4];
//    scn.AddObject(*inst3);
//
//    glm::mat4 mtx4;
//    InstanceObject *inst4=new InstanceObject(&tree);
//    mtx4 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,0.0f));
//    inst4->SetMatrix(mtx4);
//    inst4->SetMaterial(&mtl[0]);
//    inst4->MultiMaterialFlag = true;
//    inst4->highMtl = &mtl[0];
//    inst4->lowMtl = &mtl[4];
//    scn.AddObject(*inst4);
    
    MetalMaterial metal;
    metal.SetColor(Color(0.7f,0.7f,1.0f));
    
    // Create ground
    MeshObject ground;
    ground.MakeBox(WIDTH,0.025f,HEIGHT,&metal);
//    ground.Translate(glm::vec3(0.5f*WIDTH,-70.0f,0.5f*HEIGHT));
    ground.Translate(glm::vec3(0,-70.0f,0));
//    scn.AddObject(ground);
    
    // Create lights
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
    sunlgt.SetIntensity(1.0f);
    sunlgt.SetDirection(glm::vec3 (2.0f, -3.0f, -2.0f));
    scn.AddLight(sunlgt);
    
    // Create camera
    Camera cam;
    cam.SetResolution(1600,1200);
    cam.LookAt(glm::vec3(-0.5f*WIDTH,3000,-0.5f*HEIGHT), glm::vec3(-0.18f*WIDTH,0,-0.18f*HEIGHT),glm::vec3(0,1,0));
    
    SHOWVEC(cam.pos);
    SHOWVEC(cam.look_at);
    cam.SetFOV(60.0f);
    cam.SetAspect(1.33f);
    cam.SetSuperSample(12,12);
    cam.SetJitter(true);
    cam.SetShirley(true);
//    cam.ShootSingleRay(358, 182, scn);
    // Render image
    cam.Render(scn);
    cam.SaveBitmap("terrain2.bmp");
}


////////////////////////////////////////////////////////////////////////////////

