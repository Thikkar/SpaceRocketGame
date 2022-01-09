#include "model.hxx"
#include <stdlib.h>

//Constructs a model (no parameters)
Model::Model()
        :Model(Position{492,364})
{ }

//Constructs a model with specified initial player position
Model::Model(Position player_pos)
        : our_player_(player_pos)
{
    Laser player_laser_{our_player_.get_player_pos().down_right_by({38,38})};
    player_lasers.push_back(player_laser_);
}

// Player methods

//moves the player right or left
void
Model::player_horiz_move(bool move_right)
{
    our_player_.player_horiz_move(move_right, config);
}

//moves the player up or down
void
Model::player_vert_move(bool move_up)
{
    our_player_.player_vert_move(move_up, config);
}

//stops player's right/left movement
void
Model::player_stop_horiz_move()
{
    our_player_.player_stop_horiz_move();
}

//stops player's up/down movement
void
Model::player_stop_vert_move()
{
    our_player_.player_stop_vert_move();
}

//regenerates player's laser after it has been fired
void
Model::regenerate_player_laser()
{
    player_lasers.push_back(Laser{our_player_.get_player_pos().down_right_by
            ({38,38})});
    unfired_laser_index++;
}

//updates the player's state after each frame
void
Model::on_frame_player(double dt)
{
    if (is_game_over()) {
        set_game_over();
        return;
    }
    Player nextPlayer = our_player_.player_next(dt);
    nextPlayer.player_attacked(asteroids, monsters, spaceships);
    nextPlayer.player_health_boost(healthboosts);
    nextPlayer.laser_hits_player(spaceship_lasers);

    if (config.hits_bottom(nextPlayer.get_player_pos())) {
        our_player_.player_emerge_top(config);
        return;
    } else if (config.hits_top(nextPlayer.get_player_pos())) {
        our_player_.player_emerge_bottom(config);
        return;
    } else if (config.hits_left(nextPlayer.get_player_pos())) {
        our_player_.player_emerge_right(config);
        return;
    } else if (config.hits_right(nextPlayer.get_player_pos())) {
        our_player_.player_emerge_left(config);
        return;
    } else {
        our_player_ = nextPlayer;
        return;
    }
}

//healthboost methods

//spawns healthboosts at random positions
void
Model::generate_healthboost()
{
    float x = rand() % config.scene_dims.width;
    float y = rand() % config.scene_dims.height;
    HealthBoost newHealthboost{Position{x,y}};
    healthboosts.push_back(newHealthboost);
}

//updates the healthboosts (vector) in a game. They spawn at random times.
void
Model::on_frame_healthboost(double dt)
{
    if (is_game_over()) {
        set_game_over();
        return;
    }
    int prob = rand() % 500;
    if (prob == 0) {
        generate_healthboost();
    }
}

//laser methods

//launches a laser (used for the player only) in the specified direction
void
Model::laser_launch(Position launch_direction)
{
    player_lasers[unfired_laser_index].laser_launch(launch_direction);
    regenerate_player_laser();
}

//updates the laser vectors (both the player's and the enemy spaceships') in a
//game.
void
Model::on_frame_laser(double dt)
{
    if (is_game_over()) {
        set_game_over();
        return;
    }

    // player lasers
    for (int i = 0; i < unfired_laser_index + 1; i++) {
        if (!player_lasers[i].laser_is_live()) {
            Laser newLaser{our_player_.get_player_pos().down_right_by({38,
                                                                       38})};
            player_lasers[i] = newLaser;
            continue;
        }
        Laser nextLaser = player_lasers[i].laser_next(dt);
        our_player_.update_player_score(nextLaser.laser_destroy_entity(asteroids,
                                                                       monsters,
                                                                       spaceships));
        if (config.hits_bottom(nextLaser.get_laser_pos())
            || config.hits_top(nextLaser.get_laser_pos())
            || config.hits_left(nextLaser.get_laser_pos())
            || config.hits_right(nextLaser.get_laser_pos()))
        {
            player_lasers[i].laser_collide();
        } else {
            player_lasers[i] = nextLaser;
        }
    }

    //enemy spaceship lasers
    for (unsigned int i = 0; i < spaceship_lasers.size(); i++) {
        if (!spaceship_lasers[i].laser_is_live()) {
            Laser newLaser{Position{-5,-5}};
            spaceship_lasers[i] = newLaser;
            continue;
        }
        Laser nextLaser = spaceship_lasers[i].laser_next(dt);

        if (config.hits_bottom(nextLaser.get_laser_pos())
            || config.hits_top(nextLaser.get_laser_pos())
            || config.hits_left(nextLaser.get_laser_pos())
            || config.hits_right(nextLaser.get_laser_pos()))
        {
            spaceship_lasers[i].laser_collide();
        } else {
            spaceship_lasers[i] = nextLaser;
        }
    }
}

