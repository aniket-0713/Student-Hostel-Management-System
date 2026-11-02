#include <stdio.h>
#define MAX 100
#define ROOMS 10
#define FIRST 101
#define BEDS 2
struct Student { int roll; char name[50]; int room; };
struct Student s[MAX];
int total = 0;
int readNumber(void) {
    int n, c;
    while (scanf("%d", &n) != 1) {
        if (feof(stdin)) return 0;
        printf("Enter a number: ");
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    return n;
}
void load(void) {
    FILE *fp = fopen("hostel.dat", "rb");
    if (fp == NULL) return;
    if (fread(&total, sizeof(int), 1, fp) != 1 || total < 0 || total > MAX) total = 0;
    fread(s, sizeof(struct Student), total, fp);
    fclose(fp);
}
void save(void) {
    FILE *fp = fopen("hostel.dat", "wb");
    if (fp == NULL) return;
    fwrite(&total, sizeof(int), 1, fp);
    fwrite(s, sizeof(struct Student), total, fp);
    fclose(fp);
}
int find(int roll) {
    int i;
    for (i = 0; i < total; i++)
        if (s[i].roll == roll) return i;
    return -1;
}
int inRoom(int room) {
    int i, count = 0;
    for (i = 0; i < total; i++)
        if (s[i].room == room) count++;
    return count;
}
void addStudent(void) {
    int roll;
    printf("Roll number: ");
    roll = readNumber();
    if (total >= MAX || roll <= 0 || find(roll) != -1) {
        printf("Cannot add: list full, or invalid/duplicate roll number.\n");
        return;
    }
    s[total].roll = roll;
    printf("Name: ");
    scanf(" %49[^\n]", s[total].name);
    s[total].room = 0;
    total++;
    save();
    printf("Student added.\n");
}
void showRooms(void) {
    int r, filled;
    printf("\nRoom  Filled  Status\n");
    for (r = FIRST; r < FIRST + ROOMS; r++) {
        filled = inRoom(r);
        printf("%-5d %-7d %s\n", r, filled, filled < BEDS ? "Available" : "Full");
    }
}
void allotRoom(void) {
    int i, room;
    printf("Roll number: ");
    i = find(readNumber());
    if (i == -1) { printf("Student not found.\n"); return; }
    if (s[i].room != 0) { printf("Already has room %d.\n", s[i].room); return; }
    showRooms();
    printf("Room number: ");
    room = readNumber();
    if (room < FIRST || room >= FIRST + ROOMS) { printf("No such room.\n"); return; }
    if (inRoom(room) >= BEDS) { printf("Room is full.\n"); return; }
    s[i].room = room;
    save();
    printf("Room %d given to %s.\n", room, s[i].name);
}
void vacateRoom(void) {
    int i;
    printf("Roll number: ");
    i = find(readNumber());
    if (i == -1 || s[i].room == 0) { printf("Student not found, or has no room.\n"); return; }
    s[i].room = 0;
    save();
    printf("Room vacated.\n");
}
void showStudents(void) {
    int i;
    printf("\nRoll    Name                 Room (0 = none)\n");
    for (i = 0; i < total; i++)
        printf("%-7d %-20s %d\n", s[i].roll, s[i].name, s[i].room);
    printf("Total students: %d\n", total);
}
int main(void) {
    int choice;
    load();
    do {
        printf("\n1. Add student\n2. Room availability\n3. Allot room\n");
        printf("4. Vacate room\n5. Show all students\n0. Exit\nChoice: ");
        choice = readNumber();
        switch (choice) {
            case 1: addStudent(); break;
            case 2: showRooms(); break;
            case 3: allotRoom(); break;
            case 4: vacateRoom(); break;
            case 5: showStudents(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
    printf("Goodbye!\n");
    return 0;
}
