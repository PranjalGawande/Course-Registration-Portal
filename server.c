#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

#include "./structures/admin.h"
#include "./structures/faculty.h"
#include "./structures/student.h"
#include "./structures/course.h"
#include "./structures/enrollment.h"
#include "./functions/validate.h"

#define DEFAULT_PASS "abc123"
#define STUDENT_DB "/home/pranjal-gawande/Software System/Course Registration portal/database/student_db"
#define FACULTY_DB "/home/pranjal-gawande/Software System/Course Registration portal/database/faculty_db"
#define COURSE_DB "/home/pranjal-gawande/Software System/Course Registration portal/database/course_db"
#define ENROLL_DB "/home/pranjal-gawande/Software System/Course Registration portal/database/enroll_db"

int servercall(int);

bool addstudent(int, struct student);
bool viewStudentDetails(int, struct student);
bool addfaculty(int, struct faculty);
bool viewFacultyDetails(int, struct faculty);
bool activateStudent(int, struct student);
bool blockStudent(int, struct student);
bool modifyStudent(int, struct student);
bool modifyFaculty(int, struct faculty);

bool addCourse(int, struct course);
bool viewCourses(int, struct course);
bool removeCourse(int, struct course);
bool updateCourse(int, struct course);
bool facultyChangePass(int, struct faculty);

bool studentChangePass(int, struct student);
bool viewAllCourses(int, struct course);
bool enrollCourse(int, struct enrollment);
bool dropCourse(int, struct enrollment);
bool viewEnrolledCourses(int, struct enrollment);

int available_seats(int);
void reduceSeats(int);
void increaseSeats(int);
void coursesDetails(int, struct course);

// int id;

int main()
{
	struct sockaddr_in server, client;
	int sockfd, clifd, c_size;

	sockfd = socket(AF_INET, SOCK_STREAM, 0); // Create socket
	if (sockfd == -1)
	{
		perror("Error in creating socket");
		return 1;
	}

	printf("Server socket is created\n");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(8080);

	int status = bind(sockfd, (struct sockaddr *)&server, sizeof(server));
	if (status == -1)
	{
		perror("Error during bind");
		return 1;
	}
	printf("Bind successful\n");

	status = listen(sockfd, 5); // 5 request will be queued, after that request will be rejected
	if (status == -1)
	{
		perror("Error during listen");
		return 1;
	}
	printf("listening...\n");
	printf("Welcome to Course Registration Portal\n\n");
	printf("Waiting for client to connect...\n");

	while (1)
	{
		c_size = sizeof(client);
		clifd = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&c_size);
		if (clifd == -1)
		{
			perror("Error during accepting a connection");
			return 1;
		}
		// printf("Client accepted connection\n");

		if (!fork())
		{
			close(sockfd);
			printf("Connected to client\n");
			servercall(clifd);
			exit(EXIT_SUCCESS);
		}
		else
		{
			close(clifd);
		}
	}
	// close(sockfd);

	return 0;
}

int servercall(int clifd)
{
	bool result;
	int choice, option, read_size;
	while (1)
	{ // for login
		recv(clifd, &choice, sizeof(choice), 0);
		printf("Choice: %d\n", choice);
		switch (choice)
		{
		case 1:
			struct admin curr_userA;
			recv(clifd, &curr_userA, sizeof(struct admin), 0);
			result = validateAdmin(curr_userA);
			send(clifd, &result, sizeof(result), 0);
			break;
		case 2:
			struct faculty curr_userF;
			recv(clifd, &curr_userF, sizeof(struct faculty), 0);
			result = validateFaculty(curr_userF);
			send(clifd, &result, sizeof(result), 0);
			break;
		case 3:
			struct student curr_userS;
			recv(clifd, &curr_userS, sizeof(struct student), 0);
			result = validateStudent(curr_userS);
			send(clifd, &result, sizeof(result), 0);
			break;
		default:
			// result = false;
			// send(clifd, &result, sizeof(result), 0);
			break;
		}
		if (result == true)
		{
			break;
		}
	}

	while (1)
	{

		recv(clifd, &option, sizeof(option), 0);
		// printf("option: %d\n", option);
		switch (choice)
		{
		case 1:
			// recv(clifd, &option, sizeof(option), 0);
			switch (option)
			{
			case 1: // add student
				struct student record1;
				recv(clifd, &record1, sizeof(struct student), 0);
				result = addstudent(clifd, record1);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 2:
				struct student record2;
				recv(clifd, &record2, sizeof(struct student), 0);
				result = viewStudentDetails(clifd, record2);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 3:
				struct faculty record3;
				recv(clifd, &record3, sizeof(struct faculty), 0);
				result = addfaculty(clifd, record3);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 4:
				struct faculty record4;
				recv(clifd, &record4, sizeof(struct faculty), 0);
				result = viewFacultyDetails(clifd, record4);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 5:
				struct student record5;
				recv(clifd, &record5, sizeof(struct student), 0);

				result = activateStudent(clifd, record5);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 6:
				struct student record6;
				recv(clifd, &record6, sizeof(struct student), 0);
				result = blockStudent(clifd, record6);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 7:
				struct student record7;
				recv(clifd, &record7, sizeof(struct student), 0);
				result = modifyStudent(clifd, record7);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 8:
				struct faculty record8;
				recv(clifd, &record8, sizeof(struct faculty), 0);
				result = modifyFaculty(clifd, record8);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 9:
				break;
			default:
				break;
			}
			break;

		case 2:
			// recv(clifd, &option, sizeof(option), 0);
			switch (option)
			{
			case 1:
				printf("\noption: %d\n", option);
				struct course record9;
				recv(clifd, &record9, sizeof(struct course), 0);
				result = viewCourses(clifd, record9);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 2:
				printf("\noption: %d\n", option);
				struct course record10;
				recv(clifd, &record10, sizeof(struct course), 0);
				result = addCourse(clifd, record10);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 3:
				struct course record11;
				recv(clifd, &record11, sizeof(struct course), 0);
				result = removeCourse(clifd, record11);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 4:
				struct course record12;
				recv(clifd, &record12, sizeof(struct course), 0);
				result = updateCourse(clifd, record12);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 5:
				struct faculty record13;
				recv(clifd, &record13, sizeof(struct faculty), 0);
				result = facultyChangePass(clifd, record13);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 6:
				break;
			default:
				break;
			}
			break;

		case 3:
			// recv(clifd, &option, sizeof(option), 0);
			switch (option)
			{
			case 1:
				printf("Entering in student view all course\n");
				printf("student option: %d\n", option);
				struct course record14;
				recv(clifd, &record14, sizeof(struct course), 0);
				result = viewAllCourses(clifd, record14);
				send(clifd, &result, sizeof(result), 0);
				// // viewAvailableCourse (clifd);
				break;
			case 2:
				printf("Entering in student enrollment\n");
				printf("student option: %d\n", option);
				struct enrollment record15;
				recv(clifd, &record15, sizeof(struct enrollment), 0);
				result = enrollCourse(clifd, record15);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 3:
				printf("Entering in drop course\n");
				printf("student option: %d\n", option);
				struct enrollment record16;
				recv(clifd, &record16, sizeof(struct enrollment), 0);
				result = dropCourse(clifd, record16);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 4:
				printf("Entering in view enrolled courses\n");
				printf("student option: %d\n", option);
				struct enrollment record17;
				recv(clifd, &record17, sizeof(struct enrollment), 0);
				result = viewEnrolledCourses(clifd, record17);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 5:
				struct student record18;
				recv(clifd, &record18, sizeof(struct student), 0);
				result = studentChangePass(clifd, record18);
				send(clifd, &result, sizeof(result), 0);
				break;
			}
			break;
		default:
			break;
		}
		// if (option == 4)
		// {
		// 	break;
		// }
	}
	close(clifd);
	printf("client session end");

	return 0;
}

