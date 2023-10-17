#ifndef VALIDATE_H
#define VALIDATE_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 

#include "../structures/admin.h"
#include "../structures/faculty.h"
// #include "../server.c"

int uid;
bool result;

bool validateAdmin(struct admin curr_user) {
    // bool result;
    int fd = open("/home/pranjal-gawande/Software System/Course Registration portal/database/admin_db", O_RDONLY);
    if (fd == -1) {
        perror ("Error in opening admin_db file");
        exit(EXIT_FAILURE);
    }

    struct admin temp;
    int status;
    
    // Setting Read lock
    struct flock lock;
    lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;   	    
	lock.l_len=0;       
	lock.l_pid=getpid();

    status = fcntl(fd, F_SETLKW, &lock);
    if (status == -1) {
        perror("Error in setting the lock");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // reading record
    while(read(fd, &temp, sizeof(temp)) > 0) {
        if (strcmp(temp.login_id, curr_user.login_id) == 0 && strcmp(temp.password, curr_user.password) == 0) {
            uid = temp.id;
            result = true;
            break;
        }
        else {
            result = false;
        }
    }

    // release lock
    lock.l_type = F_UNLCK;
    status = fcntl(fd, F_SETLK, &lock);
    if (status == -1) {
        perror ("Error in unlocking");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    return result;

}

bool validateFaculty(struct faculty curr_user) {
    int fd = open("/home/pranjal-gawande/Software System/Course Registration portal/database/faculty_db", O_RDONLY);
    if (fd == -1) {
        perror ("Error in opening faculty_db file");
        exit(EXIT_FAILURE);
    }

    struct faculty temp;
    int status;
    
    // Setting Read lock
    struct flock lock;
    lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;   	    
	lock.l_len=0;       
	lock.l_pid=getpid();

    status = fcntl(fd, F_SETLKW, &lock);
    if (status == -1) {
        perror("Error in setting the lock");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // reading record
    while(read(fd, &temp, sizeof(temp)) > 0) {
        if (strcmp(temp.login_id, curr_user.login_id) == 0 && strcmp(temp.password, curr_user.password) == 0) {
            uid = temp.id;
            result = true;
            break;
        }
        else {
            result = false;
            
        }
    }

    // release lock
    lock.l_type = F_UNLCK;
    status = fcntl(fd, F_SETLK, &lock);
    if (status == -1) {
        perror ("Error in unlocking");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    return result;

}


bool validateStudent(struct student curr_user) {
    int fd = open("/home/pranjal-gawande/Software System/Course Registration portal/database/student_db", O_RDONLY);
    if (fd == -1) {
        perror ("Error in opening student_db file");
        exit(EXIT_FAILURE);
    }

    struct student temp;
    int status;
    
    // Setting Read lock
    struct flock lock;
    lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;   	    
	lock.l_len=0;       
	lock.l_pid=getpid();

    status = fcntl(fd, F_SETLKW, &lock);
    if (status == -1) {
        perror("Error in setting the lock");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // reading record
    while(read(fd, &temp, sizeof(temp)) > 0) {
        if (strcmp(temp.login_id, curr_user.login_id) == 0 && strcmp(temp.password, curr_user.password) == 0) {
            uid = temp.id;
            result = true;
            break;
        }
        else {
            result = false;

        }
    }

    // release lock
    lock.l_type = F_UNLCK;
    status = fcntl(fd, F_SETLK, &lock);
    if (status == -1) {
        perror ("Error in unlocking");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    return result;

}

#endif