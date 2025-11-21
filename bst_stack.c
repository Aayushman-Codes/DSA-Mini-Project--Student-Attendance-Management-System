#include "attendance.h"

/* STACK (DEFAULTERS) */

void initDefaulterStack(void) {
    ds.top = -1;
}

bool isStackFull(void) {
    return ds.top == MAX_DEFAULTERS - 1;
}

bool isStackEmpty(void) {
    return ds.top == -1;
}

void pushDefaulter(int roll, const char* name, int attendance, int required) {
    if (isStackFull()) {
        printf("Defaulter stack is full!\n");
        return;
    }
    ds.top++;
    ds.defaulters[ds.top].rollNumber = roll;
    strncpy(ds.defaulters[ds.top].name, name, sizeof(ds.defaulters[ds.top].name)-1);
    ds.defaulters[ds.top].name[sizeof(ds.defaulters[ds.top].name)-1] = '\0';
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
    ds = temp;
}

/* BST FUNCTIONS */

BSTNode* createBSTNode(int roll, const char* name, int attendance) {
    BSTNode* n = (BSTNode*)malloc(sizeof(BSTNode));
    n->rollNumber = roll;
    strncpy(n->name, name, sizeof(n->name)-1);
    n->name[sizeof(n->name)-1] = '\0';
    n->attendanceCount = attendance;
    n->left = n->right = NULL;
    return n;
}

BSTNode* insertBST(BSTNode* root, int roll, const char* name, int attendance) {
    if (!root) return createBSTNode(roll, name, attendance);
    if (roll < root->rollNumber) root->left = insertBST(root->left, roll, name, attendance);
    else if (roll > root->rollNumber) root->right = insertBST(root->right, roll, name, attendance);
    else {
        /* update */
        root->attendanceCount = attendance;
        strncpy(root->name, name, sizeof(root->name)-1);
        root->name[sizeof(root->name)-1] = '\0';
    }
    return root;
}

BSTNode* searchBST(BSTNode* root, int roll) {
    if (!root) return NULL;
    if (root->rollNumber == roll) return root;
    if (roll < root->rollNumber) return searchBST(root->left, roll);
    return searchBST(root->right, roll);
}

void buildBSTFromList(void) {
    freeBST(bstRoot);
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
    if (minN) printf("\nLowest Attendance: %s (Roll %d) : %d days\n", minN->name, minN->rollNumber, minN->attendanceCount);
    if (maxN) printf("Highest Attendance: %s (Roll %d) : %d days\n", maxN->name, maxN->rollNumber, maxN->attendanceCount);
}

void rangePrint(BSTNode* r, int lo, int hi) {
    if (!r) return;
    if (r->rollNumber > lo) rangePrint(r->left, lo, hi);
    if (r->rollNumber >= lo && r->rollNumber <= hi) printf("Roll: %d | %s | Attendance: %d\n", r->rollNumber, r->name, r->attendanceCount);
    if (r->rollNumber < hi) rangePrint(r->right, lo, hi);
}

void displayRangeByRollBST(void) {
    if (!bstRoot) { printf("BST is empty.\n"); return; }
    int lo = validateRollPrompt("Enter min roll: ");
    int hi = validateRollPrompt("Enter max roll: ");
    if (lo > hi) { int t = lo; lo = hi; hi = t; }
    printf("\n=== Students Between %d and %d ===\n", lo, hi);
    rangePrint(bstRoot, lo, hi);
}

void freeBST(BSTNode* r) {
    if (!r) return;
    freeBST(r->left);
    freeBST(r->right);
    free(r);
}
