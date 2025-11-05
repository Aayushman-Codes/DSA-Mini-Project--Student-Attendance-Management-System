#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_DAYS 30
#define MAX_DEFAULTERS 100
#define MAX_STUDENTS 100

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

Student* head = NULL;
AttendanceQueue aq = {-1, -1, 0};
DefaulterStack ds = {-1};
BSTNode* bstRoot = NULL;

// ------------------------ LINKED LIST ------------------------
Student* createStudent(int roll, const char* name) {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    newStudent->rollNumber = roll;
    strcpy(newStudent->name, name);
    newStudent->next = NULL;
    return newStudent;
}

void insertStudent(int roll, const char* name) {
    Student* newStudent = createStudent(roll, name);
    if (head == NULL || head->rollNumber > roll) {
        newStudent->next = head;
        head = newStudent;
        printf("Student %s (Roll: %d) added successfully.\n", name, roll);
        return;
    }
    Student* current = head;
    while (current->next != NULL && current->next->rollNumber < roll) {
        current = current->next;
    }
    if (current->rollNumber == roll || (current->next != NULL && current->next->rollNumber == roll)) {
        printf("Error: Roll number %d already exists!\n", roll);
        free(newStudent);
        return;
    }
    newStudent->next = current->next;
    current->next = newStudent;
    printf("Student %s (Roll: %d) added successfully.\n", name, roll);
}

bool deleteStudent(int roll) {
    if (head == NULL) {
        printf("No students in record.\n");
        return false;
    }
    if (head->rollNumber == roll) {
        Student* temp = head;
        head = head->next;
        printf("Student %s (Roll: %d) deleted successfully.\n", temp->name, temp->rollNumber);
        free(temp);
        return true;
    }
    Student* current = head;
    while (current->next != NULL && current->next->rollNumber != roll) {
        current = current->next;
    }
    if (current->next == NULL) {
        printf("Student with roll number %d not found.\n", roll);
        return false;
    }
    Student* temp = current->next;
    current->next = temp->next;
    printf("Student %s (Roll: %d) deleted successfully.\n", temp->name, temp->rollNumber);
    free(temp);
    return true;
}

Student* searchStudent(int roll) {
    Student* current = head;
    while (current != NULL) {
        if (current->rollNumber == roll)
            return current;
        current = current->next;
    }
    return NULL;
}

void displayStudents() {
    if (head == NULL) {
        printf("No students in record.\n");
        return;
    }
    printf("\n=== STUDENT RECORDS (Linked List) ===\n");
    printf("Roll No | Name\n");
    printf("--------|---------------------\n");
    Student* current = head;
    while (current != NULL) {
        printf("%7d | %-19s\n", current->rollNumber, current->name);
        current = current->next;
    }
}

void modifyStudentDetails() {
    if (head == NULL) {
        printf("No students to modify.\n");
        return;
    }

    int roll;
    printf("Enter roll number of student to modify: ");
    scanf("%d", &roll);

    Student* student = searchStudent(roll);
    if (student == NULL) {
        printf("Student with roll number %d not found.\n", roll);
        return;
    }

    printf("\nCurrent Details:\nRoll: %d\nName: %s\n", student->rollNumber, student->name);
    printf("Enter new name (press ENTER to keep unchanged): ");
    getchar(); // clear newline
    char newName[50];
    fgets(newName, sizeof(newName), stdin);
    newName[strcspn(newName, "\n")] = '\0';

    if (strlen(newName) > 0) {
        strcpy(student->name, newName);
        printf("Name updated successfully!\n");
    } else {
        printf("No changes made.\n");
    }
}

// ------------------------ QUEUE (Attendance) ------------------------
void initAttendanceQueue() {
    aq.front = aq.rear = -1;
    aq.count = 0;
}

bool isQueueFull() { return aq.count == MAX_DAYS; }
bool isQueueEmpty() { return aq.count == 0; }

int calculateStudentAttendance(int rollNumber) {
    if (isQueueEmpty() || head == NULL) return 0;
    int attendanceCount = 0;
    int i = aq.front;
    for (int c = 0; c < aq.count; c++) {
        for (int j = 0; j < aq.days[i].presentCount; j++) {
            if (aq.days[i].presentStudents[j] == rollNumber) {
                attendanceCount++;
                break;
            }
        }
        i = (i + 1) % MAX_DAYS;
    }
    return attendanceCount;
}

