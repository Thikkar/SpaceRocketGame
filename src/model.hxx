#pragma once

#include <ge211.hxx>
#include "player.hxx"

class Model
{
public:

    using Position = ge211::Posn<float>;
    using Velocity = ge211::Dims<float>;

    Model();

    Model(Position player_pos);

    //Player methods

    void on_frame_player(double dt);

    void player_horiz_move(bool move_right);

    void player_vert_move(bool move_up);

    void player_stop_horiz_move();

    void player_stop_vert_move();

    void regenerate_player_laser();

    //healthboost methods

    void on_frame_healthboost(double dt);

    void generate_healthboost();

    //laser methods

    void laser_launch(Position launch_direction);

    void on_frame_laser(double dt);

    //asteroid methods

    void generate_asteroid(int switch_num);

    void on_frame_asteroid(double dt);

    //monster methods

    void generate_monster(int switch_num, int color_switch);

    void on_frame_monster(double dt);

    //spaceship methods

    void generate_spaceship(int switch_num);

    void on_frame_spaceship(double dt);

    void generate_spaceship_laser(Spaceship& spaceship);

    void launch_spaceship_laser(Spaceship& spaceship, Velocity launch_direction);

    //general methods

    std::vector<Position> generate_random_pos(int switch_num);

    Position make_unit(float x, float y);

    bool is_game_over() const {return our_player_.get_player_health() <= 0;};

    Game_config const config;

    //getters

    Position get_player_pos() const {return our_player_.get_player_pos();};

    int get_player_score() const {return our_player_.get_player_score();};

    double get_player_health() const {return our_player_.get_player_health();};

    std::vector<HealthBoost> get_healthboosts() const {return healthboosts;};

    std::vector<Laser> get_player_lasers() const {return player_lasers;};

    std::vector<Asteroid> get_asteroids() const {return asteroids;};

    std::vector<Monster> get_monsters() const {return monsters;};

    std::vector<Spaceship> get_spaceships() const {return spaceships;};

    std::vector<Laser> get_spaceship_lasers() const {return spaceship_lasers;};

#ifdef CS211_TESTING
#endif

private:
    //player fields
    Player our_player_;

    //healthboost fields
    std::vector<HealthBoost> healthboosts;

    //laser fields
    std::vector<Laser> player_lasers;
    int unfired_laser_index = 0;

    //asteroid fields
    std::vector<Asteroid> asteroids;

    //monster fields
    std::vector<Monster> monsters;

    //(enemy) spaceship fields
    std::vector<Spaceship> spaceships;
    std::vector<Laser> spaceship_lasers;
    int unfired_spaceship_index = -1;
    int frame_counter = 0;
    int every_nth_frame = 100;

    // allows the private variables to be accessed when testing
    friend struct Test_access;

    //private methods
    void set_game_over();

};
