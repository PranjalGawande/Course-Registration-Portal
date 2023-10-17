#ifndef COURSE_RECORD
#define COURSE_RECORD

struct course
{
    int id; // 0, 1, 2 ....
    char name[30];
    int faculty_id;
    char department [30];
    int no_of_seats;
    int no_of_available_seats;
    int credits;
    // char course_code[6];
    char status[10];    
};

#endif