#pragma once

#include "BlackHole.hpp"
#include "Photon.hpp"
#include "Constants.hpp"
#include "RayTracer.hpp"
#include "raylib.h"
#include <vector>

class Simulation {
public:
    explicit Simulation(Vector3 bhPosition, double bhMass = Physics::DEFAULT_MASS);

    void update(Camera3D& camera);
    void draw(const Camera3D& camera) const;

    void spawnPhoton(Vector3 pos, Vector3 vel, Color color, bool showBody);
    void spawnDisk(int count);
    void clearPhotons();

    BlackHole& getBlackHole()             { return blackHole_; }
    const BlackHole& getBlackHole() const { return blackHole_; }
    int getPhotonCount() const            { return (int)photons_.size(); }

    
    void toggleRayTracing() { rayTracingEnabled_ = !rayTracingEnabled_; }
    bool isRayTracingEnabled() const { return rayTracingEnabled_; }

private:
    BlackHole           blackHole_;
    std::vector<Photon> photons_;
    mutable RayTracer          rayTracer_;      
    bool                rayTracingEnabled_ = false;  
    int                 subSteps_ = 8;

   
    mutable int renderFrameSkip_ = 0;

    void updatePhoton(Photon& p, float dt) const;
    void removeDeadPhotons();
    Vector3 computeAcceleration(Vector3 pos) const;
};