// *************************************
// ********* ADMIN FUNCTOINS ***********
// *************************************

bool addstudent(int clifd, struct student record)
{
	// printf("at add student\n");
	bool result;
	struct student prev_record;

	int fd = open(STUDENT_DB, O_RDWR, 0777);
	if (fd == -1 && errno == ENOENT)
	{
		fd = open(STUDENT_DB, O_RDWR | O_CREAT | O_APPEND, 0777);
		if (fd == -1)
		{
			perror("Error in opening student_db");
			exit(EXIT_FAILURE);
		}
		record.id = 1; // if student_db was not created
	}
	else if (fd == -1)
	{
		perror("Error in opening student_db file");
		exit(EXIT_FAILURE);
	}
	else
	{

		// using read lock for fetching id from prev student
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence = SEEK_END;
		lock.l_start = (-1) * sizeof(struct student);
		lock.l_len = sizeof(struct student);
		lock.l_pid = getpid();

		int status = fcntl(fd, F_SETLKW, &lock);
		if (status == -1)
		{
			perror("Error while locking student record");
			exit(EXIT_FAILURE);
		}

		lseek(fd, (-1) * sizeof(struct student), SEEK_END);

		ssize_t fd_read = read(fd, &prev_record, sizeof(struct student));
		if (fd_read == -1)
		{
			perror("Error in reading prev_student record");
			exit(EXIT_FAILURE);
		}

		lock.l_type = F_UNLCK;
		status = fcntl(fd, F_SETLK, &lock);
		if (status == -1)
		{
			perror("Error while unlocking student record");
			exit(EXIT_FAILURE);
		}
		// printf("prev_id: %d\n", prev_record.id);
		// printf("new_id: %d\n", record.id);
		record.id = prev_record.id + 1;
		// printf("generated_id: %d\n", record.id);
	}

	// record.status = 1;
	strcpy(record.status, "ACTIVE");

	// generate login_id -> MT{id}
	// strcpy(record.login_id, "MT");
	sprintf(record.login_id, "MT%d", record.id);

	int id = record.id;
	send(clifd, &id, sizeof(id), 0);

	printf("Generated_id: %d\n", record.id);

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();

	ssize_t fd_write = write(fd, &record, sizeof(struct student));
	if (fd_write == -1)
	{
		perror("Error in writing student record");
		result = false;
		exit(EXIT_FAILURE);
	}
	else
	{
		result = true;
	}

	lock.l_type = F_UNLCK;
	int status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error while unlocking student record");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return result;
}

bool viewStudentDetails(int clifd, struct student record)
{
	int i = record.id - 1;
	bool result;
	struct student curr_record;

	int fd = open(STUDENT_DB, O_RDONLY, 0777);
	if (fd == -1)
	{
		perror("Error in opening student_db");
		exit(EXIT_FAILURE);
	}

	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct student);
	lock.l_len = sizeof(struct student);
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking student record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (i) * sizeof(struct student), SEEK_SET);
	ssize_t fd_read = read(fd, &curr_record, sizeof(struct student));
	if (fd_read == -1)
	{
		perror("Error while reading current record");
		exit(EXIT_FAILURE);
	}

	if (curr_record.id == record.id)
	{
		send(clifd, &curr_record, sizeof(struct student), 0);
		result = true;
	}
	else
	{
		send(clifd, &curr_record, sizeof(struct student), 0);
		result = false;
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}

	close(fd);

	return result;
}

