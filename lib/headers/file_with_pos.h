#ifndef SEM4_3_1_FILE_WITH_POS_H
#define SEM4_3_1_FILE_WITH_POS_H

#include "file_work.h"
#include <iostream>

/* Class to work with file and save some statistics */

class File_pos:
        public File_work {
public:
    File_pos(char *file_name);
    /* Setting first values */

    int my_fgetc();
    /* Reads the next character from fin and returns it as an unsigned char cast to an int,
     * or EOF on end of file or error. Also fills the fields with position */

    int get_pos(int *posx, int *posy);
    /* Copying two positions: base[0] and base[1] into posx and posy
     * Results : 0 - success */

    int my_fseek();
    /* Offset back 1 position in the file fin and refresh the fields with position
     * Results : 0 - success */

    int change_pos();
    /* Change base[0] and base[1] in x and y from pos_struct fields
     * Results : 0 - success */

    int end_or_not();
    /* Results : 0 - not end of file
     *           1 - end of file
     *           2 - error */

    ~File_pos();
    /* Delete struct pos_struct and close FILE *fin */

private:
    FILE *fin;
    int x;
    int y;
    int base[2];
    int temp;
};


#endif
