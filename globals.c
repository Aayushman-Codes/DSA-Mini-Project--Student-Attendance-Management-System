#include "attendance.h"

/* Global instances */
Student* head = NULL;
AttendanceQueue aq = { .front = -1, .rear = -1, .count = 0 };
DefaulterStack ds = { .top = -1 };
BSTNode* bstRoot = NULL;

/* Utility input functions (single implementation shared by all files) */

void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

int readInt(const char* prompt) {
    int x;
    while (1) {
        if (prompt && *prompt) printf("%s", prompt);
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
        if (x < minV || x > maxV) {
            printf("Please enter a value between %d and %d.\n", minV, maxV);
        } else {
            return x;
        }
    }
}

int read01(const char* prompt) {
    int x;
    while (1) {
        if (prompt && *prompt) printf("%s", prompt);
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
        if (prompt && *prompt) printf("%s", prompt);
        if (scanf("%49s", out) != 1) {
            clearInput();
            continue;
        }
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

/* Keep prompting until a positive (>=1) roll number is entered */
int validateRollPrompt(const char* prompt) {
    int roll;
    while (1) {
        roll = readInt(prompt);
        if (roll > 0) return roll;
        printf("Error: Roll number must be a positive integer.\n");
    }
}
