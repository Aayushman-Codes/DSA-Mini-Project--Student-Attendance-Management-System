#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* CONFIG */
#define MAX_DAYS 30
#define MAX_STUDENTS 100
#define MAX_DEFAULTERS 100

/* STRUCTS */

typedef struct Student {
    int rollNumber;
    char name[50];
    struct Student* next;
} Student;

typedef struct {
    int dayNumber;
    int presentStudents[MAX_STUDENTS];
    int presentCount;
} DayRecord;

typedef struct {
    DayRecord days[MAX_DAYS];
    int front;
    int rear;
    int count;
} AttendanceQueue;

typedef struct {
    int rollNumber;
    char name[50];
    int attendanceCount;
    int requiredAttendance;
} Defaulter;

typedef struct {
    Defaulter defaulters[MAX_DEFAULTERS];
    int top;
} DefaulterStack;

typedef struct BSTNode {
    int rollNumber;
    char name[50];
    int attendanceCount;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

/* GLOBALS */
extern Student* head;
extern AttendanceQueue aq;
extern DefaulterStack ds;
extern BSTNode* bstRoot;

/* UTILS / INPUT */
void clearInput(void);
int readInt(const char* prompt);
int readIntRange(const char* prompt, int minV, int maxV);
int read01(const char* prompt);
void readNameAlpha(const char* prompt, char* out, size_t outsz);

/* Re-prompting roll validator (keeps asking until roll > 0) */
int validateRollPrompt(const char* prompt);

/* LINKED LIST */
void displayStudents(void);
void insertStudent(int roll, const char* name);
bool deleteStudent(int roll);
Student* searchStudent(int roll);
void modifyStudentDetails(void);
void freeAllMemory(void);

/* QUEUE (ATTENDANCE) */
void initAttendanceQueue(void);
bool isQueueFull(void);
bool isQueueEmpty(void);
void addDayAttendance(void);
void displayAttendanceRecords(void);
int calculateStudentAttendance(int roll);
void displayStudentAttendance(void);
void modifyAttendanceDay(void);
void modifyAttendanceSingle(void);

/* STACK (DEFAULTERS) */
void initDefaulterStack(void);
bool isStackFull(void);
bool isStackEmpty(void);
void pushDefaulter(int roll, const char* name, int attendance, int required);
Defaulter popDefaulter(void);
void generateDefaulterList(int req);
void displayDefaulterList(void);

/* BST */
BSTNode* createBSTNode(int roll, const char* name, int attendance);
BSTNode* insertBST(BSTNode* root, int roll, const char* name, int attendance);
BSTNode* searchBST(BSTNode* root, int roll);
void buildBSTFromList(void);
void inorderBST(BSTNode* root);
void displayBST(void);
void displayMinMaxAttendanceBST(void);
void displayRangeByRollBST(void);
void freeBST(BSTNode* root);

#endif /* ATTENDANCE_H */