//asteroid methods

//generates the random positions at the window boundaries AND random
//directions that the entity will be heading for asteroids,
//monsters, and enemy spaceships
std::vector<Model::Position>
Model::generate_random_pos(int switch_num)
{
    switch(switch_num) {
    case 1: {
        //left edge
        float pos_x = 0;
        float pos_y = rand() % config.scene_dims.height;
        Position newPos{pos_x, pos_y};

        float direc_x = rand() % 10;
        float direc_y = rand() % 20 - 10;
        Position newDirec = make_unit(direc_x, direc_y);

        return std::vector<Position>{newPos, newDirec};

        break;
    }
    case 2: {
        //right edge
        float pos_x = config.scene_dims.width;
        float pos_y = rand() % config.scene_dims.height;
        Position newPos{pos_x, pos_y};

        float direc_x = rand() % 10 - 20;
        float direc_y = rand() % 20 - 10;
        Position newDirec = make_unit(direc_x, direc_y);

        return std::vector<Position>{newPos, newDirec};

        break;
    }
    case 3: {
        //top edge
        float pos_x = rand() % config.scene_dims.width;
        float pos_y = 0;
        Position newPos{pos_x, pos_y};

        float direc_x = rand() % 20 - 10;
        float direc_y = rand() % 10;
        Position newDirec = make_unit(direc_x, direc_y);

        return std::vector<Position>{newPos, newDirec};

        break;
    }
    case 4: {
        //bottom edge
        float pos_x = rand() % config.scene_dims.width;
        float pos_y = config.scene_dims.height;
        Position newPos{pos_x, pos_y};

        float direc_x = rand() % 20 - 10;
        float direc_y = rand() % 10 - 20;
        Position newDirec = make_unit(direc_x, direc_y);

        return std::vector<Position>{newPos, newDirec};

        break;
    }
    }
    return std::vector<Position>{Position{0,0}, Position{0,0}};
}

//generates the asteroid at a random position, moving in a random direction
void
Model::generate_asteroid(int switch_num)
{
    Position newPos = generate_random_pos(switch_num)[0];
    Position newDirec = generate_random_pos(switch_num)[1];
    Asteroid newAsteroid{newPos, newDirec};
    asteroids.push_back(newAsteroid);
}

//updates the state of the asteroids (vector) in the game. Also generates
// asteroids at random times.
void
Model::on_frame_asteroid(double dt)
{
    if (is_game_over()) {
        set_game_over();
        return;
    }
    int switch_num = rand() % 4 + 1;
    int prob = rand() % 100;
    if (prob == 0) {
        generate_asteroid(switch_num);
    }

    for (unsigned int i = 0; i < asteroids.size(); i++) {

        Asteroid nextAsteroid = asteroids[i].asteroid_next(dt);

        if (config.hits_bottom(nextAsteroid.get_enemy_pos())
            || config.hits_top(nextAsteroid.get_enemy_pos()))
        {
            nextAsteroid.reflect_vertical();
            asteroids[i] = nextAsteroid;
        } else if (config.hits_left(nextAsteroid.get_enemy_pos())
                   || config.hits_right(nextAsteroid.get_enemy_pos()))
        {
            nextAsteroid.reflect_horizontal();
            asteroids[i] = nextAsteroid;
        } else {
            asteroids[i] = nextAsteroid;
        }
    }
}

//monster methods

//generates the monster at a random position, moving in a random direction,
// and with a random color (red, yellow, or green)
void
Model::generate_monster(int switch_num, int color_switch)
{
    switch(color_switch) {
    case 1: {
        Position newPos = generate_random_pos(switch_num)[0];
        Position newDirec = generate_random_pos(switch_num)[1];
        Monster newMonster{newPos, newDirec, 1};
        monsters.push_back(newMonster);
        break;
    }
    case 2: {
        Position newPos = generate_random_pos(switch_num)[0];
        Position newDirec = generate_random_pos(switch_num)[1];
        Monster newMonster{newPos, newDirec, 2};
        monsters.push_back(newMonster);
        break;
    }
    case 3: {
        Position newPos = generate_random_pos(switch_num)[0];
        Position newDirec = generate_random_pos(switch_num)[1];
        Monster newMonster{newPos, newDirec, 3};
        monsters.push_back(newMonster);
        break;
    }
    default: {
        Position newPos = generate_random_pos(switch_num)[0];
        Position newDirec = generate_random_pos(switch_num)[1];
        Monster newMonster{newPos, newDirec, 0};
        monsters.push_back(newMonster);
        break;
    }
    }
}

