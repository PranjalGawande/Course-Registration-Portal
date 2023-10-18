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
#include "./functions/admin_functions.h"
#include "./functions/faculty_functions.h"
#include "./functions/student_functions.h"

#include "./functions/declarations.h"


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
