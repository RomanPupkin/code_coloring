#include "token_current.h"
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

Token_Cur::Token_Cur()
{
    type = DEFAULT;
    str_size = 0;
}

int Token_Cur::get_tok_size()
{
    return str_size;
}

const std::string &Token_Cur::get_tok()
{
    return token_str;
}

int
Token_Cur::get_type()
{
    return type;
}

int Token_Cur::put_str_token(const string &buf, int type)
{
    this->type = type;
    token_str.clear();
    if (type == DEFAULT) {
        return 0;
    }
    token_str = buf;
    return 0;
}

