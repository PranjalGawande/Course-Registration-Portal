#ifndef ADMIN_FUNCS
#define ADMIN_FUNCS

#include "../structures/admin.h"
#include "../structures/faculty.h"
#include "../structures/student.h"
#include "../structures/course.h"
#include "../structures/enrollment.h"

#define DEFAULT_PASS "abc123"
#define STUDENT_DB "/home/pranjal-gawande/Software System/Course Registration portal/database/student_db"
#define FACULTY_DB "/home/pranjal-gawande/Software System/Course Registration portal/database/faculty_db"
#define COURSE_DB "/home/pranjal-gawande/Software System/Course Registration portal/database/course_db"
#define ENROLL_DB "/home/pranjal-gawande/Software System/Course Registration portal/database/enroll_db"


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

#endif