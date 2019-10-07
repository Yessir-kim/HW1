# Homework 1
* Q: How to build and run the programs?

- A: Follow this step

1. Type "make clean" and then type "make" to build a program. 
2. The programs names are united as "puzzle". So type like "./puzzle <Input_file_name>". 
3. Then, the program will show the solution for <Input_file_name>.

- Example: As an example, I want to get a solution for "test" that is Inputfile name.

input:    
    
    20 23 30 29 34 6 9 21 19
    8 2 1 8 1 3 5 7 6 18
    9 1 4 2 5 6 3 1 7 28
    3 5 1 4 9 1 3 9 1 8
    8 6 6 3 5 1 1 4 1 4
    8 6 6 2 6 8 3 3 9 31
    8 7 8 8 4 5 2 1 1 18
    4 8 3 5 5 2 1 2 8 24

Step 1:

    
    s21600108@peace:~/DMclass$ 


Step 2:


    s21600108@peace:~/DMclass$ make clean
    rm *.o puzzle formula

    s21600108@peace:~/DMclass$ make 
    gcc -c -o number.o number.c
    gcc -o puzzle number.o


Step 3:


    s21600108@peace:~/DMclass$ ./puzzle test

    
output:    
    
    1 1 0 1 0 0 1 0 0
    0 1 1 0 1 0 0 0 0
    1 1 1 0 1 0 0 1 1
    1 1 1 1 1 1 1 0 1
    0 0 0 1 1 0 0 1 1
    0 1 1 1 0 0 1 1 0
    0 0 1 0 0 0 1 1 1
    1 1 1 1 0 1 0 1 0
    0 0 0 0 1 1 0 1 0
    
