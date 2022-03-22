Name: Cristian-Tanase Paris
Group: 331CAb

# Homework <NR> 1 (Multi-platform Development)

General structure
-

1. The basic functionality starts within the parse_file function. It reads
the input file (stdin or given in command line) and feeds the choose_action
method which is the core function of this HW.
2. The choose_action calls other functions based on the current line and it
can recursively call itself to solve nested code portions or different kind
of defines, includes and conditions within other files.
3. Each function that treats different kinds of directives can recursively
call other functions through choose_action which was previously mentioned.
4. The expand_str and replace_str (utils.c) functions are designed to replace
all occurences of a substring in a string, while handling the corner case in which
a recently substituted substring contains the exact needle we had to replace
(which would result in a buffer overflow by recursively calling again the
function). The expand_str also handles the " " inside string case (we should
not substitute any macro if it happens to lie inside the quotes). 
5. The linkedlist and hashmap have deep-copy capabilities. So there is no need
to dinamically allocate memory in the main function. Just pass the pointer of
your static object to your linkedlist/hashmap and it will automatically allocate
heap memory for it.
6. The hashmap has a max size of 10 buckets, and it can handle collisions via
chaining.
7. The parsing of command line arguments was implemented in such a way that
should handle all error cases of empty strings/values/files etc.
8. For certain memory allocations I've used the DIE macro and for the others
I've checked the return value and exited with return or exit().
9. There are no memory leaks and all the memtests are passed.


* I found this HW to be very useful because it teaches you how you should
carefully play with the pointers, strings, files and the program memory.
Also, learned how important memory safety really and why we should always
check the return values of system calls.
* I think this HW could have been solved a bit more efficiently (especially
for the #include and #if cases), because a true c-preprocessor makes use of
an AST tree for handling those cases.
* If there are multiple nested #if's, then the behaviour could be undefined.
* Cache locality could have been improved for checking certain things.
* Some string manipulations could have been done in a more direct way, but
I was afraid of buffer overflows.





Implementation
-

* The entire functionality is implemented.
* The coding style checker is horrible. Please fix it.
* The fact that the cl on windows is using c89 standard was a bit problematic.
* One interesting thing I found is that you can play as much as you want
with the memory if you are careful enough :).

How should I compile and run the program?
-
* If you run the executable with the help of the checker, just do
 make -f Makefile.checker. Otherwise, go in the src/ folder and execute make.
Now you should see an executable called ./so-cpp which takes command line arguments
as follows: ./so-cpp [-D SYMBOL[=MAPPING]] [-I DIR] [INFILE] [ [-o] OUTFILE]

Resources
-

* First year Data Structure lab-made linkedlist and hashmap. (I understand
that some colleagues may have similar implementations because the skel was
identical but this is my code - my implementation/tweaked and I have the code
base on my computer, so it is not a case of plagiarism).
* Google for official ISO C functions.
* Any other resource shoud have a source link attached in the code.
* https://www.cplusplus.com/reference/
* SO labs for the Makefiles.



Git
-
* Link:  [C-preprocessor](https://github.com/Cristi29P/c-preprocessor.git) - repo
should be public after 24th of March. Otherwise, please contact me on Teams or e-mail.
