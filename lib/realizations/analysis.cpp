#include <iostream>
#include "token_basis.h"
#include "token_current.h"
#include "analysis.h"
#include <string>

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

enum STATIC
{
    KEY_WORD_MAX_LENGTH = 8,
    KEY_COUNT = 15,
    PUNC_COUNT = 31,
    FIRST_PUNC_COUNT = 12,
    HEX_COUNT = 16,
    FOUR = 4,
    DOT = 7,
    FRSTCOM = 1,
    SECCOM = 2,
};

static int comment_check(File_work *pos_struct, string *word, int type);
/* Looking for the end of comment_check. End of comment_check are symbols '\n' in one case and (* /) in other
 * Results : EOF - in case of end of file
 *           0 - success */

static int literal(File_work *pos_struct, string *word, Token *tok);
/* Looking for the end of literal. End of literal are symbols '\n' or '\"'
 * Results : EOF - in case of end of file
 *           0 - success */

static int identifier(File_work *pos_struct, string *word, Token *tok);
/* Looking for the end of identifiers sequence. End of indentifiers sequence are symbols which are not identifiers
 * Results : 0 - success
 *           EOF -end of file */

static int hex_check(int symbol);
/* Checking for 0-9 and A-F symbols
 * Results: 0 - symbol is not included in one of the intervals
 *          1 - symbol is included in one of the intervals */

static int hex_float_const(File_work *pos_struct, string *word, Token *tok);
/* Checking for hexadecimal-floating-constant after found digit == 0.
 * Results : 0 - success
 *           MEM - not enough memory
 *           EOF - going to the end of program */

static int dec_float_const(File_work *pos_struct, string *word, Token *tok, int dec_type);
/* Checking for decimal-floating-constant after found digit or '.'. Status indicates that 0 or dot is found
 * Results : 0 - success
 *           MEM - not enough memory
 *           EOF - going to the end of program */

static int punc_check(File_work *pos_struct, string *word, Token *tok);
/* Checking for punctuator
 * Results:  0 - success
 *           EOF - going to the end of program */

static const char hex_keks[HEX_COUNT] = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
        'A', 'B', 'C', 'D', 'E', 'F'
};

static const string keywords[] = {
        "if", "inline", "int",
        "long", "register", "restrict",
        "return", "short", "signed",
        "sizeof", "static", "struct",
        "switch", "typedef", "union"
};

static const char punc_first_symbol[FIRST_PUNC_COUNT] = {
        '%', '>', '<', '^', '|', '=', '!', '&', '*', '+', '-', ':'
};

static const string puncs[] = {
        "/", "%", "<<", ">>", "<", ">", "<=", ">=", "==", "!=", "^", "|", "&&", "||",
        "=", "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "^=", "|=",
        "<:", ":>", "<%", "%>", "%:", "%:%:"
};

