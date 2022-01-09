#pragma once

#include <ge211.hxx>

#include "asteroid.hxx"
#include "monster.hxx"
#include "spaceship.hxx"

class Laser
{
public:

    using Velocity = ge211::Dims<float>;
    using Position = ge211::Posn<float>;

    Laser();

    Laser(Position laser_pos);

    void laser_launch(Position destination);

    void laser_spaceship_launch(Velocity vel);

    void laser_collide();

    bool laser_hits_entity(Enemy& enemy) const;

    double laser_destroy_entity(std::vector<Asteroid>& asteroids,
                                std::vector<Monster>& monsters,
                                std::vector<Spaceship>& spaceships);

    Laser laser_next(double dt) const;

    //getters

    bool laser_is_live() const {return live_;};

    Position get_laser_pos() const {return laser_pos_;};

    double get_laser_radius() const {return radius;};

    double get_laser_damage() const {return attack_damage;};


private:

    Velocity velocity_;
    Position laser_pos_;
    Position orientation_;
    bool live_;
    double attack_damage = 10;
    float radius = 2;
};