// #include <stdio.h>
// #include <stdlib.h>
// #include <fcntl.h>
// #include <stdbool.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h> 


// #include "../structures/admin.h"
// #include "../client.c"
// int welcome_menu(int);
// void admin_login(int sockfd) {
//     struct admin curr_user;
//     bool res;
//     int choice = 1;
//     printf("Login id: ");
//     scanf("%s", curr_user.login_id);
//     printf("Password: ");
//     char* password = getpass("");
//     strcpy(curr_user.password, password);

//     send(sockfd, &choice, sizeof(choice), 0);
//     send(sockfd, &curr_user, sizeof(struct admin), 0);

//     recv(sockfd, &res, sizeof(res), 0);

//     if (res == true) {
//         printf("Login Successful \n\n");
//     }
//     else {
//         printf("Invalid credentials\n\n");
//         welcome_menu(sockfd);
//     }

//     return;
// }

// // #endif