static int
punc_check(File_work *pos_struct, string *word, Token *tok)
{
    int symbol;
    int check = -2;
    symbol = word->back();
    if (symbol == '=') {
        symbol = pos_struct ->my_fgetc();
        if (symbol == '=') {
            *word += (char) symbol;
            tok->put_str_token(*word, MAGENTA);
            return 0;
        }
        if (symbol == EOF) {
            tok->put_str_token(*word, MAGENTA);
            return EOF;
        }
        pos_struct -> my_fseek();
        tok->put_str_token(*word, MAGENTA);
        return 0;
    }
    if ((symbol == '*') || (symbol == '+') || (symbol == '-') || (symbol == '!') || (symbol == '&') ||
        (symbol == ':')) {
        if ((symbol = pos_struct ->my_fgetc()) != EOF) {
            *word += (char) symbol;
            for (int i = 0; i < PUNC_COUNT; i++) {
                if (*word == puncs[i]) {
                    tok->put_str_token(*word, MAGENTA);
                    return 0;
                }
            }
            word ->pop_back();
            pos_struct->my_fseek();
            tok -> put_str_token(*word, CLASSIC);
            return 0;
        } else {
            tok -> put_str_token(*word, CLASSIC);
            return EOF;
        }
    }
    if ((symbol == '|') || (symbol == '^')) {
        if ((symbol = pos_struct ->my_fgetc()) != EOF) {
            *word += (char) symbol;
            for (int i = 0; i < PUNC_COUNT; i++) {
                if (*word == puncs[i]) {
                    tok->put_str_token(*word, MAGENTA);
                    return 0;
                }
            }
            pos_struct -> my_fseek();
            word -> pop_back();
            tok -> put_str_token(*word, MAGENTA);
            return 0;
        } else {
            tok -> put_str_token(*word, MAGENTA);
            return EOF;
        }
    }
    if ((symbol == '<') || (symbol == '>')) {
        if ((symbol = pos_struct ->my_fgetc()) != EOF) {
            *word += (char) symbol;
            for (int i = 0; i < PUNC_COUNT; i++) {
                if (*word == puncs[i]) {
                    check = 1;
                    break;
                }
            }
            if (check == 1) {
                if ((symbol = pos_struct ->my_fgetc()) != EOF) {
                    *word += (char) symbol;
                    for (int i = 0; i < PUNC_COUNT; i++) {
                        if (*word == puncs[i]) {
                            tok->put_str_token(*word, MAGENTA);
                            return 0;
                        }
                    }
                    pos_struct -> my_fseek();
                    word->pop_back();
                    tok->put_str_token(*word, MAGENTA);
                    return 0;
                } else {
                    tok ->put_str_token(*word, MAGENTA);
                    return EOF;
                }
            }
            pos_struct -> my_fseek();
            word -> pop_back();
            tok -> put_str_token(*word, MAGENTA);
            return 0;
        } else {
            tok ->put_str_token(*word, MAGENTA);
            return EOF;
        }
    }
    if (symbol == '%') {
        if ((symbol = pos_struct ->my_fgetc()) != EOF) {
            if ((symbol == '=') || (symbol == '>')) {
                *word += (char) symbol;
                tok ->put_str_token(*word, MAGENTA);
                return 0;
            }
            if (symbol == ':') {
                *word += (char) symbol;
                if ((symbol = pos_struct ->my_fgetc()) != EOF) {
                    if (symbol == '%') {
                        *word += (char) symbol;
                        if ((symbol = pos_struct ->my_fgetc()) != EOF) {
                            if ((symbol == '=') || (symbol == '>')) {
                                pos_struct -> my_fseek();
                                pos_struct -> my_fseek();
                                word -> pop_back();
                                tok->put_str_token(*word, MAGENTA);
                                return 0;
                            } else if (symbol == ':') {
                                *word += (char) symbol;
                                tok -> put_str_token(*word, MAGENTA);
                                return 0;
                            } else {
                                pos_struct -> my_fseek();
                                tok -> put_str_token(*word, CLASSIC);
                                return 0;
                            }
                        } else {
                            tok -> put_str_token(*word, CLASSIC);
                            return EOF;
                        }
                    } else {
                        pos_struct -> my_fseek();
                        tok -> put_str_token(*word, MAGENTA);
                        return 0;
                    }
                } else {
                    tok ->put_str_token(*word, MAGENTA);
                    return EOF;
                }
            } else {
                pos_struct -> my_fseek();
                tok ->put_str_token(*word, MAGENTA);
                return 0;
            }
        } else {
            tok ->put_str_token(*word, MAGENTA);
            return EOF;
        }
    }
    return 0;
}

static int
literal(File_work *pos_struct, string *word, Token *tok)
{
    int symbol;
    while (((symbol = pos_struct ->my_fgetc()) != EOF) && (symbol != '\n') && (symbol != '\"')) {
        *word += (char) symbol;
    }
    if (symbol != EOF) {
        if (symbol == '\"') {
            *word += (char) symbol;
            tok -> put_str_token(*word, BLUE);

            return 0;
        } else {
            pos_struct -> my_fseek();
            tok -> put_str_token(*word, BLUE);
            return 0;
        }
    } else {
        tok ->put_str_token(*word, BLUE);
        return EOF;
    }
}


