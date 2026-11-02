# Student Hostel Management System (C)

A simple console program, written in C, to manage hostel room allotment. It keeps a list of students, shows which rooms are free, gives rooms to students, and saves everything to a file so the data is still there the next time you run it.

The whole program is in one file (`hostel.c`, about 120 lines) and uses only the C standard library.

---

## Features

| Option | What it does |
|--------|--------------|
| 1. Add student | Saves a student's roll number and name. Rejects duplicate or invalid roll numbers. |
| 2. Room availability | Shows every room, how many beds are filled, and whether it is Available or Full. |
| 3. Allot room | Gives a room to a student. Blocks full rooms, unknown rooms, and students who already have one. |
| 4. Vacate room | Frees a student's room. |
| 5. Show all students | Lists every student with their room (`0` means no room yet). |
| 0. Exit | Quits the program. |

The data is saved to `hostel.dat` after every change and loaded again when the program starts.

**Hostel setup:** 10 rooms (101 to 110) with 2 beds each. You can change this at the top of `hostel.c`:

```c
#define ROOMS 10     // number of rooms
#define FIRST 101    // first room number
#define BEDS 2       // students per room
```

---

## How to Run

You need a C compiler such as GCC.

```bash
gcc hostel.c -o hostel
./hostel
```

On Windows, run `hostel.exe` instead of `./hostel`.

**Using Code::Blocks:** create a new Console application (C), replace the code in `main.c` with the contents of `hostel.c`, and press F9 to build and run.

---

## Sample Output

```
1. Add student
2. Room availability
3. Allot room
4. Vacate room
5. Show all students
0. Exit
Choice: 2

Room  Filled  Status
101   1       Available
102   0       Available
103   0       Available
...
110   0       Available
```

```
Roll    Name                 Room (0 = none)
101     Rahul Sharma         101
102     Simran Kaur          101
Total students: 2
```

---

## How It Works

**Data**

```c
struct Student {
    int  roll;
    char name[50];
    int  room;      // 0 means no room yet
};

struct Student s[MAX];   // array of students
int total = 0;           // how many students are stored
```

**Functions**

| Function | Purpose |
|----------|---------|
| `readNumber` | Reads a number safely. If the user types letters, it asks again instead of crashing. |
| `find` | Finds a student by roll number and returns their position, or -1 if not found. |
| `inRoom` | Counts how many students are in a given room. |
| `addStudent` | Adds a new student. |
| `showRooms` | Prints the room availability table. |
| `allotRoom` | Gives a room to a student. |
| `vacateRoom` | Removes a student from their room. |
| `showStudents` | Prints all students. |
| `load` and `save` | Read and write the data file. |
| `main` | Shows the menu in a loop and calls the right function. |

Room occupancy is not stored separately. `inRoom` counts it from the students' room numbers each time, so the room data and the student data cannot get out of sync.

---

## C Concepts Used

- Structures and an array of structures
- Functions and return values
- `if`/`else`, `switch`-case, and the conditional operator (`? :`)
- Loops: `for`, `while`, and `do-while`
- Macros with `#define`
- Formatted input and output with `printf` and `scanf`
- Binary file handling with `fopen`, `fread`, `fwrite`, and `fclose`

---

## Project Files

```
.
|-- hostel.c      the whole program
|-- README.md     this file
`-- .gitignore    keeps the compiled program and data file out of Git
```

Suggested `.gitignore`:

```
hostel
hostel.exe
hostel.dat
```

---

## Limitations

- The list holds up to 100 students.
- Each student can have only one room, and every room has the same number of beds.
- Students are found by roll number only.
- Student records cannot be deleted or edited once added.
- The data file may not work on a different computer or compiler, because it is a raw binary copy of the structures.

## Future Improvements

- Store phone number, course and admission date
- Search students by name
- Edit and delete student records
- Export a text report of occupants
- Use dynamic memory so the list can grow without a fixed limit

---

## Author

Aniket Sharma
217-A | 26BAI10082
