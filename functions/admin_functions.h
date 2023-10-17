// #ifndef ADMIN_FUNCS
// #define ADMIN_FUNCS

// #include <stdio.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <stdlib.h>
// #include <string.h>
// #include <fcntl.h>
// #include <stdbool.h>
// #include <errno.h>

// #define DEFAULT_PASS "abc123"
// #define STUDENT_DB "/home/pranjal-gawande/Software System/Course Registration portal/database/student_db"

// #include "./showmenu.h"
// #include "../structures/student.h"

// void add_student(int, int);

// void admin_functions(int sockfd, int option)
// {
//     switch (option)
//     {
//     case 1:
//         add_student(sockfd, option);
//         break;
//     case 2:
//         //view_student_details(sockfd, option);
//         break;
//     case 3:
//         //add_faculty(sockfd, option);
//         break;
//     case 4:
//         //view_faculty_details(sockfd, option);
//         break;
//     case 5:
//         //activate_student(sockfd, option);
//         break;
//     case 6:
//         //block_student(sockfd, option);
//         break;
//     case 7:
//         //modify_student(sockfd, option);
//         break;
//     case 8:
//         //modify_faculty(sockfd, option);
//         break;
//     case 9:
//         //logout(sockfd, option);
//         break;
//     default:
//         printf("Invalid choice");
//         admin_menu();
//         break;
//     }
// }

// void add_student(int sockfd, int option) {
//     struct student new_student, login;
//     bool result;

//     send(sockfd, &option, sizeof(option), 0);

//     // Taking inputs for new student
//     printf("Enter name: ");
//     scanf("%s", new_student.name);

//     printf("Enter age: ");
//     scanf("%d", &new_student.age);
    
//     printf("Enter address: ");
//     scanf("%s", new_student.address);

//     printf("Enter email: ");
//     scanf("%s", new_student.email);

//     strcpy(new_student.login_id, "");

//     strcpy(new_student.password, DEFAULT_PASS);

//     send(sockfd, &new_student, sizeof(struct student), 0);

//     recv(sockfd, &result, sizeof(result), 0);

//     char loginid[20];
//     recv(sockfd, &login.login_id, sizeof(login.login_id), 0);

//     if (result == true) {
//         printf ("Student record added Successfully\n");
//         printf("login Id generated is: %s\n", login.login_id);
//     }
//     else {
//         printf ("Error during adding student record\n");
//     }
//     admin_menu();

// }

// #endif