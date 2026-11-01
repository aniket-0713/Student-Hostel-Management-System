Name - Aniket Sharma

Class / Section - 217-A

UID - 26BAI10082

# Student Hostel Management System (C Language)

A console-based hostel room allotment system written in **C (C99)** using only the standard library. Student records are saved in a binary file, so allotment data **persists across sessions**.

This is the mini project from the C Programming (LTPS) syllabus. It uses structures, arrays, functions and file handling, and also covers pointers, dynamic memory, recursion, unions, macros and command-line arguments.

---

## 1. Features

| # | Menu option | What it does |
|---|-------------|--------------|
| 1 | Add student | Stores name, roll number, course and contact (phone or email). Rejects duplicate roll numbers and invalid phone/email. |
| 2 | Check room availability | Shows capacity, occupied beds, free beds and status of every room, plus overall occupancy %. |
| 3 | Allot room | Gives a student a room. Blocks full rooms and students who already have a room. |
| 4 | Vacate room | Frees the student's room. |
| 5 | Display all current occupants | Table of everyone who has a room, with total count. |
| 6 | Search student | Finds a student by roll number. |
| 7 | Export report | Writes the occupants to a text file (`occupants_report.txt`). |
| 0 | Save and exit | Saves data and quits. |

Data is also saved automatically after every change.

**Hostel setup:** 10 rooms (101 to 110), 2 students per room. Change `MAX_ROOMS` and `ROOM_CAPACITY` in `hostel.h` to resize.

---

## 2. Syllabus Coverage

| Unit | Topic | Where it is used |
|------|-------|------------------|
| I | Structure of a C program, header files | All files, `main.c` |
| I | Formatted input/output | `printf` tables everywhere, `fgets`/`strtol` in `utils.c` |
| I | Decision making: `if`, `switch-case`, ternary `?:` | Menu in `main.c`; "Available/Full" in `hostel.c` |
| I | Loops: `for`, `while`, `do-while`, `break`/`continue` | Menu loop (`do-while`), searching and displaying (`for`, `continue`) |
| I | Typecasting | `(float)totalOccupied / totalBeds * 100` in `checkAvailability` |
| II | 1-D arrays | `Room rooms[MAX_ROOMS]`, character arrays for name and course |
| II | Strings, manual string handling, library functions | `capitalizeName`, newline removal in `utils.c`; `strcpy`, `strlen`, `strchr`, `memset` |
| II | Pointers, pointer-to-pointer | `findStudent` returns a pointer; `Student **list` in `addStudent` and `loadStudents` |
| II | `malloc`, `realloc`, `free`, NULL pointer | Growing student array in `addStudent`; `free` in `main.c` |
| III | Functions, parameter passing | Call by value (`readInt`) and by pointer (`allotRoom`, `getToday`) |
| III | Recursion | `countAllotted` in `hostel.c` |
| III | Structures, array of structures, nested structures | `Student`, `Room`, `Date` inside `Student` |
| III | Unions | `Contact` (phone or email) in `hostel.h` |
| III | File handling: text and binary | Binary: `fread`/`fwrite` in `storage.c`. Text: `fprintf` report |
| III | Command-line arguments | `./hostel myfile.dat` in `main.c` |
| III | Preprocessor directives and macros | `#define`, `#ifndef` header guard in `hostel.h` |

---

## 3. Project Structure

```
Student-Hostel-Management-System/
|-- hostel.h      Macros, structures, union, function prototypes
|-- main.c        Entry point, menu, load and save
|-- hostel.c      Core features (add, allot, vacate, display, search)
|-- storage.c     File handling (binary save/load, text report)
|-- utils.c       Safe input, validation, string helpers
|-- Makefile      Build script
|-- README.md     This file
`-- .gitignore    Keeps compiled files and data files out of Git
```

### Data structures

```c
typedef struct { int day, month, year; } Date;

typedef union {              /* phone and email share the same memory */
    char phone[11];
    char email[40];
} Contact;

typedef struct {
    int     rollNo;
    char    name[50];
    char    course[30];
    int     roomNo;          /* 0 = no room allotted */
    Date    admissionDate;   /* nested structure     */
    int     contactType;     /* 1 = phone, 2 = email */
    Contact contact;         /* union                */
} Student;

