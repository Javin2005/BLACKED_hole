#include "physics.hpp"
#include "raymath.h"
#include "constants.hpp"


/** 
 * Updates the physics for a photon
 * @param photons - a mutable vector of the active and unactive photons
 * @param blackHole - a blackHole object
 * 
 * */ 
void UpdatePhysics(std::vector<Photon>& photons, const blackHole& bh, float dt, bool saveHistory) {
    for(auto& photon: photons){
        if(photon.active){
            
            
            if(saveHistory) {
                photon.history.push_back(photon.position);
                if (photon.history.size() > 150) photon.history.erase(photon.history.begin());
            }

            //Hastighet och direction just nu, kan ändra de senare för mer realism
            Vector2 direction = Vector2Subtract(bh.position, photon.position);
            double distance = Vector2Length(direction);

            if(distance < bh.eventHorizonRadius){
                photon.active = false;
                continue;
            }



            // Hur svarta hålet påverkar photonen just nu bättre fysik senare trust
            double force = (bh.mass * G) / (distance * distance);
            force += (3.0 * bh.mass * G * 600.0) / (pow(distance, 4)); // <-- för dum för den riktiga matten Samuel cook
            Vector2 acceleration = Vector2Scale(Vector2Normalize(direction), force);
            photon.velocity = Vector2Add(photon.velocity, Vector2Scale(acceleration, dt));

            
            if(Vector2Length(photon.velocity) > C) {
                photon.velocity = Vector2Scale(Vector2Normalize(photon.velocity), C);
            }

           
            photon.position = Vector2Add(photon.position, Vector2Scale(photon.velocity, dt));
            

            if(photon.position.x < -200 || photon.position.x > 1000 || 
               photon.position.y < -200 || photon.position.y > 1000) {
                 photon.active = false;
            }
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

        photons.push_back({ pos, vel, {}, true, randomColor, false });
    }
}