static int
identifier(File_work *pos_struct, string *word, Token *tok)
{
    int status = 0;
    int symbol;
    int cycle;
    int check;
    symbol = word->back();
    while (1) {
        switch (status) {
            case 0:
                check = 0;
                if (isalpha(symbol) || (symbol == '_') || isdigit(symbol)) {
                    status = 1;
                } else if (symbol == '\\') { // check for universal
                    status = 3;
                }
                break;
            case 1:
                if (symbol == '\\') { // check for universal
                    *word += (char) symbol;
                    status = 3;
                } else if ((isdigit(symbol)) || (isalpha(symbol)) || (symbol == '_')) {
                    *word += (char) symbol;
                    status = 1;
                } else if (symbol == EOF) {
                    tok ->put_str_token(*word, ORANGE);
                    return EOF;
                } else {
                    pos_struct -> my_fseek();
                    check = 0;
                    tok ->put_str_token(*word, ORANGE);
                    return 0;
                }
                break;
            case 3: // '\\'
                if ((symbol == 'U') || (symbol == 'u')) {
                    *word += (char) symbol;
                    if (symbol == 'u') {
                        cycle = 1;
                    } else {
                        cycle = 2;
                    }
                    for (int i = 0; i < FOUR * cycle; i++) {
                        symbol = pos_struct ->my_fgetc();
                        if (symbol == EOF) {
                            tok->put_str_token(*word, CLASSIC);
                            return EOF;
                        }
                        *word += (char) symbol;
                        if (!hex_check(symbol)) {
                            check = 1;
                            break;
                        }
                    }
                    if (check == 0) {
                        status = 1;
                    } else if (check == 1) {
                        tok ->put_str_token(*word, CLASSIC);
                        return 0;
                    }
                    check = 0;
                } else if (symbol == EOF) {
                    tok ->put_str_token(*word, CLASSIC);
                    return EOF;
                } else {
                    pos_struct -> my_fseek();
                    tok ->put_str_token(*word, CLASSIC);
                    return 0;
                }
                break;
        }
        symbol = pos_struct ->my_fgetc();
    }
    return 0;
}

static int
comment_check(File_work *pos_struct, string *word, int type)
{
    int status = 0;
    int symbol;
    switch (type) {
        case FRSTCOM:
            while (((symbol = pos_struct ->my_fgetc()) != '\n') && (symbol != EOF)) {
                *word += (char) symbol;
            }
            if (symbol == '\n') {
                pos_struct -> my_fseek();
                return 0;
            } else {
                return EOF;
            }
        case SECCOM:
            while (((symbol = pos_struct ->my_fgetc()) != EOF) && (status != 2)) {
                *word += (char) symbol;
                if ((status == 1) && (symbol == '/')) {
                    status = 2;
                } else if ((symbol == '*') && (status != 2)) {
                    status = 1;
                } else {
                    status = 0;
                }
            }
            if (status == 2) {
                pos_struct -> my_fseek();
            }
            if (symbol == EOF) {
                return EOF;
            } else {
                return 0;
            }
            break;
    }
    return 0;
}

static int
hex_check(int symbol)
{
    for (int i = 0; i < HEX_COUNT; i++) {
        if (symbol == hex_keks[i]) {
            return 1;
        }
    }
    return 0;
}

