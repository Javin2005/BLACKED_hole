#include "RayTracer.hpp"
#include "Constants.hpp"
#include "raymath.h"
#include <cmath>



RayTracer::RayTracer() {
    image_ = GenImageColor(RENDER_W, RENDER_H, BLACK);

    texture_ = LoadTextureFromImage(image_);
    textureReady_ = true;
}

RayTracer::~RayTracer(){
    if(textureReady_) {UnloadTexture(texture_);}
    UnloadImage(image_);
}


Vector3 RayTracer::computeRayDir(int px, int py, const Camera3D& camera) const {
    float nx = (2.0f * (float)px / (float)RENDER_W - 1.0f);
    float ny = (1.0f - 2.0f * (float)py / (float)RENDER_H);


    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward,{0.0f,1.0f,0.0f}));
    Vector3 up = Vector3CrossProduct(right, forward);


    float tanHalfFov = tanf(camera.fovy * 0.5f*DEG2RAD);
    float aspect = (float)RENDER_W /(float)RENDER_H;


    Vector3 dir = Vector3Add(
        forward,
        Vector3Add(
            Vector3Scale(right, nx * tanHalfFov * aspect),
            Vector3Scale(up, ny * tanHalfFov)
        )

    );

    Vector3Normalize(dir);
}




