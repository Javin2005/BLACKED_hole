#include "3d/graphics3d.hpp"
#include "raylib.h"
#include "3d/photon3d.hpp"
#include "3d/constants3d.hpp"
#include <cmath>
#include "raymath.h"



static Color ColorFromShift(float t) {
    t = fmaxf(0.01f, t);
    float r = fminf(1.0f, 2.0f/t);
    float g = fminf(1.0f, fmaxf(0.0f, 1.0f - fabsf(t - 1.0f) * 2.0f));
    float b = fminf(1.0f, fmaxf(0.0f, (t - 0.5f) * 2.0f));
    return {(unsigned char)(r*255), (unsigned char)(g*255), (unsigned char)(b*255), 255};
}

Color GetSpaceColor(Color baseColor, Vector3 velocity, Vector3 position,
                             Vector3 cameraPos, float distance, float rs) 
{
    float beta = 0.0f;
    Vector3 toCamera = Vector3Subtract(cameraPos, position);
    float toCamLen = Vector3Length(toCamera);
    
    
    if(toCamLen > 0.001f) {
        Vector3 dir = Vector3Scale(toCamera, 1.0f/toCamLen);
        float radialSpeed = Vector3DotProduct(velocity, dir);
        beta = fmaxf(-0.999f, fminf(0.999f, radialSpeed / (float)C));
    }
    float dopplerFactor = sqrtf((1.0f + beta) / (1.0f - beta));



    float gravFactor = (distance > rs * 1.01f)
        ? sqrtf(fmaxf(0.0f, 1.0f - rs / distance))
        : 0.0f;

    float totalShift = dopplerFactor * gravFactor;
    Color shiftColor = ColorFromShift(totalShift);

    Color result;
    result.r = (unsigned char)(shiftColor.r * 0.6f + baseColor.r * 0.4f);
    result.g = (unsigned char)(shiftColor.g * 0.6f + baseColor.g * 0.4f);
    result.b = (unsigned char)(shiftColor.b * 0.6f + baseColor.b * 0.4f);
    result.a = 255;


    float distFactor = fminf(1.0f, rs * 3.0f / fmaxf(rs,distance));
    return ColorBrightness(result, distFactor * 0.4f);

}

Color GetSpaceColor(Color baseColor, Vector3 velocity, float distance, float rs) {
    return GetSpaceColor(baseColor, velocity, {0,0,0}, {0,300,0}, distance, rs);
}

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
        
       
    //DrawGrid(30, 10.0f);
    
    float rs = (float)(2.0f * G * bh.mass/(C*C));

       
    DrawSphereWires(bh.position, rs,       12, 12, {120, 30,  0, 180});
    DrawSphereWires(bh.position, rs * 1.5f, 8,  8, {80,  80,  0, 80});
    DrawSphereWires(bh.position, rs * 3.0f, 8,  8, {40,  30,  0, 40});
    DrawSphere(bh.position, rs * 0.95f, BLACK);  

    BeginBlendMode(BLEND_ADDITIVE);
    for (const auto& photon : photons) {
        if (!photon.active) continue;

        float dist = Vector3Distance(photon.position, bh.position);
        Color col = GetSpaceColor(photon.baseColor, photon.velocity,photon.position,
                    camera.position, dist, rs);

        
        if (photon.historyCount > 1) {
            for (int i = 0; i < photon.historyCount - 1; i++) {
                int currIdx = (photon.historyIndex - photon.historyCount + i + MAX_HISTORY) % MAX_HISTORY;
                int nextIdx = (currIdx + 1) % MAX_HISTORY;
                float t = (float)i / photon.historyCount;
                DrawLine3D(photon.history[currIdx], photon.history[nextIdx], Fade(col, t * 0.8f));
            }
        }

        if (photon.body) {
            DrawSphere(photon.position, 0.4f, col);
        }
    }
    EndBlendMode();
    EndMode3D();


    DrawText(TextFormat("Massa: %.0f  Rs: %.1f  Fotonsfar: %.1f",
            (float)bh.mass, rs, rs*1.5f), 10, 10, 16, GREEN);
    DrawText(TextFormat("Fotoner: %i  |  Kamera: (%.0f %.0f %.0f)",
            (int)photons.size(), camera.position.x, camera.position.y, camera.position.z),
            10, 34, 15, SKYBLUE);
    DrawText("WASD+QE: flyg  Shift: snabb  D: disk  Space: rensa  Pil: massa  F: fullskarm",
            10, GetScreenHeight()-26, 13, DARKGRAY);
    EndDrawing();
}


void DrawLine3DEx(Vector3 startPos, Vector3 endPos, float thickness, Color color) {
    DrawCylinderEx(startPos, endPos, thickness/2.0f, thickness/2.0f, 8, color);
}