#ifndef SEM4_3_1_ANALYSIS_H
#define SEM4_3_1_ANALYSIS_H

#include <stdio.h>
#include "token_basis.h"
#include "token_current.h"
#include "file_work.h"
#include "file_with_pos.h"
#include "analysis_all.h"

/* Class to analyze text for tokens with second variant */

class Analysis_cur:
        public Analyzer {
public:
    int analyse_text(File_work *pos_struct, Token *tok);
    /* analyzing the text in search of the necessary tokens
            * Results : 0 - when token found
    *           EOF - end of file */
};

#endif
