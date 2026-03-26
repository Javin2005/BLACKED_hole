#include "Simulation.hpp"
#include "raymath.h"
#include <cmath>




Simulation::Simulation(Vector3 bhPosition, double bhMass)
    : blackHole_(bhPosition,bhMass)
{
    photons_.reserve(Physics::MAX_PHOTONS);
}

Vector3 Simulation::computeAcceleration(Vector3 pos) const {
    Vector3 diff = Vector3Subtract(blackHole_.getPosition(),pos);
    float distSq = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
    float dist = sqrtf(distSq);
    if(dist < 0.001f)return {0,0,0};

    float rs = blackHole_.getRadius();

    double fN = Physics::G * blackHole_.getMass()/(double)distSq;
    double fGR = fN * (1.0 + 1.5 * (double)rs / (double)dist);

    return Vector3Scale(diff, (float)(fGR /dist));
}

struct State {Vector3 pos,vel; };

template<typename AccelFn>
static State rk4Step(State s, AccelFn accel, float dt) {
    auto k1v = accel(s.pos);
    auto k1p = s.vel;
    
    auto k2v = accel(Vector3Add(s.pos, Vector3Scale(k1p, dt*0.5f)));
    auto k2p = Vector3Add(s.vel, Vector3Scale(k1v, dt*0.5f));

    auto k3v = accel(Vector3Add(s.pos, Vector3Scale(k2p, dt*0.5f)));
    auto k3p = Vector3Add(s.vel, Vector3Scale(k2v, dt*0.5f));
    
    auto k4v = accel(Vector3Add(s.pos, Vector3Scale(k3p, dt)));
    auto k4p = Vector3Add(s.vel, Vector3Scale(k3v, dt));

    
    State result;
    result.pos = Vector3Add(s.pos, Vector3Scale(
        Vector3Add(Vector3Add(k1p, Vector3Scale(k2p,2)),
                   Vector3Add(Vector3Scale(k3p,2), k4p)), dt/6.0f));
    result.vel = Vector3Add(s.vel, Vector3Scale(
        Vector3Add(Vector3Add(k1v, Vector3Scale(k2v,2)),
                   Vector3Add(Vector3Scale(k3v,2), k4v)), dt/6.0f));
    return result;
}

void Simulation::updatePhoton(Photon& p, float dt) const {
    float dist = Vector3Distance(p.position, blackHole_.getPosition());
    if (dist < blackHole_.getRadius()) { p.active = false; return; }
    if (dist > Physics::MAX_PHOTON_DIST) { p.active = false; return; }
 
    p.savePosition();
 
    State s = { p.position, p.velocity };
    State next = rk4Step(s, [this](Vector3 pos){ return computeAcceleration(pos); }, dt);
 
    p.position = next.pos;
    p.velocity = next.vel;
 
    float speed = Vector3Length(p.velocity);
    if (speed > Physics::PHOTON_SPEED)
        p.velocity = Vector3Scale(p.velocity, Physics::PHOTON_SPEED / speed);
}

void Simulation::update(Camera3D& camera) {
    float dt = GetFrameTime();
    float subDt = dt / (float)subSteps_;
 
    for (auto& p : photons_) {
        if (!p.active) continue;
        for (int i = 0; i < subSteps_; i++)
            updatePhoton(p, subDt);
    }
 
    
    static int frameCounter = 0;
    if (++frameCounter % 10 == 0)
        removeDeadPhotons();
}

void Simulation::removeDeadPhotons() {
    int i = 0;
    while (i < (int)photons_.size()) {
        if (!photons_[i].active) {
            photons_[i] = photons_.back();  
            photons_.pop_back();           
        } else {
            i++;
        }
    }
}

void Simulation::spawnPhoton(Vector3 pos, Vector3 vel, Color color, bool showBody) {
    if ((int)photons_.size() >= Physics::MAX_PHOTONS) return;
    Photon p;
    p.reset(pos, vel, color, showBody);
    photons_.push_back(p);
}

void Simulation::spawnDisk(int count) {
    float rs   = blackHole_.getRadius();
    float rMin = blackHole_.getISCO();   
    float rMax = rs * 10.0f;
 
    Color diskColors[] = {ORANGE, GOLD, YELLOW, RED, {255,140,0,255}};
 
    for (int i = 0; i < count; i++) {
        if ((int)photons_.size() >= Physics::MAX_PHOTONS) break;
 
        float dist  = rMin + (float)GetRandomValue(0,1000)/1000.0f * (rMax - rMin);
        float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
        float yOff  = (float)GetRandomValue(-5, 5);
 
        Vector3 pos = {
            blackHole_.getPosition().x + cosf(angle) * dist,
            blackHole_.getPosition().y + yOff,
            blackHole_.getPosition().z + sinf(angle) * dist
        };
 
       
        float vK  = sqrtf((float)(Physics::G * blackHole_.getMass()) / dist);
        float vGR = vK * sqrtf(1.0f + rs / (2.0f * dist));
 
        Vector3 vel = { -sinf(angle)*vGR, 0.0f, cosf(angle)*vGR };
 
        Photon p;
        p.reset(pos, vel, diskColors[GetRandomValue(0,4)], false);
        photons_.push_back(p);
    }
}
 
void Simulation::clearPhotons() { photons_.clear(); }

#include "Renderer.hpp"

void Simulation::draw(const Camera3D& camera) const {
    Renderer::drawFrame(photons_, blackHole_, camera);
}