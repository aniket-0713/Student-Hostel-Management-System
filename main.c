/*
 * main.c
 * Entry point: loads saved data, shows the menu, saves after changes.
 *
 * Syllabus links: Unit I (structure of a C program, do-while loop,
 * switch-case, formatted I/O), Unit III (command-line arguments,
 * functions), Unit II (malloc/free, pointers).
 *
 * Usage:  ./hostel               uses the default file hostel.dat
 *         ./hostel mydata.dat    uses a file name given on the command line
 */
#include <stdio.h>
#include <stdlib.h>
#include "hostel.h"

static void showMenu(void)
{
    printf("\n");
    printLine('=', 44);
    printf("      STUDENT HOSTEL MANAGEMENT SYSTEM\n");
    printLine('=', 44);
    printf("  1. Add student\n");
    printf("  2. Check room availability\n");
    printf("  3. Allot room\n");
    printf("  4. Vacate room\n");
    printf("  5. Display all current occupants\n");
    printf("  6. Search student by roll number\n");
    printf("  7. Export occupants report (text file)\n");
    printf("  0. Save and exit\n");
    printLine('-', 44);
}

int main(int argc, char *argv[])
{
    /* COMMAND-LINE ARGUMENT: argv[1], if given, is the data file name. */
    const char *filename = (argc > 1) ? argv[1] : DEFAULT_FILE;

    Student *students = NULL;           /* dynamic array, starts empty */
    int      count    = 0;              /* students stored             */
    int      capacity = 0;              /* space allocated             */
    Room     rooms[MAX_ROOMS];          /* ordinary array of structures */
    int      choice;
    int      changed;

    initRooms(rooms);

    if (!loadStudents(filename, &students, &count, &capacity))
        printf("Warning: could not read '%s'. Starting with an empty list.\n"
               "         The file will be overwritten when you save.\n", filename);
    else if (count > 0)
        printf("Loaded %d student(s) from '%s'.\n", count, filename);

    rebuildRooms(rooms, students, count);

    do {
        showMenu();
        choice  = readInt("Enter your choice (0-7): ", 0, 7);
        changed = 0;

        switch (choice) {
        case 1:  changed = addStudent(&students, &count, &capacity);       break;
        case 2:  checkAvailability(rooms);                                 break;
        case 3:  changed = allotRoom(students, count, rooms);              break;
        case 4:  changed = vacateRoom(students, count, rooms);             break;
        case 5:  displayOccupants(students, count);                        break;
        case 6:  searchStudent(students, count);                           break;
        case 7:
            if (exportReport(REPORT_FILE, students, count))
                printf("  Report written to %s\n", REPORT_FILE);
            else
                printf("  Could not write the report file.\n");
            break;
        case 0:
            printf("  Saving data. Goodbye!\n");
            changed = 1;
            break;
        }

        /* Save after every change so nothing is lost if the program closes. */
        if (changed && !saveStudents(filename, students, count))
            printf("  ERROR: could not save to '%s'.\n", filename);

    } while (choice != 0);

    free(students);                     /* give the memory back */
    students = NULL;                    /* avoid a dangling pointer */
    return 0;
}
