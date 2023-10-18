#ifndef DECLARATION
#define DECLARATION

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

#endif