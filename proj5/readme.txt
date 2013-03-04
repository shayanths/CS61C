CS 61C Fall 2012 Project 5: Matrix Multiply Parallelization Extra Credit
TA: Anirudh Garg, Loc Do
Due Sunday, Nov 18, 2012 at 11:59PM (Extra Credit so there will be no late submission)
This project is optional but you can get up to 5% of your grade so we encourage you to do this project!

Getting Started
This project extends project 3-2. In particular, you will improve the matrix multiplication performance more than what you have done in project 3-2. You can re-use all the files you did in project 3-2

For this project, you will be required to work in groups of two. You are not allowed to show your code to other students, teaching assistants, professors, loved ones, pets, or any other individuals. You are to write and debug your own code in groups of 2. Looking at solutions from previous semesters is also strictly prohibited.

We will grade your average performance over a range of tall and skinny matrices with m=[1000,10000] and n=[32,100]. We will try strange matrix sizes, such of powers-of-2 and primes...so be prepared. Any valid solution that achieves an average performance of more than 50Gflop/s without using prohibited optimizations (aligned loads and stores, the fact that we are doing AA') will receive credits.

Testing
We will be disabling remote logins on a few of the machines early in this coming week to allow for reliable testing of your matrix multiply code.

Submission and Grading
Place your solution in a file called sgemm-openmp.c and your partner's login into MYPARTNER.txt, and put into a folder proj5. Test it using make bench-openmp. Submit by pushing to the repo with the tag proj5

This is the last project so you should know how to turn in and tag properly. We will not deal with wrong tags and wrong submissions again like proj3 (which delayed our grading process). Any problem that prevents us from pulling and compiling your codes automatically will not get any point

Since this is an extra credit, we will not accept any late submission. No partial credit after Sunday

Matrix Range Grading
For every 2 Gflop/s above 50 Gflop/s, you will get 1 point. The highest points in class will get full 5% of the total grade (That's a lot!). The immediate Gflop/s in between 50 Glop/s and the highest will be scaled linearly