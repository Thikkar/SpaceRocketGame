#include "monster.hxx"

static const float red_monster_vel = 200;
static const float yellow_monster_vel = 300;
static const float green_monster_vel = 400;

// Constructs a monster with initial position, initial velocity (in the
// specified direction, and color (1: "red", 2: "yellow", 3: "green")
Monster::Monster(Position monster_pos, Position direction, int color)
            : monster_pos(monster_pos)
            , color(color)
{
    if (color == 1) {
        monster_velocity = Velocity {red_monster_vel * direction.x,
                                     red_monster_vel * direction.y};
    } else if (color == 2) {
        monster_velocity = Velocity {yellow_monster_vel * direction.x,
                                     yellow_monster_vel * direction.y};
    } else if (color == 3) {
        monster_velocity = Velocity {green_monster_vel * direction.x,
                                     green_monster_vel * direction.y};
    } else {
        monster_velocity = Velocity {0,0};
    }
}

//Produces a copy of the monster at its next state (dt seconds later)
Monster
Monster::monster_next(double dt)
{
    Monster result{*this};

    float d_x = result.monster_velocity.width * dt;
    float d_y = result.monster_velocity.height * dt;

    result.monster_pos.x += d_x;
    result.monster_pos.y += d_y;
    return result;
}