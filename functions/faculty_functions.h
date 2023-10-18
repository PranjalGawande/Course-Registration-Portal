#ifndef FACULTY_FUNCS
#define FACULTY_FUNCS

#include "../structures/admin.h"
#include "../structures/faculty.h"
#include "../structures/student.h"
#include "../structures/course.h"
#include "../structures/enrollment.h"
#include "./declarations.h"


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

#endif