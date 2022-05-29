#include "Figure.h"


/*-------------------------------------------------------------
             ����������� � ����������������� ��������
--------------------------------------------------------------*/
Figure::Figure(ARRAY_10_20& field, AR_BRUSH& brush, Size_Window& sz, Parrent* p_game)
    : Parrent(), field{ field }, brush{ brush }, sz{ sz }, p_game{p_game}
{
    n_type_figure = rand() % 7; //������� ������� ��������� ������
    next_n_type_figure = rand() % 7; //������� ��������� ��������� ������
    this_figure = figure_set[n_type_figure]; //������� ��������� ������ �� ������� �����
    coord = starting_position(n_type_figure);; //������� ��������� ���������
    color = 3 + rand() % 7; //������� ��������� ����� �� 3 �� 9
    next_figure_color = 3 + rand() % 7; //���� ��������� ������
    state = Figure_State::MOVE; //����� ��������� ���������
    state_game = Game_State::PLAY; //��������� ����
    t_move.set_time_point(); //��������� ��������� �����
    nt = 800; //������ �������
}

/*-------------------------------------------------------------
               ������������ �������� ������������
--------------------------------------------------------------*/
Figure& Figure::operator=(Figure&& fig) { 
    copy(fig);
    return *this;
}

/*-------------------------------------------------------------
                    �������� ������������
--------------------------------------------------------------*/
Figure& Figure::operator=(const Figure& fig) { //�������� �����������
    copy(fig);
    return *this;
}
/*-------------------------------------------------------------
                   ����������� ����� ������
--------------------------------------------------------------*/
void Figure::copy(const Figure& fig) {
    n_type_figure = fig.n_type_figure;
    next_n_type_figure = fig.next_n_type_figure;
    this_figure = fig.this_figure;
    coord = fig.coord;
    color = fig.color;
    next_figure_color = fig.next_figure_color;
    state = fig.state;
    state_game = fig.state_game;
    t_move = fig.t_move;
    nt = fig.nt;
}


/*-------------------------------------------------------------
                    �������� ��������� ������
--------------------------------------------------------------*/
Figure Figure::next() {
    
    Figure temp(*this);
    temp.n_type_figure = next_n_type_figure; //������������ ��������� ������ �������
    temp.next_n_type_figure = rand() % 7; //������� ��������� ��������� ��������� ������
    temp.this_figure = figure_set[temp.n_type_figure]; //������������ ������ �� ������� �����
    temp.coord = starting_position(temp.n_type_figure); //������� ��������� ���������
    temp.color = next_figure_color; //������� ��������� ����� �� 3 �� 9
    temp.next_figure_color = 3 + rand() % 7; //���� ��������� ������
    temp.state = Figure_State::MOVE; //������� ��������� ���������
    temp.t_move.set_time_point(); //��������� ��������� �����
    return temp; //������ �� ���� � ���������� ��������� �� ������� � ������
}

/*-------------------------------------------------------------
              ���������� ��������� ������� ������
--------------------------------------------------------------*/
POINT Figure::starting_position(int n) {
    POINT temp{ 4, -1 };
    if (n == 1 || n == 5 || n == 2)
        ++temp.x;
    if (n == 6)
        ++temp.y;

    return temp;
}


/*-------------------------------------------------------------
               ����� ���� - ������� ����� ������
--------------------------------------------------------------*/
void Figure::init_new_figure() {
    n_type_figure = rand() % 7; //������� ��������� ������� ������
    next_n_type_figure = rand() % 7; //������� ��������� ��������� ������
    this_figure = figure_set[n_type_figure]; //������������ ������ �� ������� �����
    coord = starting_position(n_type_figure); //������� ��������� ���������
    color = 3 + rand() % 7; //������� ��������� ����� �� 3 �� 9
    next_figure_color = 3 + rand() % 7; //������� ��������� ����� �� 3 �� 9
    state = Figure_State::MOVE; //������� ��������� ���������
    state_game = Game_State::PLAY; //��������� ����
    t_move.set_time_point(); //��������� ��������� �����
    nt = 800;//������ �������
}

/*-------------------------------------------------------------
                   ���������� ������� ������
--------------------------------------------------------------*/
void Figure::update_figure() {

    if (state_game == Game_State::PLAY && state == Figure_State::DESTROY && t_move.duration() >= nt) {
        *this = next();


        if (!check_move()) { //���� ��������� ������ �� �������� �� ���������� GAME_OVER 
            state = Figure_State::DESTROY;
            state_game = Game_State::GAME_OVER;
        }
       
        update_window();
    }
}


/*-------------------------------------------------------------
                       �������� ������
---------------------------------------------------------------
                            �����
--------------------------------------------------------------*/
void Figure::left() {
    if (state_game == Game_State::PLAY && state == Figure_State::MOVE) {
        Figure temp(*this);
        --temp.coord.x;
        if (temp.check_move()) {
            *this = temp;
            update_window();
        }
    }
}


