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


Student* head = NULL;
AttendanceQueue aq = { .front = -1, .rear = -1, .count = 0 };
DefaulterStack ds = { .top = -1 };
BSTNode* bstRoot = NULL;

/* ===================== UTILS: INPUT & PRINT ===================== */

void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

int readInt(const char* prompt) {
    int x;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &x) == 1) { clearInput(); return x; }
        printf("Invalid input! Please enter an integer value.\n");
        clearInput();
    }
}

int readIntRange(const char* prompt, int minV, int maxV) {
    int x;
    while (1) {
        x = readInt(prompt);
        if (x < minV || x > maxV) {
            printf("Please enter a value between %d and %d.\n", minV, maxV);
        } else return x;
    }
}

int read01(const char* prompt) {
    int x;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &x) == 1 && (x == 0 || x == 1)) { clearInput(); return x; }
        printf("Invalid input! Enter only 1 or 0.\n");
        clearInput();
    }
}

void readNameAlpha(const char* prompt, char* out, size_t outsz) {
    while (1) {
        printf("%s", prompt);
        if (scanf("%49s", out) != 1) { clearInput(); continue; }
        clearInput();
        int ok = 1;
        for (size_t i = 0; out[i]; ++i) {
            if (!isalpha((unsigned char)out[i])) { ok = 0; break; }
        }
        if (!ok) {
            printf("Error: Name must contain only alphabets (A-Z/a-z), no spaces.\n");
            continue;
        }
        if (strlen(out) == 0) {
            printf("Error: Name cannot be empty.\n");
            continue;
        }
        return;
    }
}

/* ===================== LINKED LIST (Students) ===================== */

Student* createStudent(int roll, const char* name) {
    Student* s = (Student*)malloc(sizeof(Student));
    s->rollNumber = roll;
    strcpy(s->name, name);
    s->next = NULL;
    return s;
}

void insertStudent(int roll, const char* name) {
    if (roll <= 0) {
        printf("Error: Roll number must be a positive integer.\n");
        return;
    }

    // Enforce alphabetic name (already validated in input, but double-check)
    for (size_t i = 0; name[i]; ++i) {
        if (!isalpha((unsigned char)name[i])) {
            printf("Error: Name must contain only alphabets.\n");
            return;
        }
    }

    Student* s = createStudent(roll, name);

    if (head == NULL || head->rollNumber > roll) {
        // Check duplicate with head as well
        if (head && head->rollNumber == roll) {
            printf("Error: Roll number %d already exists!\n", roll);
            free(s);
            return;
        }
        s->next = head;
        head = s;
        printf("Student %s (Roll: %d) added successfully.\n", name, roll);
        return;
    }

    Student* cur = head;
    while (cur->next != NULL && cur->next->rollNumber < roll) {
        cur = cur->next;
    }
    if (cur->rollNumber == roll || (cur->next && cur->next->rollNumber == roll)) {
        printf("Error: Roll number %d already exists!\n", roll);
        free(s);
        return;
    }
    s->next = cur->next;
    cur->next = s;
    printf("Student %s (Roll: %d) added successfully.\n", name, roll);
}

bool deleteStudent(int roll) {
    if (!head) { printf("No students in record.\n"); return false; }
    if (head->rollNumber == roll) {
        Student* t = head;
        head = head->next;
        printf("Student %s (Roll: %d) deleted successfully.\n", t->name, t->rollNumber);
        free(t);
        return true;
    }
    Student* cur = head;
    while (cur->next && cur->next->rollNumber != roll) cur = cur->next;
    if (!cur->next) { printf("Student with roll number %d not found.\n", roll); return false; }
    Student* t = cur->next;
    cur->next = t->next;
    printf("Student %s (Roll: %d) deleted successfully.\n", t->name, t->rollNumber);
    free(t);
    return true;
}

Student* searchStudent(int roll) {
    for (Student* c = head; c; c = c->next) if (c->rollNumber == roll) return c;
    return NULL;
}

void displayStudents(void) {
    if (!head) { printf("No students in record.\n"); return; }
    printf("\n=== STUDENT RECORDS (Linked List) ===\n");
    printf("Roll No | Name\n");
    printf("--------|---------------------\n");
    for (Student* c = head; c; c = c->next) {
        printf("%7d | %-19s\n", c->rollNumber, c->name);
    }
}

