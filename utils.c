/*
 * utils.c
 * Helper functions for safe input and simple string handling.
 *
 * Syllabus links: Unit I formatted I/O, Unit II strings
 * (character arrays, manual string manipulation, library functions),
 * Unit II pointers (strings are passed as char pointers).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "hostel.h"

/* Print a prompt and read one line into buf (at most size-1 characters).
   The '\n' is removed by hand. Extra characters are discarded.
   Returns 1 on success, 0 if the input has ended (EOF). */
int readLine(const char *prompt, char *buf, int size)
{
    int i = 0;
    int c;

    printf("%s", prompt);
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return 0;
    }

    /* manual string manipulation: find and remove the newline */
    while (buf[i] != '\0' && buf[i] != '\n')
        i++;

    if (buf[i] == '\n') {
        buf[i] = '\0';
    } else {
        /* line was longer than the buffer: throw away the rest */
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }
    return 1;
}

/* Keep asking until the user types something that is not empty. */
int readNonEmpty(const char *prompt, char *buf, int size)
{
    do {
        if (!readLine(prompt, buf, size))
            return 0;
        if (buf[0] == '\0')
            printf("  This field cannot be empty.\n");
    } while (buf[0] == '\0');
    return 1;
}

/* Read a whole number between min and max. Repeats until valid.
   (If input ends, min is returned so the program can exit cleanly.) */
int readInt(const char *prompt, int min, int max)
{
    char  buf[32];
    char *end;
    long  value;

    for (;;) {
        if (!readLine(prompt, buf, (int)sizeof buf))
            return min;

        value = strtol(buf, &end, 10);
        if (end != buf && *end == '\0' && value >= min && value <= max)
            return (int)value;

        printf("  Invalid input. Enter a number from %d to %d.\n", min, max);
    }
}

/* Make "rAHUL sharma" into "Rahul Sharma" (manual, character by character). */
void capitalizeName(char *s)
{
    int startOfWord = 1;

    for (; *s != '\0'; s++) {
        if (*s == ' ') {
            startOfWord = 1;
        } else if (startOfWord) {
            *s = (char)toupper((unsigned char)*s);
            startOfWord = 0;
        } else {
            *s = (char)tolower((unsigned char)*s);
        }
    }
}

/* A valid phone number has exactly 10 digits. */
int isValidPhone(const char *s)
{
    int i;

    if (strlen(s) != 10)
        return 0;
    for (i = 0; i < 10; i++)
        if (!isdigit((unsigned char)s[i]))
            return 0;
    return 1;
}

/* Very simple e-mail check: has an '@' and a '.' after it. */
int isValidEmail(const char *s)
{
    const char *at = strchr(s, '@');

    if (at == NULL || at == s)
        return 0;
    return strchr(at, '.') != NULL;
}

/* Fill a Date structure with today's date (pointer to structure). */
void getToday(Date *d)
{
    time_t     now = time(NULL);
    struct tm *t   = localtime(&now);

    if (t == NULL) {
        d->day = 1; d->month = 1; d->year = 2026;
        return;
    }
    d->day   = t->tm_mday;
    d->month = t->tm_mon + 1;
    d->year  = t->tm_year + 1900;
}

/* Print a line of n copies of one character, e.g. ------- */
void printLine(char ch, int n)
{
    int i;

    for (i = 0; i < n; i++)
        putchar(ch);
    putchar('\n');
}
