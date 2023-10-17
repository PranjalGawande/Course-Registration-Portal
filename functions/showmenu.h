// #ifndef SHOWMENU_H
// #define SHOWMENU_H

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

// // Menu of Admin
// int admin_menu(int sockfd) {
//     int option;
//     printf("********** Welcome to Admin Menu **********\n\n");
//     printf("1. Add Student\n");
//     printf("2. View Student Details\n");
//     printf("3. Add Faculty\n");
//     printf("4. View Faculty Details\n");
//     printf("5. Activate Student\n");
//     printf("6. Block Student\n");
//     printf("7. Modify Student Details\n");
//     printf("8. Modify Faculty Details\n");
//     printf("9. Logout and Exit\n\n");

//     printf("Enter your choice: \n");
//     scanf("%d", &option);
//     // admin_functions(sockfd, option);

//     return option;
// }

// // Menu of Faculty
// int faculty_menu() {
//     int option;
//     printf("********** Welcome to Faculty Menu **********\n\n");
//     printf("1. View Offering Courses\n");
//     printf("2. Add New Course\n");
//     printf("3. Remove Course\n");
//     printf("4. Update Course Details\n");
//     printf("5. Change Password\n");
//     printf("6. Logout and Exit\n\n");
    
//     printf("Enter your choice: \n");

//     scanf("%d", &option);
//     return option;
// }

// // Menu of Student
// int student_menu() {
//     int option;
//     printf("********** Welcome to Student Menu **********\n\n");
//     printf("1. View All Courses\n");
//     printf("2. Enroll New Course\n");
//     printf("3. Drop Course\n");
//     printf("4. View Enrolled Course Details\n");
//     printf("5. Change Password\n");
//     printf("6. Logout and Exit\n\n");
//     printf("Enter your choice: \n");

//     scanf("%d", &option);
//     return option;
// }

// #endif