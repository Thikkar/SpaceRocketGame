#include "game_config.hxx"

//constructs the game_config with the initial window dimensions
Game_config::Game_config()
        : scene_dims(1024, 768)
{ }

//returns whether the entity hits the top of the window
bool
Game_config::hits_top(Position entity_pos) const
{
    return entity_pos.y < 0;
}

//returns whether the entity hits the bottom of the window
bool
Game_config::hits_bottom(Position entity_pos) const
{
    return entity_pos.y > scene_dims.height;
}

//returns whether the entity hits the left of the window
bool
Game_config::hits_left(Position entity_pos) const
{
    return entity_pos.x < 0;
}

//returns whether the entity hits the right of the window
bool
Game_config::hits_right(Position entity_pos) const
{
    return entity_pos.x > scene_dims.width;
}

