#include "Figure.h"


/*-------------------------------------------------------------
             Конструктор с инициализирующими ссылками
--------------------------------------------------------------*/
Figure::Figure(ARRAY_10_20& field, AR_BRUSH& brush, Size_Window& sz, Parrent* p_game)
    : Parrent(), field{ field }, brush{ brush }, sz{ sz }, p_game{p_game}
{
    n_type_figure = rand() % 7; //выбираю текущую случайную фигуру
    next_n_type_figure = rand() % 7; //выбираю следующую случайную фигуру
    this_figure = figure_set[n_type_figure]; //выбираю случайную фигуру из шаблона фигур
    coord = starting_position(n_type_figure);; //выбираю начальное положение
    color = 3 + rand() % 7; //выбираю случайную кисть от 3 до 9
    next_figure_color = 3 + rand() % 7; //цвет следующей фигуры
    state = Figure_State::MOVE; //задаю начальное состояние
    state_game = Game_State::PLAY; //состояние игры
    t_move.set_time_point(); //записываю временную точку
    nt = 800; //период падения
}

/*-------------------------------------------------------------
               Перемещающий оператор присваивания
--------------------------------------------------------------*/
Figure& Figure::operator=(Figure&& fig) { 
    copy(fig);
    return *this;
}

/*-------------------------------------------------------------
                    Оператор присваивания
--------------------------------------------------------------*/
Figure& Figure::operator=(const Figure& fig) { //оператор копирования
    copy(fig);
    return *this;
}
/*-------------------------------------------------------------
                   Копирование полей класса
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
                    Создание следующей фигуры
--------------------------------------------------------------*/
Figure Figure::next() {
    
    Figure temp(*this);
    temp.n_type_figure = next_n_type_figure; //устанавливаю следующую фигуру текущей
    temp.next_n_type_figure = rand() % 7; //выбираю следующую следующую случайную фигуру
    temp.this_figure = figure_set[temp.n_type_figure]; //устанавливаю фигуру из шаблона фигур
    temp.coord = starting_position(temp.n_type_figure); //выбираю начальное положение
    temp.color = next_figure_color; //выбираю случайную кисть от 3 до 9
    temp.next_figure_color = 3 + rand() % 7; //цвет следующей фигуры
    temp.state = Figure_State::MOVE; //выбираю начальное состояние
    temp.t_move.set_time_point(); //записываю временную точку
    return temp; //ссылки на поле и дескриптор переходят из объекта в объект
}

/*-------------------------------------------------------------
              Установить начальную позицию фигуры
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
               Новая игра - создать новую фигуру
--------------------------------------------------------------*/
void Figure::init_new_figure() {
    n_type_figure = rand() % 7; //выбираю случайную текущую фигуру
    next_n_type_figure = rand() % 7; //выбираю следующую случайную фигуру
    this_figure = figure_set[n_type_figure]; //устанавливаю фигуру из шаблона фигур
    coord = starting_position(n_type_figure); //выбираю начальное положение
    color = 3 + rand() % 7; //выбираю случайную кисть от 3 до 9
    next_figure_color = 3 + rand() % 7; //выбираю случайную кисть от 3 до 9
    state = Figure_State::MOVE; //выбираю начальное состояние
    state_game = Game_State::PLAY; //состояние игры
    t_move.set_time_point(); //записываю временную точку
    nt = 800;//период падения
}

/*-------------------------------------------------------------
                   Обновление текущей фигуры
--------------------------------------------------------------*/
void Figure::update_figure() {

    if (state_game == Game_State::PLAY && state == Figure_State::DESTROY && t_move.duration() >= nt) {
        *this = next();


        if (!check_move()) { //если очередная фигура не ставится то установить GAME_OVER 
            state = Figure_State::DESTROY;
            state_game = Game_State::GAME_OVER;
        }
       
        update_window();
    }
}


