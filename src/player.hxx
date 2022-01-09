#pragma once
#include <ge211.hxx>
#include "game_config.hxx"
#include "laser.hxx"
#include "healthboost.hxx"

class Player
{
public:

    using Position = ge211::Posn<float>;
    using Velocity = ge211::Dims<float>;

    //empty constructor
    Player();

    Player(Position player_pos);

    void player_horiz_move(bool move_right, Game_config const& config);

    void player_vert_move(bool move_up, Game_config const& config);

    void player_stop_horiz_move();

    void player_stop_vert_move();

    void player_emerge_bottom(Game_config const& config);

    void player_emerge_top(Game_config const& config);

    void player_emerge_left(Game_config const& config);

    void player_emerge_right(Game_config const& config);

    void update_player_score(double amount);

    bool player_hits_entity(Enemy& enemy);

    void laser_hits_player(std::vector<Laser>& lasers);

    void player_attacked(std::vector<Asteroid>& asteroids,
                         std::vector<Monster>& monsters,
                         std::vector<Spaceship>& spaceships);

    bool player_hits_boost(HealthBoost& healthboost);

    void player_health_boost(std::vector<HealthBoost>& healthboosts);

    Player player_next(double dt) const;

    void set_player_dead();

    //getters

    double get_player_health() const {return health;};

    int get_player_score() const {return score;};

    Position get_player_pos() const {return player_pos;};

    float get_player_radius() const {return radius;};

private:

    Position player_pos;
    double health;
    int score = 0;
    Velocity player_vel{0,0};
    float radius = 40;
};