void modifyStudentDetails(void) {
    if (!head) { printf("No students to modify.\n"); return; }
    int roll = readInt("Enter roll number of student to modify: ");
    Student* s = searchStudent(roll);
    if (!s) { printf("Student with roll number %d not found.\n", roll); return; }

    printf("\nCurrent Details:\nRoll: %d\nName: %s\n", s->rollNumber, s->name);
    char newName[50];
    readNameAlpha("Enter new name (letters only): ", newName, sizeof(newName));
    strcpy(s->name, newName);
    printf("Name updated successfully!\n");
}

/* ===================== CIRCULAR QUEUE (Attendance) ===================== */

void initAttendanceQueue(void) {
    aq.front = aq.rear = -1;
    aq.count = 0;
}

bool isQueueFull(void) { return aq.count == MAX_DAYS; }
bool isQueueEmpty(void) { return aq.count == 0; }

int calculateStudentAttendance(int roll) {
    if (isQueueEmpty()) return 0;
    int total = 0;
    int i = aq.front;
    for (int c = 0; c < aq.count; ++c) {
        for (int j = 0; j < aq.days[i].presentCount; ++j) {
            if (aq.days[i].presentStudents[j] == roll) {
                total++;
                break;
            }
        }
        i = (i + 1) % MAX_DAYS;
    }
    return total;
}

void addDayAttendance(void) {
    if (!head) { printf("No students in record. Please add students first.\n"); return; }
    if (isQueueFull()) {
        printf("Attendance queue is full! Removing oldest record.\n");
        aq.front = (aq.front + 1) % MAX_DAYS;
        aq.count--;
    }
    if (isQueueEmpty()) aq.front = aq.rear = 0;
    else aq.rear = (aq.rear + 1) % MAX_DAYS;

    aq.days[aq.rear].dayNumber = aq.count + 1;
    aq.days[aq.rear].presentCount = 0;

    printf("\n========================================\n");
    printf("        RECORDING ATTENDANCE - DAY %d\n", aq.count + 1);
    printf("========================================\n");

    for (Student* c = head; c; c = c->next) {
        int status = read01(
            (printf("Was %s (Roll: %d) present? ", c->name, c->rollNumber),
            printf("(1=Present, 0=Absent): "), "")
        );
        if (status == 1) {
            if (aq.days[aq.rear].presentCount < MAX_STUDENTS)
                aq.days[aq.rear].presentStudents[aq.days[aq.rear].presentCount++] = c->rollNumber;
        }
    }
    aq.count++;
    printf("\nDay %d attendance recorded successfully!\n", aq.count);
}

void displayAttendanceRecords(void) {
    if (isQueueEmpty()) { printf("No attendance records.\n"); return; }
    printf("\n=== ATTENDANCE RECORDS (Circular Queue) ===\n");
    int i = aq.front;
    for (int c = 0; c < aq.count; ++c) {
        printf("\nDay %d - Present Students:\n", aq.days[i].dayNumber);
        printf("----------------------------------------\n");
        if (aq.days[i].presentCount == 0) {
            printf("No students were present this day.\n");
        } else {
            for (int j = 0; j < aq.days[i].presentCount; ++j) {
                Student* s = searchStudent(aq.days[i].presentStudents[j]);
                if (s) printf("%s (Roll: %d)\n", s->name, s->rollNumber);
            }
        }
        i = (i + 1) % MAX_DAYS;
    }
}

void displayStudentAttendance(void) {
    if (!head) { printf("No students in record.\n"); return; }
    int roll = readInt("Enter roll number to view attendance: ");
    Student* s = searchStudent(roll);
    if (!s) { printf("Student with roll number %d not found.\n", roll); return; }
    int att = calculateStudentAttendance(roll);
    printf("\n=== ATTENDANCE REPORT ===\n");
    printf("Student: %s (Roll: %d)\n", s->name, s->rollNumber);
    printf("Total Attendance: %d days\n", att);
    printf("Total Days Recorded: %d days\n", aq.count);
    if (aq.count > 0) printf("Attendance Percentage: %.2f%%\n", (float)att / aq.count * 100.0f);
}