static int
dec_float_const(File_work *pos_struct, string *word, Token *tok, int dec_type) {
    int symbol;
    symbol = word -> back();
    int check = -2; // -2 - start without putting in buffer
    int status = dec_type;
    while (1) {
        switch (status) {
            case 0:
                if (isdigit(symbol)) {
                    status = 7;
                } else if (symbol == '.') {
                    status = 1;
                } else {
                    check = 0; // not success
                }
                break;
            case 1:
                if (isdigit(symbol)) {
                    status = 2;
                } else {
                    check = 0; // not success
                }
                break;
            case 2:
                if ((symbol == 'e') || (symbol == 'E')) {
                    status = 3;
                } else if (isdigit(symbol)) {
                    status = 2;
                } else {
                    check = 1; // success
                }
                break;
            case 3:
                if ((symbol == '+') || (symbol == '-')) {
                    status = 4;
                } else if (isdigit(symbol)) {
                    status = 5;
                } else {
                    check = 0; // not success
                }
                break;
            case 4:
                if (isdigit(symbol)) {
                    status = 5;
                } else {
                    check = 0;
                }
                break;
            case 5:
                if (isdigit(symbol)) {
                    // just waiting
                } else if ((symbol == 'f') || (symbol == 'F') || (symbol == 'l') || (symbol == 'L')) {
                    status = 6;
                } else {
                    check = 1;
                }
                break;
            case 6:
                if (!isdigit(symbol) && !isalpha(symbol) && (symbol != '_')) {
                    check = 1;
                } else {
                    check = 0;
                }
                break;
            case 7:
                if (symbol == '.') {
                    status = 2;
                } else if ((symbol == 'e') || (symbol == 'E')) {
                    status = 3;
                } else if (isdigit(symbol)) {
                    status = 7;
                } else {
                    check = 0;
                }
                break;
        }

        if (symbol == EOF) {
            if ((check == -1) || (check == 0)) {
                tok->put_str_token(*word, CLASSIC);
            } else if (check == 1) {
                tok->put_str_token(*word, RED);
            }
            return EOF;
        }

        if ((check == 0) || (check == 1)) {
            pos_struct -> my_fseek();
            if (check == 0) { // not success
                tok->put_str_token(*word, CLASSIC);
            } else { // success
                tok->put_str_token(*word, RED);
            }
            return 0;
        }
        if (check != -2) *word += (char) symbol; // чтобы не засунуть начальный символ дважды
        if (check == -2) check = -1;
        symbol = pos_struct ->my_fgetc();
    }
}

static int
hex_float_const(File_work *pos_struct, string *word, Token *tok)
{
    int status = 0;
    int check = -1;
    int symbol = pos_struct ->my_fgetc();
    while (1) {
        switch (status) {
            case 0:
                if (symbol == '.') {
                    status = 1;
                } else if (hex_check(symbol)) {
                    status = 7;
                } else {
                    check = 0; // not success
                }
                break;
            case 1:
                if (hex_check(symbol)) {
                    status = 2;
                } else {
                    check = 0;
                }
                break;
            case 2:
                if ((symbol == 'p') || (symbol == 'P')) {
                    status = 3;
                } else if (hex_check(symbol)) {
                    status = 2;
                } else {
                    check = 0;
                }
                break;
            case 3:
                if ((symbol == '-') || (symbol == '+')) {
                    status = 5;
                } else if (isdigit(symbol)) {
                    status = 4;
                } else {
                    check = 0;
                }
                break;
            case 4:
                if (isdigit(symbol)) {
                    status = 4;
                } else if ((symbol == 'f') || (symbol == 'F') || (symbol == 'l') || (symbol == 'L')) {
                    status = 6;
                } else {
                    check = 1;
                }
                break;
            case 5:
                if (isdigit(symbol)) {
                    status = 4;
                } else {
                    check = 0;
                }
                break;
            case 6:
                if (!isalpha(symbol) && !isdigit(symbol) && (symbol != '_')) {
                    check = 1;
                } else {
                    check = 0; // not success
                }
                break;
            case 7:
                if (symbol == '.') {
                    status = 2;
                } else if (hex_check(symbol)) {
                    status = 7;
                } else if ((symbol == 'p') || (symbol == 'P')) {
                    status = 3;
                } else {
                    check = 0;
                }
                break;
        }

        if (symbol == EOF) {
            if ((check == -1) || (check == 0)) {
                tok->put_str_token(*word, CLASSIC);
            } else if (check == 1) {
                tok->put_str_token(*word, RED);
            }
            return EOF;
        }

        if ((check == 0) || (check == 1)) {
            pos_struct -> my_fseek();
            if (check == 0) {
                // not success
                tok->put_str_token(*word, CLASSIC);
            } else if (check == 1) {
                tok->put_str_token(*word, RED);
            }
            return 0;
        }
        *word += (char) symbol;
        symbol = pos_struct ->my_fgetc();
    }
}

