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

bool removeAllEnroll(int);
bool dropLastStudents(struct course);
int available_seats(int);
void reduceSeats(int);
void increaseSeats(int);
void coursesDetails(int, struct course);


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

	status = listen(sockfd, 5); // maximum number of clients that can wait in the queue
	if (status == -1)
	{
		perror("Error during listen");
		return 1;
	}
	printf("listening...\n\n");
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

	return 0;
}

int servercall(int clifd)
{
	bool result;
	int choice, option, read_size;
	while (1)
	{ 	
		// Login
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
			break;
		}
		if (result == true)
		{
			break;
		}
	}

	while (1)
	{
		// After Login Successful
		recv(clifd, &option, sizeof(option), 0);
		switch (choice)
		{
		case 1:			// Admin Menu
			switch (option)
			{
			case 1: 	// Add student
				struct student record1;
				recv(clifd, &record1, sizeof(struct student), 0);
				result = addstudent(clifd, record1);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 2:		// View Student details
				struct student record2;
				recv(clifd, &record2, sizeof(struct student), 0);
				result = viewStudentDetails(clifd, record2);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 3:		// Add Faculty
				struct faculty record3;
				recv(clifd, &record3, sizeof(struct faculty), 0);
				result = addfaculty(clifd, record3);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 4:		// View Faculty details
				struct faculty record4;
				recv(clifd, &record4, sizeof(struct faculty), 0);
				result = viewFacultyDetails(clifd, record4);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 5:		//Activate student
				struct student record5;
				recv(clifd, &record5, sizeof(struct student), 0);
				result = activateStudent(clifd, record5);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 6:		// Block Student
				struct student record6;
				recv(clifd, &record6, sizeof(struct student), 0);
				result = blockStudent(clifd, record6);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 7:		// Modify student
				struct student record7;
				recv(clifd, &record7, sizeof(struct student), 0);
				result = modifyStudent(clifd, record7);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 8:		// Modify Faculty
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

		case 2:			// Faculty Menu
			switch (option)
			{
			case 1:		// View Courses offered
				struct course record9;
				recv(clifd, &record9, sizeof(struct course), 0);
				result = viewCourses(clifd, record9);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 2:		// Add Course
				struct course record10;
				recv(clifd, &record10, sizeof(struct course), 0);
				result = addCourse(clifd, record10);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 3:		// Remove Course
				struct course record11;
				recv(clifd, &record11, sizeof(struct course), 0);
				result = removeCourse(clifd, record11);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 4:		// Update Course
				struct course record12;
				recv(clifd, &record12, sizeof(struct course), 0);
				result = updateCourse(clifd, record12);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 5:		// Faculty change password
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

		case 3:			// Student Menu
			switch (option)
			{
			case 1:		// View All courses
				struct course record14;
				recv(clifd, &record14, sizeof(struct course), 0);
				result = viewAllCourses(clifd, record14);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 2:		// Enroll Course
				struct enrollment record15;
				recv(clifd, &record15, sizeof(struct enrollment), 0);
				result = enrollCourse(clifd, record15);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 3:		// Drop course
				struct enrollment record16;
				recv(clifd, &record16, sizeof(struct enrollment), 0);
				result = dropCourse(clifd, record16);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 4:		// View Enrolled Courses
				struct enrollment record17;
				recv(clifd, &record17, sizeof(struct enrollment), 0);
				result = viewEnrolledCourses(clifd, record17);
				send(clifd, &result, sizeof(result), 0);
				break;
			case 5:		// Student Change password
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
	}
	close(clifd);
	printf("client session end");

	return 0;
}

// *************************************
// ********* ADMIN FUNCTOINS ***********
// *************************************


// Add Student
bool addstudent(int clifd, struct student record)
{
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

		// Unlocking
		lock.l_type = F_UNLCK;
		status = fcntl(fd, F_SETLK, &lock);
		if (status == -1)
		{
			perror("Error while unlocking student record");
			exit(EXIT_FAILURE);
		}
		
		record.id = prev_record.id + 1;		// increasing the student id
		
	}

	strcpy(record.status, "ACTIVE");

	// generate login_id -> MT{id}
	sprintf(record.login_id, "MT%d", record.id);

	int id = record.id;
	send(clifd, &id, sizeof(id), 0);

	printf("Generated_id: %d\n", record.id);


	// Write lock for writing the new record
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

	// Unlocking
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


// View Student Details
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
	lock.l_start = (i) * sizeof(struct student);		// Record Locking
	lock.l_len = sizeof(struct student);
	lock.l_pid = getpid();

	int status = fcntl(fd, F_SETLKW, &lock);
	if (status == -1)
	{
		perror("Error while locking student record");
		exit(EXIT_FAILURE);
	}

	lseek(fd, (i) * sizeof(struct student), SEEK_SET);		// Reading only 1 record
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


// Add Faculty
bool addfaculty(int clifd, struct faculty record)
{
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
		
		record.id = prev_record.id + 1;
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


// View Faculty details
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


// Activate Students
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


// Block Student
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


// Modify Student
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


// Modify Faculty
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


// Add Course
bool addCourse(int clifd, struct course record)
{
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

		lseek(fd, (-1) * sizeof(struct course), SEEK_END);

		ssize_t fd_read = read(fd, &prev_record, sizeof(struct course));
		if (fd_read == -1)
		{
			perror("Error in reading prev_course record");
			exit(EXIT_FAILURE);
		}

		record.id = prev_record.id + 1;

		lock.l_type = F_UNLCK;
		status = fcntl(fd, F_SETLK, &lock);
		if (status == -1)
		{
			perror("Error while unlocking course record");
			exit(EXIT_FAILURE);
		}
	}

	strcpy(record.status, "ACTIVE");
	record.faculty_id = uid;
	record.no_of_available_seats = record.no_of_seats;

	int id = record.id;
	send(clifd, &id, sizeof(id), 0);

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

	ssize_t fd_write = write(fd, &record, sizeof(struct course));
	if (fd_write == -1)
	{
		perror("Error in writing course record");
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
		perror("Error while unlocking student record");
		exit(EXIT_FAILURE);
	}
	close(fd);
	return result;
}


// View Course
bool viewCourses(int clifd, struct course record)
{
	bool result;
	struct course curr_record;

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

	// to count no. of courses offered by faculty
	while ((byteread = read(fd, &curr_record, sizeof(struct course))) > 0)
	{
		if (curr_record.faculty_id == uid && strcmp(curr_record.status, "ACTIVE") == 0)
		{
			count++;
		}
	}

	offset = lseek(fd, 0, SEEK_SET);
	if (offset == -1)
	{
		perror("Error in lseek");
		close(fd);
		exit(EXIT_FAILURE);
	}

	send(clifd, &count, sizeof(count), 0);

	// Sending the records to client
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


// Remove Course
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
	result = removeAllEnroll(record.id);			// Remove Enrolled Students
	close(fd);

	return result;
}


// Remove All Enrolled students
bool removeAllEnroll(int cid)
{
	bool result;
	struct enrollment curr_record;
	ssize_t fd_read;

	int fd = open(ENROLL_DB, O_RDWR, 0777);
	if (fd == -1)
	{
		perror("Error in opening enroll db");
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
		perror("Error while locking course record");
		exit(EXIT_FAILURE);
	}

	while ((fd_read = read(fd, &curr_record, sizeof(struct enrollment))) > 0)
	{
		if (curr_record.courseid == cid)
		{
			strcpy(curr_record.status, "DROPPED");
			lseek(fd, (-1) * sizeof(struct enrollment), SEEK_CUR);
			write(fd, &curr_record, sizeof(struct enrollment));
			result = true;
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
	return result;
}


// Update Course
bool updateCourse(int clifd, struct course record)
{
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
	curr_record.credits = record.credits;

	if (curr_record.id == record.id && strcmp(curr_record.status, "ACTIVE") == 0)
	{
		int new_seats = curr_record.no_of_seats - record.no_of_seats;
		curr_record.no_of_seats = record.no_of_seats;
		curr_record.no_of_available_seats = curr_record.no_of_available_seats - new_seats;

		if (curr_record.no_of_available_seats < 0)
		{
			curr_record.no_of_available_seats = 0;
			result = dropLastStudents(record);				// Unenroll the students from last enrolled if seats is reduced
		}

		lseek(fd, (-1) * sizeof(struct course), SEEK_CUR);
		ssize_t fd_write = write(fd, &curr_record, sizeof(struct course));
		if (fd_write == -1)
		{
			perror("Error in updating the course");
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


// Drop Last Students
bool dropLastStudents(struct course record)
{
	struct enrollment curr_record;
	bool result;
	ssize_t fd_read;
	int seats = record.no_of_seats;

	int fd = open(ENROLL_DB, O_RDWR);
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
		perror("Error while locking enroll record");
		exit(EXIT_FAILURE);
	}

	if (seats == 0)
	{	
		// Unenrolling All students
		while ((fd_read = read(fd, &curr_record, sizeof(struct enrollment))) > 0)
		{
			if (curr_record.courseid == record.id)
			{
				strcpy(curr_record.status, "DROPPED");
				lseek(fd, (-1) * sizeof(struct enrollment), SEEK_CUR);
				write(fd, &curr_record, sizeof(struct enrollment));
				result = true;
			}
		}
	}
	else
	{	
		// reading records till seats become zero
		while ((fd_read = read(fd, &curr_record, sizeof(struct enrollment))) > 0)		
		{
			if (seats > 0 && curr_record.courseid == record.id && strcmp(curr_record.status, "ENROLLED") == 0)
			{
				seats--;
				if (seats == 0)
				{
					break;
				}
			}
		}

		// Now unenrolling last students
		while ((fd_read = read(fd, &curr_record, sizeof(struct enrollment))) > 0)
		{
			if (curr_record.courseid == record.id && strcmp(curr_record.status, "ENROLLED") == 0)
			{
				strcpy(curr_record.status, "DROPPED");

				lseek(fd, (-1) * sizeof(struct enrollment), SEEK_CUR);
				size_t fd_write = write(fd, &curr_record, sizeof(struct enrollment));
				if (fd_write == -1)
				{
					perror("Error in updating the course");
					result = false;
				}
				else
				{
					result = true;
				}
			}
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
	return result;
}


// Faculty Change password
bool facultyChangePass(int clifd, struct faculty record)
{
	struct faculty curr_record;
	bool result;
	int fd = open(FACULTY_DB, O_RDWR, 0777);
	if (fd == -1)
	{
		perror("Error in opening faculty_db");
		exit(EXIT_FAILURE);
	}

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


// View All Courses
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


// Enroll Course
bool enrollCourse(int clifd, struct enrollment record)
{
	bool result = false;
	ssize_t fd_read;
	struct enrollment curr_record;

	// checking if seats are available
	int cid = record.courseid;
	int seats = available_seats(cid);

	int msg;
	send(clifd, &seats, sizeof(seats), 0);

	// if seats is greater then 0 then only student can enroll
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
		
		while ((fd_read = read(fd, &curr_record, sizeof(struct enrollment))) > 0)
		{
			if (curr_record.studentid == uid && curr_record.courseid == record.courseid && strcmp(curr_record.status, "ENROLLED") == 0 )
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
	else
	{
		msg = 2;
		result = false;
		send(clifd, &msg, sizeof(msg), 0);
	}
	return result;
}


// Getting Available Seats in a course
int available_seats(int cid)
{
	struct course record;
	ssize_t fd_read;

	int fd = open(COURSE_DB, O_RDONLY, 0777);
	if (fd == -1)
	{
		perror("Error in opening course_db file");
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
		perror("Error locking file");
		close(fd);
		exit(EXIT_FAILURE);
	}

	while ((fd_read = read(fd, &record, sizeof(struct course))) > 0)
	{
		if (record.id == cid && strcmp(record.status,"ACTIVE") == 0)
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


// Reducing available Seats
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


// Drop Course
bool dropCourse(int clifd, struct enrollment record)
{
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
		if (curr_record.courseid == record.courseid && curr_record.studentid == uid && strcmp(curr_record.status, "ENROLLED") == 0)
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
			increaseSeats(record.courseid);		// Increasing no. available of seats
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

	return result;
}


// Increase available seats
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


// View Enrolled Courses
bool viewEnrolledCourses(int clifd, struct enrollment record)
{
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
		if (curr_record.studentid == uid && strcmp(curr_record.status, "ENROLLED") == 0)
		{
			count++;
		}
	}

	send(clifd, &count, sizeof(count), 0);

	lseek(fd, 0, SEEK_SET);
	int c = 0;
	while ((fd_read = read(fd, &curr_record, sizeof(struct enrollment))) > 0)
	{
		if (curr_record.studentid == uid && strcmp(curr_record.status, "ENROLLED") == 0)
		{
			struct course courseRecord;
			courseRecord.id = curr_record.courseid;
			coursesDetails(clifd, courseRecord);		// Getting Course Details to print
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


// Getting details of the Course
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


// Student Change Password
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