#include "raylib.h"
#include "Constants.hpp"
#include <cmath>




class BlackHole {
public:

    BlackHole(Vector3 position, double mass = Physics::DEFAULT_MASS)
        : position_(position)
        , mass_(mass)
        , active_(true)
    {
        clampMass();
        updateRadius();
    }



    Vector3 getPosition()    const { return position_; }
    double  getMass()        const { return mass_; }
    float   getRadius()      const { return eventHorizonRadius_; }
    float   getPhotonSphere()const { return eventHorizonRadius_ * 1.5f; }
    float   getISCO()        const { return eventHorizonRadius_ * 3.0f; }
    bool    isActive()       const { return active_; }

    void setPosition(Vector3 pos) { position_ = pos; }


    void setMass(double m) {
        mass_ = m;
        clampMass();
        updateRadius();
    }

    void addMass(double delta) {
        setMass(mass_ + delta );
    }

    void updateRadius() {
        eventHorizonRadius_ = (float)(2.0 * Physics::G * mass_ / (Physics::C * Physics::C));
    }

private:
    Vector3 position_;
    double  mass_;
    float   eventHorizonRadius_;
    bool    active_;

    void clampMass() {
        if (mass_ < Physics::MIN_MASS) mass_ = Physics::MIN_MASS;
        if (mass_ > Physics::MAX_MASS) mass_ = Physics::MAX_MASS;
    }

};