void addDayAttendance() {
    if (head == NULL) {
        printf("No students in record. Please add students first.\n");
        return;
    }
    if (isQueueFull()) {
        printf("Attendance queue is full! Removing oldest record.\n");
        aq.front = (aq.front + 1) % MAX_DAYS;
        aq.count--;
    }
    if (isQueueEmpty()) {
        aq.front = aq.rear = 0;
    } else {
        aq.rear = (aq.rear + 1) % MAX_DAYS;
    }
    aq.days[aq.rear].dayNumber = aq.count + 1;
    aq.days[aq.rear].presentCount = 0;
    printf("\n========================================\n");
    printf("        RECORDING ATTENDANCE - DAY %d\n", aq.count + 1);
    printf("========================================\n");
    Student* current = head;
    while (current != NULL) {
        int status;
        printf("Was %s (Roll: %d) present? (1=Present, 0=Absent): ", current->name, current->rollNumber);
        scanf("%d", &status);
        if (status == 1 && aq.days[aq.rear].presentCount < MAX_STUDENTS)
            aq.days[aq.rear].presentStudents[aq.days[aq.rear].presentCount++] = current->rollNumber;
        current = current->next;
    }
    aq.count++;
    printf("\nDay %d attendance recorded successfully!\n", aq.count);
}

void displayAttendanceRecords() {
    if (isQueueEmpty()) {
        printf("No attendance records.\n");
        return;
    }
    printf("\n=== ATTENDANCE RECORDS (Circular Queue) ===\n");
    int i = aq.front;
    for (int c = 0; c < aq.count; c++) {
        printf("\nDay %d - Present Students:\n", aq.days[i].dayNumber);
        printf("----------------------------------------\n");
        if (aq.days[i].presentCount == 0)
            printf("No students were present this day.\n");
        else {
            for (int j = 0; j < aq.days[i].presentCount; j++) {
                Student* student = searchStudent(aq.days[i].presentStudents[j]);
                if (student != NULL)
                    printf("%s (Roll: %d)\n", student->name, student->rollNumber);
            }
        }
        i = (i + 1) % MAX_DAYS;
    }
}

void displayStudentAttendance() {
    if (head == NULL) {
        printf("No students in record.\n");
        return;
    }
    int roll;
    printf("Enter roll number to view attendance: ");
    scanf("%d", &roll);
    Student* student = searchStudent(roll);
    if (student == NULL) {
        printf("Student not found.\n");
        return;
    }
    int att = calculateStudentAttendance(roll);
    printf("\nStudent: %s (Roll: %d)\nAttendance: %d/%d days (%.2f%%)\n",
           student->name, roll, att, aq.count, aq.count ? (float)att / aq.count * 100 : 0);
}

void modifyAttendance() {
    if (isQueueEmpty()) {
        printf("No attendance records to modify.\n");
        return;
    }

    int dayNum;
    printf("Enter the day number to modify (1 - %d): ", aq.count);
    scanf("%d", &dayNum);

    int i = aq.front;
    DayRecord* target = NULL;
    for (int c = 0; c < aq.count; c++) {
        if (aq.days[i].dayNumber == dayNum) {
            target = &aq.days[i];
            break;
        }
        i = (i + 1) % MAX_DAYS;
    }
    if (!target) {
        printf("Invalid day number!\n");
        return;
    }

    target->presentCount = 0;
    Student* cur = head;
    while (cur != NULL) {
        int status;
        printf("Was %s (Roll: %d) present? (1=Present, 0=Absent): ", cur->name, cur->rollNumber);
        scanf("%d", &status);
        if (status == 1 && target->presentCount < MAX_STUDENTS)
            target->presentStudents[target->presentCount++] = cur->rollNumber;
        cur = cur->next;
    }

    printf("Day %d attendance modified successfully!\n", dayNum);
}

// ------------------------ STACK (Defaulters) ------------------------
void initDefaulterStack() { ds.top = -1; }
bool isStackFull() { return ds.top == MAX_DEFAULTERS - 1; }
bool isStackEmpty() { return ds.top == -1; }

void pushDefaulter(int roll, const char* name, int att, int req) {
    if (isStackFull()) return;
    ds.defaulters[++ds.top] = (Defaulter){roll, "", att, req};
    strcpy(ds.defaulters[ds.top].name, name);
}

Defaulter popDefaulter() {
    Defaulter empty = {-1, "", -1, -1};
    if (isStackEmpty()) return empty;
    return ds.defaulters[ds.top--];
}

void generateDefaulterList(int req) {
    if (head == NULL) {
        printf("No students in record.\n");
        return;
    }
    initDefaulterStack();
    Student* c = head;
    while (c != NULL) {
        int att = calculateStudentAttendance(c->rollNumber);
        if (att < req)
            pushDefaulter(c->rollNumber, c->name, att, req);
        c = c->next;
    }
    printf("Defaulter list generated.\n");
}

void displayDefaulterList() {
    if (isStackEmpty()) {
        printf("No defaulters found.\n");
        return;
    }
    DefaulterStack temp = ds;
    printf("\n=== DEFAULTER LIST ===\n");
    while (!isStackEmpty()) {
        Defaulter d = popDefaulter();
        printf("Roll: %d | %s | %d/%d days\n", d.rollNumber, d.name, d.attendanceCount, d.requiredAttendance);
    }
    ds = temp;
}