//updates the state of the monsters (vector) in the game. Also generates
// monsters at random times with randomized colors.
void
Model::on_frame_monster(double dt)
{
    if (is_game_over()) {
        set_game_over();
        return;
    }
    int switch_num = rand() % 4 + 1;
    int prob = rand() % 100;
    if (prob == 0) {
        int color_switch = rand() % 3 + 1;
        generate_monster(switch_num, color_switch);
    }

    for (unsigned int i = 0; i < monsters.size(); i++) {

        Monster nextMonster = monsters[i].monster_next(dt);

        if (config.hits_bottom(nextMonster.get_enemy_pos())
            || config.hits_top(nextMonster.get_enemy_pos())
            || config.hits_left(nextMonster.get_enemy_pos())
            || config.hits_right(nextMonster.get_enemy_pos()))
        {
            monsters[i] = monsters.back().monster_next(dt);
            monsters.pop_back();
        } else {
            monsters[i] = nextMonster;
        }
    }
}

//spaceship methods

//generates the spaceship at a random position, moving in a random direction
void
Model::generate_spaceship(int switch_num)
{
    Position newPos = generate_random_pos(switch_num)[0];
    Position newDirec = generate_random_pos(switch_num)[1];
    Spaceship newSpaceship{newPos, newDirec};
    spaceships.push_back(newSpaceship);
}

//updates the state of the spaceships (vector) in the game. Also makes each
// spaceship fire lasers in the direction they are heading every 100th frame.
void
Model::on_frame_spaceship(double dt)
{
    if (is_game_over()) {
        set_game_over();
        return;
    }
    int switch_num = rand() % 4 + 1;
    int prob = rand() % 100;
    if (prob == 0) {
        generate_spaceship(switch_num);
    }

    frame_counter++;
    if ((frame_counter % every_nth_frame) == 0) {
        for (unsigned int i = 0; i < spaceships.size(); i++) {
            if (i + 1 >= spaceships.size()) {
                Velocity direction{spaceships[0].get_enemy_vel().width,
                                   spaceships[0].get_enemy_vel().height};
                launch_spaceship_laser(spaceships[0], direction);
            } else {
                Velocity direction{spaceships[i+1].get_enemy_vel().width,
                                   spaceships[i+1].get_enemy_vel().height};
                launch_spaceship_laser(spaceships[i+1], direction);
            }
        }
    }

    for (unsigned int i = 0; i < spaceships.size(); i++) {

        Spaceship nextSpaceship = spaceships[i].spaceship_next(dt);

        if (config.hits_bottom(nextSpaceship.get_enemy_pos())
            || config.hits_top(nextSpaceship.get_enemy_pos())
            || config.hits_left(nextSpaceship.get_enemy_pos())
            || config.hits_right(nextSpaceship.get_enemy_pos()))
        {
            spaceships[i] = spaceships.back().spaceship_next(dt);
            spaceships.pop_back();
        } else {
            spaceships[i] = nextSpaceship;
        }
    }
}

//generates the lasers for the enemy spaceships when they are to be fired.
void
Model::generate_spaceship_laser(Spaceship& spaceship)
{
    spaceship_lasers.push_back(Laser{spaceship.get_enemy_pos().down_right_by
            ({spaceship.get_enemy_radius()-2,spaceship.get_enemy_radius()-2})});
    unfired_spaceship_index++;
}

//launches the laser for the enemy spaceship in the specified direction
void
Model::launch_spaceship_laser(Spaceship& spaceship, Velocity launch_direction)
{
    generate_spaceship_laser(spaceship);
    spaceship_lasers[unfired_spaceship_index].laser_spaceship_launch(launch_direction);
}

//helpers

//makes a unit vector out of two float values.
Model::Position
Model::make_unit(float x, float y)
{
    float direc_magnitude = sqrt(pow(x,2) + pow(y,2));
    return Position{x/direc_magnitude, y/direc_magnitude};
}

//sets the game over by clearing all entities (vectors)
void
Model::set_game_over()
{
    asteroids.clear();
    monsters.clear();
    spaceships.clear();
    healthboosts.clear();
    player_lasers.clear();
    spaceship_lasers.clear();
    our_player_.set_player_dead();
}
