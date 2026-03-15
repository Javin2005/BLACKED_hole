#include "3d/physics3d.hpp"
#include "raymath.h"
#include "3d/constants3d.hpp"


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

        
        Vector3 diff = Vector3Subtract(bh.position, photon.position);
        float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z; 
        float dist = sqrtf(distSq);

        
        if(dist < bh.eventHorizonRadius) {
            photon.active = false;
            continue;
        }

        
        double force = (bh.mass * G) / distSq;
        
        force += (3.0 * bh.mass * G * 600.0) / (distSq * distSq);
        
        
        Vector3 acc = Vector3Scale(diff, (float)(force / dist)); 
        photon.velocity = Vector3Add(photon.velocity, Vector3Scale(acc, dt));

        
        float speed = Vector3Length(photon.velocity);
        if(speed > C) {
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
    for (int i = 0; i < count; i++) {
       
        float dist = (float)GetRandomValue(bh.eventHorizonRadius * 3.0f, bh.eventHorizonRadius * 8.0f);
        
        float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
        
        
        Vector3 pos = {
            bh.position.x + cosf(angle) * dist,
            bh.position.y, 
            bh.position.z + sinf(angle) * dist
        };

        
        float newtonianSpeed = sqrt((G * bh.mass) / dist);
        float relativisticCorrection = 1.0f + (bh.eventHorizonRadius / dist); 
        float speed = newtonianSpeed * relativisticCorrection * 1.05f; 

        
        Vector3 vel = {
            -sinf(angle) * speed, 
             0, 
             cosf(angle) * speed
        };

        Color diskColors[] = {ORANGE, GOLD, YELLOW, RED, BLUE};
        Color randomColor = diskColors[GetRandomValue(0, 4)];

       
        
        Photon p = { pos, vel, {}, 0, 0, true, randomColor, false };
        photons.push_back(p);
    }
}