/*-------------------------------------------------------------
                       Движение фигуры
---------------------------------------------------------------
                            Влево
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
                            Вправо
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
                         Вниз(таймер)
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
                    write_figure(field); //записать в поле
                    state = Figure_State::DESTROY;
                    state_game = Game_State::GAME_OVER;
                }
                else { 
                    set_flash(); //установить мигание если ряды полные
                }
            }
        }
        else {
            
            write_figure(field); //записать в поле
            state = Figure_State::DESTROY;
            auto y_r = full_row(get_Y(), field); //получаю полные ряды
            if (y_r.size() != 0) { //если есть полные ряды то удалить их
                p_game->destroy_row(y_r);
            }
        }
        update_window();
    }
}


/*-------------------------------------------------------------
                   Установка мигания рядов
 --------------------------------------------------------------*/
void Figure::set_flash() {
    ARRAY_10_20 temp_field{ field };
    write_figure(temp_field); //записать фигуру во временное поле

    auto y_r = full_row(get_Y(), temp_field); //получаю полные ряды
    if (y_r.size() != 0) { //если есть полные ряды то передать информацию в Game
        p_game->set_flash(y_r);
    }
}






/*-------------------------------------------------------------
                        Вниз (уронить)
--------------------------------------------------------------*/
void Figure::fast_down() {
    if (state_game == Game_State::PLAY && state == Figure_State::MOVE) {
        t_move.set_time_point();
        Figure temp(*this);
        
        while (temp.check_move()) {
            ++temp.coord.y;
        }

        if (temp.coord.y != coord.y) {
            --temp.coord.y; //если предел достигнут отступить на шаг назад
            *this = temp;
            set_flash();
            update_window();
        } 
    }
}

/*-------------------------------------------------------------
                        Крутить фигуру
--------------------------------------------------------------*/
void Figure::rotation() {
    if (state_game == Game_State::PLAY && state == Figure_State::MOVE) {

        if (n_type_figure != 6) { //если фигура не квадрат то вращать
            Figure temp(*this);
            temp.this_figure = transpose(this_figure);
            if (temp.check_move()) {
                *this = temp;
                update_window();
            }
            else if (temp.this_figure[2][0] == 1) { //палка слева
                ++temp.coord.x;
                if (temp.check_move()) {
                    *this = temp;
                    update_window();
                }
            }
            else if (temp.this_figure[2][4] == 1) { //палка справа
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
        Крутить матрицу по часовой стрелке на 90 градусов
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
            Проверка возможности поставить фигуру
 --------------------------------------------------------------*/
bool Figure::check_move() {
    
    int count = 0; //счетчик
    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i < 5; ++i) {
            int x = coord.x - 2 + i; //x - перевожу в координаты поля
            int y = coord.y - 2 + j; //y - перевожу в координаты поля
            if (this_figure[j][i] == 1 && x >= 0 && x < 10 
                && y < 20 && (y < 0 || (y >= 0 && field[y][x] == 0)))
                ++count; //если клетка ставится, то увеличиваю счетчик
            if (count == 4)
                return true; //return - фигура поместилась в новом месте
        }
    }
    return false;
}


/*-------------------------------------------------------------
                    Проверка окончания игры
--------------------------------------------------------------*/
bool Figure::check_game_over()
{
    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i < 5; ++i) {
            int x = coord.x - 2 + i; //x - перевожу в координаты поля
            int y = coord.y - 2 + j; //y - перевожу в координаты поля
            if (this_figure[j][i] == 1 && y < 0) {
                return true; //фигура не поместилась по вертикали
            }
        }    
    }
    return false;
}


/*-------------------------------------------------------------
                  Получаю Y-координаты фигуры
--------------------------------------------------------------*/
set<int> Figure::get_Y() {

    set<int> Y_cont{};
    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i < 5; ++i) {
            int y = coord.y - 2 + j; //y - перевожу в координаты поля
            if (this_figure[j][i] == 1 && y >= 0) { //предотвращение выхода за пределы массива
                Y_cont.insert(y);
            }
        }
    }
    return Y_cont;
}