int
Analysis_cur::analyse_text(File_work *pos_struct, Token *tok)
{
    int status = 0;
    int check;                  // 1, если ключевое, 0 - иначе
    int symbol;
    int end_back = 0;
    int dec_type = 0;
    string word;
    //word.clear();
    //word.clear();
    while (1) {
        switch (status) {
            case 0: //motion detection from the beginning of the automaton with status = 0
                check = 0;
                dec_type = 0;
                symbol = pos_struct->my_fgetc();
                if (symbol == EOF) {
                    tok -> put_str_token("1", DEFAULT);
                    return EOF;
                }
                if ((symbol == '\n') || (symbol == ' ')) { //*|| (symbol == '\0')*
                    word += (char) symbol;
                    tok -> put_str_token(word, CLASSIC);
                    return 0;
                } else {
                    word += (char) symbol;
                    if (symbol == '/') {
                        status = 1; // состояние : комментарий или пунктуатор
                    } else if ((symbol == 'L') || (symbol == 'U')) {
                        status = 4; // надежда на литерал
                    } else if (symbol == 'u') {
                        status = 5; // надежда на литерал
                    } else if (symbol == '\"') {
                        status = 8;
                    } else if (isalpha(symbol)) {
                        status = 2; // состояние с буквами : проверка на ключевое
                    } else if (symbol == '0') {
                        status = 7; // состояние с цифрой 0
                    } else if (isdigit(symbol) || (symbol == '.')) {
                        status = 6; // состояние с цифрами
                    } else if ((symbol == '_') || (symbol == '\\')) {
                        status = 3;
                    } else {
                        status = 10; // unknown symbol = maybe be punctuator
                    }
                }
                break;
            case 1:  // comment_check + punctuator with '/'
                symbol = pos_struct ->my_fgetc();
                if (symbol == EOF) {
                    tok -> put_str_token(word, MAGENTA);
                    return EOF;
                }
                if (symbol == '/') { // because 100% comment_check to the end of the line => return to the beginning status = 0;
                    word += (char) symbol;
                    end_back = comment_check(pos_struct, &word, FRSTCOM);
                    tok->put_str_token(word, GREEN);
                    if (end_back == EOF) {
                        return EOF;
                    }
                    return 0;
                } else if (symbol == '*') {
                    word += (char) symbol;
                    end_back = comment_check(pos_struct, &word, SECCOM);
                    tok -> put_str_token(word, GREEN);
                    if (end_back == EOF) {
                        return EOF;
                    }
                    return 0;
                } else if (symbol == '=') {  // punctuator -> status = 0
                    word += (char) symbol;
                    tok->put_str_token(word, MAGENTA);
                    return 0;
                } else { // punctuator -> status = 0;
                    pos_struct -> my_fseek();
                    tok->put_str_token(word, MAGENTA);
                    return 0;
                }
                break;
            case 2: // состояния с буквами : проверка на ключевое, если не ключевое, то переходим в идентификатор
                symbol = pos_struct ->my_fgetc();
                if (symbol == EOF) {
                    if (check == 1) {
                        tok->put_str_token(word, YELLOW);
                    } else {
                        tok->put_str_token(word, ORANGE);
                    }
                    return EOF;
                }
                if (isdigit(symbol) || (symbol == '_')) {
                    word += (char) symbol;
                    status = 3;   // 99% identifier
                } else if (isalpha(symbol)) {
                    word += (char) symbol;
                    if (check == 1) {
                        status = 3;
                        check = 0;
                        break;
                    }
                    for (int i = 0; i < KEY_COUNT; i++) {
                        if (word == keywords[i]) {
                            check = 1;
                            break;
                        }
                    }
                    if (word.size() == KEY_WORD_MAX_LENGTH) {
                        status = 3;
                        check = 0;
                        //word.pop_back();
                    }
                } else {
                    pos_struct -> my_fseek();
                    if (check == 1) {
                        tok->put_str_token(word, YELLOW);
                    } else {
                        tok->put_str_token(word, ORANGE);
                    }
                    return 0;
                }
                break;
            case 3: // Identifier
                //word += (char) symbol;
                end_back = identifier(pos_struct, &word, tok);
                if (end_back == 0) {
                    return 0;
                } else if (end_back == EOF) {
                    return EOF;
                }
                break;
            case 4: // checking for literal after 'L' 'U'
                symbol = pos_struct ->my_fgetc();
                if (symbol == EOF) {
                    tok -> put_str_token(word, ORANGE);
                    return EOF;
                }
                if (symbol == '\"') {
                    word += (char) symbol;
                    if ((end_back = literal(pos_struct, &word, tok)) == 0) { // ended on \"
                        return 0;
                    } else {
                        return EOF;
                    }
                } else if (isalpha(symbol) || isdigit(symbol) || (symbol == '_')) {
                    word += (char) symbol;
                    status = 2; // To identifier or keyword
                } else {
                    word += (char) symbol;
                    tok -> put_str_token(word, ORANGE);
                    return 0;
                }
                break;
            case 5: // if symbol == 'u' looking for literal
                symbol = pos_struct ->my_fgetc();
                if (symbol == EOF) {
                    tok -> put_str_token(word, ORANGE);
                    return EOF;
                }
                if (symbol == '8') {
                    word += (char) symbol;
                    status = 4;  // надежда на литерал
                } else if (isalpha(symbol) || isdigit(symbol) || (symbol == '_')) {
                    word += (char) symbol;
                    status = 2;
                } else if (symbol == '\"') {
                    word += (char) symbol;
                    status = 8;
                } else {
                    pos_struct -> my_fseek();
                    tok ->put_str_token(word, ORANGE);
                    return 0;
                }
                break;
            case 6: // digit
                if (dec_type == 7) { // from case 7
                    end_back = dec_float_const(pos_struct, &word, tok, DOT);
                } else {
                    end_back = dec_float_const(pos_struct, &word, tok, 0);
                }
                if (end_back == 0) {  // dec float
                    return 0;
                } else if (end_back == EOF) {
                    return EOF;
                }
                break;
            case 7: // when '0' found
                symbol = pos_struct ->my_fgetc();
                if (symbol == EOF) {
                    tok ->put_str_token(word, CLASSIC);
                    return EOF;
                }
                if ((symbol == 'x') || (symbol == 'X')) {
                    word += (char) symbol;
                    status = 9; // hex real const
                } else if (isdigit(symbol)) {
                    word += (char) symbol;
                    status = 6; // dec real const
                } else if (symbol == '.') {
                    word += (char) symbol;
                    status = 6; // dec real const
                    dec_type = 7;
                } else {
                    pos_struct -> my_fseek();
                    tok ->put_str_token(word, CLASSIC);
                    return 0;
                }
                break;
            case 8:
                end_back = literal(pos_struct, &word, tok);
                if (end_back == EOF) {
                    return EOF;
                }
                return 0;
            case 9: // Looking for Hexademical after prefix == '0x' or '0X'
                if ((end_back = hex_float_const(pos_struct, &word, tok)) == 0) {
                    return 0;
                } else if (end_back == EOF) {
                    return EOF;
                }
                break;
            case 10: // punctuators
                check = 0;
                for (int i = 0; i < FIRST_PUNC_COUNT; i++) {
                    if (symbol == punc_first_symbol[i]) {
                        check = 1;
                        break;
                    }
                }
                if (check == 0) {
                    tok -> put_str_token(word, CLASSIC);
                    return 0;
                } else {
                    end_back = punc_check(pos_struct, &word, tok);
                    if (end_back == 0) {
                        return 0;
                    } else {
                        return EOF;
                    }
                }
                break;
            default:
                break;
        }
    }
    return 0;
}