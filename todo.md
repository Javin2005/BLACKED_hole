Helt rätt att släppa tekniken ett tag och fokusera på planen! Här är en steg-för-steg-lista (To-Do) för hur ni faktiskt bygger själva simuleringen, från enklaste "prick på skärmen" till avancerad fysik.

---

### Fas 1: Fundamentet (2D-rörelse)
Innan ni gör 3D och krökt rumtid måste ni ha ett system som kan flytta saker.
- [ ] **Skapa en "Photon"-struct:** Definiera en partikel som har `Vector2 position` och `Vector2 velocity`.
- [ ] **Skapa en huvudloop:** I `main.cpp`, rita en svart cirkel i mitten (det svarta hålet) och en liten vit prick (fotonen).
- [ ] **Implementera konstant fart:** Få fotonen att röra sig i en rät linje över skärmen: `pos.x += vel.x; pos.y += vel.y;`.
- [ ] **Hantera minne:** Se till att fotonen tas bort eller "dör" om den åker utanför skärmen.

### Fas 2: Newtonsk Gravitation (Uppvärmning)
Detta är inte "rätt" fysik för svarta hål, men det är nödvändigt för att lära sig hur man kodar krafter.
- [ ] **Beräkna avstånd:** Räkna ut avståndet $r$ mellan fotonen och centrum.
- [ ] **Implementera $1/r^2$:** Räkna ut en kraft som drar fotonen mot mitten. Ju närmare den är, desto starkare blir draget.
- [ ] **Uppdatera Velocity:** Kraften ändrar fotonens *velocity*, som i sin tur ändrar dess *position*.
- [ ] **Resultat:** Nu bör fotonen kunna "böja" sin bana runt cirkeln om den åker förbi nära.

### Fas 3: Runge-Kutta (Noggrannhet)
I C++ (och fysik) räcker det inte med `pos += vel`. Om man gör så, kommer partiklarna "skjuta iväg" felaktigt på grund av avrundningsfel.
- [ ] **Lär er RK4-algoritmen:** Det är en standard-formel i C++ för att räkna ut rörelse mycket mer exakt.
- [ ] **Implementera RK4:** Skriv en funktion som tar nuvarande position och räknar ut nästa position med högre precision. Detta är hjärtat i alla seriösa simuleringar.

### Fas 4: Allmän Relativitetsteori (The Real Deal)
Nu byter ni ut Newtons matte mot Einsteins.
- [ ] **Schwarzschild-radien ($R_s$):** Räkna ut var "Event Horizon" går. Om en foton hamnar innanför denna radie ska den ritas som svart eller tas bort (ljuset kan inte fly).
- [ ] **Geodes-ekvationen:** Detta är den svåra matten. Ni behöver koda formeln för hur ljus böjs i en Schwarzschild-metrik. 
    * *Tips: Ni behöver inte förstå hela teorin bakom, bara hur man skriver in formeln i C++.*
- [ ] **Ljusets böjning:** Nu bör ni se att ljus som kommer riktigt nära det svarta hålet kan göra hela varv runt det innan det flyger iväg (eller sugs in).

### Fas 5: Ray-Tracing (Det visuella genombrottet)
För att det ska se ut som på riktigt kan ni inte bara skjuta partiklar från sidan.
- [ ] **Baklänges-skjutning:** Skjut strålar *från kameran* (varje pixel på skärmen) ut i rymden.
- [ ] **Stjärnbakgrund:** Om en stråle flyger ut i oändligheten, ge den färgen av en stjärna (ladda in en bild på stjärnhimlen).
- [ ] **Gravitationslins:** Om strålen böjs runt det svarta hålet kommer den visa stjärnorna som egentligen finns *bakom* hålet. Det är detta som skapar den coola "ringen" av ljus.

### Fas 6: Ackretionsskiva (Interstellar-looken)
- [ ] **Disken:** Skapa en platt skiva av partiklar (gas) som snurrar runt hålet.
- [ ] **Kollisionsdetektering:** Om en ljusstråle korsar disken, rita den pixeln i en lysande färg (t.ex. orange/vit).
- [ ] **Doppler-skift:** (Överkurs) Gör ena sidan av disken blåare och ljusare, och den andra rödare, eftersom gasen snurrar mot/från oss.

---

### Var ska ni börja IDAG?
Börja med **Fas 1**. 
1. Skapa en `Photon`-struct i en ny fil som heter `physics.hpp`.
2. Försök få `main.cpp` att skapa en lista (`std::vector<Photon>`) med 10 fotoner som flyger över skärmen.
3. Använd Raylibs `DrawPixelV` eller `DrawCircleV` för att rita dem.

**Vill du ha ett kod-exempel på hur en "Photon-struct" och rörelse-loop ser ut i C++ för att kickstarta?**