bool addfaculty(int clifd, struct faculty record)
{
	// printf("at add student\n");
	bool result;
	struct faculty prev_record;

	int fd = open(FACULTY_DB, O_RDWR, 0777);
	if (fd == -1 && errno == ENOENT)
	{
		fd = open(FACULTY_DB, O_RDWR | O_CREAT | O_APPEND, 0777);
		if (fd == -1)
		{
			perror("Error in opening faculty_db");
			exit(EXIT_FAILURE);
		}
		record.id = 1; // if student_db was not created
	}
	else if (fd == -1)
	{
		perror("Error in opening faculty_db file");
		exit(EXIT_FAILURE);
	}
	else
	{

		// using read lock for fetching id from prev student
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence = SEEK_END;
		lock.l_start = (-1) * sizeof(struct faculty);
		lock.l_len = sizeof(struct faculty);
		lock.l_pid = getpid();

		int status = fcntl(fd, F_SETLKW, &lock);
		if (status == -1)
		{
			perror("Error while locking faculty record");
			exit(EXIT_FAILURE);
		}

		lseek(fd, (-1) * sizeof(struct faculty), SEEK_END);

		ssize_t fd_read = read(fd, &prev_record, sizeof(struct faculty));
		if (fd_read == -1)
		{
			perror("Error in reading prev_student record");
			exit(EXIT_FAILURE);
		}

		lock.l_type = F_UNLCK;
		status = fcntl(fd, F_SETLK, &lock);
		if (status == -1)
		{
			perror("Error while unlocking student record");
			exit(EXIT_FAILURE);
		}
		// printf("prev_id: %d\n", prev_record.id);
		// printf("new_id: %d\n", record.id);
		record.id = prev_record.id + 1;
		// printf("generated_id: %d\n", record.id);
	}

	// generate login_id -> FT{id}
	sprintf(record.login_id, "FT%d", record.id);

	int id = record.id;
	send(clifd, &id, sizeof(id), 0);

	printf("Generated_id: %d\n", record.id);

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();

	ssize_t fd_write = write(fd, &record, sizeof(struct faculty));
	if (fd_write == -1)
	{
		perror("Error in writing faculty record");
		result = false;
		// exit(EXIT_FAILURE);
	}
	else
	{
		result = true;
	}

	lock.l_type = F_UNLCK;
	int status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error while unlocking student record");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return result;
}

bool viewFacultyDetails(int clifd, struct faculty record)
{
	int i = record.id - 1;
	bool result;
	struct faculty curr_record;

	int fd = open(FACULTY_DB, O_RDONLY, 0777);
	if (fd == -1)
	{
		perror("Error in opening faculty_db");
		exit(EXIT_FAILURE);
	}

	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct faculty);
	lock.l_len = sizeof(struct faculty);
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking faculty record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (i) * sizeof(struct faculty), SEEK_SET);
	ssize_t fd_read = read(fd, &curr_record, sizeof(struct faculty));
	if (fd_read == -1)
	{
		perror("Error while reading current record");
		exit(EXIT_FAILURE);
	}

	if (curr_record.id == record.id)
	{
		send(clifd, &curr_record, sizeof(struct faculty), 0);
		result = true;
	}
	else
	{
		send(clifd, &curr_record, sizeof(struct faculty), 0);
		result = false;
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}

	close(fd);

	return result;
}

bool activateStudent(int clifd, struct student record)
{
	int i = record.id - 1;
	int msg;
	bool result;
	int fd = open(STUDENT_DB, O_RDWR, 0777);

	struct student curr_record;

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct student);
	lock.l_len = sizeof(struct student);
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking student record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (i) * sizeof(struct student), SEEK_SET);
	ssize_t fd_read = read(fd, &curr_record, sizeof(struct student));
	if (fd_read == -1)
	{
		perror("Error while reading current record");
		exit(EXIT_FAILURE);
	}

	if (record.id == curr_record.id)
	{
		if (!strcmp(curr_record.status, "ACTIVE"))
		{
			msg = 0;
			send(clifd, &msg, sizeof(msg), 0);
		}
		else
		{
			strcpy(curr_record.status, "ACTIVE");
			msg = 1;
			send(clifd, &msg, sizeof(msg), 0);
			lseek(fd, (-1) * sizeof(struct student), SEEK_CUR);
			int fd_write = write(fd, &curr_record, sizeof(struct student));
			if (fd_write == -1)
			{
				perror("Error in updating status");
				result = false;
			}
			else
			{
				printf("NOt Found\n");
				result = true;
			}
		}
	}
	else
	{
		msg = 2;
		send(clifd, &msg, sizeof(msg), 0);
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}
	close(fd);
	return result;
}