// ------------------------ BST ------------------------
BSTNode* createBSTNode(int roll, const char* name, int att) {
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    node->rollNumber = roll;
    strcpy(node->name, name);
    node->attendanceCount = att;
    node->left = node->right = NULL;
    return node;
}

BSTNode* insertBST(BSTNode* root, int roll, const char* name, int att) {
    if (!root) return createBSTNode(roll, name, att);
    if (roll < root->rollNumber) root->left = insertBST(root->left, roll, name, att);
    else if (roll > root->rollNumber) root->right = insertBST(root->right, roll, name, att);
    return root;
}

void buildBSTFromList() {
    bstRoot = NULL;
    Student* c = head;
    while (c != NULL) {
        int att = calculateStudentAttendance(c->rollNumber);
        bstRoot = insertBST(bstRoot, c->rollNumber, c->name, att);
        c = c->next;
    }
    printf("BST built successfully.\n");
}

void inorderBST(BSTNode* r) {
    if (!r) return;
    inorderBST(r->left);
    printf("Roll: %d | %s | Attendance: %d\n", r->rollNumber, r->name, r->attendanceCount);
    inorderBST(r->right);
}

void displayBST() {
    if (!bstRoot) {
        printf("BST is empty.\n");
        return;
    }
    printf("\n=== BST ===\n");
    inorderBST(bstRoot);
}

void rangePrintBST(BSTNode* r, int lo, int hi) {
    if (!r) return;
    if (r->rollNumber > lo) rangePrintBST(r->left, lo, hi);
    if (r->rollNumber >= lo && r->rollNumber <= hi)
        printf("Roll: %d | %s | Attendance: %d\n", r->rollNumber, r->name, r->attendanceCount);
    if (r->rollNumber < hi) rangePrintBST(r->right, lo, hi);
}

void displayMinMaxBST() {
    if (!bstRoot) {
        printf("BST is empty.\n");
        return;
    }
    int min, max;
    printf("Enter min roll: ");
    scanf("%d", &min);
    printf("Enter max roll: ");
    scanf("%d", &max);

    printf("\n=== Students Between %d and %d ===\n", min, max);
    rangePrintBST(bstRoot, min, max);
}

// ------------------------ MAIN ------------------------
void freeAllMemory() {
    Student* c = head;
    while (c) {
        Student* t = c;
        c = c->next;
        free(t);
    }
    printf("All memory freed.\n");
}

void displayMainMenu() {
    printf("\n==============================================================\n");
    printf("           STUDENT ATTENDANCE MANAGEMENT SYSTEM\n");
    printf("==============================================================\n");
}

void mainMenu() {
    int ch, roll, req;
    char name[50];
    initAttendanceQueue();
    initDefaulterStack();
    displayMainMenu();

    while (1) {
        printf("\n==================== MAIN MENU ====================\n");
        printf(" LINKED LIST (Students)\n");
        printf(" 1. ADD Student\n 2. DELETE Student\n 3. SEARCH Student\n 4. DISPLAY All Students\n 5. MODIFY Student Details\n");
        printf("\n CIRCULAR QUEUE (Attendance)\n");
        printf(" 6. RECORD Attendance\n 7. DISPLAY Attendance Records\n 8. VIEW Student Attendance\n 9. MODIFY Attendance\n");
        printf("\n STACK (Defaulters)\n");
        printf(" 10. GENERATE Defaulter List\n 11. DISPLAY Defaulter List\n");
        printf("\n BINARY SEARCH TREE (BST)\n");
        printf(" 12. BUILD BST\n 13. DISPLAY BST\n 14. DISPLAY Range (Min/Max)\n");
        printf("\n 15. EXIT\n");
        printf("==================================================\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: printf("Enter roll: "); scanf("%d", &roll); printf("Enter name: "); scanf("%s", name); insertStudent(roll, name); break;
            case 2: printf("Enter roll to delete: "); scanf("%d", &roll); deleteStudent(roll); break;
            case 3: printf("Enter roll to search: "); scanf("%d", &roll);
                    { Student* s = searchStudent(roll); if (s) printf("Found: %s (Roll %d)\n", s->name, s->rollNumber); else printf("Not found.\n"); } break;
            case 4: displayStudents(); break;
            case 5: modifyStudentDetails(); break;
            case 6: addDayAttendance(); break;
            case 7: displayAttendanceRecords(); break;
            case 8: displayStudentAttendance(); break;
            case 9: modifyAttendance(); break;
            case 10: printf("Enter required attendance: "); scanf("%d", &req); generateDefaulterList(req); break;
            case 11: displayDefaulterList(); break;
            case 12: buildBSTFromList(); break;
            case 13: displayBST(); break;
            case 14: displayMinMaxBST(); break;
            case 15: freeAllMemory(); exit(0);
            default: printf("Invalid choice.\n");
        }
    }
}

int main() {
    mainMenu();
    return 0;
}
