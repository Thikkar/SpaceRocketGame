#include "model.hxx"
#include <catch.hxx>

//**DISCLAIMER**
//_______________
// This game utilizes random generation. We have designed these model tests
// to the best of our ability to mitigate any randomness. However, it may be
// possible that a random number causes one or more of our model tests to fail.
// For example, a random asteroid, spaceship, or monster might spawn as a
// result of on_frame that causes the .size() check to be off. None of this
// affects actual gameplay.

using Position = ge211::Posn<float>;

//friend class to access model's private members for testing
struct Test_access
{
    Model& model;

    Player
    get_player()
    {
        return model.our_player_;
    }

    int
    get_every_nth()
    {
        return model.every_nth_frame;
    }

    std::vector<Asteroid>
    get_asteroids()
    {
        return model.get_asteroids();
    }

    std::vector<Spaceship>
    get_spaceships()
    {
        return model.get_spaceships();
    }

};

//Tests that all the initial conditions of the model are correct
TEST_CASE("model init")
{
    Model m{};
    Test_access t{m};

    CHECK( m.get_player_pos() == Position{492,364} );
    CHECK( m.get_player_lasers()[0].get_laser_pos() == Position{m
                                                                        .get_player_pos().down_right_by({38,38})} );
    CHECK( m.get_player_lasers().size() == 1 );
    CHECK( m.get_spaceship_lasers().size() == 0);
    CHECK( m.get_asteroids().size() == 0 );
    CHECK( m.get_monsters().size() == 0 );

    CHECK( t.get_player().get_player_health() == 100 );
    CHECK( t.get_player().get_player_score() == 0 );
}

//Tests basic player movements (up, down, left, right) and makes sure the
// player moves accordingly
TEST_CASE("Player move")
{
    Model m{};

    CHECK( m.get_player_pos() == Position{492,364} );
    m.player_vert_move(true);
    m.player_horiz_move(false);
    CHECK( m.get_player_pos() == Position{482,354} );
    m.player_vert_move(false);
    m.player_vert_move(false);
    m.player_horiz_move(true);
    m.player_horiz_move(true);
    CHECK( m.get_player_pos() == Position{502,374} );
}

//tests asteroids generation and movement using asteroid_next and
// on_frame_asteroid
TEST_CASE("model asteroid")
{
    Model m{};

    m.generate_asteroid(1);
    m.generate_asteroid(2);
    m.generate_asteroid(3);
    m.generate_asteroid(4);

    CHECK( m.get_asteroids()[0].get_enemy_pos().x == 0 );
    CHECK( m.get_asteroids()[0].get_enemy_vel().width >= 0 );
    CHECK( m.get_asteroids()[1].get_enemy_pos().x == m.config.scene_dims.width );
    CHECK( m.get_asteroids()[1].get_enemy_vel().width <= 0 );
    CHECK( m.get_asteroids()[2].get_enemy_pos().y == 0 );
    CHECK( m.get_asteroids()[2].get_enemy_vel().height >= 0 );
    CHECK( m.get_asteroids()[3].get_enemy_pos().y == m.config.scene_dims.height );
    CHECK( m.get_asteroids()[3].get_enemy_vel().height <= 0 );

    CHECK( m.get_asteroids().size() == 4 );

    Asteroid nextAstLeft = m.get_asteroids()[0].asteroid_next(0.01);
    Asteroid nextAstRight = m.get_asteroids()[1].asteroid_next(0.01);
    Asteroid nextAstTop = m.get_asteroids()[2].asteroid_next(0.01);
    Asteroid nextAstBottom = m.get_asteroids()[3].asteroid_next(0.01);

    m.on_frame_asteroid(0.01);
    CHECK( m.get_asteroids()[0].get_enemy_pos() == nextAstLeft.get_enemy_pos());
    CHECK( m.get_asteroids()[0].get_enemy_vel() == nextAstLeft.get_enemy_vel());
    CHECK( m.get_asteroids()[1].get_enemy_pos() == nextAstRight.get_enemy_pos());
    CHECK( m.get_asteroids()[1].get_enemy_vel() == nextAstRight.get_enemy_vel());
    CHECK( m.get_asteroids()[2].get_enemy_pos() == nextAstTop.get_enemy_pos());
    CHECK( m.get_asteroids()[2].get_enemy_vel() == nextAstTop.get_enemy_vel());
    CHECK( m.get_asteroids()[3].get_enemy_pos() == nextAstBottom.get_enemy_pos());
    CHECK( m.get_asteroids()[3].get_enemy_vel() == nextAstBottom.get_enemy_vel());
}

//tests enemy spaceship generation and movement as well as their ability to
// shoot lasers every nth frame
TEST_CASE("model enemy spaceship")
{
    Model m{};
    Test_access t{m};

    m.generate_spaceship(3);

    CHECK( m.get_spaceships().size() == 1 );
    CHECK( m.get_spaceships()[0].get_enemy_pos().y == 0 );
    CHECK( m.get_spaceships()[0].get_enemy_vel().height >= 0);
    CHECK( m.get_spaceship_lasers().size() == 0 );

    Spaceship nextSpaceship = m.get_spaceships()[0].spaceship_next(0.0001);

    m.on_frame_spaceship(0.0001);
    //CHECK( m.get_spaceships().size() == 1 );
    CHECK( m.get_spaceships()[0].get_enemy_pos() == nextSpaceship.get_enemy_pos());

    for (int i = 0; i < t.get_every_nth() + 1; i++) {
        m.on_frame_spaceship(0.0001);
    }

    int spaceships_size = m.get_spaceships().size();
    CHECK( m.get_spaceship_lasers().size() == m.get_spaceships().size() );

    for (int i = 0; i < t.get_every_nth() + 1; i++) {
        m.on_frame_spaceship(0.0001);
    }

    CHECK( m.get_spaceship_lasers().size() == m.get_spaceships().size() + spaceships_size );

}

