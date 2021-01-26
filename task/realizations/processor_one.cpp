#include "processor_one.h"
#include <string>
#include <iostream>

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

Process_one::Process_one()
{
    check = 0;
    buf_size = -1;
    color = DEFAULT;
}

int
Process_one::finalize(Token *tok)
{
    color = tok->get_type();
    if (color == DEFAULT) {
        return 0;
    }
    buf = tok->get_tok();
    return 0;
}

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
Process_one::output()
{
    if (color == DEFAULT) {
        return 0;
    }
    cout << get_color(color) << buf;
    return 0;
}

