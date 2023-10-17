#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>     

#include "../structures/admin.h"

void main() {
    struct admin my_admin;
    
    int fd = open("/home/pranjal-gawande/Software System/Course Registration portal/database/admin_db", O_RDWR, 0777);
    if (fd == -1) {
        perror ("Error in opening admin_db");
        return;
    }

    my_admin.id = 0;
    strcpy(my_admin.name, "iiitb admin");
    strcpy(my_admin.login_id, "iiitb");
    strcpy(my_admin.password, "1234");

    int fd_write = write(fd, &my_admin, sizeof(my_admin));
    if (fd_write == -1) {
        perror ("Error in writing");
        return;
    }
    close(fd);
}