/*-------------------------------------------------------------
                            ������
--------------------------------------------------------------*/
void Figure::right() {
    if (state_game == Game_State::PLAY && state == Figure_State::MOVE) {
        Figure temp(*this);
        ++temp.coord.x;
        if (temp.check_move()) {
            *this = temp;
            update_window();
        }
    }
}

/*-------------------------------------------------------------
                         ����(������)
--------------------------------------------------------------*/
void Figure::down() {
    if (state_game == Game_State::PLAY && state == Figure_State::MOVE && t_move.duration() >= nt) {

        t_move.set_time_point();
        Figure temp(*this);
        ++temp.coord.y;
           
        if (temp.check_move()) {
            *this = temp;

            ++temp.coord.y;
            if (!temp.check_move()) {
                if (temp.check_game_over()) {
                    write_figure(field); //�������� � ����
                    state = Figure_State::DESTROY;
                    state_game = Game_State::GAME_OVER;
                }
                else { 
                    set_flash(); //���������� ������� ���� ���� ������
                }
            }
        }
        else {
            
            write_figure(field); //�������� � ����
            state = Figure_State::DESTROY;
            auto y_r = full_row(get_Y(), field); //������� ������ ����
            if (y_r.size() != 0) { //���� ���� ������ ���� �� ������� ��
                p_game->destroy_row(y_r);
            }
        }
        update_window();
    }
}


/*-------------------------------------------------------------
                   ��������� ������� �����
 --------------------------------------------------------------*/
void Figure::set_flash() {
    ARRAY_10_20 temp_field{ field };
    write_figure(temp_field); //�������� ������ �� ��������� ����

    auto y_r = full_row(get_Y(), temp_field); //������� ������ ����
    if (y_r.size() != 0) { //���� ���� ������ ���� �� �������� ���������� � Game
        p_game->set_flash(y_r);
    }
}






/*-------------------------------------------------------------
                        ���� (�������)
--------------------------------------------------------------*/
void Figure::fast_down() {
    if (state_game == Game_State::PLAY && state == Figure_State::MOVE) {
        t_move.set_time_point();
        Figure temp(*this);
        
        while (temp.check_move()) {
            ++temp.coord.y;
        }

        if (temp.coord.y != coord.y) {
            --temp.coord.y; //���� ������ ��������� ��������� �� ��� �����
            *this = temp;
            set_flash();
            update_window();
        } 
    }
}

/*-------------------------------------------------------------
                        ������� ������
--------------------------------------------------------------*/
void Figure::rotation() {
    if (state_game == Game_State::PLAY && state == Figure_State::MOVE) {

        if (n_type_figure != 6) { //���� ������ �� ������� �� �������
            Figure temp(*this);
            temp.this_figure = transpose(this_figure);
            if (temp.check_move()) {
                *this = temp;
                update_window();
            }
            else if (temp.this_figure[2][0] == 1) { //����� �����
                ++temp.coord.x;
                if (temp.check_move()) {
                    *this = temp;
                    update_window();
                }
            }
            else if (temp.this_figure[2][4] == 1) { //����� ������
                --temp.coord.x;
                if (temp.check_move()) {
                    *this = temp;
                    update_window();
                }
            }
        }
    }
}

/*-------------------------------------------------------------
        ������� ������� �� ������� ������� �� 90 ��������
--------------------------------------------------------------*/
AR_5_5 Figure::transpose(const AR_5_5& arr) {
    AR_5_5 arr2{ {} };
    for (int j = 0, size_y = arr.size(); j < size_y; ++j) {
        for (int i = 0, size_x = arr.size(); i < size_x; ++i)
            arr2[i][size_y - 1 - j] = arr[j][i];
    }
    return arr2;
}

/*-------------------------------------------------------------
            �������� ����������� ��������� ������
 --------------------------------------------------------------*/
bool Figure::check_move() {
    
    int count = 0; //�������
    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i < 5; ++i) {
            int x = coord.x - 2 + i; //x - �������� � ���������� ����
            int y = coord.y - 2 + j; //y - �������� � ���������� ����
            if (this_figure[j][i] == 1 && x >= 0 && x < 10 
                && y < 20 && (y < 0 || (y >= 0 && field[y][x] == 0)))
                ++count; //���� ������ ��������, �� ���������� �������
            if (count == 4)
                return true; //return - ������ ����������� � ����� �����
        }
    }
    return false;
}


/*-------------------------------------------------------------
                    �������� ��������� ����
--------------------------------------------------------------*/
bool Figure::check_game_over()
{
    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i < 5; ++i) {
            int x = coord.x - 2 + i; //x - �������� � ���������� ����
            int y = coord.y - 2 + j; //y - �������� � ���������� ����
            if (this_figure[j][i] == 1 && y < 0) {
                return true; //������ �� ����������� �� ���������
            }
        }    
    }
    return false;
}


