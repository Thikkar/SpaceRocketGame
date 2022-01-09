#include <math.h>
#include "view.hxx"

#define PI 3.1415926535

using Color = ge211::Color;

static const Color red_monster_color{255,0,0};
static const Color yellow_monster_color{255,255,0};
static const Color green_monster_color{0,255,0};
static const Color healthboost_color{255,0,255};
static const Color white{255,255,255};
static const Color red{255,0,0};
static const Color blue{0,0,255};
static const ge211::Dims<int> monster_dims{50,50};
static const int asteroid_radius = 50;
static const int laser_radius = 2;
static const int healthboost_radius = 10;

//initializes all the members of view
View::View(Model const& model)
        : model_(model)
        , player_laser_sprite(laser_radius, blue)
        , spaceship_laser_sprite(laser_radius, red)
        , asteroid_sprite(asteroid_radius)
        , healthboost_sprite(healthboost_radius, healthboost_color)
        , red_monster_sprite(monster_dims, red_monster_color)
        , yellow_monster_sprite(monster_dims, yellow_monster_color)
        , green_monster_sprite(monster_dims,green_monster_color)
{
    ge211::Text_sprite::Builder score_builder(sans30);
    score_builder.color(red_monster_color);
    player_score_sprite.reconfigure(score_builder);

    ge211::Text_sprite::Builder health_builder(sans30);
    health_builder.color(red_monster_color);
    player_health_sprite.reconfigure(health_builder);
}

//draws all the sprites to the screen
void
View::draw(ge211::Sprite_set& set, ge211::Posn<int> mouse_pos, int score,
           double health)
{
    //game over state
    if(model_.is_game_over()) {
        set.add_sprite(game_over_label, ge211::Posn<int>{275, 200},4);
        ge211::Text_sprite::Builder score_builder(sans30);
        score_builder.color(white) << score;
        player_score_sprite.reconfigure(score_builder);
        ge211::Posn<int> const score_label_pos = ge211::Posn<int>{280,
                                                                  325};
        ge211::Posn<int> const score_pos{score_label_pos.x + 100,
                                         score_label_pos.y};
        set.add_sprite(player_score_sprite, score_pos, 3);
        set.add_sprite(score_label, score_label_pos, 3);
        return;
    }

    // our player view
    ge211::Posn<int> player_posn = model_.get_player_pos().into<int>();
    double rise = (double) (mouse_pos.y - player_posn.y);
    double run = (double) (mouse_pos.x - player_posn.x);
    double player_mouse_slope = rise/run;
    double player_rot_deg = atan(player_mouse_slope) * 180/PI + 90;
    if (run < 0) {
        player_rot_deg += 180;
    }

    ge211::Transform player_rot_trans = ge211::Transform{}.rotation
                                                                  (player_rot_deg);
    ge211::Transform const player_scale_trans = ge211::Transform{}.scale(0.1);
    ge211::Transform player_trans = player_rot_trans * player_scale_trans;

    set.add_sprite(our_spaceship, player_posn, 2, player_trans);

    //player lasers view
    for (Laser laser : model_.get_player_lasers()) {
        int laser_pos_x = laser.get_laser_pos().x;
        int laser_pos_y = laser.get_laser_pos().y;
        ge211::Posn<int> newLaserPos{laser_pos_x, laser_pos_y};
        set.add_sprite(player_laser_sprite, newLaserPos, 1);
    }

    //(enemy) spaceship lasers view
    for (Laser laser : model_.get_spaceship_lasers()) {
        int laser_pos_x = laser.get_laser_pos().x;
        int laser_pos_y = laser.get_laser_pos().y;
        ge211::Posn<int> newLaserPos{laser_pos_x, laser_pos_y};
        set.add_sprite(spaceship_laser_sprite, newLaserPos, 1);
    }

    //asteroid view
    for (Asteroid asteroid : model_.get_asteroids()) {
        ge211::Posn<int> ast_pos_int = asteroid.get_enemy_pos().into<int>();
        set.add_sprite(asteroid_sprite, ast_pos_int, 4);
    }

    //monster view
    for (Monster monster : model_.get_monsters()) {
        ge211::Posn<int> monster_pos_int = monster.get_enemy_pos().into<int>();
        if (monster.get_monster_color() == 1) {
            set.add_sprite(red_monster_sprite, monster_pos_int, 4);
        } else if (monster.get_monster_color() == 2) {
            set.add_sprite(yellow_monster_sprite, monster_pos_int, 4);
        } else if (monster.get_monster_color() == 3) {
            set.add_sprite(green_monster_sprite, monster_pos_int, 4);
        }
    }

    //spaceship view
    for (Spaceship spaceship : model_.get_spaceships()) {
        ge211::Posn<int> spaceship_pos_int = spaceship.get_enemy_pos()
                                                      .into<int>();
        ge211::Posn<int> origin{0,0};
        double spaceship_rise = (double) spaceship.get_enemy_vel().height;
        double spaceship_run = (double) spaceship.get_enemy_vel().width;
        double spaceship_vel_slope = spaceship_rise/spaceship_run;
        double spaceship_rot_deg = atan(spaceship_vel_slope) * 180/PI + 90;
        if (spaceship_run < 0) {
            spaceship_rot_deg += 180;
        }

        ge211::Transform spaceship_rot_trans = ge211::Transform{}.rotation(spaceship_rot_deg);
        ge211::Transform spaceship_scale_trans = ge211::Transform{}.scale(0.1);
        ge211::Transform spaceship_trans = spaceship_rot_trans * spaceship_scale_trans;
        set.add_sprite(enemy_spaceship, spaceship_pos_int, 3, spaceship_trans);
    }

    //healthboost view
    for (HealthBoost healthboost : model_.get_healthboosts()) {
        ge211::Posn<int> boost_pos_int = healthboost.get_healthboost_pos().into<int>();
        set.add_sprite(healthboost_sprite, boost_pos_int, 3);
    }

    //Text sprites
    ge211::Posn<int> const score_label_pos = ge211::Posn<int>{5,
                                                              0};
    ge211::Posn<int> const score_pos{score_label_pos.x + 100,
                                     score_label_pos.y};
    ge211::Posn<int> const health_label_pos{score_label_pos.x,
                                            score_label_pos.y + 50};
    ge211::Posn<int> const health_pos{health_label_pos.x + 100,
                                      health_label_pos.y };

    ge211::Text_sprite::Builder score_builder(sans30);
    score_builder.color(white) << score;
    player_score_sprite.reconfigure(score_builder);
    set.add_sprite(player_score_sprite, score_pos, 5);
    set.add_sprite(score_label, score_label_pos, 5);

    ge211::Text_sprite::Builder health_builder(sans30);
    health_builder.color(white) << health;
    player_health_sprite.reconfigure(health_builder);
    set.add_sprite(player_health_sprite, health_pos, 5);
    set.add_sprite(health_label, health_label_pos, 5);
}

//sets the window dimensions as specified by game_config
ge211::Dims<int>
View::initial_window_dimensions() const
{
    return model_.config.scene_dims;
}