bool blockStudent(int clifd, struct student record)
{
	int i = record.id - 1;
	int msg;
	bool result;
	int fd = open(STUDENT_DB, O_RDWR, 0777);

	struct student curr_record;

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct student);
	lock.l_len = sizeof(struct student);
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking student record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (i) * sizeof(struct student), SEEK_SET);
	ssize_t fd_read = read(fd, &curr_record, sizeof(struct student));
	if (fd_read == -1)
	{
		perror("Error while reading current record");
		exit(EXIT_FAILURE);
	}

	if (record.id == curr_record.id)
	{
		if (!strcmp(curr_record.status, "BLOCKED"))
		{
			msg = 0;
			send(clifd, &msg, sizeof(msg), 0);
		}
		else
		{
			strcpy(curr_record.status, "BLOCKED");
			msg = 1;
			lseek(fd, (-1) * sizeof(struct student), SEEK_CUR);
			int fd_write = write(fd, &curr_record, sizeof(struct student));
			if (fd_write == -1)
			{
				perror("Error in updating status");
				result = false;
			}
			else
			{
				result = true;
			}
			send(clifd, &msg, sizeof(msg), 0);
		}
	}
	else
	{
		msg = 2;
		send(clifd, &msg, sizeof(msg), 0);
	}
	// lseek (fd, (-1) * sizeof(struct student), SEEK_CUR);
	// int fd_write = write(fd, &curr_record, sizeof(struct student));
	// if (fd_write == -1) {
	// 	perror("Error in updating status");
	// 	result = false;
	// }
	// else {
	// 	result = true;
	// }

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}
	close(fd);
	return result;
}

bool modifyStudent(int clifd, struct student record)
{
	bool result;
	int i = record.id - 1;
	struct student curr_record;

	int fd = open(STUDENT_DB, O_RDWR, 0777);
	if (fd == -1)
	{
		perror("Error in opening student_db");
		exit(EXIT_FAILURE);
	}

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct student);
	lock.l_len = sizeof(struct student);
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking student record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (i) * sizeof(struct student), SEEK_SET);
	ssize_t fd_read = read(fd, &curr_record, sizeof(struct student));
	if (fd_read == -1)
	{
		perror("Error while reading current record");
		exit(EXIT_FAILURE);
	}

	strcpy(curr_record.name, record.name);
	curr_record.age = record.age;
	strcpy(curr_record.address, record.address);
	strcpy(curr_record.email, record.email);

	lseek(fd, (-1) * sizeof(struct student), SEEK_CUR);
	ssize_t fd_write = write(fd, &curr_record, sizeof(struct student));
	if (fd_write != 0)
	{
		result = true;
	}
	else
	{
		result = false;
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return result;
}

bool modifyFaculty(int clifd, struct faculty record)
{
	bool result;
	int i = record.id - 1;
	struct faculty curr_record;

	int fd = open(FACULTY_DB, O_RDWR, 0777);
	if (fd == -1)
	{
		perror("Error in opening faculty_db");
		exit(EXIT_FAILURE);
	}

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct faculty);
	lock.l_len = sizeof(struct faculty);
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking faculty record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (i) * sizeof(struct faculty), SEEK_SET);
	ssize_t fd_read = read(fd, &curr_record, sizeof(struct faculty));
	if (fd_read == -1)
	{
		perror("Error while reading current record");
		exit(EXIT_FAILURE);
	}

	strcpy(curr_record.name, record.name);
	strcpy(curr_record.department, record.department);
	strcpy(curr_record.designation, record.designation);
	strcpy(curr_record.address, record.address);
	strcpy(curr_record.email, record.email);

	lseek(fd, (-1) * sizeof(struct faculty), SEEK_CUR);
	ssize_t fd_write = write(fd, &curr_record, sizeof(struct faculty));
	if (fd_write != 0)
	{
		result = true;
	}
	else
	{
		result = false;
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return result;
}

// ***************************************
// ********* FACULTY FUNCTOINS ***********
// ***************************************

bool addCourse(int clifd, struct course record)
{
	printf("\nEntered add course\n");
	bool result;
	struct course prev_record;

	int fd = open(COURSE_DB, O_RDWR, 0777);
	if (fd == -1 && errno == ENOENT)
	{
		fd = open(COURSE_DB, O_RDWR | O_CREAT | O_APPEND, 0777);
		if (fd == -1)
		{
			perror("Error in opening course_db");
			exit(EXIT_FAILURE);
		}
		record.id = 1; // if course_db was not created
	}
	else if (fd == -1)
	{
		perror("Error in opening course_db file");
		exit(EXIT_FAILURE);
	}
	else
	{

		// using read lock for fetching id from prev course
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = 0;
		lock.l_len = 0;
		lock.l_pid = getpid();

		int status = fcntl(fd, F_SETLKW, &lock);
		if (status == -1)
		{
			perror("Error while locking course record");
			exit(EXIT_FAILURE);
		}

		printf("\nLocked\n");

		lseek(fd, (-1) * sizeof(struct course), SEEK_END);

		ssize_t fd_read = read(fd, &prev_record, sizeof(struct course));
		if (fd_read == -1)
		{
			perror("Error in reading prev_course record");
			exit(EXIT_FAILURE);
		}

		record.id = prev_record.id + 1;

		printf("\nUNLocked\n");

		lock.l_type = F_UNLCK;
		status = fcntl(fd, F_SETLK, &lock);
		if (status == -1)
		{
			perror("Error while unlocking course record");
			exit(EXIT_FAILURE);
		}
	}

	// record.status = 1;
	strcpy(record.status, "ACTIVE");
	record.faculty_id = uid;
	record.no_of_available_seats = record.no_of_seats;

	// generate login_id -> MT{id}
	// strcpy(record.login_id, "MT");
	// char new_code[6];
	// int res = snprintf(new_code, sizeof(new_code), "%s%d", record.course_code, record.id);
	// if (res >= 0 && result < sizeof(new_code))
	// {
	// 	// The string fit within the buffer
	// 	strcpy(record.course_code, new_code);
	// }

	int id = record.id;
	send(clifd, &id, sizeof(id), 0);

	printf("\nsended id to client\n");

	// send(clifd, &new_code, sizeof(new_code), 0);

	printf("Generated_id: %d\n", record.id);

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error in locking");
		exit(EXIT_FAILURE);
	}

	printf("course id: %d", record.id);
	printf("course faculty id: %d", record.faculty_id);
	// printf ("course id: %d", record.id);
	// printf ("course id: %d", record.id);
	// printf ("course id: %d", record.id);

	// lseek(fd, 0, SEEK_END);
	printf("\nwriting the record to the file\n");
	ssize_t fd_write = write(fd, &record, sizeof(struct course));
	if (fd_write == -1)
	{
		perror("Error in writing course record");
		result = false;
	}
	else
	{
		printf("\nresult = true\n");
		result = true;
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error while unlocking student record");
		exit(EXIT_FAILURE);
	}
	close(fd);
	printf("\nclose fd\n");
	return result;
}

