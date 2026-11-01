/*
 * hostel.c
 * The main features: add student, check availability, allot room,
 * vacate room, display occupants, search.
 *
 * Syllabus links:
 *  Unit I   - decisions (if, ternary ?:), loops, typecasting, formatted output
 *  Unit II  - arrays (rooms[]), strings, pointers, malloc/realloc
 *  Unit III - functions, parameter passing (by value and by pointer),
 *             recursion (countAllotted), structures, nested structures, union
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hostel.h"

/* ---------------------------------------------------------------
   Rooms
   --------------------------------------------------------------- */

/* Set up the rooms array: 101, 102, ... each empty.
   An array is passed to a function as a pointer, so changes made
   here are visible in main(). */
void initRooms(Room rooms[])
{
    int i;

    for (i = 0; i < MAX_ROOMS; i++) {
        rooms[i].roomNo   = FIRST_ROOM + i;
        rooms[i].capacity = ROOM_CAPACITY;
        rooms[i].occupied = 0;
    }
}

/* Room occupancy is not stored separately. It is recalculated from the
   students' roomNo values, so the two can never disagree. */
void rebuildRooms(Room rooms[], const Student *list, int count)
{
    int i, index;

    initRooms(rooms);
    for (i = 0; i < count; i++) {
        index = list[i].roomNo - FIRST_ROOM;
        if (list[i].roomNo != NOT_ALLOTTED && index >= 0 && index < MAX_ROOMS)
            rooms[index].occupied++;
    }
}

/* ---------------------------------------------------------------
   Helpers
   --------------------------------------------------------------- */

/* Linear search by roll number. Returns a pointer to the record,
   or NULL if there is no such student. */
Student *findStudent(Student *list, int count, int rollNo)
{
    int i;

    for (i = 0; i < count; i++)
        if (list[i].rollNo == rollNo)
            return &list[i];
    return NULL;
}

/* RECURSION: count students who have a room.
   Base case: no students left (n == 0) -> 0.
   Recursive case: look at the last student, then solve for n-1. */
int countAllotted(const Student *list, int n)
{
    if (n == 0)
        return 0;
    return (list[n - 1].roomNo != NOT_ALLOTTED) + countAllotted(list, n - 1);
}

/* Copy the student's contact (phone or email) into out.
   The union holds only one of them; contactType says which. */
static void formatContact(const Student *s, char *out, int size)
{
    if (s->contactType == 1)
        snprintf(out, (size_t)size, "%s", s->contact.phone);
    else if (s->contactType == 2)
        snprintf(out, (size_t)size, "%s", s->contact.email);
    else
        snprintf(out, (size_t)size, "-");
}

/* ---------------------------------------------------------------
   1. Add student
   --------------------------------------------------------------- */

/* list is a pointer to the array pointer, because realloc may move the
   array. Returns 1 if a student was added, 0 otherwise. */
int addStudent(Student **list, int *count, int *capacity)
{
    Student  s;
    Student *bigger;
    char     tmp[EMAIL_LEN];
    int      roll, newCap, ok;

    printf("\n--- Add Student ---\n");

    if (*count >= MAX_STUDENTS) {
        printf("  Student limit reached.\n");
        return 0;
    }

    roll = readInt("Roll number: ", 1, 99999999);
    if (findStudent(*list, *count, roll) != NULL) {
        printf("  A student with this roll number already exists.\n");
        return 0;
    }

    memset(&s, 0, sizeof s);            /* clear the whole structure */
    s.rollNo = roll;

    if (!readNonEmpty("Name: ", s.name, NAME_LEN))
        return 0;
    capitalizeName(s.name);

    if (!readNonEmpty("Course: ", s.course, COURSE_LEN))
        return 0;

    s.contactType = readInt("Contact type (1 = Phone, 2 = Email): ", 1, 2);
    if (s.contactType == 1) {
        do {
            if (!readLine("Phone (10 digits): ", tmp, (int)sizeof tmp))
                return 0;
            ok = isValidPhone(tmp);
            if (!ok)
                printf("  Phone must be exactly 10 digits.\n");
        } while (!ok);
        strcpy(s.contact.phone, tmp);
    } else {
        do {
            if (!readLine("Email: ", tmp, (int)sizeof tmp))
                return 0;
            ok = isValidEmail(tmp);
            if (!ok)
                printf("  Enter a valid email, e.g. name@example.com\n");
        } while (!ok);
        strcpy(s.contact.email, tmp);
    }

    getToday(&s.admissionDate);         /* fill nested structure */
    s.roomNo = NOT_ALLOTTED;

    /* DYNAMIC MEMORY: grow the array when it is full (double the size). */
    if (*count == *capacity) {
        newCap = (*capacity == 0) ? 4 : *capacity * 2;
        bigger = realloc(*list, (size_t)newCap * sizeof(Student));
        if (bigger == NULL) {
            printf("  Out of memory. Student not added.\n");
            return 0;
        }
        *list     = bigger;
        *capacity = newCap;
    }

    (*list)[*count] = s;                /* structure assignment */
    (*count)++;

    printf("  Student %s (Roll %d) added. Use option 3 to allot a room.\n",
           s.name, s.rollNo);
    return 1;
}

/* ---------------------------------------------------------------
   2. Check room availability
   --------------------------------------------------------------- */
