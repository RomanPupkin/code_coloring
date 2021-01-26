#ifndef SEM4_3_1_ANALYSIS_ALL_H
#define SEM4_3_1_ANALYSIS_ALL_H

#include "file_work.h"
#include "token_basis.h"

/* Class to analyze text for tokens */

class Analyzer {
public:
    virtual int analyse_text(File_work *, Token *) = 0;
    /* Analyzes text for necessary tokens */

    virtual ~Analyzer() = default;
};

#endif