bool viewCourses(int clifd, struct course record)
{
	printf("\nEntered in view course\n");
	bool result;
	struct course curr_record;

	printf("\nopened course db\n");
	int fd = open(COURSE_DB, O_RDONLY, 0777);
	if (fd == -1)
	{
		perror("Error in opening course_db");
		exit(EXIT_FAILURE);
	}

	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking course record");
		exit(EXIT_FAILURE);
	}

	// reading record one by one
	int count = 0;
	ssize_t byteread;

	off_t offset = lseek(fd, 0, SEEK_SET);
	if (offset == -1)
	{
		perror("Error in lseek");
	}

	printf("\nloop to get count\n");
	// to count no. of courses offered by faculty
	while ((byteread = read(fd, &curr_record, sizeof(struct course))) > 0)
	{
		printf("curr_record.faculty_id : %d\t\ncurr_record.status: %s\n", curr_record.faculty_id, curr_record.status);
		if (curr_record.faculty_id == uid && strcmp(curr_record.status, "ACTIVE") == 0)
		{
			count++;
		}
	}
	// printf ("Count: %d\n", count);

	offset = lseek(fd, 0, SEEK_SET);
	if (offset == -1)
	{
		perror("Error in lseek");
		close(fd);
		exit(EXIT_FAILURE);
	}

	send(clifd, &count, sizeof(count), 0);
	printf("\nsended count to client: %d\n", count);
	printf("\nloop to send records\n");
	// reading records and sending to client
	while ((byteread = read(fd, &curr_record, sizeof(struct course))) > 0)
	{
		if (curr_record.faculty_id == uid && strcmp(curr_record.status, "ACTIVE") == 0)
		{
			send(clifd, &curr_record, sizeof(struct course), 0);
		}
	}

	if (count == 0)
	{
		result = false;
	}
	else
	{
		result = true;
	}
	// result = true;
	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}

	close(fd);

	return result;
}

bool removeCourse(int clifd, struct course record)
{
	int i = record.id - 1;
	struct course curr_record;
	bool result;

	int fd = open(COURSE_DB, O_RDWR, 0777);
	if (fd == -1)
	{
		perror("Error in opening course_db");
		exit(EXIT_FAILURE);
	}

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct course);
	lock.l_len = sizeof(struct course);
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking course record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (i) * sizeof(struct course), SEEK_SET);
	ssize_t fd_read = read(fd, &curr_record, sizeof(struct course));
	if (fd_read == -1)
	{
		perror("Error while reading current record");
		exit(EXIT_FAILURE);
	}
	int msg;
	if (curr_record.id == record.id && strcmp(curr_record.status, "ACTIVE") == 0)
	{
		strcpy(curr_record.status, "CLOSED");
		msg = 0;
		send(clifd, &msg, sizeof(msg), 0);

		lseek(fd, (-1) * sizeof(struct course), SEEK_CUR);
		int fd_write = write(fd, &curr_record, sizeof(struct course));
		if (fd_write == -1)
		{
			perror("Error in updating status");
			result = false;
		}
		else
		{
			result = true;
		}
	}
	else if (curr_record.id == record.id && strcmp(curr_record.status, "CLOSED") == 0)
	{
		msg = 1;
		send(clifd, &msg, sizeof(msg), 0);
	}
	else
	{
		msg = 2;
		send(clifd, &msg, sizeof(msg), 0);
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}
	close(fd);

	return result;
}

bool updateCourse(int clifd, struct course record)
{
	// printf("Entered in updaate course\n");
	struct course curr_record;
	bool result;
	int i = record.id - 1;

	int fd = open(COURSE_DB, O_RDWR, 0777);
	if (fd == -1)
	{
		perror("Error in opening course_db");
		exit(EXIT_FAILURE);
	}

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct course);
	lock.l_len = sizeof(struct course);
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking course record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (i) * sizeof(struct course), SEEK_SET);
	ssize_t fd_read = read(fd, &curr_record, sizeof(struct course));
	if (fd_read == -1)
	{
		perror("Error while reading current record");
		exit(EXIT_FAILURE);
	}

	strcpy(curr_record.name, record.name);
	strcpy(curr_record.department, record.department);
	curr_record.no_of_seats = record.no_of_seats;
	curr_record.credits = record.credits;
	// sprintf(curr_record.course_code, "%s%d", record.course_code, record.id);

	// char new_code[6];
	// int res = snprintf(new_code, sizeof(new_code), "%s %d", record.course_code, record.id);
	// if (res >= 0 && result < sizeof(new_code))
	// {
	// 	// The string fit within the buffer
	// 	strcpy(curr_record.course_code, new_code);
	// }

	lseek(fd, (-1) * sizeof(struct course), SEEK_CUR);
	if (strcmp(curr_record.status, "ACTIVE") == 0)
	{
		ssize_t fd_write = write(fd, &curr_record, sizeof(struct course));
		if (fd_write == -1)
		{
			result = false;
		}
		else
		{
			result = true;
		}
	}
	else
	{
		result = false;
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return result;
}

