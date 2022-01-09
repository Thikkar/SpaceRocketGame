#pragma once
#include "game_config.hxx"

class HealthBoost {
public:
    using Position = ge211::Posn<float>;

    HealthBoost(Position healthboost_pos);

    //getters

    float get_healthboost_radius() const {return radius;};

    double get_healthboost_bonus() const {return health_bonus;};

    Position get_healthboost_pos() const {return healhboost_pos;};

private:
    Position healhboost_pos;
    float radius = 10;
    double health_bonus = 10;
};