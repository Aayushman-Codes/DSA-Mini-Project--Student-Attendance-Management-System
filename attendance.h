#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_DAYS 30
#define MAX_DEFAULTERS 100
#define MAX_STUDENTS 100

/* ===================== STRUCTURES ===================== */

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

/* ===================== GLOBAL VARIABLES ===================== */
extern Student* head;
extern AttendanceQueue aq;
extern DefaulterStack ds;
extern BSTNode* bstRoot;

/* ===================== LINKED LIST ===================== */
void clearInput(void);
int readInt(const char* prompt);
int readIntRange(const char* prompt, int minV, int maxV);
int read01(const char* prompt);
void readNameAlpha(const char* prompt, char* out, size_t outsz);
void insertStudent(int roll, const char* name);
bool deleteStudent(int roll);
Student* searchStudent(int roll);
void displayStudents(void);
void modifyStudentDetails(void);
void freeAllMemory(void);

/* ===================== QUEUE ===================== */
void initAttendanceQueue(void);
void addDayAttendance(void);
void displayAttendanceRecords(void);
void modifyAttendanceSingle(void);
void modifyAttendanceDay(void);
int calculateStudentAttendance(int roll);
void displayStudentAttendance(void);

/* ===================== STACK ===================== */
void initDefaulterStack(void);
void generateDefaulterList(int req);
void displayDefaulterList(void);

/* ===================== BST ===================== */
void buildBSTFromList(void);
void displayBST(void);
void displayMinMaxAttendanceBST(void);
void displayRangeByRollBST(void);
BSTNode* searchBST(BSTNode* root, int roll);
void freeBST(BSTNode* r);

#endif