bool facultyChangePass(int clifd, struct faculty record)
{
	// printf ("Entered change pass\n");
	struct faculty curr_record;
	bool result;
	int fd = open(FACULTY_DB, O_RDWR, 0777);
	if (fd == -1)
	{
		perror("Error in opening faculty_db");
		exit(EXIT_FAILURE);
	}

	// printf("uid: %d\n", uid);

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (uid - 1) * sizeof(struct faculty);
	lock.l_len = sizeof(struct faculty);
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking faculty record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (uid - 1) * sizeof(struct faculty), SEEK_SET);
	ssize_t fd_read = read(fd, &curr_record, sizeof(struct faculty));
	if (fd_read == -1)
	{
		perror("Error while reading current record");
		exit(EXIT_FAILURE);
	}

	strcpy(curr_record.password, record.password);

	lseek(fd, (-1) * sizeof(struct faculty), SEEK_CUR);
	ssize_t fd_write = write(fd, &curr_record, sizeof(struct faculty));
	if (fd_write == -1)
	{
		perror("Error in updating status");
		result = false;
	}
	else
	{
		result = true;
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}
	close(fd);

	return result;
}

// ***************************************
// ********* STUDENT FUNCTOINS ***********
// ***************************************

// void viewAvailableCourse (int sd)
// {
//     // DEBUG("Entering viewAvailableCourse().\n")
//     bool result;
//     int fd = open (COURSE_DB, O_RDONLY, 0644);
//     if (-1 == fd) {
//         result = false;
//     }
//     else  {
//         result = true;
//     }

//     if (result) {
//         send (sd, &result, sizeof(bool), 0);

//         bool course_result, faculty_result;
//         struct course currRec;

//         struct flock lock;
//         lock.l_type   = F_RDLCK;
//         lock.l_whence = SEEK_SET;
//         lock.l_start  = 0;
//         lock.l_len    = 0;
//         lock.l_pid    = getpid();

//         fcntl(fd, F_SETLKW, &lock);
//         char availCourse[1000], currCourse[50];
//         memset (availCourse, '\0', sizeof(availCourse));

//         while (read (fd, &currRec, sizeof(struct course)) > 0) {
//             memset(currCourse, '\0', sizeof(currCourse));
//             sprintf (currCourse, "\t%d - %s\n", currRec.id, currRec.name);
//             strcat (availCourse, currCourse);
//         }

//         send (sd, &availCourse, sizeof(availCourse), 0);

//         lock.l_type = F_UNLCK;
//         fcntl(fd, F_SETLK, &lock);

//         close(fd);
//     }
//     else {
//         send (sd, &result, sizeof(bool), 0);
//     }
//     // DEBUG("Leaving viewAvailableCourse().\n")
// }

// void viewAllCourses(int sd)
// {
//         int fd = open(COURSE_DB, O_RDWR | O_CREAT, 0744);
//         struct course buffer;
//         ssize_t bytesRead;
//         int count = 0;

//         struct flock lock;
//         lock.l_type = F_RDLCK;
//         lock.l_whence = SEEK_SET;
//         lock.l_start = 0;
//         lock.l_len = 0;

//         if (fcntl(fd, F_SETLKW, &lock) == -1)
//         {
//                 perror("Error locking Course.data");
//                 close(fd);
//                 return;
//         }

//         // count number of course offered by faculty
//         while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
//         {
//                 count++;
//         }

//         // send this count to client
//         ssize_t bytesWritten = write(sd, &count, sizeof(int));
//         if (bytesWritten == -1 || bytesWritten != sizeof(int))
//         {
//                 perror("Error sending count to client");
//                 lock.l_type = F_UNLCK;
//                 fcntl(fd, F_SETLK, &lock);
//                 close(fd);
//                 return;
//         }

//         // reset pointer back to start
//         if (lseek(fd, 0, SEEK_SET) == -1)
//         {
//                 perror("Error seeking in Course.data");
//                 lock.l_type = F_UNLCK;
//                 fcntl(fd, F_SETLK, &lock);
//                 close(fd);
//                 return;
//         }

//         // send course details to client
//         while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
//         {
//                 write(sd, &buffer, sizeof(struct course));
//         }

//         lock.l_type = F_UNLCK;
//         fcntl(fd, F_SETLK, &lock);

//         close(fd);
// }

