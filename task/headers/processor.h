#ifndef SEM4_3_1_PROCESSOR_H
#define SEM4_3_1_PROCESSOR_H

#include "token_basis.h"

/* Class to work with class: Token and do some actions with it */

class Processor_out {
public:
    virtual int finalize(Token *) = 0;
    /* Save token's string and other information from void* if necessary
    * Results : 0 - success
    *           1 - error */
    virtual int output() = 0;
    /* Output saved information */

    virtual ~Processor_out() = default;
    /* Delete struct Processor_out and other fields if necessary */
};


#endif