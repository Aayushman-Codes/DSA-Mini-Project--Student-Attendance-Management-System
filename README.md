# Student Attendance Management System (C Project)

This is a menu-driven C program that manages student attendance using multiple data structures.  
It demonstrates the use of **Linked Lists**, **Circular Queues**, **Stacks**, and **Binary Search Trees (BSTs)** to store, modify, and analyze attendance data efficiently.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Data Structures Used](#data-structures-used)
- [Menu Options](#menu-options)
- [Compilation and Execution](#compilation-and-execution)
- [Input Validation](#input-validation)
- [Memory Management](#memory-management)
- [Example Output](#example-output)
- [Project Summary](#project-summary)
- [Author](#author)

---

## Overview

The **Student Attendance Management System** is designed to:
- Manage student records (add, delete, search, modify)
- Record attendance day by day
- Generate and display defaulter lists
- Quickly find students or attendance stats using a BST

It is a complete **Data Structures mini project in C**, combining multiple DS concepts into one coherent application.

---

## Features

### Linked List (Student Records)
- Add a new student (Roll number and Name)
- Delete a student
- Search a student by roll number
- Display all students
- Modify student details (name)

### Circular Queue (Attendance)
- Record daily attendance
- Display all attendance records
- View individual student attendance
- Modify attendance for a single student on a given day
- Re-enter or modify attendance for an entire day

### Stack (Defaulter List)
- Generate a list of students below a required attendance threshold
- Display the defaulter list (LIFO order)

### Binary Search Tree (BST)
- Build a BST from student records for fast searching
- Display all records (in-order traversal)
- Search for a student by roll number
- Display students with the minimum and maximum attendance
- Display students within a specified roll number range

---

## Data Structures Used

| Data Structure | Purpose | Description |
|----------------|----------|-------------|
| **Linked List** | Store student details | Dynamic insertion, deletion, and traversal |
| **Circular Queue** | Manage daily attendance | Stores attendance in FIFO order |
| **Stack** | Handle defaulters | Generates a LIFO list of students below threshold |
| **Binary Search Tree** | Fast searching and analysis | Used for efficient lookup and min/max queries |

---

## Menu Options

<img width="571" height="686" alt="image" src="https://github.com/user-attachments/assets/b1a450a1-218f-4802-a446-95d17c09479e" />


---

## Compilation and Execution

### Compile
```bash
gcc attendance_system.c -o attendance_system


./attendance_system

```

## Input Validation

- Roll numbers: Must be positive integers
- Names: Only alphabetic characters (A–Z, a–z) allowed (no spaces)
- Attendance input: Only 1 (Present) or 0 (Absent)
- Menu options: Must be valid integers within the displayed range
