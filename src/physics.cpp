#include "physics.hpp"
#include "raymath.h"
#include "constants.hpp"


/** 
 * Updates the physics for a photon
 * @param photons - a mutable vector of the active and unactive photons
 * @param blackHole - a blackHole object
 * 
 * */ 
void UpdatePhysics(std::vector<Photon>& photons, const blackHole& bh, float dt) {
    for(auto& photon: photons){
        if(photon.active){
            
            photon.history.push_back(photon.position);
            if (photon.history.size() > 75) photon.history.erase(photon.history.begin());

            Vector2 direction = Vector2Subtract(bh.position, photon.position);
            double distance = Vector2Length(direction);

            if(distance < bh.eventHorizonRadius){
                photon.active = false;
                continue;
            }

            
            double force = (bh.mass * G) / (distance * distance);
            force += (3.0 * bh.mass * G * 1000.0) / (pow(distance, 4)); // <-- för dum för den riktiga matten Samuel cook

            Vector2 acceleration = Vector2Scale(Vector2Normalize(direction), (float)force);

            
            photon.velocity = Vector2Add(photon.velocity, Vector2Scale(acceleration, dt));

            
            if(Vector2Length(photon.velocity) > C) {
                photon.velocity = Vector2Scale(Vector2Normalize(photon.velocity), (float)C);
            }

           
            photon.position = Vector2Add(photon.position, Vector2Scale(photon.velocity, dt));
            

            if(photon.position.x < -100 || photon.position.x > 900 || 
               photon.position.y < -100 || photon.position.y > 900) {
                 photon.active = false;
            }
        }
    }
}

/**
 * FUCKKKK gör kommentaren sen
 */

void SpawnDisk(std::vector<Photon>& photons, const blackHole& bh, int count) {
    for (int i = 0; i < count; i++) {
        float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
        float dist = (float)GetRandomValue(bh.eventHorizonRadius * 2, bh.eventHorizonRadius * 5);
        
        Vector2 pos = {
            bh.position.x + cosf(angle) * dist,
            bh.position.y + sinf(angle) * dist
        };

        
        float speed = sqrt((G * bh.mass) / dist);
        Vector2 vel = {
            -sinf(angle) * speed, 
             cosf(angle) * speed
        };

        photons.push_back({ pos, vel, {},true, RED});
    }
}