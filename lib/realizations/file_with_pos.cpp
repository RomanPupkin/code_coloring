#include "file_with_pos.h"
#include <string.h>
#include <iostream>

using namespace std;

FILE *
input_file_type(char *input_file_name) {
    if ((strcmp(input_file_name, ".") == 0) || (strcmp(input_file_name,"..") == 0)) {
        cout << "File can't be opened" << endl;
        return nullptr;
    }
    FILE *input_file = fopen(input_file_name, "rb");
    if (input_file == nullptr) {
        cout << "File can't be opened" << endl;
        return nullptr;
    }
    if (fseek(input_file, 0, SEEK_SET) == -1) {
        fclose(input_file);
        cout << "fseek error: " << endl;
        return nullptr;
    }
    return input_file;
}

File_pos::File_pos(char *file_name)
{
    x = 0;
    y = 0;
    base[0] = 0;
    base[1] = 0;
    temp = 0;
    if ((fin = input_file_type(file_name)) == nullptr) {
        y = -1;
    }
}

int
File_pos::end_or_not()
{
    if (y < 0) {
        return 2;
    }
    if (x < 0) {
        return 1;
    } else {
        return 0;
    }
}

int
File_pos::change_pos()
{
    base[0] = x;
    base[1] = y;
    return 0;
}

int
File_pos::my_fgetc() {
    int symbol;
    symbol = fgetc(fin);
    //symbol = cin.get();
    if (symbol == EOF) {
        x = -1;  // END OF FILE
        return EOF;
    }
    if (symbol == '\n') {
        temp = x;
        x = 0;
        y++;
    } else {
        x++;
    }
    return symbol;
}

int
File_pos::get_pos(int *posx, int *posy)
{
    *posx = base[0];
    *posy = base[1];
    return 0;
}


int
File_pos::my_fseek()
{
    //cin.unget();
    fseek(fin, (-1) * sizeof(char), SEEK_CUR);
    if (x == 0) {
        x = temp;
        y--;
    }
    return 0;
}

File_pos::~File_pos()
{
    if (y >= 0) fclose(fin);
}