/*-------------------------------------------------------------
                  ������� Y-���������� ������
--------------------------------------------------------------*/
set<int> Figure::get_Y() {

    set<int> Y_cont{};
    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i < 5; ++i) {
            int y = coord.y - 2 + j; //y - �������� � ���������� ����
            if (this_figure[j][i] == 1 && y >= 0) { //�������������� ������ �� ������� �������
                Y_cont.insert(y);
            }
        }
    }
    return Y_cont;
}

/*-------------------------------------------------------------
                      ��������� ������ ����
--------------------------------------------------------------*/
set<int> Figure::full_row(set<int> Y_cont, const ARRAY_10_20& fd) {

    for (auto it = Y_cont.begin(); it != Y_cont.end(); ) {

        if (std::find(fd[*it].begin(), fd[*it].end(), 0) != fd[*it].end()) {  //��������
            it = Y_cont.erase(it); //���� ���� ������ ������ ������ �� ������ - ������� �������
        }
        else {
            ++it;
        }
    }

    return Y_cont; //���� ��� ������ ������ �� ���� �� ������ ������� ���� �������� ������ ���������
}

/*-------------------------------------------------------------
                 ������������ - �������� ����
--------------------------------------------------------------*/
void Figure::update_window(){

    HWND hwnd = FindWindow(L"My_Window", nullptr); //����� ������� ����

    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);  
}

/*-------------------------------------------------------------
            ������ ������ � ���� - ���� ������ �����
--------------------------------------------------------------*/
void Figure::write_figure(ARRAY_10_20& fd) {

    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i < 5; ++i) {
            int x = coord.x - 2 + i; //x - �������� � ���������� ����
            int y = coord.y - 2 + j; //y - �������� � ���������� ����
            if (y >= 0 && this_figure[j][i] == 1) {
                fd[y][x] = color; //��������� ������ ����   
            }
        }
    }
}


/*-------------------------------------------------------------
                        �������� ������
--------------------------------------------------------------*/
void Figure::paint(HDC hdc)
{
    if (state == Figure_State::MOVE) { //���� ������ �� ����� - �� ��������

        auto flag_flash = p_game->get_flash();
        auto flash_row = p_game->get_row();
        RECT rect;
        for (int j = 0; j < 5; ++j) {

            for (int i = 0; i < 5; ++i) {
                 int x = coord.x - 2 + i; //x - �������� � ���������� ����
                 int y = coord.y - 2 + j; //y - �������� � ���������� ����

                 if (y >= 0 && this_figure[j][i] == 1) {
                      rect.left = sz.a_edging + x + x * sz.a_ceil;
                      rect.top = sz.a_edging + y + y * sz.a_ceil;
                      rect.right = sz.a_ceil + rect.left;
                      rect.bottom = sz.a_ceil + rect.top;
                      
                      if (flag_flash == Flash::ON  && flash_row.find(y) != flash_row.end()) {
                          FillRect(hdc, &rect, brush[0]); //���� ���� �������� - ����� ������ ������ ������
                      }
                      else {
                          FillRect(hdc, &rect, brush[color]); //����� ������ ������
                      }
                 }
            }
        }    
    }
}


/*-------------------------------------------------------------
           ���������� ��������� ������ �� ������� ������
--------------------------------------------------------------*/
void Figure::paint_next(HDC hdc) {
    if (state_game != Game_State::GAME_OVER) {
        AR_5_5 next_figure = figure_set[next_n_type_figure];
        int X_indent = sz.xb1 + sz.a_10 + sz.a_ceil / 2; //������ �� �����������
        int Y_indent = sz.yb1 + sz.a_10 + sz.a_ceil / 2; //������ �� ���������
        RECT rect;

        for (int j = 0; j < 4; ++j) {
            for (int i = 0; i < 4; ++i) {
                rect.left = X_indent + i + i * sz.a_ceil + 2;
                rect.top = Y_indent + j + j * sz.a_ceil + 2;
                rect.right = sz.a_ceil + rect.left;
                rect.bottom = sz.a_ceil + rect.top;

                int m_brush, type{ next_n_type_figure };
                int dx = (type == 0 || type == 4 || type == 6) ? 1 : 0;
                if (next_figure[j][i + dx] == 1)
                    m_brush = next_figure_color;
                else
                    m_brush = 2; //������ �����

                FillRect(hdc, &rect, brush[m_brush]); //����� ������ ������
            }
        }
    }
}





/*-------------------------------------------------------------
         �������� ������� ������ �� ������ �����������
--------------------------------------------------------------*/
bool Figure::check_figure_xy(int x, int y) {
    
    int i = x + 2 - coord.x;
    int j = y + 2 - coord.y;
    if (state != Figure_State::DESTROY && i >= 0 && i < 5 && j >= 0 && j < 5 && this_figure[j][i] == 1)
        return true;
    else
        return false;
}


/*-------------------------------------------------------------
                            �����
--------------------------------------------------------------*/
void Figure::pause_figure() {
    if (state_game == Game_State::PLAY) {
        state_game = Game_State::PAUSE;
    }
    else if (state_game == Game_State::PAUSE){
        state_game = Game_State::PLAY;
    }
}