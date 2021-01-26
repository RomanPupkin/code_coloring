#ifndef SEM4_3_1_TOKEN_CURRENT_H
#define SEM4_3_1_TOKEN_CURRENT_H

#include "token_basis.h"
#include <string>

/* Class to work with text and search for tokens from second variant */

using namespace std;

class Token_Cur:
        public Token {
public:
    Token_Cur();
    /* Setting first values */

    int put_str_token(const string &buf, int type);
    /* Put string from buf to field in tok_cur and fills type field in tok_cur
     * Results : 0 - success
     *           1 - error */

    const std::string &get_tok();
    /* Get token's string in str, str_size must have length including '\0'
     * Results : 0 - success
     *           1 - error */

    int get_tok_size();
    /* Return token's string size */

    int get_type();
    /* Return token's type */

    //~Token_Cur();
    /* Deleting struct tok and free necessary fields */
private:
    int type;
    string token_str;
    int str_size; // with '\0'
};


#endif
