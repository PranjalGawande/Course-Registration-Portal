#ifndef ENROLLMENT_RECORD
#define ENROLLMENT_RECORD

struct enrollment
{
    // int id; // 0, 1, 2 ....
    int courseid;
    int studentid;
    char status[12];
    //time_t enroll_time;    
};

#endif