#include <iostream>
#include <string.h>
#include "token_basis.h"
#include "token_current.h"
#include "analysis_all.h"
#include "analysis.h"
#include "file_work.h"
#include "file_with_pos.h"
#include "processor.h"
#include "processor_one.h"
#include "processor_two.h"

using namespace std;

enum {
    PARAM_NUM = 4,
    COL_NUM = 7
};

static char colors[COL_NUM][2] = {"0", "1", "2", "3", "4", "5", "6"};


int
processing(Processor_out *proc, Token *tok)
{
    proc->finalize(tok);
    proc->output();
    return 0;
}

int
main(int argc, char *argv[]) {
    if (argc != PARAM_NUM) {
        cout << "Wrong number of parameters" << endl;
        return 0;
    }

    // For work with file
    File_pos pos_struct(argv[1]);
    if (pos_struct.end_or_not() == 2) {
        return 0;
    }
    Analyzer *analyzer = new Analysis_cur;

    // For work with token
    Token *tok_cur = new Token_Cur;

    // For output tokens with additional information
    Processor_out *choose;

    int check = -1;
    int type = 0;

    if (strcmp(argv[2], "1") == 0) {
        for (int i = 0; i < COL_NUM; i++) {
            if (strcmp(argv[3], colors[i]) == 0) {
                check = i;
                break;
            }
        }
        if (check == -1) {
            cout << argv[3] << " - Wrong parameter" << endl;
            return 0;

        } else {
            type = check;
            Process_two *proc_two = new Process_two(type);
            proc_two->put_pos_class(&pos_struct);
            choose = proc_two;
        }
    } else if (strcmp(argv[2], "0") == 0) {
        Process_one *proc_one = new Process_one;
        choose = proc_one;
        check = 0;
    } else {
        cout << argv[3] << " - Wrong parameter" << endl;
        return 0;
    }

    while (check != EOF) {
        check = analyzer->analyse_text(&pos_struct, tok_cur);
        processing(choose, tok_cur);
        pos_struct.change_pos();
    }

    delete analyzer;
    delete tok_cur;
    delete choose;
    return 0;
}