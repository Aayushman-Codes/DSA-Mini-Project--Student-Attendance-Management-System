#include "attendance.h"
//version 3
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
        printf(" 13. Organize Data for Faster Searching\n");
        printf(" 14. Show All Students (Sorted by Roll Number)\n");
        printf(" 15. Fast Search Student by Roll Number\n");
        printf(" 16. Show Student with Lowest & Highest Attendance\n");
        printf(" 17. Show Students Between Two Roll Numbers\n");
        printf("\n MISC\n");
        printf(" 18. EXIT\n");
        printf("==================================================\n");

        int choice = readIntRange("Enter your choice (1-18): ", 1, 18);
        switch (choice) {
            case 1: {
                int roll = validateRollPrompt("Enter roll number to add: ");
                char name[50];
                readNameAlpha("Enter name (letters only, no spaces): ", name, sizeof(name));
                insertStudent(roll, name);
            } break;

            case 2: {
                printf("\n--- Before Delete ---\n");
                displayStudents();
                int roll = validateRollPrompt("Enter roll number to delete: ");
                deleteStudent(roll);
            } break;

            case 3: {
                int roll = validateRollPrompt("Enter roll number to search: ");
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
                int roll = validateRollPrompt("Enter roll number to search (BST): ");
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

int main(void) {
    mainMenu();
    return 0;
}
