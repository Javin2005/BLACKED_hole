#include "physics.hpp"
#include "raymath.h"
#include "constants.hpp"


/** 
 * Updates the physics for a photon
 * @param photons - a mutable vector of the active and unactive photons
 * @param blackHole - a blackHole object
 * 
 * */ 
void UpdatePhysics(std::vector<Photon>& photons, const blackHole& bh) {
    for(auto& photon: photons){
        if(photon.active){
            photon.history.push_back(photon.position);

            if (photon.history.size() > 100) {
                photon.history.erase(photon.history.begin()); 
            }


            Vector2 direction = Vector2Subtract(bh.position, photon.position);
            double distance = Vector2Length(direction);

            if(distance < bh.eventHorizonRadius){
                photon.active = false;
                continue;
            }

            double force = (bh.mass * G)/(distance*distance);
            Vector2 acceleration = Vector2Scale(Vector2Normalize(direction), force);
            photon.velocity = Vector2Add(photon.velocity, acceleration);

            if( Vector2Length(photon.velocity) > C){
                photon.velocity = Vector2Scale(Vector2Normalize(photon.velocity), (float)C);
            }


            
            photon.position = Vector2Add(photon.position, photon.velocity);
            

            if(photon.position.x < 0 || photon.position.x > 800 || 
                photon.position.y < 0 || photon.position.y > 800) {
                 photon.active = false;
             }


        }
    }
}