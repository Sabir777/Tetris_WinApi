#pragma once
#include <set>
#include "Options.h"

using std::set;

class Parrent {
public:
    Parrent() = default;
    virtual ~Parrent() = default;
    virtual Game_State get_game_state() { return Game_State::PLAY; } //Figure - ����� �� Game
    virtual Figure_State get_figure_state() { return Figure_State::MOVE; } //Figure - ����� �� Game
    virtual void pause_figure(){} //Figure - ����� �� Game
    virtual void init_new_figure(){} //Figure - ����� �� Game
    virtual void set_time_down(int){} //Figure - ����� �� Game
    virtual void destroy_row(set<int>){} //Game - ����� �� Figure
    virtual void set_flash(set<int>){} //Game - ����� �� Figure
    virtual Flash get_flash() { return Flash::OFF; } //Game - ����� �� Figure
    virtual set<int> get_row() { return set<int>{}; } //Game - ����� �� Figure
};