#include "attendance.h"

/* ===================== STACK ===================== */

void initDefaulterStack(void) { ds.top = -1; }

void generateDefaulterList(int req) {
    if (!head) { printf("No students.\n"); return; }
    if (aq.count == 0) { printf("No attendance yet.\n"); return; }
    if (req > aq.count) {
        printf("Required attendance exceeds total days.\n");
        return;
    }
    ds.top = -1;
    for (Student* s = head; s; s = s->next) {
        int att = calculateStudentAttendance(s->rollNumber);
        if (att < req) {
            ds.defaulters[++ds.top] = (Defaulter){s->rollNumber, "", att, req};
            strcpy(ds.defaulters[ds.top].name, s->name);
        }
    }
    printf("Defaulter list generated.\n");
}

void displayDefaulterList(void) {
    if (ds.top == -1) { printf("No defaulters.\n"); return; }
    printf("\n=== DEFAULTERS ===\n");
    for (int i = ds.top; i >= 0; i--)
        printf("Roll %d | %s | %d/%d\n", ds.defaulters[i].rollNumber,
               ds.defaulters[i].name,
               ds.defaulters[i].attendanceCount,
               ds.defaulters[i].requiredAttendance);
}

/* ===================== BST ===================== */

BSTNode* createBSTNode(int roll, const char* name, int attendance) {
    BSTNode* n = malloc(sizeof(BSTNode));
    n->rollNumber = roll;
    strcpy(n->name, name);
    n->attendanceCount = attendance;
    n->left = n->right = NULL;
    return n;
}

BSTNode* insertBST(BSTNode* root, int roll, const char* name, int attendance) {
    if (!root) return createBSTNode(roll, name, attendance);
    if (roll < root->rollNumber)
        root->left = insertBST(root->left, roll, name, attendance);
    else if (roll > root->rollNumber)
        root->right = insertBST(root->right, roll, name, attendance);
    return root;
}

BSTNode* searchBST(BSTNode* root, int roll) {
    if (!root || root->rollNumber == roll) return root;
    if (roll < root->rollNumber) return searchBST(root->left, roll);
    return searchBST(root->right, roll);
}

void inorderBST(BSTNode* r) {
    if (!r) return;
    inorderBST(r->left);
    printf("Roll: %d | %s | Attendance: %d\n", r->rollNumber, r->name, r->attendanceCount);
    inorderBST(r->right);
}

void buildBSTFromList(void) {
    bstRoot = NULL;
    for (Student* s = head; s; s = s->next) {
        int att = calculateStudentAttendance(s->rollNumber);
        bstRoot = insertBST(bstRoot, s->rollNumber, s->name, att);
    }
    printf("BST built.\n");
}

void displayBST(void) {
    if (!bstRoot) { printf("BST empty.\n"); return; }
    printf("\n=== BST Inorder ===\n");
    inorderBST(bstRoot);
}

void freeBST(BSTNode* r) {
    if (!r) return;
    freeBST(r->left);
    freeBST(r->right);
    free(r);
}

/* ===== Display Min & Max Attendance ===== */
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
    if (!bstRoot) { printf("BST not built yet.\n"); return; }
    BSTNode* minN = findMinAttendance(bstRoot, NULL);
    BSTNode* maxN = findMaxAttendance(bstRoot, NULL);
    printf("\n=== MIN & MAX ATTENDANCE ===\n");
    printf("Lowest  : %s (Roll %d) - %d days\n", minN->name, minN->rollNumber, minN->attendanceCount);
    printf("Highest : %s (Roll %d) - %d days\n", maxN->name, maxN->rollNumber, maxN->attendanceCount);
}

/* ===== Display Range by Roll ===== */
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
