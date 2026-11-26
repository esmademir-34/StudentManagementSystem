# Student Management System

A modular student management system developed using the C programming language.

## About the Project

This project is a console-based student management system designed to manage student records efficiently. It uses a modular structure to separate student operations, file operations, and statistics.

## Features

- Add students
- List students
- Search students by student number or name
- Update student information
- Delete student records
- Filter students by department and class
- Sort students by GPA
- Display statistics
- Save and load student data using binary files
- Export student records to CSV

## Technologies

- C
- GCC Compiler
- Visual Studio Code

## Project Structure

```text
StudentManagementSystem/
├── main.c
├── student.c
├── student.h
├── file_operations.c
├── file_operations.h
├── statistics.c
└── statistics.h
```

## Compilation

```bash
gcc main.c student.c file_operations.c statistics.c -o StudentManagementSystem
```

## Version

v1.0 – Initial release
