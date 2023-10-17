#ifndef STUDENT_RECORD
#define STUDENT_RECORD

struct student
{
    int id;
    char name[25];
    int age; 
    char address[160];
    char email[30];

    // Login Credentials
    char login_id[15]; // Format : MT{id}
    char password[15];
    char status[15];
    
};

#endif