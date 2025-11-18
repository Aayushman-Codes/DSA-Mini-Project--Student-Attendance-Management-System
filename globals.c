#include "attendance.h"

Student* head = NULL;
AttendanceQueue aq = { .front = -1, .rear = -1, .count = 0 };
DefaulterStack ds = { .top = -1 };
BSTNode* bstRoot = NULL;
