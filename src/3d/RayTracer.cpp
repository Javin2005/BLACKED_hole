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

    return Vector3Normalize(dir);
}


Vector3 RayTracer:: computeAcceleration(Vector3 pos, const BlackHole& bh) const {
    Vector3 diff = Vector3Subtract(bh.getPosition(), pos);
    float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
    float dist = sqrtf(distSq);
    if (dist < 0.001f) return {0.0f,0.0f,0.0f};

    float rs = bh.getRadius();
    double fN = Physics::G*bh.getMass() / (double)distSq;
    double fGR = fN * (1.0 + 1.5*(double)rs/(double)dist);

    return Vector3Scale(diff, (float)(fGR/dist));
}

Color RayTracer::skyColor(Vector3 dir) const {
    float galactic = 1.0f-fabsf(dir.y);
    galactic = galactic * galactic * galactic;

    unsigned char r = (unsigned char)(8 + galactic * 20);
    unsigned char g = (unsigned char)(5 + galactic * 10);
    unsigned char b = (unsigned char)(15 + galactic * 30);

    float hash = sinf(dir.x * 127.1f + dir.y * 311.7f + dir.z * 74.3f) * 43758.5f;
    hash = hash - floorf(hash);

    if (hash > 0.992f) {
        float brightness = (hash - 0.992f)/0.008f;
        unsigned char sb = (unsigned char)(150 + brightness * 105);
        return {sb, sb, sb, 255};
    }

    return {r, g, b, 255};
}

Color RayTracer::diskColor(Vector3 pos, const BlackHole& bh) const {
    float rs = bh.getRadius();
    float dist = sqrtf(pos.x*pos.x + pos.z*pos.z);

    //temp vitt blåaktigt nära 
    float t = (dist - bh.getISCO()) / (rs*9.0f);
    t = fmaxf(0.0f, fminf(1.0f, t));
    //gul längre borr
    unsigned char r, g, b;
    if (t < 0.3f) {
        float f = t / 0.3f;
        r = 255;
        g = (unsigned char)(255 - f * 55);
        b = (unsigned char)(200 - f * 180);
    } else if (t < 0.7f) {
        float f = (t - 0.3f) / 0.4f;
        r = 255;
        g = (unsigned char)(200 - f * 140);
        b = 20;
    } else {
        float f = (t - 0.7f) / 0.3f;
        r = (unsigned char)(255 - f * 155);
        g = (unsigned char)(60 - f * 50);
        b = 0;
    }

    float intensity = 1.0f - t*0.6f;
    r = (unsigned char)(r * intensity);
    g = (unsigned char)(g * intensity);
    b = (unsigned char)(b * intensity);

    return {r, g, b, 255};
}

Color RayTracer::tracePixel(int px, int py, const Camera3D& camera, const BlackHole& bh) const {
    Vector3 pos = camera.position;
    Vector3 vel = Vector3Scale(computeRayDir(px, py, camera), Physics::PHOTON_SPEED);

    float rs = bh.getRadius();
    float diskMin = bh.getISCO();
    float diskMax = rs * 12.0f;

    float prevY = pos.y;

    for (int step = 0; step < MAX_STEPS; step++){
        float dist = Vector3Distance(pos, bh.getPosition());

        float dt = fmaxf(0.05f, STEP_SIZE * sqrtf(dist / rs));

        if (dist < rs) return BLACK;

        float relY = pos.y - bh.getPosition().y;
        float prevRelY = prevY - bh.getPosition().y;

        if (relY * prevRelY < 0.0f) {
            float t = prevRelY / (prevRelY -relY);
            float xCross = pos.x - vel.x / Physics::PHOTON_SPEED * dt * (1.0f - t);
            float zCross = pos.z - vel.z / Physics::PHOTON_SPEED * dt * (1.0f - t);
            float rCross = sqrtf(xCross*xCross + zCross*zCross);

            if (rCross > diskMin && rCross < diskMax) {
                Vector3 crossPos = {xCross, bh.getPosition().y, zCross};
                return diskColor(crossPos, bh);
            }
        }

        if (dist > 4000.0f) {
            return skyColor(Vector3Normalize(vel));
        }

        Vector3 k1v = computeAcceleration(pos, bh);
        Vector3 k1p = vel;

        Vector3 pos2 = Vector3Add(pos, Vector3Scale(k1p, dt * 0.5f));
        Vector3 vel2 = Vector3Add(vel, Vector3Scale(k1v, dt * 0.5f));
        Vector3 k2v = computeAcceleration(pos2, bh);
        Vector3 k2p = vel2;

        Vector3 pos3 = Vector3Add(pos, Vector3Scale(k2p, dt * 0.5f));
        Vector3 vel3 = Vector3Add(vel, Vector3Scale(k2v, dt * 0.5f));
        Vector3 k3v = computeAcceleration(pos3, bh);
        Vector3 k3p = vel3;

        Vector3 pos4 = Vector3Add(pos, Vector3Scale(k3p, dt));
        Vector3 vel4 = Vector3Add(vel, Vector3Scale(k3v, dt * 0.5f));
        Vector3 k4v = computeAcceleration(pos4, bh);
        Vector3 k4p = vel3; // kanske ska ändra till vel3 för mer konsistant

        prevY = pos.y;
        pos = Vector3Add(pos, Vector3Scale(
            Vector3Add(Vector3Add(k1p, Vector3Scale(k2p, 2)), 
                    Vector3Add(Vector3Scale(k3p, 2), k4p)), dt/6.0f));
        vel = Vector3Add(vel, Vector3Scale(
            Vector3Add(Vector3Add(k1v, Vector3Scale(k2v, 2)),
                        Vector3Add(Vector3Scale(k3v, 2), k4v)), dt/6.0f));

        
        float speed = Vector3Length(vel);
        if(speed > Physics::PHOTON_SPEED * 1.01f){
            vel = Vector3Scale(vel, Physics::PHOTON_SPEED / speed);
        }

    }
    return {2, 3, 8, 255};
}


void RayTracer::render(const Camera3D& camera, const BlackHole& bh){
    Color* pixels = (Color*)image_.data;

    for (int py = 0; py < RENDER_H; py++) {
        for (int px = 0; px < RENDER_W; px++) {
            pixels[py * RENDER_W + px] = tracePixel(px, py, camera, bh);
        }
    }
}

void RayTracer::uploadTexture() {
    UpdateTexture(texture_, image_.data);
}