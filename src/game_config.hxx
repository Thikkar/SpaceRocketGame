#pragma once
#include <ge211.hxx>

class Game_config
{
public:
    Game_config();
    using Position = ge211::Posn<float>;

    ge211::Dims<int> scene_dims;

    bool hits_top(Position entity_pos) const;

    bool hits_bottom(Position entity_pos) const;

    bool hits_left(Position entity_pos) const;

    bool hits_right(Position entity_pos) const;
};