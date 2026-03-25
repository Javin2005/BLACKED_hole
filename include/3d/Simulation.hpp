#include "BlackHole.hpp"
#include "Photon.hpp"
#include "Constants.hpp"
#include "raylib.h"
#include <vector>


class Simulation {
public: 
    explicit Simulation(Vector3 bhPosition, double bhMass = Physics::DEFAULT_MASS);

    void update(Camera3D& camera);

    void draw(const Camera3D& camera) const;

    void spawnPhoton(Vector3 pos, Vector3 vel, Color color, bool showBody);

    void spawnDisk(int count);

    void clearPhotons();

    BlackHole& getBlackHole() {return BlackHole_:}
    const BlackHole& getBlackHole() const { return blackHole_; }

    int getPhotonCount() const { return (int)photons_.size();}

private:
    BlackHole blackHole_;
    std::vector<Photon> photons_;
    int subSteps_=8;

    void updatePhoton(Photons& p, float dt) const;

    void removeDeadPhotons();

    Vector3 computeAcceleration(Vector3 pos) const;
}