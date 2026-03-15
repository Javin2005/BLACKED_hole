#include "3d/graphics3d.hpp"
#include "raylib.h"
#include "3d/photon3d.hpp"
#include "3d/constants3d.hpp"
#include <cmath>
#include "raymath.h"

/** 
 * Draws a Scene(frame) of the space we see
 * @param photolns - a mutable vector of the active and unactive photons
 * @param blackHole - a blackHole object
 * 
 * */ 
void DrawScene(const std::vector<Photon>& photons, const blackHole& bh, Camera3D& camera) {
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(camera);
        
       
        DrawGrid(20, 10.0f); 

       
        DrawSphere(bh.position, bh.eventHorizonRadius + 0.5f, DARKGRAY); 
        DrawSphere(bh.position, bh.eventHorizonRadius, BLACK);

        BeginBlendMode(BLEND_ADDITIVE);
        for (const auto& photon : photons) {
            if (!photon.active) continue;

            float dist = Vector3Distance(photon.position, bh.position);
            Color renderColor = GetSpaceColor(photon.baseColor, photon.velocity, dist, bh.eventHorizonRadius);

            
            if (photon.historyCount > 1) {
                for (int i = 0; i < photon.historyCount - 1; i++) {
                    int currIdx = (photon.historyIndex - photon.historyCount + i + MAX_HISTORY) % MAX_HISTORY;
                    int nextIdx = (currIdx + 1) % MAX_HISTORY;
                    float t = (float)i / photon.historyCount;
                    DrawLine3D(photon.history[currIdx], photon.history[nextIdx], Fade(renderColor, t));
                }
            }

            
            if (photon.body) {
                DrawSphere(photon.position, 0.5f, renderColor);
            }
        }
        EndBlendMode();
    EndMode3D();

    
    DrawText(TextFormat("Mass: %.0f", (float)bh.mass), 10, 10, 20, GREEN);
    DrawText(TextFormat("Photons: %i", (int)photons.size()), 10, 40, 20, BLUE);
    DrawText("F11: Fullscreen | Mouse: Orbit | Space: Clear | D: Spawn Disk", 10, GetScreenHeight() - 30, 20, GRAY);

    EndDrawing();
}


/**
 * Hjälp funktion som kommer att räkna ut doppler effekten. och gravitations effekten vi antar att användaren
 * tittar på svarta hålet från sidan då räknar vi ut om ljuset är på väg mot eller
 * ifrån användaren blir lite kostigt just nu i 2 d
 * @param baseColor - vilken färg den har i början
 * @param velocity hur snabbt den rör sig
 */
Color GetSpaceColor(Color baseColor, Vector3 velocity, float distance, float rs) {

    float speedRatio = velocity.y / (float)C;
    float brightness = -speedRatio * 0.8f;
    Color shiftedColor = baseColor;

    if (speedRatio < 0) { //<- mot oss blir blåare
        shiftedColor.r = (unsigned char)fmin(255, shiftedColor.r + 100 * fabs(speedRatio));
        shiftedColor.g = (unsigned char)fmin(255, shiftedColor.g + 100 * fabs(speedRatio));
        shiftedColor.b = 255; 
    } else {//<- ifrån oss blir rödare 
        
        shiftedColor.r = 200; 
        shiftedColor.g = (unsigned char)(shiftedColor.g * (1.0f - speedRatio));
        shiftedColor.b = (unsigned char)(shiftedColor.b * (1.0f - speedRatio));
    }


    float distFactor = distance / (rs * 4.0f); 
    if (distFactor < 1.0f) {
        shiftedColor.g = (unsigned char)(shiftedColor.g * distFactor);
        shiftedColor.b = (unsigned char)(shiftedColor.b * distFactor);
        brightness -= (1.0f - distFactor) * 0.5f;
    }

    return ColorBrightness(shiftedColor, brightness);
}

void DrawLine3DEx(Vector3 startPos, Vector3 endPos, float thickness, Color color) {
    float radius = thickness / 2.0f;
    DrawCylinderEx(startPos, endPos, radius, radius, 8, color);
}