#include "2d/graphics.hpp"
#include "raylib.h"
#include "2d/photon.hpp"
#include "2d/constants.hpp"
#include <cmath>
#include "raymath.h"

/** 
 * Draws a Scene(frame) of the space we see
 * @param photolns - a mutable vector of the active and unactive photons
 * @param blackHole - a blackHole object
 * 
 * */ 
void DrawScene(const std::vector<Photon>& photons, const blackHole& bh){
    BeginDrawing();
    ClearBackground(BLACK);

    DrawCircleV(bh.position, bh.eventHorizonRadius + 3, DARKGRAY);
    DrawCircleV(bh.position, bh.eventHorizonRadius, BLACK);


    BeginBlendMode(BLEND_ADDITIVE);
    for(const auto& photon : photons) {
        if(!photon.active) continue;
        

        float dist = Vector2Distance(photon.position, bh.position);
        Color renderColor = GetSpaceColor(photon.baseColor, photon.velocity, dist, bh.eventHorizonRadius);


        if (photon.historyCount > 1) {
            float maxThickness = 4.0f;


            for (int i = 0; i < photon.historyCount - 1; i++) {

                int currIdx = (photon.historyIndex - photon.historyCount + i + MAX_HISTORY) % MAX_HISTORY;
                int nextIdx = (currIdx + 1) % MAX_HISTORY;

                float t = (float)i / MAX_HISTORY;
                float thickness = maxThickness * t;
                Color tailColor = Fade(renderColor, t);
                DrawLineEx(photon.history[currIdx], photon.history[nextIdx], thickness, tailColor);

            }
            int lastIdx = (photon.historyIndex - 1 + MAX_HISTORY) % MAX_HISTORY;
            DrawLineEx(photon.history[lastIdx], photon.position, maxThickness, renderColor);

            
        }

        if(photon.body){
            DrawCircleV(photon.position, 4, renderColor);
        }
            
        
    }

    EndBlendMode();
    EndDrawing();
}


/**
 * Hjälp funktion som kommer att räkna ut doppler effekten. och gravitations effekten vi antar att användaren
 * tittar på svarta hålet från sidan då räknar vi ut om ljuset är på väg mot eller
 * ifrån användaren blir lite kostigt just nu i 2 d
 * @param baseColor - vilken färg den har i början
 * @param velocity hur snabbt den rör sig
 */
Color GetSpaceColor(Color baseColor, Vector2 velocity, float distance, float rs) {

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