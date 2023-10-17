#ifndef FACULTY_RECORD
#define FACULTY_RECORD

struct faculty
{
    int id;
    char name[25];
    char department[6];
    char designation[50]; 
    char address[160];
    char email[80];

    // Login Credentials
    char login_id[15];          // Format : FT{id}
    char password[15];
    
};

#endif