/*-------------------------------------------------------------
                      Вычислить полные ряды
--------------------------------------------------------------*/
set<int> Figure::full_row(set<int> Y_cont, const ARRAY_10_20& fd) {

    for (auto it = Y_cont.begin(); it != Y_cont.end(); ) {

        if (std::find(fd[*it].begin(), fd[*it].end(), 0) != fd[*it].end()) {  //алгоритм
            it = Y_cont.erase(it); //если ноль найден значит строка не полная - удалить элемент
        }
        else {
            ++it;
        }
    }

    return Y_cont; //если для данной фигуры не было ни одного полного ряда вернется пустой контейнер
}

/*-------------------------------------------------------------
                 Перерисовать - обновить окно
--------------------------------------------------------------*/
void Figure::update_window(){

    HWND hwnd = FindWindow(L"My_Window", nullptr); //найти главное окно

    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);  
}

/*-------------------------------------------------------------
            Запись фигуры в поле - если фигура упала
--------------------------------------------------------------*/
void Figure::write_figure(ARRAY_10_20& fd) {

    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i < 5; ++i) {
            int x = coord.x - 2 + i; //x - перевожу в координаты поля
            int y = coord.y - 2 + j; //y - перевожу в координаты поля
            if (y >= 0 && this_figure[j][i] == 1) {
                fd[y][x] = color; //окрашиваю клетку поля   
            }
        }
    }
}


/*-------------------------------------------------------------
                        Рисовать фигуру
--------------------------------------------------------------*/
void Figure::paint(HDC hdc)
{
    if (state == Figure_State::MOVE) { //если фигура не упала - то рисовать

        auto flag_flash = p_game->get_flash();
        auto flash_row = p_game->get_row();
        RECT rect;
        for (int j = 0; j < 5; ++j) {

            for (int i = 0; i < 5; ++i) {
                 int x = coord.x - 2 + i; //x - перевожу в координаты поля
                 int y = coord.y - 2 + j; //y - перевожу в координаты поля

                 if (y >= 0 && this_figure[j][i] == 1) {
                      rect.left = sz.a_edging + x + x * sz.a_ceil;
                      rect.top = sz.a_edging + y + y * sz.a_ceil;
                      rect.right = sz.a_ceil + rect.left;
                      rect.bottom = sz.a_ceil + rect.top;
                      
                      if (flag_flash == Flash::ON  && flash_row.find(y) != flash_row.end()) {
                          FillRect(hdc, &rect, brush[0]); //если есть мерцание - рисую темный контур фигуры
                      }
                      else {
                          FillRect(hdc, &rect, brush[color]); //рисую клетку фигуры
                      }
                 }
            }
        }    
    }
}


/*-------------------------------------------------------------
           Прорисовка следующей фигуры на боковой панели
--------------------------------------------------------------*/
void Figure::paint_next(HDC hdc) {
    if (state_game != Game_State::GAME_OVER) {
        AR_5_5 next_figure = figure_set[next_n_type_figure];
        int X_indent = sz.xb1 + sz.a_10 + sz.a_ceil / 2; //отступ по горизонтали
        int Y_indent = sz.yb1 + sz.a_10 + sz.a_ceil / 2; //отступ по вертикали
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
                    m_brush = 2; //черная кисть

                FillRect(hdc, &rect, brush[m_brush]); //рисую клетку фигуры
            }
        }
    }
}





/*-------------------------------------------------------------
         Проверка наличия фигуры по данным координатам
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
                            Пауза
--------------------------------------------------------------*/
void Figure::pause_figure() {
    if (state_game == Game_State::PLAY) {
        state_game = Game_State::PAUSE;
    }
    else if (state_game == Game_State::PAUSE){
        state_game = Game_State::PLAY;
    }
}