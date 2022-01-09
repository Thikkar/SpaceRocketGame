#pragma once
#include "enemy.hxx"

class Spaceship : public Enemy
{
public:
    using Position = ge211::Posn<float>;
    using Velocity = ge211::Dims<float>;

    Spaceship(Position asteroid_pos, Position direction);

    Spaceship spaceship_next(double dt);

    //getters

    double get_enemy_attack_damage() override {return attack_damage;};

    Position get_enemy_pos() override {return spaceship_pos;};

    double get_enemy_score_total() override {return score_total;};

    float get_enemy_radius() override {return radius;};

    Velocity get_enemy_vel() override {return spaceship_velocity;};

private:
    Position spaceship_pos;
    Velocity spaceship_velocity;
    double attack_damage = 100;
    double score_total = 300;
    float radius = 35;
};