bool viewAllCourses(int clifd, struct course record)
{
	int fd = open(COURSE_DB, O_RDONLY, 0777);
	if (fd == -1)
	{
		perror("Error in opening course_db file");
		exit(EXIT_FAILURE);
	}

	struct course curr_record;
	int count = 0;
	ssize_t fd_read;

	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking course record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, 0, SEEK_SET);
	while ((fd_read = read(fd, &curr_record, sizeof(struct course))) > 0)
	{
		if (strcmp(curr_record.status, "ACTIVE") == 0)
		{
			count++;
		}
	}

	send(clifd, &count, sizeof(count), 0);

	int c = 0;
	lseek(fd, 0, SEEK_SET);
	while ((fd_read = read(fd, &curr_record, sizeof(struct course))) > 0)
	{
		if (strcmp(curr_record.status, "ACTIVE") == 0)
		{
			send(clifd, &curr_record, sizeof(struct course), 0);
			c++;
		}
	}

	if (c == count)
	{
		result = true;
	}
	else
	{
		result = false;
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return result;
}

bool enrollCourse(int clifd, struct enrollment record)
{
	bool result = false;
	ssize_t fd_read;
	struct enrollment curr_record;
	printf("record id: %d\n", record.courseid);

	// checking if seats are available
	int cid = record.courseid;
	int seats = available_seats(cid);

	// ssize_t bytesRead;

	send(clifd, &seats, sizeof(seats), 0);
	// if seats available add entry
	if (seats > 0)
	{
		int fd = open(ENROLL_DB, O_RDWR | O_CREAT | O_APPEND, 0777);
		if (fd == -1)
		{
			perror("Error opening Enroll.data");
			return false;
		}

		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = 0;
		lock.l_len = 0;
		lock.l_pid = getpid();

		if (fcntl(fd, F_SETLKW, &lock) == -1)
		{
			perror("Error locking Enroll.data");
			close(fd);
			return false;
		}
		int msg;
		while ((fd_read = read(fd, &curr_record, sizeof(struct enrollment))) > 0)
		{
			if (curr_record.studentid == uid && curr_record.courseid == record.courseid && strcmp(curr_record.status, "ENROLLED") == 0)
			{
				msg = 0;
				send(clifd, &msg, sizeof(msg), 0);
				return false;
			}
		}

		record.studentid = uid;
		strcpy(record.status, "ENROLLED");
		ssize_t fd_write = write(fd, &record, sizeof(struct enrollment));
		if (fd_write == -1)
		{
			perror("Error in writing the record");
			exit(EXIT_FAILURE);
		}

		msg = 1;
		send(clifd, &msg, sizeof(msg), 0);

		reduceSeats(cid);

		lock.l_type = F_UNLCK;
		if (fcntl(fd, F_SETLK, &lock) == -1)
		{
			perror("Error unlocking Enroll.data");
		}

		close(fd);
		result = true;
	}
	return result;
}

int available_seats(int cid)
{
	struct course record;
	ssize_t fd_read;

	int fd = open(COURSE_DB, O_RDONLY, 0777);
	if (fd == -1)
	{
		perror("Error in opening course_db file");
		return 1;
	}

	struct flock lock;
	// memset(&lock, 0, sizeof(lock));
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error locking file");
		close(fd);
		return 1;
	}

	while ((fd_read = read(fd, &record, sizeof(struct course))) > 0)
	{
		if (record.id == cid)
		{
			return record.no_of_available_seats;
		}
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return record.no_of_available_seats;
}

void reduceSeats(int cid)
{
	struct course curr_record;
	int fd = open(COURSE_DB, O_RDWR, 0777);
	if (fd == -1)
	{
		perror("Error in opening course_db file");
		exit(EXIT_FAILURE);
	}

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (cid - 1) * sizeof(struct course);
	lock.l_len = sizeof(struct course);
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking course record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (cid - 1) * sizeof(struct course), SEEK_SET);
	ssize_t fd_read = read(fd, &curr_record, sizeof(struct course));
	if (fd_read == -1)
	{
		perror("Error while reading from course_db file");
		exit(EXIT_FAILURE);
	}

	if (curr_record.id == cid)
	{
		curr_record.no_of_available_seats -= 1;

		lseek(fd, (-1) * sizeof(struct course), SEEK_CUR);
		ssize_t fd_write = write(fd, &curr_record, sizeof(struct course));
		if (fd_write == -1)
		{
			perror("Error in updating no of available seats");
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("Seats reduced\n");
		}
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}
	close(fd);

	return;
}

bool dropCourse(int clifd, struct enrollment record)
{
	printf("Entered in drop course\n");
	struct enrollment curr_record;
	bool result = true;
	ssize_t fd_read;

	int fd = open(ENROLL_DB, O_RDWR, 0777);
	if (fd == -1)
	{
		perror("Error in opening enroll_db");
		exit(EXIT_FAILURE);
	}

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error in locking");
		exit(EXIT_FAILURE);
	}

	int msg;
	while ((fd_read = read(fd, &curr_record, sizeof(struct enrollment))) > 0)
	{
		printf ("INSIDE WHILE LOOP\n");
		printf ("COURSE ID: %d\n", curr_record.courseid);
		printf ("STUDENT ID: %d\n", curr_record.studentid);
		printf ("STATUS: %s\n", curr_record.status);
		if (curr_record.courseid == record.courseid && curr_record.studentid == uid && strcmp(curr_record.status,"ENROLLED") == 0)
		{

			strcpy(curr_record.status, "DROPPED");

			lseek(fd, (-1) * sizeof(struct enrollment), SEEK_CUR);
			ssize_t fd_write = write(fd, &curr_record, sizeof(struct enrollment));
			if (fd_write == -1)
			{
				perror("Error in writing in enroll_db");
				result = false;
			}
			result = true;
			msg = 1;
			increaseSeats(record.courseid);
			break;
		}
		else
		{
			msg = 0;
		}
	}

	send(clifd, &msg, sizeof(msg), 0);

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking");
		exit(EXIT_FAILURE);
	}

	printf ("\nMSG: %d\n",msg);
	printf ("Result: %d\n", result);

	return result;
}

