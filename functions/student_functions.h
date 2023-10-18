#ifndef STUDENT_FUNCS
#define STUDENT_FUNCS

#include "../structures/admin.h"
#include "../structures/faculty.h"
#include "../structures/student.h"
#include "../structures/course.h"
#include "../structures/enrollment.h"
#include "./declarations.h"


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

#endif