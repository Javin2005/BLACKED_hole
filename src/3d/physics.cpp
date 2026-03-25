#include "3d/physics3d.hpp"
#include "raymath.h"
#include "3d/constants3d.hpp"
#include <cmath>


struct State{
    Vector3 pos;
    Vector3 vel;
};

static Vector3 ComputeAcceleration(const Vector3& pos, const blackHole&bh){


    Vector3 diff = Vector3Subtract(bh.position,pos);
    float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
    float dist   = sqrtf(distSq);

    if (dist<0.001f) return {0,0,0};

    float rs= (float)(2.0 * G * bh.mass / (C*C));


    double force_newton     = (G * bh.mass) / (double)distSq;
    double gr_correction    = 1.0 + 1.5 * (rs/dist);
    double force_magnitude  = force_newton * gr_correction;
    
    
    Vector3 acc = Vector3Scale(diff, (float)(force_magnitude /dist));
    return acc; 

}

static State RK4Step(const State& s, const blackHole& bh, float dt){

    Vector3 k1_vel = ComputeAcceleration(s.pos, bh);
    Vector3 k1_pos = s.vel;

    State s2 = {
        Vector3Add(s.pos, Vector3Scale(k1_pos, dt * 0.5f)),
        Vector3Add(s.vel, Vector3Scale(k1_vel, dt * 0.5f))
    };
    Vector3 k2_vel = ComputeAcceleration(s2.pos, bh);
    Vector3 k2_pos = s2.vel;

    State s3 = {
        Vector3Add(s.pos, Vector3Scale(k2_pos, dt * 0.5f)),
        Vector3Add(s.vel, Vector3Scale(k2_vel, dt * 0.5f))
    };
    Vector3 k3_vel = ComputeAcceleration(s3.pos, bh);
    Vector3 k3_pos = s3.vel;

    State s4 = {
        Vector3Add(s.pos, Vector3Scale(k3_pos, dt * 0.5f)),
        Vector3Add(s.vel, Vector3Scale(k3_vel, dt * 0.5f))
    };
    Vector3 k4_vel = ComputeAcceleration(s4.pos, bh);
    Vector3 k4_pos = s4.vel;



    State result;
    result.pos = Vector3Add(s.pos,Vector3Scale(
                    Vector3Add(Vector3Add(k1_pos, Vector3Scale(k2_pos, 2.0f)), 
                                Vector3Add(Vector3Scale(k3_pos, 2.0f), k4_pos)),
                                dt/6.0f
                                
    ));
    result.vel = Vector3Add(s.vel,Vector3Scale(
                    Vector3Add(Vector3Add(k1_vel, Vector3Scale(k2_vel, 2.0f)), 
                                Vector3Add(Vector3Scale(k3_vel, 2.0f), k4_vel)),
                                dt/6.0f
                                
    ));

    return result;
}

/** 
 * Updates the physics for a photon
 * @param photons - a mutable vector of the active and unactive photons
 * @param blackHole - a blackHole object
 * 
 * */ 
void UpdatePhysics(std::vector<Photon>& photons, const blackHole& bh, float dt, bool saveHistory) {
    for(auto& photon : photons) {
        if(!photon.active) continue;

        if(saveHistory) photon.savePosition();

        
        float dist = Vector3Distance(photon.position,bh.position);
        
        if(dist < bh.eventHorizonRadius) {
            photon.active = false;
            continue;
        }

        State current = {photon.position, photon.velocity};
        State next = RK4Step(current,bh,dt);


        photon.position = next.pos;
        photon.velocity = next.vel;

        
        float speed = Vector3Length(photon.velocity);
        if(speed > (float)C) {
            photon.velocity = Vector3Scale(photon.velocity, (float)C / speed);
        }

        
        photon.position = Vector3Add(photon.position, Vector3Scale(photon.velocity, dt));

        
        float maxDistance = 2000.0f; 
        if(dist > maxDistance) {
             photon.active = false;
        }
    }
}


/**
 * FUCKKKK gör kommentaren sen har inte riktigt fått det att funka 
 */

 void SpawnDisk(std::vector<Photon>& photons, const blackHole& bh, int count) {
    float rs    = (float)(2.0 * G * bh.mass/(C*C));
    float r_min = rs * 3.0f;
    float r_max = rs*10.0f;


    for(int i = 0; i < count; i++ ) {

        float dist  = r_min * (float)GetRandomValue(0, 1000) / 1000.0f * (r_max - r_min);

        float angle = (float)GetRandomValue(0,360) * DEG2RAD;

        Vector3 pos = {
            bh.position.x * cosf(angle) * dist,
            bh.position.y * (float)GetRandomValue(-3,3),
            bh.position.z * sinf(angle) * dist

        };


        float v_kepler  = sqrtf((float)(G*bh.mass)/dist);
        float gr_boost  = sqrtf(1.0f + rs/(2.0f * dist));
        float speed     = v_kepler * gr_boost;

        Vector3 vel = {
            -sinf(angle) * speed,
            0.0f,
            cosf(angle) * speed
        };

        Color diskColor[] = {ORANGE, YELLOW, RED, GOLD, {255, 140, 0, 255}};
        Color randomColor = diskColor[GetRandomValue(0,4)];


        photons.push_back({pos, vel, {}, 0, 0, true, randomColor, false});

    }
}