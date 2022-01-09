#include "spaceship.hxx"

static const float spaceship_vel = 100;

//Constructs an enemy spaceship with initial position and initial velocity (in
// the specified direction)
Spaceship::Spaceship(Position spaceship_pos, Position direction)
            : spaceship_pos(spaceship_pos)
            , spaceship_velocity(Velocity{spaceship_vel*direction.x,
                                          spaceship_vel*direction.y})
{ }

//Produces a copy of the monster at its next state (dt seconds later)
Spaceship
Spaceship::spaceship_next(double dt)
{
    Spaceship result{*this};

    float d_x = result.spaceship_velocity.width * dt;
    float d_y = result.spaceship_velocity.height * dt;

    result.spaceship_pos.x += d_x;
    result.spaceship_pos.y += d_y;
    return result;
}