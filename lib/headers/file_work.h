#ifndef SEM4_3_1_FILE_WORK_H
#define SEM4_3_1_FILE_WORK_H

/* Virtual class to work with file */

class File_work {
public:
    //virtual File_work() = 0;
    virtual int my_fgetc() = 0;
    /* Get symbol from file and something else if necessary */
    virtual int get_pos (int *, int *) = 0;
    /* Copying positions: x and y into first and second parameters of function
    * Results : 0 - success */
    virtual int my_fseek () = 0;
    /* Change position in file */
    virtual int change_pos () = 0;
    /* Change saved positions */
    virtual int end_or_not () = 0;
    /* Results : 0 - not end of file
     *           1 - end of file
     *           2 - error */
    virtual ~File_work() = default;
    /* Delete struct File_work */
};



#endif