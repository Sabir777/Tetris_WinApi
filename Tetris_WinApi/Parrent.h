#pragma once
#include <set>
#include "Options.h"

using std::set;

class Parrent {
public:
    Parrent() = default;
    virtual ~Parrent() = default;
    virtual Game_State get_game_state() { return Game_State::PLAY; } //Figure - גûחמג טח Game
    virtual Figure_State get_figure_state() { return Figure_State::MOVE; } //Figure - גûחמג טח Game
    virtual void pause_figure(){} //Figure - גûחמג טח Game
    virtual void init_new_figure(){} //Figure - גûחמג טח Game
    virtual void set_time_down(int){} //Figure - גûחמג טח Game
    virtual void destroy_row(set<int>){} //Game - גûחמג טח Figure
    virtual void set_flash(set<int>){} //Game - גûחמג טח Figure
    virtual Flash get_flash() { return Flash::OFF; } //Game - גûחמג טח Figure
    virtual set<int> get_row() { return set<int>{}; } //Game - גûחמג טח Figure
};