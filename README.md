# Student Attendance Management System (C Project)

This is a menu-driven C program that manages student attendance using multiple data structures.  
The project is organized into **six modular C files**, improving readability, maintainability, and separation of concerns.

It demonstrates the use of **Linked Lists**, **Circular Queues**, **Stacks**, and **Binary Search Trees (BSTs)** to store, modify, and analyze attendance data efficiently.

---

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Data Structures Used](#data-structures-used)
- [File Structure](#file-structure)
- [Menu Options](#menu-options)
- [Compilation and Execution](#compilation-and-execution)
- [Input Validation](#input-validation)

---

## Overview

The **Student Attendance Management System** allows you to:

- Maintain student records
- Record daily attendance
- Modify and analyze attendance
- Generate defaulter lists
- Use a BST for fast searching and attendance statistics

This project is a complete **Data Structures mini project in C**, now modularized into separate source files.

---

## Features

### Linked List (Student Records)
- Add a new student (Roll number + Name)
- Delete a student
- Search by roll number
- Display all students
- Modify student names  
- Students are stored in sorted order

### Circular Queue (Attendance)
- Record attendance for each day
- Display attendance for all days
- Show attendance for a single student
- Modify attendance:
  - For a specific student on a day
  - Re-enter attendance for an entire day

### Stack (Defaulter List)
- Generate a list of students below a required attendance threshold
- Display defaulters (LIFO order)

### Binary Search Tree (BST)
- Build a BST from student list + attendance
- Fast search by roll number
- Display all students in sorted order (in-order traversal)
- Show minimum and maximum attendance
- Display students within a roll number range

---

## Data Structures Used

| Data Structure        | Purpose                        | Description                                   |
|----------------------|--------------------------------|-----------------------------------------------|
| **Linked List**      | Student database               | Sorted insertion, search, modify, traversal   |
| **Circular Queue**   | Attendance system              | Stores daily attendance in FIFO manner        |
| **Stack**            | Defaulters                     | LIFO storage for under-threshold students     |
| **Binary Search Tree (BST)** | Fast roll lookups + analysis | Efficient search, min/max, range queries |

---

## File Structure

The project is modularized into the following 6 files:

├── attendance.h # Struct definitions, function prototypes, global declarations
├── globals.c # Definitions of global variables
├── linkedlist.c # Student linked list operations
├── queue.c # Circular queue for attendance
├── bst_stack.c # Defaulter stack + BST implementation
├── main.c # Main menu system + program entry point


This separation of logic makes the code easier to maintain and understand.

---

## Menu Options

<img width="571" height="686" alt="Menu Screenshot" src="https://github.com/user-attachments/assets/b1a450a1-218f-4802-a446-95d17c09479e" />

---

## Compilation and Execution

Since the project is modular, you must compile **all six C files together**.

### Compile:
```bash
gcc main.c linkedlist.c queue.c bst_stack.c globals.c -o attendance_system
```

### Run:
```bash
./attendance_system
```

## Input Validation

- Roll numbers: Must be positive integers
- Names: Only alphabetic characters (A–Z, a–z) allowed (no spaces)
- Attendance input: Only 1 (Present) or 0 (Absent)
- Menu options: Must be valid integers within the displayed range


## Versioning

This project maintains two major versions to show the evolution from a single-file implementation to a fully modular data-structures-based system.

### **v1 — Single File Version**
- Entire attendance system implemented in **one C file**
- Basic functionality demonstrating linked lists, queues, stacks, and simple search
- Useful as an introductory version or baseline for comparison

### **v2 — Modular 6-File Version (Current Version)**
- Complete refactor into **six separate C files**:
  - `main.c`
  - `linkedlist.c`
  - `queue.c`
  - `bst_stack.c`
  - `globals.c`
  - `attendance.h`
- Improved readability, maintainability, and separation of concerns
- Clear boundaries between data structures and logic
- Same functionality as v1, but structured professionally

### **How to Access Versions on GitHub**
Each version is stored as a **Git tag**:

- `v1` → Old single-file implementation  
- `v2` → Modular 6-file project
- `v4` → Updated files and made required changes   

You can switch between them on GitHub by navigating to:

**Repository → Tags → Select version**