/* Modify attendance: Option A - re-enter entire day */
void modifyAttendanceDay(void) {
    if (isQueueEmpty()) { printf("No attendance records to modify.\n"); return; }
    int day = readIntRange("Enter the day number to re-enter (1 - total days): ", 1, aq.count);

    /* Find the circular index that has this dayNumber */
    int idx = aq.front;
    DayRecord* target = NULL;
    for (int c = 0; c < aq.count; ++c) {
        if (aq.days[idx].dayNumber == day) { target = &aq.days[idx]; break; }
        idx = (idx + 1) % MAX_DAYS;
    }
    if (!target) { printf("Invalid day number!\n"); return; }

    target->presentCount = 0;
    for (Student* cur = head; cur; cur = cur->next) {
        int status = read01(
            (printf("Was %s (Roll: %d) present? ", cur->name, cur->rollNumber),
            printf("(1=Present, 0=Absent): "), "")
        );
        if (status == 1 && target->presentCount < MAX_STUDENTS)
            target->presentStudents[target->presentCount++] = cur->rollNumber;
    }
    printf("Day %d attendance modified successfully!\n", day);
}

/* Modify attendance: Option B - change one student's status on a given day */
void modifyAttendanceSingle(void) {
    if (isQueueEmpty()) { printf("No attendance records to modify.\n"); return; }
    int day = readIntRange("Enter day number to modify (1 - total days): ", 1, aq.count);

    /* Locate circular index */
    int idx = aq.front;
    DayRecord* target = NULL;
    for (int c = 0; c < aq.count; ++c) {
        if (aq.days[idx].dayNumber == day) { target = &aq.days[idx]; break; }
        idx = (idx + 1) % MAX_DAYS;
    }
    if (!target) { printf("Invalid day number!\n"); return; }

    int roll = readInt("Enter Roll Number to modify for that day: ");
    Student* s = searchStudent(roll);
    if (!s) { printf("Roll number not found.\n"); return; }

    /* Check if present already */
    int pos = -1;
    for (int i = 0; i < target->presentCount; ++i) {
        if (target->presentStudents[i] == roll) { pos = i; break; }
    }

    int newStatus = read01("Enter new status (1 = Present, 0 = Absent): ");

    if (newStatus == 1 && pos == -1) {
        if (target->presentCount < MAX_STUDENTS) {
            target->presentStudents[target->presentCount++] = roll;
            printf("Marked PRESENT for roll %d on Day %d.\n", roll, day);
        } else {
            printf("Cannot mark present: day list is full.\n");
        }
    } else if (newStatus == 0 && pos != -1) {
        for (int i = pos; i < target->presentCount - 1; ++i)
            target->presentStudents[i] = target->presentStudents[i + 1];
        target->presentCount--;
        printf("Marked ABSENT for roll %d on Day %d.\n", roll, day);
    } else {
        printf("No change needed.\n");
    }
}

/* ===================== STACK (Defaulters) ===================== */

void initDefaulterStack(void) { ds.top = -1; }
bool isStackFull(void) { return ds.top == MAX_DEFAULTERS - 1; }
bool isStackEmpty(void) { return ds.top == -1; }

void pushDefaulter(int roll, const char* name, int attendance, int required) {
    if (isStackFull()) { printf("Defaulter stack is full!\n"); return; }
    ds.top++;
    ds.defaulters[ds.top].rollNumber = roll;
    strcpy(ds.defaulters[ds.top].name, name);
    ds.defaulters[ds.top].attendanceCount = attendance;
    ds.defaulters[ds.top].requiredAttendance = required;
}

Defaulter popDefaulter(void) {
    Defaulter empty = { -1, "", -1, -1 };
    if (isStackEmpty()) return empty;
    return ds.defaulters[ds.top--];
}

void generateDefaulterList(int req) {
    if (!head) { printf("No students in record.\n"); return; }
    if (aq.count == 0) { printf("No attendance recorded yet.\n"); return; }
    if (req < 0) { printf("Error: Required attendance cannot be negative.\n"); return; }
    if (req > aq.count) {
        printf("Error: Required attendance (%d) cannot be more than total recorded days (%d).\n", req, aq.count);
        return;
    }
    initDefaulterStack();
    for (Student* c = head; c; c = c->next) {
        int att = calculateStudentAttendance(c->rollNumber);
        if (att < req) pushDefaulter(c->rollNumber, c->name, att, req);
    }
    printf("Defaulter list generated successfully.\n");
}

void displayDefaulterList(void) {
    if (isStackEmpty()) { printf("No defaulters found.\n"); return; }
    printf("\n=== DEFAULTER LIST (Stack - LIFO Order) ===\n");
    printf("Roll No | Name                | Current | Required\n");
    printf("--------|---------------------|---------|----------\n");
    DefaulterStack temp = ds;
    while (!isStackEmpty()) {
        Defaulter d = popDefaulter();
        printf("%7d | %-19s | %7d | %8d\n", d.rollNumber, d.name, d.attendanceCount, d.requiredAttendance);
    }
    ds = temp; // restore
}

