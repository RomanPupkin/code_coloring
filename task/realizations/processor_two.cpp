#include "processor_two.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

enum COLORS
{
    CLASSIC = 0,
    YELLOW = 1, // keywords
    ORANGE = 2, // identifiers
    RED = 3, // real constants
    BLUE = 4, // literal
    MAGENTA = 5, // operations
    GREEN = 6, // comment
    DEFAULT = 7 // doesn't has any color
};

static const char *
get_color(int color)
{
    switch (color) {
        case GREEN:
            return ("\033[0;32m");
        case ORANGE:
            return ("\033[01;33m");
        case YELLOW:
            return ("\033[0;33m");
        case BLUE:
            return ("\033[0;34m");
        case RED:
            return ("\033[0;31m");
        case MAGENTA:
            return ("\033[0;35m");
        case CLASSIC:
        default:
            return ("\033[0m");
    }
}

int
Process_two::put_pos_class(File_work *pos)
{
    file_pos = pos;
    return 0;
}

Process_two::Process_two(int &type)
{
    this->type = type;
    print_or_not = 0;
}

int
Process_two::finalize(Token *tok)
{
    int type_temp = tok->get_type();
    int x = 0,y = 0;
    print_or_not = file_pos ->end_or_not();
    if (type_temp == type) {
        file_pos->get_pos(&x, &y);
        vector <int> temp;
        temp.push_back(x);
        temp.push_back(y);
        mas.emplace_back(tok->get_tok(), temp);
    }
    return 0;
}

int
Process_two::output()
{
    sort(mas.begin(), mas.end());
    if (print_or_not) {
        for (int i = 0; i < (int) mas.size(); i++) {
            cout << get_color(type) << mas[i].first << get_color(DEFAULT);
            cout << " ";
            cout << "x=" << mas[i].second[0] << " y=" << mas[i].second[1] << endl;
        }
        //mas.~vector();
    }
    return 0;
}


