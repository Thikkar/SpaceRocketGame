#include "laser.hxx"
#include <math.h>

static float const launch_velocity = 400;

//Constructs a laser (no parameters)
Laser::Laser()
        : Laser(Position{0,0})
{ }

//Constructs a laser with specified initial laser position
Laser::Laser(Position laser_pos)
        : velocity_(Velocity{0,0})
        , laser_pos_(laser_pos)
        , orientation_(Position{0,0})
        , live_(false)
{ }

//launches the laser in the given direction (used by player)
void
Laser::laser_launch(Position destination)
{
    live_ = true;
    ge211::Posn<float> dest_int{destination.x, destination.y};
    ge211::Posn<float> direction{dest_int.x - laser_pos_.x,
                               dest_int.y - laser_pos_.y};

    float direc_magnitude = sqrt(pow(direction.x,2) + pow(direction.y,2));
    ge211::Posn<float> unit_direc = ge211::Posn<float>{direction.x/direc_magnitude,
                                                   direction.y/direc_magnitude};
    Position unit_direc_float = unit_direc.into<float>();

    velocity_ = Velocity{launch_velocity*unit_direc_float.x,
                         launch_velocity*unit_direc_float.y};
}

//launches the laser in the direction of vel (used by the enemy spaceships)
void
Laser::laser_spaceship_launch(Velocity vel)
{
    live_ = true;
    float vel_magnitude = sqrt(pow(vel.width,2) + pow(vel.height,2));
    Velocity unit_direc{vel.width/vel_magnitude, vel.height/vel_magnitude};
    velocity_ = Velocity{launch_velocity*unit_direc.width,
                         launch_velocity*unit_direc.height};
}

//when the laser collies with something, it is no longer live
void
Laser::laser_collide()
{
    live_ = false;
}

//Produces a copy of the laser at its next state (dt seconds later)
Laser
Laser::laser_next(double dt) const
{
    Laser result(*this);

    float d_x = result.velocity_.width * dt;
    float d_y = result.velocity_.height * dt;

    result.laser_pos_.x += d_x;
    result.laser_pos_.y += d_y;
    return result;
}

//returns whether or not the laser has hit any enemy
bool
Laser::laser_hits_entity(Enemy& enemy) const
{
    float enemy_rad = enemy.get_enemy_radius();
    Position enemy_center{enemy.get_enemy_pos().down_right_by({enemy_rad,
                                                               enemy_rad})};
    Position laser_center{laser_pos_.down_right_by({radius, radius})};
    float rad_sum = enemy_rad + radius;

    float dx = laser_center.x - enemy_center.x;
    float dy = laser_center.y - enemy_center.y;
    return dx*dx + dy*dy <= rad_sum*rad_sum;
}

//if the laser has hit any of the following: asteroid, monster, enemy
// spaceship -- it will destroy that object and itself. It returns the score
// total for shooting that object (used to update player's score)
double
Laser::laser_destroy_entity(
        std::vector<Asteroid>& asteroids,
        std::vector<Monster>& monsters,
        std::vector<Spaceship>& spaceships)
{
    for (Asteroid& asteroid : asteroids) {
        if (laser_hits_entity(asteroid)) {
            asteroid = asteroids.back();
            asteroids.pop_back();
            laser_collide();
            return asteroid.get_enemy_score_total();
        }
    }

    for (Monster& monster : monsters) {
        if (laser_hits_entity(monster)) {
            monster = monsters.back();
            monsters.pop_back();
            laser_collide();
            return monster.get_enemy_score_total();
        }
    }

    for (Spaceship& spaceship : spaceships) {
        if (laser_hits_entity(spaceship)) {
            spaceship = spaceships.back();
            spaceships.pop_back();
            laser_collide();
            return spaceship.get_enemy_score_total();
        }
    }
    return 0;
}