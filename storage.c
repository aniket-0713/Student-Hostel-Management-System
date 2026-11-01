/*
 * storage.c
 * Saving and loading data so it persists between runs.
 *
 * Syllabus links: Unit III file handling (fopen, fread, fwrite,
 * fprintf, fclose, text and binary files), Unit II dynamic memory
 * (realloc) and pointer-to-pointer (Student **list).
 */
#include <stdio.h>
#include <stdlib.h>
#include "hostel.h"

/* Load all students from a BINARY file.
   File layout:  [int count][Student][Student]...
   list, count, capacity are changed through pointers, so the caller
   sees the new values.  Student **list is a pointer to a pointer,
   because realloc may move the array to a new address.
   Returns 1 on success (a missing file is fine: first run), 0 on error. */
int loadStudents(const char *filename, Student **list, int *count, int *capacity)
{
    FILE    *fp = fopen(filename, "rb");
    int      n  = 0;
    Student *bigger;

    if (fp == NULL)
        return 1;                       /* no file yet = no students yet */

    if (fread(&n, sizeof(int), 1, fp) != 1 || n < 0 || n > MAX_STUDENTS) {
        fclose(fp);
        return 0;
    }

    if (n > *capacity) {                /* make room for n records */
        bigger = realloc(*list, (size_t)n * sizeof(Student));
        if (bigger == NULL) {
            fclose(fp);
            return 0;
        }
        *list     = bigger;
        *capacity = n;
    }

    if (n > 0 && fread(*list, sizeof(Student), (size_t)n, fp) != (size_t)n) {
        fclose(fp);
        return 0;
    }

    *count = n;
    fclose(fp);
    return 1;
}

/* Save all students to a BINARY file (overwrites the old file). */
int saveStudents(const char *filename, const Student *list, int count)
{
    FILE *fp = fopen(filename, "wb");

    if (fp == NULL)
        return 0;

    if (fwrite(&count, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }
    if (count > 0 && fwrite(list, sizeof(Student), (size_t)count, fp) != (size_t)count) {
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return 1;
}

/* Write a readable report of current occupants to a TEXT file. */
int exportReport(const char *filename, const Student *list, int count)
{
    FILE *fp = fopen(filename, "w");
    int   i;
    int   written = 0;

    if (fp == NULL)
        return 0;

    fprintf(fp, "HOSTEL OCCUPANTS REPORT\n");
    fprintf(fp, "=======================\n");
    fprintf(fp, "%-10s %-20s %-15s %-5s\n", "Roll No", "Name", "Course", "Room");

    for (i = 0; i < count; i++) {
        if (list[i].roomNo != NOT_ALLOTTED) {
            fprintf(fp, "%-10d %-20.20s %-15.15s %-5d\n",
                    list[i].rollNo, list[i].name,
                    list[i].course, list[i].roomNo);
            written++;
        }
    }
    fprintf(fp, "\nTotal occupants: %d\n", written);

    fclose(fp);
    return 1;
}
