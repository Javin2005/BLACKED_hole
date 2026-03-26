#pragma once


#include "raylib.h"
#include "BlackHole.hpp"
#include <vector>


class RayTracer {
public:
    static constexpr int RENDER_W = 320;
    static constexpr int RENDER_H = 180;

    static constexpr int MAX_STEPS = 500;
    static constexpr float STEP_SIZE = 0.8f;

    RayTracer();
    ~RayTracer();


    void render(const Camera3D& camera, const BlackHole& bh);

    const Texture2D& getTexture() const { return texture_; }

    void uploadTexture();

private:
    Image image_;
    Texture2D texture_;
    bool textureReady_ = false;

    Color tracePixel(int px, int py, const Camera3D& camera, const BlackHole& bh) const;

    Vector3 computeRayDir(int px, int py, const Camera3D& camera) const;


    // Denna är fan samma som den i simulation vill helst att det ska gå att stänga av raytracer så måste ha den här. Extrahera senare
    Vector3 computeAcceleration(Vector3 pos, const BlackHole& bh) const;

    Color skyColor(Vector3 direction) const;

    Color diskColor(Vector3 pos, BlackHole& bh) const;

};