/* ===================== BST (Search + Min/Max by Attendance) ===================== */

BSTNode* createBSTNode(int roll, const char* name, int attendance) {
    BSTNode* n = (BSTNode*)malloc(sizeof(BSTNode));
    n->rollNumber = roll;
    strcpy(n->name, name);
    n->attendanceCount = attendance;
    n->left = n->right = NULL;
    return n;
}

BSTNode* insertBST(BSTNode* root, int roll, const char* name, int attendance) {
    if (!root) return createBSTNode(roll, name, attendance);
    if (roll < root->rollNumber) root->left = insertBST(root->left, roll, name, attendance);
    else if (roll > root->rollNumber) root->right = insertBST(root->right, roll, name, attendance);
    else {
        // Update attendance/name if roll already exists (shouldn't happen if list is unique)
        root->attendanceCount = attendance;
        strcpy(root->name, name);
    }
    return root;
}

BSTNode* searchBST(BSTNode* root, int roll) {
    if (!root || root->rollNumber == roll) return root;
    if (roll < root->rollNumber) return searchBST(root->left, roll);
    return searchBST(root->right, roll);
}

void buildBSTFromList(void) {
    bstRoot = NULL;
    int count = 0;
    for (Student* c = head; c; c = c->next) {
        int attendance = calculateStudentAttendance(c->rollNumber);
        bstRoot = insertBST(bstRoot, c->rollNumber, c->name, attendance);
        count++;
    }
    printf("Binary Search Tree built successfully with %d students.\n", count);
}

void inorderBST(BSTNode* root) {
    if (!root) return;
    inorderBST(root->left);
    printf("Roll: %d, Name: %s, Attendance: %d\n", root->rollNumber, root->name, root->attendanceCount);
    inorderBST(root->right);
}

void displayBST(void) {
    if (!bstRoot) { printf("BST is empty. Build it first from student records.\n"); return; }
    printf("\n=== BINARY SEARCH TREE (In-order Traversal) ===\n");
    inorderBST(bstRoot);
}

BSTNode* findMinAttendance(BSTNode* root, BSTNode* best) {
    if (!root) return best;
    if (!best || root->attendanceCount < best->attendanceCount) best = root;
    best = findMinAttendance(root->left, best);
    best = findMinAttendance(root->right, best);
    return best;
}

BSTNode* findMaxAttendance(BSTNode* root, BSTNode* best) {
    if (!root) return best;
    if (!best || root->attendanceCount > best->attendanceCount) best = root;
    best = findMaxAttendance(root->left, best);
    best = findMaxAttendance(root->right, best);
    return best;
}

void displayMinMaxAttendanceBST(void) {
    if (!bstRoot) { printf("BST not built yet. Please build BST first.\n"); return; }
    BSTNode* minN = findMinAttendance(bstRoot, NULL);
    BSTNode* maxN = findMaxAttendance(bstRoot, NULL);
    printf("\n=== MIN & MAX ATTENDANCE (From BST) ===\n");
    printf("Lowest  Attendance: %s (Roll %d): %d days\n", minN->name, minN->rollNumber, minN->attendanceCount);
    printf("Highest Attendance: %s (Roll %d): %d days\n", maxN->name, maxN->rollNumber, maxN->attendanceCount);
}

/* Range view by roll number via BST  */
void rangePrint(BSTNode* r, int lo, int hi) {
    if (!r) return;
    if (r->rollNumber > lo) rangePrint(r->left, lo, hi);
    if (r->rollNumber >= lo && r->rollNumber <= hi)
        printf("Roll: %d | %s | Attendance: %d\n", r->rollNumber, r->name, r->attendanceCount);
    if (r->rollNumber < hi) rangePrint(r->right, lo, hi);
}

void displayRangeByRollBST(void) {
    if (!bstRoot) { printf("BST is empty.\n"); return; }
    int lo = readInt("Enter min roll: ");
    int hi = readInt("Enter max roll: ");
    if (lo > hi) { int t = lo; lo = hi; hi = t; }
    printf("\n=== Students Between %d and %d ===\n", lo, hi);
    rangePrint(bstRoot, lo, hi);
}


void freeAllMemory(void) {
    Student* c = head;
    while (c) {
        Student* t = c;
        c = c->next;
        free(t);
    }
    head = NULL;

}

void freeBST(BSTNode* r) {
    if (!r) return;
    freeBST(r->left);
    freeBST(r->right);
    free(r);
}