typedef struct { int roomNo, capacity, occupied; } Room;
```

### How persistence works

- Students are kept in a dynamic array (`malloc`/`realloc`).
- `saveStudents` writes the count and then all records to `hostel.dat` with `fwrite`.
- `loadStudents` reads them back with `fread` when the program starts.
- Room occupancy is **not** stored. It is recalculated from each student's `roomNo` when the program starts (`rebuildRooms`), so rooms and students can never disagree.

---

## 4. Compile and Run

### GCC (Linux / macOS / Windows with MinGW)

```
make
./hostel                 # Windows: hostel.exe
```

Or without `make`:

```
gcc -std=c99 -Wall -Wextra -pedantic -o hostel main.c hostel.c storage.c utils.c
./hostel
```

Use a different data file:

```
./hostel mydata.dat
```

### Code::Blocks

1. File > New > Project > **Console application** > **C**.
2. Remove the default `main.c`.
3. Project > **Add files** and select all `.c` files and `hostel.h`.
4. Build and run (F9).

The program compiles with **no errors and no warnings** using `-Wall -Wextra -pedantic`.

---

## 5. Sample Output

### Menu

```
============================================
      STUDENT HOSTEL MANAGEMENT SYSTEM
============================================
  1. Add student
  2. Check room availability
  3. Allot room
  4. Vacate room
  5. Display all current occupants
  6. Search student by roll number
  7. Export occupants report (text file)
  0. Save and exit
--------------------------------------------
```

### Validation and allotment

```
Phone (10 digits):   Phone must be exactly 10 digits.
Phone (10 digits):   Student Rahul Sharma (Roll 101) added. Use option 3 to allot a room.
  A student with this roll number already exists.
  Room 101 allotted to Rahul Sharma.
  Room 101 allotted to Simran Kaur.
  Room 101 is full. Choose another room.
  Rahul Sharma already has room 101. Vacate it first.
```

### Current occupants

```
Roll No    Name                 Course         Room   Contact                  Admitted
----------------------------------------------------------------------------------------
101        Rahul Sharma         b.tech cse     101    9876543210               01-11-2026
102        Simran Kaur          bca            101    simran@cu.in             01-11-2026
103        Amit Verma           mba            102    9812345678               01-11-2026
----------------------------------------------------------------------------------------
Total occupants: 3
Registered students waiting for a room: 0
```

---

## 6. Test Cases

| # | Test | Steps | Expected result |
|---|------|-------|-----------------|
| 1 | Duplicate roll number | Add roll 101 twice | Second attempt rejected |
| 2 | Invalid phone | Enter `12345` | Error, asks again |
| 3 | Invalid email | Enter `abc` | Error, asks again |
| 4 | Unknown student | Allot room to roll 999 | "No student found" |
| 5 | Room full | Allot 3 students to room 101 | Third is rejected |
| 6 | Double allotment | Allot a room to a student who has one | Blocked with message |
| 7 | Vacate without room | Vacate a student with no room | Blocked with message |
| 8 | Bad menu input | Type `abc` or `99` | Error, asks again |
| 9 | Persistence | Add data, exit, run again | Data is still there |
| 10 | Corrupt data file | Put text in `hostel.dat`, run | Warning, program starts empty |
| 11 | Command-line file | `./hostel other.dat` | Uses `other.dat` |

Memory check (optional):

```
gcc -std=c99 -g -fsanitize=address,undefined -o hostel_san *.c
./hostel_san
```

---

## 7. Limitations

- Each student has exactly one room; rooms have a fixed capacity.
- The binary data file is not portable between different compilers or operating systems.
- Searching is by roll number only and scans the array in order (fine for hostel-sized data).
- No login or password.

## 8. Future Scope

- Search by name, course or room.
- Separate rooms by block or floor, and by room type (single, double).
- Fee tracking and payment status.
- Sort occupants by room or roll number.
- Replace the binary file with a database such as SQLite.
- Add a login for the hostel warden.

## 9. Conclusion

The project shows how structures, arrays, pointers, dynamic memory, functions, recursion and file handling work together in a complete C program. It is split into small modules, validates all input, and keeps its data safe between runs.
