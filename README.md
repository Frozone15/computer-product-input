# computer-product-input
First Project in GitHub!

This project is done when I was a new student in Universitas Multimedia Nusantara, it takes me about 1 weeks for do all of this.
Project for portofolio.

## PC Store Inventory Management System
A CLI (Command Line Interface) based simulation of a custom PC store inventory management system. Built in C, this program serves as a practical implementation of dynamic Singly Linked List manipulation, data sorting, permanent file processing, and a real-time bilingual interface (i18n).

## Key Features
1. Inventory Management (Linked List)
All hardware components are stored dynamically in the computer's RAM using a Singly Linked List data structure:

Create: Dynamically allocates a new node when registering a new item.

Read: Traverses the linked list to display a formatted table of all inventory.

Update: Modifies the stock value of a specific targeted node.

2. File Processing
Data persists across sessions by automatically synchronizing with gudang.txt:

Read Mode (r): On startup, the program reads the text file line-by-line using fscanf() and rebuilds the Linked List inside the RAM.

Append Mode (a): When adding a new item, the system only appends a single new line to the very bottom of the text file without rewriting existing data.

Write Mode (w): When updating stock, the system updates the RAM value and completely overwrites (w) the text file to ensure data accuracy.

3. Multi-Criteria Sorting
Implements a modified Bubble Sort tailored for Linked Lists (swaps data payloads rather than node pointers to eliminate the risk of Segmentation Faults):

Sort by Lowest Price: Orders items from cheapest to most expensive (Ascending).

Sort by Lowest Stock: Orders items by remaining stock (extremely useful for store owners prioritizing restocks).

4. Real-Time Bilingual Interface (i18n)
The interface natively supports two languages that can be toggled on the fly:

🇮🇩 Indonesian

🇬🇧 English

Engineered using C Preprocessor Macros (#define) and Ternary Operators to prevent code duplication (Anti-Spaghetti Code).

## Installation & Getting Started
1. Database File Setup
Before compiling, ensure a file named gudang.txt exists in the same directory as your C source file. Populate it with sample data (comma-separated, no spaces after commas).
(Note: If this file is missing, the program will safely generate a new, empty file upon execution).

2. Compilation (Terminal / CMD)
Compile the code using GCC.

## Development Notes (Memory Management)
This program strictly adheres to Zero Memory Leak principles. Upon selecting the exit option (Menu 7), the application triggers a freeList() subroutine that loops through the entire heap memory allocation, safely calling free() on every existing node before the process terminates.
