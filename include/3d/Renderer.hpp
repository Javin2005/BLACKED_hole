#pragma once

#include "Photon.hpp"
#include "BlackHole.hpp"
#include "raylib.h"
#include <vector>


class Renderer {

public: 
    static void drawFrame(const std::vector<Photon>& photons,
                            const BlackHole& bh, const Camera3D& camera );
    static void initStars();  
    static void drawStars(const Camera3D& camera);


private:
    static Color computeColor(const Photon& p,
                                Vector3 cameraPos,
                                    float rs);
    

    static void drawAllTrails(const std::vector<Photon>& photons,
                                const BlackHole& bh,
                                Vector3 cameraPos);
    
    static void drawHUD(const std::vector<Photon>& photons,
                            const BlackHole& bh,
                            const Camera3D& camera);


    



};