void increaseSeats(int cid)
{
	struct course curr_record;

	int fd = open(COURSE_DB, O_RDWR, 0777);
	if (fd == -1)
	{
		perror("Error in opening in course_db file");
		return;
	}

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (cid - 1) * sizeof(struct course);
	lock.l_len = sizeof(struct course);
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking course record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (cid - 1) * sizeof(struct course), SEEK_SET);
	ssize_t fd_read = read(fd, &curr_record, sizeof(struct course));
	if (fd_read == -1)
	{
		perror("Error while reading from course_db file");
		exit(EXIT_FAILURE);
	}

	if (curr_record.id == cid)
	{
		curr_record.no_of_available_seats += 1;

		lseek(fd, (-1) * sizeof(struct course), SEEK_CUR);
		ssize_t fd_write = write(fd, &curr_record, sizeof(struct course));
		if (fd_write == -1)
		{
			perror("Error in updating no of available seats");
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("Seats increased\n");
		}
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}
	close(fd);

	return;
}

bool viewEnrolledCourses(int clifd, struct enrollment record)
{
	printf ("\nEntered in VIew enrolled course\n");
	struct enrollment curr_record;
	int count = 0;
	bool result;
	ssize_t fd_read;

	int fd = open(ENROLL_DB, O_RDONLY | O_CREAT, 0777);
	if (fd == -1)
	{
		perror("Error in opening enroll_db");
		exit(EXIT_FAILURE);
	}

	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking enrollment record");
		exit(EXIT_FAILURE);
	}

	while ((fd_read = read(fd, &curr_record, sizeof(struct enrollment))) > 0)
	{
		printf("Course id: %d\n", curr_record.courseid);
		printf("student id: %d\n", curr_record.studentid);
		printf("status: %s\n", curr_record.status);
		if (curr_record.studentid == uid && strcmp(curr_record.status, "ENROLLED") == 0)
		{
			count++;
		}
	}
	printf ("count: %d\n", count);
	send(clifd, &count, sizeof(count), 0);

	lseek(fd, 0, SEEK_SET);
	int c = 0;
	while ((fd_read = read(fd, &curr_record, sizeof(struct enrollment))) > 0)
	{
		if (curr_record.studentid == uid && strcmp(curr_record.status, "ENROLLED") == 0)
		{
			// send(clifd, &curr_record, sizeof(struct enrollment), 0);
			struct course courseRecord;
			courseRecord.id = curr_record.courseid;
			coursesDetails (clifd, courseRecord);
			c++;
		}
	}

	if (c == count)
	{
		result = true;
	}
	else
	{
		result = false;
	}

	printf ("Result: %d\n", result);
	printf ("c: %d\n", c);
	printf ("count: %d\n", count);

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return result;
}

void coursesDetails(int clifd, struct course courseRecord)
{
	struct course curr_record;

	int fd = open(COURSE_DB, O_RDONLY, 0777);
	if (fd == -1)
	{
		perror("Error in opening course_db file");
		return;
	}

	int cid;
	cid = courseRecord.id;
	printf ("\n COURSE ID: %d\n", cid);

	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (cid - 1) * sizeof(struct course);
	lock.l_len = sizeof(struct course);
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking course record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (cid - 1) * sizeof(struct course), SEEK_SET);
	ssize_t fd_read = read(fd, &curr_record, sizeof(struct course));
	if (fd_read == -1)
	{
		perror("Error in reading curr_record");
		return;
	}

	if (curr_record.id == cid)
	{
		printf("\nCourse ID: %d", courseRecord.id);
        printf("\nCourse Name: %s", courseRecord.name);
        printf ("\nFaculty Id: %d", courseRecord.faculty_id);
        printf ("\nDepartment: %s", courseRecord.department);
        printf("\nAvailable Seats: %d", courseRecord.no_of_available_seats);
        printf("\nCourse Credit: %d\n\n", courseRecord.credits);
		send(clifd, &curr_record, sizeof(struct course), 0);
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return;
}

bool studentChangePass(int clifd, struct student record)
{
	struct student curr_record;
	bool result;
	int fd = open(STUDENT_DB, O_RDWR, 0777);
	if (fd == -1)
	{
		perror("Error in opening student_db");
		exit(EXIT_FAILURE);
	}

	// printf("uid: %d\n", uid);

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (uid - 1) * sizeof(struct student);
	lock.l_len = sizeof(struct student);
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking student record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (uid - 1) * sizeof(struct student), SEEK_SET);
	ssize_t fd_read = read(fd, &curr_record, sizeof(struct student));
	if (fd_read == -1)
	{
		perror("Error while reading current record");
		exit(EXIT_FAILURE);
	}

	strcpy(curr_record.password, record.password);

	lseek(fd, (-1) * sizeof(struct student), SEEK_CUR);
	ssize_t fd_write = write(fd, &curr_record, sizeof(struct student));
	if (fd_write == -1)
	{
		perror("Error in updating status");
		result = false;
	}
	else
	{
		result = true;
	}

	lock.l_type = F_UNLCK;
	status = fcntl(fd, F_SETLK, &lock);
	if (status == -1)
	{
		perror("Error in unlocking the file");
		exit(EXIT_FAILURE);
	}
	close(fd);

	return result;
}