#pragma once
#include "enemy.hxx"
#include <string>

class Monster : public Enemy
{
public:

    using Position = ge211::Posn<float>;
    using Velocity = ge211::Dims<float>;

    Monster(Position monster_pos, Position direction, int color);

    Monster monster_next(double dt);

    //getters

    Position get_enemy_pos() override {return monster_pos;};

    double get_enemy_attack_damage() override {return attack_damage;};

    int get_monster_color() {return color;};

    double get_enemy_score_total() override {return score_total;};

    float get_enemy_radius() override {return radius;};

    Velocity get_enemy_vel() override {return monster_velocity;};

private:
    Position monster_pos;
    Velocity monster_velocity;
    double attack_damage = 10;
    int color;
    double score_total = 200;
    float radius = 25;
};