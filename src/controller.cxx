#include "controller.hxx"

//constructs the controller
Controller::Controller()
        : view_(model_)
        , mouse_pos(ge211::Posn<int>{0,0}) //can change value
{ }

// used to move the player around the screen and shoot lasers
void
Controller::on_key(ge211::Key key)
{
    if (key == ge211::Key::code('w')) {
        model_.player_vert_move(true);
    }

    if (key == ge211::Key::code('s')) {
        model_.player_vert_move(false);
    }

    if (key == ge211::Key::code('d')) {
        model_.player_horiz_move(true);
    }

    if (key == ge211::Key::code('a')) {
        model_.player_horiz_move(false);
    }

    if (key == ge211::Key::code('q')) {
        model_.player_vert_move(true);
        model_.player_horiz_move(false);
    }

    if (key == ge211::Key::code('e')) {
        model_.player_vert_move(true);
        model_.player_horiz_move(true);
    }

    if (key.type() == ge211::Key::Type::shift) {
        model_.player_vert_move(false);
        model_.player_horiz_move(false);
    }

    if (key == ge211::Key::code('c')) {
        model_.player_vert_move(false);
        model_.player_horiz_move(true);
    }

    if (key == ge211::Key::code(' ')) {
        model_.laser_launch(ge211::Posn<float>{(float) mouse_pos.x,
                                               (float) mouse_pos.y});
    }

    model_.player_stop_vert_move();
    model_.player_stop_horiz_move();
}

//updates the mouse position field of controller given the mouse position on
// the screen
void
Controller::on_mouse_move(ge211::Posn<int> position)
{
    mouse_pos = position;
}

//launches the laser on left-click
void
Controller::on_mouse_down(ge211::Mouse_button, ge211::Posn<int> pos)
{
    model_.laser_launch(ge211::Posn<float>{(float) mouse_pos.x,
                                           (float) mouse_pos.y});
}

//updates the state of all entities in the game
void
Controller::on_frame(double dt)
{
    model_.on_frame_player(dt);
    model_.on_frame_healthboost(dt);
    model_.on_frame_laser(dt);
    model_.on_frame_asteroid(dt);
    model_.on_frame_monster(dt);
    model_.on_frame_spaceship(dt);
}

//calls View::draw
void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set, mouse_pos, model_.get_player_score(), model_.get_player_health());
}

//calls View:initial_window_dimensions
ge211::Dims<int>
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}
