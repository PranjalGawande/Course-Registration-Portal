# Course Registration Portal 🎓


<br>

⚫ <b><i>Project Goal:</i></b>
<br><br>To develop a course registration system which is user-friendly and multi-functional. <p>

This project mainly deals with the concurrent operations in a safe and secure environment.
- Safe: Concurrency handling.
- Secure: Only authorized faculty and students can access and modify the data.

<br>

⚫ <b><i>Features:</i></b>
- The application can handle concurrent access and operations.
- Socket programming is used to implement client-server model.
- Storage/ Database: Unix File System. i.e., data is stored typically in files only.
- Password protected login system for both admins and customers.

<br>

⚫ <b><i>How to execute?:</i></b>
- Firstly add a new Admin using a file called "initialize_admin.c"
- Change the absolute path mentioned in the code.
- Now compile and run the Server:
```
cc server.c -o server.out
./server.out
```
- Compile and run the Client:
```
cc client.c -o client.out
./client.out
```
- Done!

<br>

⚫ <b><i>Output:</i></b>

<br>

<b>Starting Application -</b>
- Start Server
# ![InitClient](./Outputs/01.StartServer.jpg)
- Start Client
# ![InitClient](./Outputs/02.StartClient.jpg)

<br>

<b>Admin Functions -</b>
- Add student:
# ![InitClient](./Outputs/03.AddStudent.jpg)
- View student details:
# ![InitClient](./Outputs/04.ViewStudent.jpg)
- Block student:
# ![InitClient](./Outputs/05.BlockStudent.jpg)
- Activate student:
# ![InitClient](./Outputs/06.ActivateStudent.jpg)
- Blocked Student Details:
# ![InitClient](./Outputs/07.AfterBlock.jpg)
- Modify Student Details:
# ![InitClient](./Outputs/08.UpdateStudent.jpg)

<br>

<b>Faculty Functions -<b>
- Add a new course:
# ![InitClient](./Outputs/09.AddCourse.jpg)
- View offered courses:
# ![InitClient](./Outputs/10.ViewOfferedCourse.jpg)
- Modify course details:
# ![InitClient](./Outputs/11.ModifyCourse.jpg)

<br>

<b>Student Functions -</b>
- View all courses:
# ![InitClient](./Outputs/12.ViewAllCourses.jpg)
- Enroll in a course:
# ![InitClient](./Outputs/13.Enroll.jpg)
- View Enrolled Courses:
# ![InitClient](./Outputs/14.AfterEnroll.jpg)
- Unenroll from a course:
# ![InitClient](./Outputs/15.Unenroll.jpg)
