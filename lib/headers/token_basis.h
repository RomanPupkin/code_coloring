#ifndef SEM4_3_1_TOKEN_BASIS_H
#define SEM4_3_1_TOKEN_BASIS_H

#include <string>

/* Class to work with text and search for tokens */

using namespace std;

class Token {
public:
    virtual int put_str_token (string const &, int) = 0;
    /* Put token's string with type int */

    virtual int get_type() = 0;
    /* Return token's type */

    virtual const std::string &get_tok () = 0;
    /* Return token's string */

    virtual int get_tok_size() = 0;
    /* Return token's string size */

    virtual ~Token() = default;
    /* Delete struct Token */
};


#endif