//tests monsters generation and movement. Also tests the differences in speed
// in different colored monsters
TEST_CASE("Model monster")
{
    Model m{};
    CHECK( m.get_asteroids().size() == 0 );

    m.generate_monster(1, 1); //red
    m.generate_monster(1, 2); //yellow
    m.generate_monster(1, 3); //green

    CHECK( m.get_monsters().size() == 3 );
    CHECK( m.get_monsters()[0].get_monster_color() == 1 );
    CHECK( m.get_monsters()[1].get_monster_color() == 2 );
    CHECK( m.get_monsters()[2].get_monster_color() == 3 );

    Monster nextRedMon = m.get_monsters()[0].monster_next(0.01);
    Monster nextYellowMon = m.get_monsters()[1].monster_next(0.01);
    Monster nextGreenMon = m.get_monsters()[2].monster_next(0.01);

    float red_mag = sqrt(pow(nextRedMon.get_enemy_vel().width,2) +
                         pow(nextRedMon.get_enemy_vel().height,2));
    float yellow_mag = sqrt(pow(nextYellowMon.get_enemy_vel().width,2) +
                            pow(nextYellowMon.get_enemy_vel().height,2));
    float green_mag = sqrt(pow(nextGreenMon.get_enemy_vel().width,2) +
                           pow(nextGreenMon.get_enemy_vel().height,2));

    CHECK( round(red_mag) == 200 );
    CHECK( round(yellow_mag) == 300 );
    CHECK( round(green_mag) == 400 );

    m.on_frame_monster(0.01);
    CHECK( m.get_monsters()[0].get_monster_color() == 1 );
    CHECK( m.get_monsters()[1].get_monster_color() == 2 );
    CHECK( m.get_monsters()[2].get_monster_color() == 3 );

    CHECK( m.get_monsters()[0].get_enemy_pos() == nextRedMon.get_enemy_pos());
    CHECK( m.get_monsters()[0].get_enemy_vel() == nextRedMon.get_enemy_vel());
    CHECK( m.get_monsters()[1].get_enemy_pos() == nextYellowMon.get_enemy_pos());
    CHECK( m.get_monsters()[1].get_enemy_vel() == nextYellowMon.get_enemy_vel());
    CHECK( m.get_monsters()[2].get_enemy_pos() == nextGreenMon.get_enemy_pos());
    CHECK( m.get_monsters()[2].get_enemy_vel() == nextGreenMon.get_enemy_vel());
}

//tests player collisions with other objects (not enemy spaceship). Tracks
// damage dealt and foreign object destruction
TEST_CASE("Player collision")
{
    Model m {};
    Test_access t {m};

    Asteroid ast {Position {492, 364}, Position {1, 0}};
    CHECK(t.get_asteroids().size() == 0);
    std::vector<Asteroid> asteroids = t.get_asteroids();
    asteroids.push_back(ast);
    CHECK(asteroids.size() == 1);

    Player our_player = t.get_player();
    CHECK(our_player.player_hits_entity(asteroids[0]));
    std::vector<Monster> monsters;
    std::vector<Spaceship> spaceships;
    our_player.player_attacked(asteroids, monsters, spaceships);
    CHECK(our_player.get_player_health() == 90);
}

//tests laser collision with other objects and tracks the destruction of the
// object as well as the laser
TEST_CASE("Laser Collisions")
{
    Model m{};
    Test_access t{m};

    Asteroid ast {Position {500, 364}, Position {-1, 0}};
    std::vector<Asteroid> asteroids = t.get_asteroids();
    asteroids.push_back(ast);
    std::vector<Laser> lasers = m.get_player_lasers();
    lasers[0].laser_launch(ast.get_enemy_pos());

    Asteroid dummyAst{Position{1,1}, Position{1,1}};
    Laser dummyLaser{};
    asteroids.push_back(dummyAst);
    lasers.push_back(dummyLaser);
    std::vector<Monster> monsters;
    std::vector<Spaceship> spaceships;

    CHECK( lasers.size() == 2 );
    CHECK( asteroids.size() == 2 );
    for (int i = 0; i < 100; i++) {
        lasers[0] = lasers[0].laser_next(0.01);
        if (lasers[0].laser_hits_entity(asteroids[0])) {
            lasers[0].laser_destroy_entity(asteroids, monsters, spaceships);
            break;
        }
    }
    CHECK( lasers.size() == 2 );
    CHECK_FALSE( lasers[0].laser_is_live() );
    CHECK( asteroids.size() == 1 );
}

//tests the game over state, when player's health is <= 0. All foreign object
// vectors should also be cleared
TEST_CASE("Game over")
{
    Model m {};
    Test_access t {m};

    Spaceship ship{Position {492, 364}, Position {1, 0}};
    Spaceship ship2{Position {700, 500}, Position {1, 0}};
    Spaceship ship3{Position {100, 100}, Position {1, 0}};
    CHECK(t.get_spaceships().size() == 0);
    std::vector<Spaceship> spaceships = t.get_spaceships();
    spaceships.push_back(ship);
    CHECK(spaceships.size() == 1);
    spaceships.push_back(ship2);
    CHECK(spaceships.size() == 2);
    spaceships.push_back(ship3);
    CHECK(spaceships.size() == 3);

    Player our_player = t.get_player();
    CHECK(our_player.player_hits_entity(spaceships[0]));
    std::vector<Asteroid> asteroids;
    std::vector<Monster> monsters;
    our_player.player_attacked(asteroids, monsters, spaceships);
    CHECK(our_player.get_player_health() == 0);
    m.on_frame_player(1);
    CHECK(t.get_spaceships().size() == 0);
}