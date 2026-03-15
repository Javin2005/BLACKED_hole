#include "2d/physics.hpp"
#include "raymath.h"
#include "2d/constants.hpp"


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

        Vector2 diff = Vector2Subtract(bh.position, photon.position);
        float distSq = diff.x * diff.x + diff.y * diff.y; 
        float dist = sqrtf(distSq);

        if(dist < bh.eventHorizonRadius) {
            photon.active = false;
            continue;
        }

        
        double force = (bh.mass * G) / distSq;
        force += (3.0 * bh.mass * G * 600.0) / (distSq * distSq);
        
        Vector2 acc = Vector2Scale(diff, (float)(force / dist)); 
        photon.velocity = Vector2Add(photon.velocity, Vector2Scale(acc, dt));

        
        float speed = Vector2Length(photon.velocity);
        if(speed > C) {
            photon.velocity = Vector2Scale(photon.velocity, (float)C / speed);
        }

        photon.position = Vector2Add(photon.position, Vector2Scale(photon.velocity, dt));

        float margin = 200.0f;
        if(photon.position.x < -margin || photon.position.x > GetScreenWidth() + margin || 
           photon.position.y < -margin || photon.position.y > GetScreenHeight() + margin) {
             photon.active = false;
        }
    }
}

/**
 * FUCKKKK gör kommentaren sen har inte riktigt fått det att funka 
 */

 void SpawnDisk(std::vector<Photon>& photons, const blackHole& bh, int count) {
    for (int i = 0; i < count; i++) {
        //något om att man måste vara 3x gånger från radianen för stabil
        float dist = (float)GetRandomValue(bh.eventHorizonRadius * 4.0f, bh.eventHorizonRadius * 7.0f);
        
        float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
        Vector2 pos = {
            bh.position.x + cosf(angle) * dist,
            bh.position.y + sinf(angle) * dist
        };

        //ensiten termen gör att du måste gå snabbt nära(ska ändras sen när vi gör riktig matte)
        float newtonianSpeed = sqrt((G * bh.mass) / dist);
        
        float relativisticCorrection = 1.0f + (bh.eventHorizonRadius / dist); 
        float speed = newtonianSpeed * relativisticCorrection * 1.09f; 

        Vector2 vel = {
            -sinf(angle) * speed, 
             cosf(angle) * speed
        };

        
        Color diskColors[] = {ORANGE, GOLD, YELLOW, RED, BLUE};
        Color randomColor = diskColors[GetRandomValue(0, 4)];

        photons.push_back({ pos, vel, {}, 0, 0, true, randomColor, false });
        
    }
}
