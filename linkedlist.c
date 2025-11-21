#include "attendance.h"

/* Display list */
void displayStudents(void) {
    if (!head) {
        printf("\nNo students in record.\n");
        return;
    }
    printf("\n=== STUDENT RECORDS (Linked List) ===\n");
    printf(" Roll No | Name\n");
    printf("---------|----------------------\n");
    for (Student* c = head; c; c = c->next) {
        printf(" %7d | %-20s\n", c->rollNumber, c->name);
    }
}

/* Insert (expects valid roll supplied by caller) */
void insertStudent(int roll, const char* name) {
    /* Show before-insert list */
    printf("\n--- Before Insert ---\n");
    displayStudents();

    Student* s = (Student*)malloc(sizeof(Student));
    if (!s) {
        printf("Memory allocation failed!\n");
        return;
    }
    s->rollNumber = roll;
    strncpy(s->name, name, sizeof(s->name)-1);
    s->name[sizeof(s->name)-1] = '\0';
    s->next = NULL;

    if (!head || roll < head->rollNumber) {
        /* check duplicate with head */
        if (head && head->rollNumber == roll) {
            printf("Error: Roll %d already exists.\n", roll);
            free(s);
            return;
        }
        s->next = head;
        head = s;
        printf("\nStudent %s (Roll %d) added successfully.\n", name, roll);
        printf("\n--- After Insert ---\n");
        displayStudents();
        return;
    }

    Student* cur = head;
    while (cur->next && cur->next->rollNumber < roll) cur = cur->next;

    if ((cur->next && cur->next->rollNumber == roll) || cur->rollNumber == roll) {
        printf("Error: Roll %d already exists.\n", roll);
        free(s);
        return;
    }

    s->next = cur->next;
    cur->next = s;

    printf("\nStudent %s (Roll %d) added successfully.\n", name, roll);
    printf("\n--- After Insert ---\n");
    displayStudents();
}

/* Delete student (roll provided by caller and validated there) */
bool deleteStudent(int roll) {
    if (!head) {
        printf("No students in record.\n");
        return false;
    }

    if (head->rollNumber == roll) {
        Student* t = head;
        head = head->next;
        free(t);
        printf("\nStudent (Roll %d) deleted successfully.\n", roll);
        printf("\n--- After Delete ---\n");
        displayStudents();
        return true;
    }

    Student* cur = head;
    while (cur->next && cur->next->rollNumber != roll) cur = cur->next;

    if (!cur->next) {
        printf("Student with roll %d not found.\n", roll);
        return false;
    }

    Student* t = cur->next;
    cur->next = t->next;
    free(t);
    printf("\nStudent (Roll %d) deleted successfully.\n", roll);
    printf("\n--- After Delete ---\n");
    displayStudents();
    return true;
}

/* Search (returns pointer or NULL). Caller can call validateRollPrompt before calling. */
Student* searchStudent(int roll) {
    for (Student* c = head; c; c = c->next)
        if (c->rollNumber == roll)
            return c;
    return NULL;
}

/* Modify details: prompts for roll inside and uses validateRollPrompt */
void modifyStudentDetails(void) {
    if (!head) {
        printf("No students to modify.\n");
        return;
    }

    printf("\n--- Before Modify ---\n");
    displayStudents();

    int roll = validateRollPrompt("Enter roll number to modify: ");
    Student* s = searchStudent(roll);
    if (!s) {
        printf("Student with roll %d not found.\n", roll);
        return;
    }

    printf("\nCurrent Details: Roll: %d | Name: %s\n", s->rollNumber, s->name);
    char newName[50];
    readNameAlpha("Enter new name (letters only): ", newName, sizeof(newName));
    strncpy(s->name, newName, sizeof(s->name)-1);
    s->name[sizeof(s->name)-1] = '\0';

    printf("\nName updated successfully!\n");
    printf("\n--- After Modify ---\n");
    displayStudents();
}

/* Free all student nodes */
void freeAllMemory(void) {
    Student* t;
    while (head) {
        t = head;
        head = head->next;
        free(t);
    }
    head = NULL;
}
