#ifndef SEM4_3_1_PROCESSOR_ONE_H
#define SEM4_3_1_PROCESSOR_ONE_H

#include "processor.h"
#include <string>
#include "token_current.h"
#include "file_with_pos.h"

/* Class to work with Token and print with necessary color */

using namespace std;

class Process_one:
        public Processor_out {
public:
    Process_one();
    /* Setting first values */

    int finalize(Token *tok);
    /* Get from tok information about token and save in fields of process_one
     * Results : 0 - success
     *           1 - error */

    int output();
    /* Output buf of proc_one with necessary color
     * Results : 0 - success */

    //~Process_one();
    /* Deleting struct proc_one and free necessary fields */
private:
    int buf_size;
    int check;
    int color;
    std::string buf;
};


#endif
