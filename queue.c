#include "attendance.h"

/* Initialize queue */
void initAttendanceQueue(void) {
    aq.front = aq.rear = -1;
    aq.count = 0;
}

bool isQueueFull(void) {
    return aq.count == MAX_DAYS;
}

bool isQueueEmpty(void) {
    return aq.count == 0;
}

/* Calculate total attendance for a roll (roll should be validated by caller or use validateRollPrompt) */
int calculateStudentAttendance(int roll) {
    if (roll <= 0) return 0;
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

/* Add a day's attendance.*/
void addDayAttendance(void) {
    if (!head) {
        printf("No students in record. Please add students first.\n");
        return;
    }

    // Show current list before taking attendance
    displayStudents();

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

    printf("\n========== RECORDING ATTENDANCE - DAY %d ==========\n", aq.days[aq.rear].dayNumber);

    // Mark all students present initially
    int studentTotal = 0;
    for (Student* c = head; c; c = c->next) {
        if (aq.days[aq.rear].presentCount < MAX_STUDENTS)
            aq.days[aq.rear].presentStudents[aq.days[aq.rear].presentCount++] = c->rollNumber;
        studentTotal++;
    }

    // Now ask which students are absent
    while (1) {
        int choice = read01("Is there an absentee? (1 = Yes, 0 = No): ");
        if (choice == 0) break;

        int roll = readInt("Enter absent roll number: ");
        Student* s = searchStudent(roll);

        if (!s) {
            printf("Roll number not found.\n");
            continue;
        }

        // Remove from present list
        int pos = -1;
        for (int i = 0; i < aq.days[aq.rear].presentCount; ++i) {
            if (aq.days[aq.rear].presentStudents[i] == roll) {
                pos = i;
                break;
            }
        }

        if (pos != -1) {
            for (int i = pos; i < aq.days[aq.rear].presentCount - 1; ++i)
                aq.days[aq.rear].presentStudents[i] = aq.days[aq.rear].presentStudents[i+1];

            aq.days[aq.rear].presentCount--;
            printf("Marked ABSENT: %s (Roll %d)\n", s->name, s->rollNumber);
        } else {
            printf("Already marked absent.\n");
        }
    }

    aq.count++;

    // Summary
    printf("\n===== DAY %d ATTENDANCE RECORDED =====\n", aq.count);
    printf("Present: %d | Absent: %d | Total: %d\n",
           aq.days[aq.rear].presentCount,
           studentTotal - aq.days[aq.rear].presentCount,
           studentTotal);
}

/* Display all attendance records */
void displayAttendanceRecords(void) {
    if (isQueueEmpty()) {
        printf("No attendance records.\n");
        return;
    }

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

/* Display attendance for a single student (prompts roll with validation) */
void displayStudentAttendance(void) {
    if (!head) { printf("No students in record.\n"); return; }

    int roll = validateRollPrompt("Enter roll number to view attendance: ");
    Student* s = searchStudent(roll);
    if (!s) { printf("Student with roll number %d not found.\n", roll); return; }

    int att = calculateStudentAttendance(roll);
    printf("\n=== ATTENDANCE REPORT ===\n");
    printf("Student: %s (Roll: %d)\n", s->name, s->rollNumber);
    printf("Total Attendance: %d days\n", att);
    printf("Total Days Recorded: %d days\n", aq.count);
    if (aq.count > 0) printf("Attendance Percentage: %.2f%%\n", (float)att / aq.count * 100.0f);
}

/* Re-enter an entire day's attendance (prompts day number and then presence for each student) */
void modifyAttendanceDay(void) {
    if (isQueueEmpty()) { printf("No attendance records to modify.\n"); return; }

    int day = readIntRange("Enter the day number to re-enter (1 - total days): ", 1, aq.count);

    /* Find circular index */
    int idx = aq.front;
    DayRecord* target = NULL;
    for (int c = 0; c < aq.count; ++c) {
        if (aq.days[idx].dayNumber == day) { target = &aq.days[idx]; break; }
        idx = (idx + 1) % MAX_DAYS;
    }
    if (!target) { printf("Invalid day number!\n"); return; }

    target->presentCount = 0;
    for (Student* cur = head; cur; cur = cur->next) {
        char prompt[128];
        snprintf(prompt, sizeof(prompt), "Was %s (Roll: %d) present? (1=Present, 0=Absent): ", cur->name, cur->rollNumber);
        int status = read01(prompt);
        if (status == 1 && target->presentCount < MAX_STUDENTS)
            target->presentStudents[target->presentCount++] = cur->rollNumber;
    }
    printf("Day %d attendance modified successfully!\n", day);
}

/* Modify single student's status on a specific day (uses validateRollPrompt for roll) */
void modifyAttendanceSingle(void) {
    if (isQueueEmpty()) { printf("No attendance records to modify.\n"); return; }

    int day = readIntRange("Enter day number to modify (1 - total days): ", 1, aq.count);

    int idx = aq.front;
    DayRecord* target = NULL;
    for (int c = 0; c < aq.count; ++c) {
        if (aq.days[idx].dayNumber == day) { target = &aq.days[idx]; break; }
        idx = (idx + 1) % MAX_DAYS;
    }
    if (!target) { printf("Invalid day number!\n"); return; }

    int roll = validateRollPrompt("Enter roll number to modify for that day: ");
    Student* s = searchStudent(roll);
    if (!s) { printf("Roll number not found.\n"); return; }

    /* Check presence */
    int pos = -1;
    for (int i = 0; i < target->presentCount; ++i) {
        if (target->presentStudents[i] == roll) { pos = i; break; }
    }

    int newStatus = read01("Enter new status (1 = Present, 0 = Absent): ");
    if (newStatus == 1 && pos == -1) {
        if (target->presentCount < MAX_STUDENTS)
            target->presentStudents[target->presentCount++] = roll;
        printf("Marked PRESENT for roll %d on Day %d.\n", roll, day);
    } else if (newStatus == 0 && pos != -1) {
        for (int i = pos; i < target->presentCount - 1; ++i)
            target->presentStudents[i] = target->presentStudents[i + 1];
        target->presentCount--;
        printf("Marked ABSENT for roll %d on Day %d.\n", roll, day);
    } else {
        printf("No change needed.\n");
    }
}
