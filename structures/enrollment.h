#ifndef ENROLLMENT_RECORD
#define ENROLLMENT_RECORD

struct enrollment
{
    int courseid;
    int studentid;
    char status[12];        // ENROLLED OR DROPPED
};

#endif