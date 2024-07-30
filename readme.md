# IMT2022002 Siddharth Palod

To run the file, follow these steps:

1. Compile the code:
    ```
    gcc -o pds_own my_tester.c imt2022002_pds.c writer.c book.c bst.c
    ```

2. Run the executable with given test case input file:
    ```
    ./pds_own my_testcase.in
    ```

This program implements a system where the parent process acts as a writer and the child process acts as a book.
It's completely made in C language with dynamic functionalities and data-independence
Code can be easily modified to work for any one-one relationship like player-game , student-project etc. Due to pds being independant of the format 
we dont have to change much logic making this program dynamic

Demo Link: https://www.youtube.com/watch?v=tvT17MmvGs4
