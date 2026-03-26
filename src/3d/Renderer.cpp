#include "Renderer.hpp"
#include "Constants.hpp"
#include "raymath.h"
#include "rlgl.h"      
#include <cmath>
#include <cstdlib> 



static std::vector<Vector3> starPositions;

void Renderer::initStars() {
    srand(42);
    starPositions.reserve(2000);
    for (int i = 0; i < 2000; i++) {
        float x = (float)(rand() % 2000 - 1000);
        float y = (float)(rand() % 2000 - 1000);
        float z = (float)(rand() % 2000 - 1000);
        float len = sqrtf(x*x + y*y + z*z);
        if (len < 0.001f) continue;
        starPositions.push_back({ x/len*4000.0f, y/len*4000.0f, z/len*4000.0f });
    }
}

void Renderer::drawStars(const Camera3D& camera) {
    
    Vector3 toBH = Vector3Normalize(Vector3Subtract({0,0,0}, camera.position));
    float   bhScreenRadius = 40.0f; // 

    int w = GetScreenWidth();
    int h = GetScreenHeight();

    for (const auto& s : starPositions) {
       
        Vector3 toStar = Vector3Normalize(Vector3Subtract(s, camera.position));

        
        float dot = Vector3DotProduct(toBH, toStar);
        if (dot > 0.998f) continue;  

        Vector2 screen = GetWorldToScreen(s, camera);
        if (screen.x >= 0 && screen.x < w &&
            screen.y >= 0 && screen.y < h) {
            DrawPixel((int)screen.x, (int)screen.y, WHITE);
        }
    }
}


static Color colorFromShift(float t) {
    t = fmaxf(0.01f, t);
    unsigned char r = (unsigned char)(fminf(255.0f, 2.0f/t * 255.0f));
    unsigned char g = (unsigned char)(fminf(255.0f, fmaxf(0.0f, 1.0f - fabsf(t-1.0f)*2.0f) * 255.0f));
    unsigned char b = (unsigned char)(fminf(255.0f, fmaxf(0.0f, (t-0.5f)*2.0f) * 255.0f));
    return {r, g, b, 255};
}

Color Renderer::computeColor(const Photon& p, Vector3 cameraPos, float rs) {
    
    float beta = 0.0f;
    Vector3 toCamera = Vector3Subtract(cameraPos, p.position);
    float len = Vector3Length(toCamera);
    if (len > 0.001f) {
        float vRadial = Vector3DotProduct(p.velocity, Vector3Scale(toCamera, 1.0f/len));
        beta = fmaxf(-0.999f, fminf(0.999f, vRadial / Physics::PHOTON_SPEED));
    }
    float doppler = sqrtf((1.0f + beta) / (1.0f - beta));
 
   
    float dist = Vector3Distance(p.position, {0,0,0});
    float grav = (dist > rs * 1.01f) ? sqrtf(fmaxf(0.0f, 1.0f - rs/dist)) : 0.0f;
 
    
    float shift = doppler * grav;
    Color sc = colorFromShift(shift);
    Color bc = p.baseColor;
    Color result = {
        (unsigned char)(sc.r*0.6f + bc.r*0.4f),
        (unsigned char)(sc.g*0.6f + bc.g*0.4f),
        (unsigned char)(sc.b*0.6f + bc.b*0.4f),
        255
    };
    
    float brightness = fminf(0.4f, rs * 3.0f / fmaxf(rs, dist) * 0.4f);
    return ColorBrightness(result, brightness);
}


//openGL kod genererad av claude orkade inte sätta mig in i vertex lära. huvudsakligen störtsta optimeringeng för tids komplexitet. sätt in dig i den.
void Renderer::drawAllTrails(const std::vector<Photon>& photons,
    const BlackHole& bh,
    Vector3 cameraPos) {
    float rs = bh.getRadius();

    rlBegin(RL_LINES);

    for (const auto& p : photons) {
        if (!p.active || p.historyCount < 2) continue;

        Color col = computeColor(p, cameraPos, rs);

        for (int i = 0; i < p.historyCount - 1; i++) {
            int ci = (p.historyIndex - p.historyCount + i     + MAX_HISTORY) % MAX_HISTORY;
            int ni = (p.historyIndex - p.historyCount + i + 1 + MAX_HISTORY) % MAX_HISTORY;

            // Kvadratisk fade: bakre del transparent, framre del solid
            float t     = (float)i / (float)(p.historyCount - 1);
            float alpha = t * t;
            unsigned char a = (unsigned char)(alpha * 220.0f);

            Vector3 c = p.history[ci];
            Vector3 n = p.history[ni];

            // Start-vertex av segmentet
            rlColor4ub(col.r, col.g, col.b, a);
            rlVertex3f(c.x, c.y, c.z);

            // Slut-vertex — lite ljusare (t+1)
            float t2 = (float)(i+1) / (float)(p.historyCount - 1);
            float a2 = (unsigned char)(t2 * t2 * 220.0f);
            rlColor4ub(col.r, col.g, col.b, (unsigned char)a2);
            rlVertex3f(n.x, n.y, n.z);
        }
    }

    rlEnd();  // ← ETT draw call för alla trails
}

void Renderer::drawFrame(const std::vector<Photon>& photons,
    const BlackHole& bh,
    const Camera3D& camera) {
    BeginDrawing();
    ClearBackground(BLACK);
    rlSetClipPlanes(0.1f, 6000.0f);
    BeginMode3D(camera);

    //DrawGrid(30, 10.0f);

    float rs = bh.getRadius();
    Vector3 bhPos = bh.getPosition();

    
    DrawSphereWires(bhPos, rs,              12, 12, {120, 30,  0, 160});
    DrawSphereWires(bhPos, bh.getPhotonSphere(), 8, 8, {80,  80,  0,  70});
    DrawSphereWires(bhPos, bh.getISCO(),     8,  8, {40,  30,  0,  40});
    DrawSphere(bhPos, rs * 0.95f, BLACK);

    
    BeginBlendMode(BLEND_ADDITIVE);
    drawAllTrails(photons, bh, camera.position);

    
    for (const auto& p : photons) {
        if (p.active && p.body) {
            Color col = computeColor(p, camera.position, rs);
            DrawSphere(p.position, 0.5f, col);
        }
    }
    EndBlendMode();

    EndMode3D();
    drawStars(camera);
    drawHUD(photons, bh, camera);
    EndDrawing();
}




void Renderer::drawHUD(const std::vector<Photon>& photons,
    const BlackHole& bh,
    const Camera3D& camera) {
    float rs = bh.getRadius();
    DrawText(TextFormat("Massa: %.0f  Rs: %.1f  Fotonsfar: %.1f  ISCO: %.1f",
    (float)bh.getMass(), rs, bh.getPhotonSphere(), bh.getISCO()),
    10, 10, 16, GREEN);
    DrawText(TextFormat("Fotoner: %i / %i  FPS: %i",
    (int)photons.size(), Physics::MAX_PHOTONS, GetFPS()),
    10, 34, 15, SKYBLUE);
    DrawText(TextFormat("Kamera: (%.0f %.0f %.0f)",
    camera.position.x, camera.position.y, camera.position.z),
    10, 56, 14, GRAY);
    DrawText("WASD+QE: flyg  Shift: snabb  D: disk  Space: rensa  Pil: massa  R: reset  F: fullskarm",
    10, GetScreenHeight()-26, 13, DARKGRAY);
}