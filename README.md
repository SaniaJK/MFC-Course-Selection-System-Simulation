# MFC Course Selection System Simulation (MFC & C++)

## Introduction

This repository contains the source code for a comprehensive **MFC Course Selection System Simulation**, developed as a curriculum project for Object-Oriented Programming (OOP).

The system is built using **C++** and the **Microsoft Foundation Classes (MFC)** framework. It features a robust multi-role architecture that supports Students, Teachers, and Administrators, strictly adhering to OOP principles such as encapsulation, inheritance, and polymorphism.

## Project Architecture

The system follows a layered architecture to ensure high cohesion and low coupling:

* **View Layer**: Implemented using MFC (Dialog-based) for the graphical user interface and a separate Win32 Console application for logic verification.
* **Controller/Logic Layer**: The `DataManager` class acts as the central controller, managing data flow and user authentication.
* **Model Layer**: Core business logic encapsulated in `Student` and `Course` classes.

### Class Hierarchy
* **Student (Abstract Base Class)**: Defines common interfaces for course selection and tuition calculation.
* **UGstudent (Derived)**: Implements tuition calculation logic for Undergraduate students.
* **Gstudent (Derived)**: Implements tuition calculation logic for Graduate students.
* **Course**: Manages course attributes and static enrollment statistics.

## Key Features

### 1. Multi-Role Access Control (RBAC)
The system supports three distinct user roles with isolated functionalities:
* **Student**: View available courses, select/drop courses, and view real-time tuition fees.
* **Teacher**: View the list of supervised students and their detailed course enrollments.
* **Administrator**: Global search for all students and capability to force-drop courses for any student.

### 2. Polymorphic Tuition Calculation
Tuition fees are calculated dynamically based on the student type using C++ polymorphism (Virtual Functions):
* **Undergraduates**: Different rates for Subject Base, Major Base, and Major Core courses.
* **Graduates**: Higher rates compared to undergraduates for specific course types.

### 3. Advanced Search & Filtering
A dual-filter mechanism is implemented for handling large datasets:
* **Permission-based Filtering**: Teachers can only search within their supervised group.
* **Keyword Filtering**: Supports fuzzy search by Student Name or Student ID.

### 4. Data Safety & Validation
* **Constraint Checking**: Prevents selecting more than 5 courses, selecting full courses, or duplicate selections.
* **Encoding Support**: Utilizes ATL/CA2W macros to handle multi-byte character sets (GBK) correctly, preventing display issues in the UI.

## Technology Stack

* **Language**: C++ (Standard 11/14)
* **Framework**: Microsoft Foundation Classes (MFC)
* **IDE**: Visual Studio 2022
* **Key Concepts**: Inheritance, Polymorphism, STL (vector, map), RTTI, Singleton Pattern.

## Getting Started

### Prerequisites
* Microsoft Visual Studio (2019 or later recommended).
* "Desktop development with C++" workload installed.
* MFC and ATL support enabled in Visual Studio Installer.

### Installation & Build
1.  Clone the repository:
    ```bash
    git clone [https://github.com/SaniaJK/MFC-Course-Selection-System-Simulation.git](https://github.com/SaniaJK/MFC-Course-Selection-System-Simulation.git)
    ```
2.  Open the solution file `CourseSystemMFC.sln` in Visual Studio.
3.  Set the solution configuration to `Debug` or `Release` (x86/x64).
4.  Build the solution (Ctrl+Shift+B).
5.  Run the application (F5).

## Usage Guide

### Login Credentials
* **Administrator**: Enter `ADMIN`
* **Teacher**: Enter `T001`,`T002`...
* **Undergraduate**: Enter `2014001`,`2014002`...
* **Graduate**: Enter `2014101`,`2014102`...

## Future Improvements

* **Persistence**: Integrate SQLite or file I/O to save registration data permanently.
* **Networking**: Migrate to a Client-Server architecture using TCP/IP sockets for concurrent access.
* **UI Modernization**: Upgrade the user interface using the Qt framework.

## Author

* **Name**: [SäniaJK]
* **Role**: Lead Developer & Architect
* **Date**: December 2025
* **Name**: [说点道理]
* **Role**: OOP Design Contributor & Documentation Engineer
* **Date**: December 2025
---
