/*
 * hostel.h
 * Shared macros, data types and function prototypes for the
 * Student Hostel Management System.
 *
 * Syllabus links: Unit III - preprocessor directives, macros,
 * structures, nested structures, unions.
 */
#ifndef HOSTEL_H          /* header guard: stops double inclusion */
#define HOSTEL_H

/* ---------------- Macros (symbolic constants) ---------------- */
#define MAX_ROOMS      10                         /* rooms 101 to 110          */
#define ROOM_CAPACITY  2                          /* students per room         */
#define FIRST_ROOM     101
#define LAST_ROOM      (FIRST_ROOM + MAX_ROOMS - 1)
#define NOT_ALLOTTED   0                          /* roomNo value = no room    */
#define NAME_LEN       50
#define COURSE_LEN     30
#define EMAIL_LEN      40
#define PHONE_LEN      11                         /* 10 digits + '\0'          */
#define MAX_STUDENTS   10000
#define DEFAULT_FILE   "hostel.dat"
#define REPORT_FILE    "occupants_report.txt"

/* ---------------- Data types ---------------- */

/* Simple structure used inside Student (nested structure) */
typedef struct {
    int day;
    int month;
    int year;
} Date;

/* Union: phone and email share the SAME memory.
   Only one of them is valid at a time (see contactType in Student). */
typedef union {
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
} Contact;

/* One student record */
typedef struct {
    int     rollNo;
    char    name[NAME_LEN];
    char    course[COURSE_LEN];
    int     roomNo;            /* NOT_ALLOTTED (0) if no room yet       */
    Date    admissionDate;     /* nested structure                      */
    int     contactType;       /* 1 = phone, 2 = email (tells the union) */
    Contact contact;           /* union                                 */
} Student;

/* One hostel room */
typedef struct {
    int roomNo;
    int capacity;
    int occupied;
} Room;

/* ---------------- utils.c : input helpers ---------------- */
int  readLine(const char *prompt, char *buf, int size);
int  readNonEmpty(const char *prompt, char *buf, int size);
int  readInt(const char *prompt, int min, int max);
void capitalizeName(char *s);
int  isValidPhone(const char *s);
int  isValidEmail(const char *s);
void getToday(Date *d);
void printLine(char ch, int n);

/* ---------------- storage.c : file handling ---------------- */
int loadStudents(const char *filename, Student **list, int *count, int *capacity);
int saveStudents(const char *filename, const Student *list, int count);
int exportReport(const char *filename, const Student *list, int count);

/* ---------------- hostel.c : main features ---------------- */
void     initRooms(Room rooms[]);
void     rebuildRooms(Room rooms[], const Student *list, int count);
Student *findStudent(Student *list, int count, int rollNo);
int      countAllotted(const Student *list, int n);       /* recursive */
int      addStudent(Student **list, int *count, int *capacity);
void     checkAvailability(const Room rooms[]);
int      allotRoom(Student *list, int count, Room rooms[]);
int      vacateRoom(Student *list, int count, Room rooms[]);
void     displayOccupants(const Student *list, int count);
void     searchStudent(Student *list, int count);

#endif /* HOSTEL_H */