void checkAvailability(const Room rooms[])
{
    int   i, available;
    int   totalBeds = 0, totalOccupied = 0;
    float percent;

    printf("\n--- Room Availability ---\n");
    printf("%-8s %-10s %-10s %-10s %s\n",
           "Room", "Capacity", "Occupied", "Free beds", "Status");
    printLine('-', 52);

    for (i = 0; i < MAX_ROOMS; i++) {
        available = rooms[i].capacity - rooms[i].occupied;
        printf("%-8d %-10d %-10d %-10d %s\n",
               rooms[i].roomNo, rooms[i].capacity, rooms[i].occupied,
               available,
               (available > 0) ? "Available" : "Full");   /* ternary operator */
        totalBeds     += rooms[i].capacity;
        totalOccupied += rooms[i].occupied;
    }
    printLine('-', 52);

    /* TYPECASTING: without (float), integer division would give 0 or 1. */
    percent = (float)totalOccupied / totalBeds * 100;
    printf("Beds used: %d of %d (%.1f%% full)\n", totalOccupied, totalBeds, percent);
}

/* ---------------------------------------------------------------
   3. Allot room
   --------------------------------------------------------------- */
int allotRoom(Student *list, int count, Room rooms[])
{
    Student *s;
    Room    *r;
    int      roll, roomNo;

    printf("\n--- Allot Room ---\n");
    roll = readInt("Roll number of student: ", 1, 99999999);

    s = findStudent(list, count, roll);
    if (s == NULL) {
        printf("  No student found with roll number %d.\n", roll);
        return 0;
    }
    if (s->roomNo != NOT_ALLOTTED) {
        printf("  %s already has room %d. Vacate it first.\n", s->name, s->roomNo);
        return 0;
    }

    checkAvailability(rooms);
    roomNo = readInt("Room number to allot: ", FIRST_ROOM, LAST_ROOM);

    r = &rooms[roomNo - FIRST_ROOM];    /* pointer to one array element */
    if (r->occupied >= r->capacity) {
        printf("  Room %d is full. Choose another room.\n", roomNo);
        return 0;
    }

    s->roomNo = roomNo;
    r->occupied++;
    printf("  Room %d allotted to %s.\n", roomNo, s->name);
    return 1;
}

/* ---------------------------------------------------------------
   4. Vacate room
   --------------------------------------------------------------- */
int vacateRoom(Student *list, int count, Room rooms[])
{
    Student *s;
    int      roll, oldRoom;

    printf("\n--- Vacate Room ---\n");
    roll = readInt("Roll number of student: ", 1, 99999999);

    s = findStudent(list, count, roll);
    if (s == NULL) {
        printf("  No student found with roll number %d.\n", roll);
        return 0;
    }
    if (s->roomNo == NOT_ALLOTTED) {
        printf("  %s does not have a room.\n", s->name);
        return 0;
    }

    oldRoom = s->roomNo;
    rooms[oldRoom - FIRST_ROOM].occupied--;
    s->roomNo = NOT_ALLOTTED;
    printf("  Room %d vacated by %s.\n", oldRoom, s->name);
    return 1;
}

/* ---------------------------------------------------------------
   5. Display all current occupants
   --------------------------------------------------------------- */
void displayOccupants(const Student *list, int count)
{
    int  i, shown = 0;
    char contact[EMAIL_LEN];

    printf("\n--- Current Occupants ---\n");
    printf("%-10s %-20s %-14s %-6s %-24s %s\n",
           "Roll No", "Name", "Course", "Room", "Contact", "Admitted");
    printLine('-', 88);

    for (i = 0; i < count; i++) {
        if (list[i].roomNo == NOT_ALLOTTED)
            continue;                    /* skip students without a room */

        formatContact(&list[i], contact, (int)sizeof contact);
        printf("%-10d %-20.20s %-14.14s %-6d %-24.24s %02d-%02d-%04d\n",
               list[i].rollNo, list[i].name, list[i].course,
               list[i].roomNo, contact,
               list[i].admissionDate.day, list[i].admissionDate.month,
               list[i].admissionDate.year);
        shown++;
    }

    if (shown == 0)
        printf("No students are currently allotted a room.\n");
    printLine('-', 88);

    /* shown (loop) and countAllotted (recursion) must give the same answer */
    printf("Total occupants: %d\n", countAllotted(list, count));
    printf("Registered students waiting for a room: %d\n", count - shown);
}

/* ---------------------------------------------------------------
   6. Search a student by roll number
   --------------------------------------------------------------- */
void searchStudent(Student *list, int count)
{
    Student *s;
    char     contact[EMAIL_LEN];
    int      roll;

    printf("\n--- Search Student ---\n");
    roll = readInt("Roll number: ", 1, 99999999);

    s = findStudent(list, count, roll);
    if (s == NULL) {
        printf("  No student found with roll number %d.\n", roll);
        return;
    }

    formatContact(s, contact, (int)sizeof contact);
    printf("\n  Roll number : %d\n", s->rollNo);
    printf("  Name        : %s\n", s->name);
    printf("  Course      : %s\n", s->course);
    printf("  %-12s: %s\n", (s->contactType == 1) ? "Phone" : "Email", contact);
    printf("  Admitted on : %02d-%02d-%04d\n",
           s->admissionDate.day, s->admissionDate.month, s->admissionDate.year);
    if (s->roomNo == NOT_ALLOTTED)
        printf("  Room        : not allotted\n");
    else
        printf("  Room        : %d\n", s->roomNo);
}
