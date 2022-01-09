#pragma once
#include "game_config.hxx"

class Enemy
{
public:
    using Position = ge211::Posn<float>;
    using Velocity = ge211::Dims<float>;

    virtual Position get_enemy_pos() = 0;
    virtual double get_enemy_attack_damage() = 0;
    virtual double get_enemy_score_total() = 0;
    virtual float get_enemy_radius() = 0;
    virtual Velocity get_enemy_vel() = 0;
};