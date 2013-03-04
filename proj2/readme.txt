CS61C Fall 2012 Project 2
MIPS Instruction Set Simulator
 
Part 1 due:  09/30@23:59:59 PST
Part 2 due:  10/07@23:59:59 PST
 
TA: James Ferguson (Framework adapted from that created by Eric Liang)
Background
In this project, you will create an instruction interpreter for a subset of the MIPS instruction set. You’ll provide the machinery to decode and execute a couple dozen MIPS instructions.  You're creating what is effectively a miniature version of MARS, but better yet, you’ll be able to run real C programs on your MIPS simulator!
 
The MIPS green sheet provides information necessary for completing this project.
Getting started
This is an individual assignment.  All of your work must be yours alone.
 
We've provided you with some skeleton code in the repo ~cs61c/proj/fa12/02.
 
$ cd ~/work
$ git pull ~cs61c/proj/fa12/02 master
 
This directory includes the following files.  You’ll only need to modify the files colored blue.  You might find it helpful to look at the files colored black.  You definitely do not need to look at the files colored red.
 
sim.c: C source file which contains the top level simulator.  This file contains the main function.
processor.c: C source file which implements the processor. (Part 2)
processor.h: C header file for processor.
disassemble.c: C source file which implements disassemble. (Part 1)
disassemble.h: C header file for disassembler.
memory.c: C source file which implements memory. (Part 2)
memory.h: C header file for memory.
load_program.c: C source file which implements loading an executable file.
load_program.h: C header file for program loader.
elf.h: C header file for ELF definitions. (ELF is the standard executable file format in Linux.)
Makefile: File which records all dependencies.
mipscode/*:  Assembly and C source files for tests.
Project
The files provided in the start kit comprise a framework for a MIPS simulator. You’ll complete the program by adding code to processor.c to decode and execute each instruction.  Additionally, you’ll add code to disassemble.c to print out the human-readable disassembly corresponding to the instruction’s machine code. Your simulator must be able to simulate the machine code versions of the following MIPS machine instructions.  We’ve already implemented the pink-colored instructions for you, so you have an example of an R-type, I-type, and J-type instruction.
 
INSTRUCTION
TYPE
OPCODE
FUNCT
OPERATION
sll rd,rt,shamt
R
0x0
0x0
R[rd] <- R[rt] << shamt
srl rd,rt,shamt
R
0x0
0x2
R[rd] <- R[rt] >> shamt
sra rd,rt,shamt
R
0x0
0x3
R[rd] <- (signed)R[rt] >> shamt
jr rs
R
0x0
0x8
PC <- R[rs]
jalr rd,rs
R
0x0
0x9
tmp <- PC + 4
PC <- R[rs]
R[rd] <- tmp
syscall
R
0x0
0xc
(perform system call)
addu rd,rs,rt
R
0x0
0x21
R[rd] <- R[rs] + R[rt]
subu rd,rs,rt
R
0x0
0x23
R[rd] <- R[rs] - R[rt]
and rd,rs,rt
R
0x0
0x24
R[rd] <- R[rs] & R[rt]
or rd,rs,rt
R
0x0
0x25
R[rd] <- R[rs] | R[rt]
xor rd,rs,rt
R
0x0
0x26
R[rd] <- R[rs] ^ R[rt]
nor rd,rs,rt
R
0x0
0x27
R[rd] <- ~(R[rs] | R[rt])
slt rd,rs,rt
R
0x0
0x2a
R[rd] <- (signed)R[rs] < (signed)R[rt]
sltu rd,rs,rt
R
0x0
0x2b
R[rd] <- R[rs] < R[rt]
j address
J
0x2
-
PC <- {(upper 4 bits of PC+4), address*4}
jal address
J
0x3
-
R[31] <- PC + 4
PC <- {(upper 4 bits of PC+4), address*4}
beq rs,rt,offset
I
0x4
-
if(R[rs] == R[rt])
 PC <- PC + 4 + signext(offset)*4
bne rs,rt,offset
I
0x5
-
if(R[rs] != R[rt])
 PC <- PC + 4 + signext(offset)*4
addiu rt,rs,imm
I
0x9
-
R[rt] <- R[rs] + signext(imm)
slti rt,rs,imm
I
0xa
-
R[rt] <- (signed)R[rs] < signext(imm)
sltiu rt,rs,imm
I
0xb
-
R[rt] <- R[rs] < signext(imm)
andi rt,rs,imm
I
0xc
-
R[rt] <- R[rs] & zeroext(imm)
ori rt,rs,imm
I
0xd
-
R[rt] <- R[rs] | zeroext(imm)
xori rt,rs,imm
I
0xe
-
R[rt] <- R[rs] ^ zeroext(imm)
lui rt,imm
I
0xf
-
R[rt] <- imm << 16
lb rt,offset(rs)
I
0x20
-
R[rt] <- signext(LoadMem(R[rs]+signext(offset), byte))
lh rt,offset(rs)
I
0x21
-
R[rt] <- signext(LoadMem(R[rs]+signext(offset), half))
lw rt,offset(rs)
I
0x23
-
R[rt] <- LoadMem(R[rs]+signext(offset), word)
lbu rt,offset(rs)
I
0x24
-
R[rt] <- zeroext(LoadMem(R[rs]+signext(offset), byte))
lhu rt,offset(rs)
I
0x25
-
R[rt] <-
zeroext(LoadMem(R[rs]+signext(offset), half))
sb rt,offset(rs)
I
0x28
-
StoreMem(R[rs]+signext(offset)], byte, R[rt])
sh rt,offset(rs)
I
0x29
-
StoreMem(R[rs]+signext(offset)], half, R[rt])
sw rt,offset(rs)
I
0x2b
-
StoreMem(R[rs]+signext(offset)], word, R[rt])
If you have any questions about the semantics of the instructions, please consult your textbook and the internet before posting to piazza.
 
 
In our variant of MIPS, beq and bne are not delayed branches.  (If you don’t know what that means, just ignore it; your default assumption about the behavior of branches is probably correct for this project.  We’ll talk about this topic later in the semester.)
Initially, you will be able to run a program called “simple”, which only uses the pink colored instructions, by running the following commands:
 
$ cd ~/work/proj2
$ make
$ ./mips-sim mipscode/simple
Hello, world!
exiting the simulator
Framework code
The framework code we’ve provided begins by doing the following.
It reads the executable program binary, whose filename is specified on the command line, into the simulated memory, starting at address 0x00001000.
It initializes all 32 MIPS registers to 0 and sets the program counter (PC) to 0x00001000.
It sets flags that govern how the program interacts with the user.  Depending on the options specified on the command line, the simulator will either show a dissassembly dump of the program on the command line, or it will execute the program.
 
It then enters the main simulation loop, which simply calls execute_one_inst() repeatedly until the simulation is complete.  execute_one_inst() performs the following tasks:
It fetches an instruction from memory, using the PC as the address.
It examines the opcode to determine what instruction was fetched.
It executes the instruction and updates the PC.
 
The framework supports a handful of command-line options:
-i runs the simulator in “interactive mode,” in which the simulator executes an instruction each time the Enter key is pressed.  The disassembly of each executed instruction is printed.
-r instructs the simulator to print the contents of all 32 registers after each instruction is executed.  It’s most useful when combined with the -i flag.
-t runs the simulator in “tracing mode,” in which each instruction executed is printed.
-d instructs the simulator to disassemble the entire program, then quit.
Your Job
Your job is ultimately to complete the implementations of the disassemble() function in disassemble.c and the execute_one_inst() function in processor.c as well as the store_mem(), load_mem(), and access_ok() functions in memory.c.  Right now, they only operate correctly for the or, ori, j, and syscall instructions.  By the time you’re finished, they should handle all of the instructions in the table above.
 
Part 1 (due 9/30)
For part 1, you will be implementing the disassembler.
Print the instruction name.  If the instruction has arguments, print a tab (\t).
Print all arguments, following the order and formatting given in the INSTRUCTION column of the table above.
Arguments are generally comma-separated (lw/sw, however, also use parentheses), but are not separated by spaces.
Register arguments are printed as a $ followed by the register number, in decimal (e.g. $0 or $31).
Zero-extended immedates (e.g. for ori) are printed as lowercase hexadecimal numbers with a leading 0x but without extra leading zeros (e.g. 0x0 or 0xffff).
Jump addresses should be printed like zero-extended immediates, but multiplied by 4 first.  (Assume the upper 4 bits of PC+4 are zero.)
Sign-extended immediates (e.g. for addiu, sw, or beq) are printed as signed decimal numbers (e.g. -12 or 48).  For branch offsets, multiply the offset by 4.
Shift amounts (e.g. for sll) are printed as unsigned decimal numbers (e.g. 0 or 31).
Print a newline (\n) at the end of an instruction.
We will be using an autograder to grade this task.  If your output differs from ours due to formatting errors, you will not receive credit.
We have provided a disassembly test for you.  Since a test is only covering a subset of possible senarios, however, passing this test does not mean that your code is bug free.  You should identify the corner cases and test them yourself.
 
Run the disassembly test by typing in “make disasmtest”.  If your disassembly matches the output, you will get a “DISASSEMBLY TEST PASSED!” message.
 
$ cd ~/work/proj2
$ make disasmtest
./mips-sim -d mipscode/insts > insts.dump
DISASSEMBLY TEST PASSED!
 
If your disassembly does not match the output, you will get the difference between the reference output and your output, and a “DISASSEMBLY TEST FAILED!” message.  Make sure you pass this test before submitting your first task.
 
$ cd ~/work/proj2
$ make disasmtest
./mips-sim -d mipscode/insts > insts.dump
56c56
< 000010dc: addiu        $30,$0,0x11
---
> 000010dc: addiu        $30,$0,11
66c66
< 00001104: bne        $4,$5, 44
---
> 00001104: bne        $4,$5,44
DISASSEMBLY TEST FAILED!
 
STOP.  You have now done enough work for part 1, due on the 30th. For the part 1, push your submission in the directory proj2 with the tag proj2-1.
 
To reiterate, your submission for part 1 should include proj2/disassemble.c (along with the other files provided in the skeleton) and be tagged as proj2-1. (If you do not follow these instructions you will not get credit for this part of the project)
 
 
Part 2 (due 10/7)
If you have not been checking piazza, there is a FAQ there that contains some clarifications about how memory works, as well as some other frequently asked questions that might be useful for you.
 
Please only modify disassemble.c/h, processor.c/h, and memory.c/h. If you have need of other helper functions please add them to one of those files, as they are the only ones that will be copied over from your submission. If you submitted other files for part 1, and as a result of not including them in compilation, your code does not compile, please email me at jferguson@berkeley.com. There will be no penalty for doing so because this was not made clear earlier, but I will have to modify the Makefile in order to run these submissions.
 
Lastly, sltiu is supposed to treat both of its operands as unsigned, however, we have been telling people to only treat R[rs] as unsigned. Because this is a late update, we will be accepting either implementation as correct for sltiu.
 
Your second task will be to actually implement the remaining instructions in the processor.
For part 2, you will be implementing the remaining functions mentioned above: execute_one_inst in processor.c, and store_mem, load_mem, and access_ok from memory.c.
 
execute_one_inst - grabs the next instruction from the address stored in the PC and simulates running it, updating the appropriate registers and making any necessary calls to memory. 
access_ok - takes the address of a memory access and the size of the value being returned and returns 1 if it is a valid access or 0 if not (explained in the next paragraph). 
store_mem - Takes an address, a size, and a value and stores the first -size- bytes of the given value at the given address. 
load_mem - Takes an address and a size and returns the next -size- bytes starting at the given address 
  
Memory is little endian. What this means is that the least significant byte of a word is stored at a lower address than the most significant byte. There are further examples/clarifications of this on piazza.
 
One issue we have not addressed thus far is address alignment for loads and stores.  As mentioned earlier, the simulator will abort if the PC is misaligned (i.e. not divisible by 4), or outside of the range [1,MEM_SIZE).  The same behavior should occur for misaligned loads and stores.  lw and sw require 4-byte alignment, like the PC.  lh, lhu, and sh require 2-byte alignment.  lb, lbu, and sb are always properly aligned.  For misaligned or out-of-range addresses (this includes the NULL address 0), you must have access_ok() return 0 in memory.c, which will cause the simulator to print an error message and abort. This is analogous to the segmentation fault you may see when an x86 program does an illegal memory access.

Testing
 
We have provided a simple self-checking assembly test that tests several of the instructions.  However, the test is not exhaustive and does not exercise every instruction.  Here’s how to run the test.  (The output is from a working processor.)
 
$ cd ~/work/proj2
$ make runtest
./mips-sim -r mipscode/insts > insts.trace
RUN TEST PASSED!
 
We have provided a few more tests and the ability to write your own.  We described simple above, which prints out “Hello, world!”, and is written in assembly. Here’s an example of how to do so:
 
Create the new assembly file in the mipscode directory.  (Use mipscode/simple.s as a template.)  Let’s assume your test is in mipscode/foo.s.
Add the base name of the test to the list of ASM_TESTS in the Makefile.  To do this, just add foo to the end of line 4.
 
Now build your assembly test, and then run it by typing in the following commands:
 
$ cd ~/work/proj2
$ make
$ ./mips-sim mipscode/foo
 
You can, and indeed should, write your own assembly tests to test specific instructions and their corner cases. Furthermore, you should be compiling and testing your code after each group of instructions you implement. It will be very hard to debug your project if you wait until the end to test. 
 
Additionally, you can write test programs in C and run them on your simulator, once you’ve implemented all of the instructions!  We provided ackermann, which computes the Ackermann function A(m,n) recursively.  It will not work until you have implemented the processor fully.  This is a sample run of the program on a correct processor:
 
$ ./mips-sim mipscode/ackermann
A(3, 3) = 61
exiting the simulator
 
To write your own C tests, stick to a subset of C that doesn’t use floating-point.  Additionally, don’t use any C library functions.  Look at ackermann.c for an example of how to print things in our restricted environment.  Here’s how to do write a C test:
 
Create the new C file in the mipscode directory.  (Use mipscode/ackermann.c as a template.)  Let’s assume your test is in mipscode/bar.c.
Add the base name of the test to the list of C_TESTS in the Makefile.  To do this, just add bar to the end of line 5.
 
Now build your C program, and then run it by typing in the following commands:
 
$ cd ~/work/proj2
$ make
$ ./mips-sim mipscode/bar
 
Grading
Part 1 - 15 points 
Part 2 - 85 points: 
    -runtest - 15 points 
    -Remaining 70 points will be distributed evenly across a set of tests each of which is designed to test one or two specific instructions, so each correct instruction is worth roughly the same amount. 
Note that non-compiling code will recieve 0 points. 
If you haven't done so already, please read the announcements at the beginning of the section for part 2 and the FAQ on piazza to make sure you are doing everything (namely memory) in the right way.
Push your final submission in the directory proj2 with the tag proj2-2. Your submission should include your modified disassemble.c, processor.c, and memory.c
 
 