/* ===================== MENU ===================== */

void displayMainMenu(void) {
    printf("\n==============================================================\n");
    printf("           STUDENT ATTENDANCE MANAGEMENT SYSTEM\n");
    printf("==============================================================\n");
    printf(" Data Structures: Linked List, Circular Queue, Stack, BST\n");
    printf("==============================================================\n");
}

void mainMenu(void) {
    initAttendanceQueue();
    initDefaulterStack();
    displayMainMenu();

    while (1) {
        printf("\n==================== MAIN MENU ====================\n");
        printf(" LINKED LIST (Students)\n");
        printf("  1. ADD Student\n");
        printf("  2. DELETE Student\n");
        printf("  3. SEARCH Student (Linked List)\n");
        printf("  4. DISPLAY All Students\n");
        printf("  5. MODIFY Student Details\n");
        printf("\n CIRCULAR QUEUE (Attendance)\n");
        printf("  6. RECORD Attendance\n");
        printf("  7. DISPLAY Attendance Records\n");
        printf("  8. VIEW Student Attendance\n");
        printf("  9. MODIFY Attendance (Single Entry)\n");
        printf(" 10. MODIFY Attendance (Re-enter Entire Day)\n");
        printf("\n STACK (Defaulters)\n");
        printf(" 11. GENERATE Defaulter List\n");
        printf(" 12. DISPLAY Defaulter List\n");
        printf("\n BINARY SEARCH TREE (BST)\n");
        printf(" 13. BUILD BST\n");
        printf(" 14. DISPLAY BST (In-order)\n");
        printf(" 15. FAST Student Search (BST by Roll)\n");
        printf(" 16. DISPLAY Min & Max Attendance (BST)\n");
        printf(" 17. DISPLAY Range by Roll (BST)\n");
        printf("\n MISC\n");
        printf(" 18. EXIT\n");
        printf("==================================================\n");

        int choice = readIntRange("Enter your choice (1-19): ", 1, 19);
        switch (choice) {
            case 1: {
                int roll = readInt("Enter roll number: ");
                char name[50]; readNameAlpha("Enter name (letters only, no spaces): ", name, sizeof(name));
                insertStudent(roll, name);
            } break;

            case 2: {
                int roll = readInt("Enter roll number to delete: ");
                deleteStudent(roll);
            } break;

            case 3: {
                int roll = readInt("Enter roll number to search: ");
                Student* s = searchStudent(roll);
                if (s) {
                    printf("\nFOUND via Linked List:\n");
                    printf("   Roll Number: %d\n", s->rollNumber);
                    printf("   Name: %s\n", s->name);
                    printf("   Attendance: %d days (from queue)\n", calculateStudentAttendance(roll));
                } else {
                    printf("Student not found in Linked List.\n");
                }
            } break;

            case 4: displayStudents(); break;
            case 5: modifyStudentDetails(); break;

            case 6: addDayAttendance(); break;
            case 7: displayAttendanceRecords(); break;
            case 8: displayStudentAttendance(); break;

            case 9: modifyAttendanceSingle(); break;
            case 10: modifyAttendanceDay(); break;

            case 11: {
                if (aq.count == 0) { printf("No attendance recorded yet.\n"); break; }
                int req = readIntRange("Enter required attendance threshold (in days): ", 0, aq.count);
                generateDefaulterList(req);
            } break;

            case 12: displayDefaulterList(); break;

            case 13: buildBSTFromList(); break;
            case 14: displayBST(); break;

            case 15: {
                if (!bstRoot) { printf("BST not built yet. Build BST first.\n"); break; }
                int roll = readInt("Enter roll number to search (BST): ");
                BSTNode* r = searchBST(bstRoot, roll);
                if (r) {
                    printf("\nFOUND via BST Search:\n");
                    printf("   Roll Number: %d\n", r->rollNumber);
                    printf("   Name: %s\n", r->name);
                    printf("   Attendance Count: %d days\n", r->attendanceCount);
                } else {
                    printf("Student with roll number %d not found in BST.\n", roll);
                }
            } break;

            case 16: displayMinMaxAttendanceBST(); break;
            case 17: displayRangeByRollBST(); break;
            case 18:
                freeBST(bstRoot);
                freeAllMemory();
                printf("All memory freed. Exiting...\n");
                exit(0);
                break;
        }
    }
}

/* ===================== MAIN ===================== */

int main(void) {
    mainMenu();
    return 0;
}