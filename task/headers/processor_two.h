#ifndef SEM4_3_1_PROCESSOR_TWO_H
#define SEM4_3_1_PROCESSOR_TWO_H

#include "processor.h"
#include "file_with_pos.h"
#include <vector>
#include <string>

/* Class to work with Token and File_work: save tokens with one type
 * and get statistics of file */

using namespace std;

class Process_two:
        public Processor_out {
public:
    //Process_two();
    /* Setting first values */

    Process_two(int &type);
    /* Setting this->type = type */

    int finalize(Token *tok);
    /* Get from tok information about token and save in fields of proc_two
     * Results : 0 - success
     *           1 - error */

    int output();
    /* Output mas_str_pos of proc_two with necessary sorting and necessary color
     * Results : 0 - success */

    int put_pos_class(File_work *pos);
    /* Put pointer to class File_work in field file_pos
     * Results : 0 - success */

    //~Process_two();
    /* Deleting struct proc_one and free necessary fields */
private:
    File_work *file_pos;
    vector < pair<string, vector<int>> > mas;
    int type;
    int print_or_not;
};

#endif
