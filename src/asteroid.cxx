#include "asteroid.hxx"
#include <stdlib.h>

//make input unit_vel then multiply by const
static const float asteroid_vel = 100;

//Constructs an asteroid with initial position and initial velocity (in the
// specified direction)
Asteroid::Asteroid(Position asteroid_pos, Position direction)
            : asteroid_pos(asteroid_pos)
            , asteroid_velocity(Velocity{asteroid_vel*direction.x,
                                         asteroid_vel*direction.y})
{}

//Produces a copy of the asteroid at its next state (dt seconds later)
Asteroid
Asteroid::asteroid_next(double dt)
{
    Asteroid result{*this};

    float d_x = result.asteroid_velocity.width * dt;
    float d_y = result.asteroid_velocity.height * dt;

    result.asteroid_pos.x += d_x;
    result.asteroid_pos.y += d_y;
    return result;
}

//reflects the asteroid horizontally when it hits the right or left edge of
//the window
void
Asteroid::reflect_horizontal()
{
    asteroid_velocity.width *= -1;
}

//reflects the asteroid vertically when it hits the top or bottom edge of
//the window
void
Asteroid::reflect_vertical()
{
    asteroid_velocity.height *= -1;
}