#pragma once
#include "enemy.hxx"


class Asteroid : public Enemy
{
public:

    using Position = ge211::Posn<float>;
    using Velocity = ge211::Dims<float>;

    Asteroid(Position asteroid_pos, Position direction);

    Asteroid asteroid_next(double dt);

    void reflect_horizontal();

    void reflect_vertical();

    //getters

    double get_enemy_attack_damage() override {return attack_damage;};

    Position get_enemy_pos() override {return asteroid_pos;};

    double get_enemy_score_total() override {return score_total;};

    float get_enemy_radius() override {return radius;};

    Velocity get_enemy_vel() override {return asteroid_velocity;};

private:
    Position asteroid_pos;
    Velocity asteroid_velocity;
    double attack_damage = 10;
    double score_total = 100;
    float radius = 50;

};