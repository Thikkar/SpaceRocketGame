#pragma once

#include "model.hxx"

class View
{
public:
    explicit View(Model const& model);

    void draw(ge211::Sprite_set& set, ge211::Posn<int> mouse_pos, int score,
              double health);

    ge211::Dims<int> initial_window_dimensions() const;

private:
    Model const& model_;

    ge211::Circle_sprite const player_laser_sprite;
    ge211::Circle_sprite const spaceship_laser_sprite;
    ge211::Circle_sprite const asteroid_sprite;
    ge211::Circle_sprite const healthboost_sprite;
    ge211::Rectangle_sprite const red_monster_sprite;
    ge211::Rectangle_sprite const yellow_monster_sprite;
    ge211::Rectangle_sprite const green_monster_sprite;

    ge211::Font sans30{"sans.ttf", 30};
    ge211::Font sans72{"sans.ttf", 72};
    ge211::Text_sprite score_label{"Score:", sans30};
    ge211::Text_sprite health_label{"Health:", sans30};
    ge211::Text_sprite player_score_sprite;
    ge211::Text_sprite player_health_sprite;
    ge211::Text_sprite game_over_label{"GAME OVER!", sans72};

    ge211::Image_sprite our_spaceship{"OurSpaceship.png"};
    ge211::Image_sprite enemy_spaceship{"EnemySpaceship.png"};


};
