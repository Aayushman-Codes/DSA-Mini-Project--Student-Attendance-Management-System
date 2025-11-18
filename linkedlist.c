#include "attendance.h"

/* ===================== UTILITY FUNCTIONS ===================== */

void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

int readInt(const char* prompt) {
    int x;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &x) == 1) {
            clearInput();
            return x;
        }
        printf("Invalid input! Please enter an integer value.\n");
        clearInput();
    }
}

int readIntRange(const char* prompt, int minV, int maxV) {
    int x;
    while (1) {
        x = readInt(prompt);
        if (x < minV || x > maxV)
            printf("Please enter a value between %d and %d.\n", minV, maxV);
        else
            return x;
    }
}

int read01(const char* prompt) {
    int x;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &x) == 1 && (x == 0 || x == 1)) {
            clearInput();
            return x;
        }
        printf("Invalid input! Enter only 1 or 0.\n");
        clearInput();
    }
}

void readNameAlpha(const char* prompt, char* out, size_t outsz) {
    while (1) {
        printf("%s", prompt);
        if (scanf("%49s", out) != 1) {
            clearInput();
            continue;
        }
        clearInput();

        int ok = 1;
        for (size_t i = 0; out[i]; ++i)
            if (!isalpha((unsigned char)out[i])) ok = 0;

        if (!ok) {
            printf("Error: Name must contain only alphabets (A–Z/a–z).\n");
            continue;
        }
        if (strlen(out) == 0) {
            printf("Error: Name cannot be empty.\n");
            continue;
        }
        return;
    }
}

/* ===================== LINKED LIST FUNCTIONS ===================== */

void displayStudents(void) {
    if (!head) {
        printf("\nNo students in record.\n");
        return;
    }

    printf("\n=== STUDENT RECORDS (Linked List) ===\n");
    printf("----------------------------------------\n");
    printf(" Roll No | Name\n");
    printf("---------|----------------------\n");

    for (Student* c = head; c; c = c->next)
        printf(" %7d | %-20s\n", c->rollNumber, c->name);
}

/* Add Student */
void insertStudent(int roll, const char* name) {
    printf("\n--- Current Student List (Before Add) ---\n");
    displayStudents();

    Student* s = (Student*)malloc(sizeof(Student));
    s->rollNumber = roll;
    strcpy(s->name, name);
    s->next = NULL;

    if (!head || roll < head->rollNumber) {
        s->next = head;
        head = s;
        printf("\nStudent %s (Roll %d) added successfully.\n", name, roll);
        printf("\n--- Updated Student List (After Add) ---\n");
        displayStudents();
        return;
    }

    Student* cur = head;
    while (cur->next && cur->next->rollNumber < roll)
        cur = cur->next;

    if ((cur->next && cur->next->rollNumber == roll) || cur->rollNumber == roll) {
        printf("Error: Roll number %d already exists!\n", roll);
        free(s);
        return;
    }

    s->next = cur->next;
    cur->next = s;
    printf("\nStudent %s (Roll %d) added successfully.\n", name, roll);
    printf("\n--- Updated Student List (After Add) ---\n");
    displayStudents();
}

/* Delete Student */
bool deleteStudent(int roll) {

    if (!head) {
        printf("No students in record.\n");
        return false;
    }

    if (head->rollNumber == roll) {
        Student* temp = head;
        head = head->next;
        free(temp);
        printf("\nStudent (Roll %d) deleted successfully.\n", roll);
        printf("\n--- Updated Student List (After Delete) ---\n");
        displayStudents();
        return true;
    }

    Student* cur = head;
    while (cur->next && cur->next->rollNumber != roll)
        cur = cur->next;

    if (!cur->next) {
        printf("Student with roll number %d not found.\n", roll);
        return false;
    }

    Student* temp = cur->next;
    cur->next = temp->next;
    free(temp);
    printf("\nStudent (Roll %d) deleted successfully.\n", roll);
    printf("\n--- Updated Student List (After Delete) ---\n");
    displayStudents();
    return true;
}

/* Search Student */
Student* searchStudent(int roll) {
    for (Student* c = head; c; c = c->next)
        if (c->rollNumber == roll)
            return c;
    return NULL;
}

/* Modify Student */
void modifyStudentDetails(void) {

    if (!head) {
        printf("No students to modify.\n");
        return;
    }

    int roll = readInt("Enter roll number of student to modify: ");
    Student* s = searchStudent(roll);
    if (!s) {
        printf("Student with roll number %d not found.\n", roll);
        return;
    }

    printf("\nCurrent Details: Roll: %d | Name: %s\n", s->rollNumber, s->name);
    char newName[50];
    readNameAlpha("Enter new name (letters only): ", newName, sizeof(newName));
    strcpy(s->name, newName);

    printf("\nName updated successfully!\n");
    printf("\n--- Updated Student List (After Modify) ---\n");
    displayStudents();
}

/* Free All Memory */
void freeAllMemory(void) {
    Student* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
    head = NULL;
}
