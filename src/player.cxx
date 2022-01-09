#include "player.hxx"

static double const max_health = 100;
static float const player_velocity = 10;

//Constructs our player (no parameters)
Player::Player()
        :Player(Position{0,0})
{}

//Constructs our player at the specified position
Player::Player(Position player_pos)
        : player_pos(player_pos)
        , health(max_health)
{}

//moves our player right or left
void
Player::player_horiz_move(bool move_right, Game_config const& config)
{
    if (move_right) {
        player_pos.x += player_velocity;
        player_vel.width = player_velocity;
    } else {
        player_pos.x -= player_velocity;
        player_vel.width = -player_velocity;
    }
}

//moves our player up or down
void
Player::player_vert_move(bool move_up, Game_config const& config)
{
    if (move_up) {
        player_pos.y -= player_velocity;
        player_vel.height = -player_velocity;
    } else {
        player_pos.y += player_velocity;
        player_vel.height = player_velocity;
    }
}

//stops player's right/left movement
void
Player::player_stop_horiz_move()
{
    player_vel.width = 0;
}

//stops player's up/down movement
void
Player::player_stop_vert_move()
{
    player_vel.height = 0;
}

//player emerges from the bottom when it goes past the top of the window
void
Player::player_emerge_bottom(const Game_config& config)
{
    player_pos.x = player_pos.x;
    player_pos.y = config.scene_dims.height;
}

//player emerges from the top when it goes past the bottom of the window
void
Player::player_emerge_top(const Game_config& config)
{
    player_pos.x = player_pos.x;
    player_pos.y = 0;
}

//player emerges from the right when it goes past the left of the window
void
Player::player_emerge_right(const Game_config& config)
{
    player_pos.x = config.scene_dims.width;
    player_pos.y = player_pos.y;
}

//player emerges from the left when it goes past the right of the window
void
Player::player_emerge_left(const Game_config& config)
{
    player_pos.x = 0;
    player_pos.y = player_pos.y;
}

//updates the player's score when it destroys an enemy
void
Player::update_player_score(double amount)
{
    score += amount;
}

//returns whether or not our player has hit any enemy/entity
bool
Player::player_hits_entity(Enemy& enemy)
{
    float enemy_rad = enemy.get_enemy_radius();

    Position enemy_center{enemy.get_enemy_pos().down_right_by({enemy_rad,
                                                               enemy_rad})};
    Position player_center{player_pos.down_right_by({radius, radius})};
    float rad_sum = enemy_rad + radius;

    float dx = player_center.x - enemy_center.x;
    float dy = player_center.y - enemy_center.y;
    return dx*dx + dy*dy <= rad_sum*rad_sum;
}

//if a laser from an enemy spaceship hits the player, the player loses some,
// fixed health and the enemy laser is destroyed
void
Player::laser_hits_player(std::vector<Laser>& lasers)
{
    for (Laser& laser : lasers) {
        float laser_rad = laser.get_laser_radius();

        Position laser_center {laser.get_laser_pos().down_right_by({laser_rad,
                                                                    laser_rad})};
        Position player_center {player_pos.down_right_by({radius, radius})};
        float rad_sum = laser_rad + radius;

        float dx = player_center.x - laser_center.x;
        float dy = player_center.y - laser_center.y;
        if (dx * dx + dy * dy <= rad_sum * rad_sum) {
            health -= laser.get_laser_damage();
            laser.laser_collide();
            return;
        };
    }
}

//if our player has hit any of the following: asteroid, monster, enemy
// spaceship -- it will destroy that object and incur some, fixed damage
void
Player::player_attacked(
        std::vector<Asteroid>& asteroids,
        std::vector<Monster>& monsters,
        std::vector<Spaceship>& spaceships)
{
    for (Asteroid& asteroid : asteroids) {
        if (player_hits_entity(asteroid)) {
            double damage = asteroid.get_enemy_attack_damage();
            asteroid = asteroids.back();
            asteroids.pop_back();
            health -= damage;
            return;
        }
    }

    for (Monster& monster : monsters) {
        if (player_hits_entity(monster)) {
            double damage = monster.get_enemy_attack_damage();
            monster = monsters.back();
            monsters.pop_back();
            health -= damage;
            return;
        }
    }

    for (Spaceship& spaceship : spaceships) {
        if (player_hits_entity(spaceship)) {
            double damage = spaceship.get_enemy_attack_damage();
            spaceship = spaceships.back();
            spaceships.pop_back();
            health -= damage;
            return;
        }
    }
}

//returns whether or nor a player hits a healthboost
bool
Player::player_hits_boost(HealthBoost& healthboost)
{
    float boost_rad = healthboost.get_healthboost_radius();

    Position healthboost_center{healthboost.get_healthboost_pos()
                                           .down_right_by({boost_rad, boost_rad})};
    Position player_center{player_pos.down_right_by({radius, radius})};
    float rad_sum = boost_rad + radius;

    float dx = player_center.x - healthboost_center.x;
    float dy = player_center.y - healthboost_center.y;
    return dx*dx + dy*dy <= rad_sum*rad_sum;
}

//if a player hits a healthboost, it will gain some, fixed amount of health
void
Player::player_health_boost(std::vector<HealthBoost>& healthboosts)
{
    for (HealthBoost& healthboost : healthboosts) {
        if (player_hits_boost(healthboost)) {
            health += healthboost.get_healthboost_bonus();
            healthboost = healthboosts.back();
            healthboosts.pop_back();
            if (health >= max_health) {
                health = max_health;
            }
        }
    }
}

//Produces a copy of our player at its next state (dt seconds later)
Player
Player::player_next(double dt) const
{
    Player result(*this);

    float d_x = result.player_vel.width * dt;
    float d_y = result.player_vel.height * dt;

    result.player_pos.x += d_x;
    result.player_pos.y += d_y;
    return result;
}

//when player dies, it moves off screen and can no longer move
void
Player::set_player_dead()
{
    player_pos = Position{-100,-100};
    player_vel = Velocity{0,0};
}
