# Activity 2: C++ Basics

[<-- back](prep-worksheet0.md#activities)

In this activity you will test your configured workspace by building and running a basic C++ program. First, explore the contents of this repo: 

## Explore the structure of this repository

Although you might have other Github configuration files, the most important files are those presented in the following structure:

```shell
.
├── src
│   └── add.h
│   └── add.cpp
│   └── main.cpp
├── tests
│   └── test_add.cpp
├── cpp-basics.md
├── csct-cloud-config.md
├── Makefile
├── prep-worksheet0.md
```

The ``src`` folder is intended for you to store the *source code* of your programs. Likewise, ``tests`` folder is to store the [*unit tests*](https://www.oreilly.com/library/view/modern-c-programming/9781941222423/f_0053.html).

The *Makefile* is the file used to configure the rules to compiling, linking, testing and ultimately, building your C++ program.

## Build and run the program

To build the program you just need to run, from the terminal, the command ``make`` this will build the program and create the executable. You might get some warnings if the program has not been fixed as that is your job. 

To fix the program go to [src/add.cpp](src/add.cpp) and complete the code for adding the two numbers `a` and `b`. Then save and run the `make` command again, warnings should have disappeared. 

No you are ready to run your program. The program just add two numbers passed as arguments, so you need to specify the numbers when running the program. For example you can run from the terminal:

```shell
./build/main 28 25
```

The output should be:

```shell
    The sum of 28 and 25 is 53.
```

Now, find out (or refresh) how this works, read through the following sections.

## Makefile Breakdown

The [Makefile](Makefile) in this repo is a simple template, with what is enough to compile our simple program. Let's break it down:

1. Compiler and Flags
   
   * CXX: Specifies the GNU C++ compiler (g++).
   * CXXFLAGS: Compiler options:
        * -std=c++17: Use the C++17 standard ([docs](https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html#C_002b_002b-Dialect-Options)).
        * -Wall and -Wextra: Enable common and extra warnings ([docs](https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html)).
   
2. Directories
   
   * SRC_DIR: Directory for source files.
   * TEST_DIR: Directory for test files.
   * BUILD_DIR: Directory for build output.

3. Targets
   
   * MAIN_EXEC: Output file for the main program.
   * TEST_EXEC: Output file for the test executable.

4. Source Files
   
   * ADD_SRC: Source file for the add module.
   * ADD_HDR: Header file for the add module.

5. Default Target:  Build Everything
   
    * The all target builds both the main program and the test executable.

6. Build Main Executable
   
    * mkdir -p $(BUILD_DIR): Creates the build directory if it doesn't exist.
    * $(CXX): Compiles the source files (main.cpp and add.cpp) with the specified flags.
    * -o $@ $(SRC_DIR)/main.cpp $(ADD_SRC): Specifies the output file ($@) and the source files (main.cpp and add.cpp) to compile.

7. Build Test Executable
    
    * Similar to the main executable target but compiles test_add.cpp instead of main.cpp.
  
8. Run Tests

    * ./$(TEST_EXEC): Executes the compiled test program.

9.  Clean Build Files
    
    * rm -rf $(BUILD_DIR): Deletes all files in the build directory recursively ([docs](https://man7.org/linux/man-pages/man1/rm.1.html))
    
10. Phony Targets
    * Declares all, test, and clean as "phony" targets.
    * Ensures these targets are always executed, even if files with the same names exist ([docs](https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html))


## Source Code

The ``src`` folder contains the files [src/add.h](src/add.h), [src/add.cpp](src/add.cpp) and [src/main.cpp](src/main.cpp).

* A header in C++ is a file that declares functions, classes, or variables to be shared across multiple source files, typically using the .h extension.

* A .cpp file in C++ is a source file that contains the implementation of functions, classes, or logic declared in header files, and it is compiled into object code.

### Inside add.h

This file contains the declaration of the ``add`` function

1. ``#ifndef ADD_H``:
   
   * Stands for "if not defined."
    * Checks if the macro ADD_H is not already defined to prevent multiple inclusions of the same header file (docs).

2. ``#define ADD_H``:
    
    * Defines the macro ADD_H to indicate that this header file has been included.
    * Ensures the file's contents are only included once during compilation (docs).
    ``int add(int a, int b);:``
   * Declares a function named add that takes two integers as arguments and returns their sum.

3. ``#endif``:
    
    * Ends the conditional directive started by ``#ifndef``.
    * Ensures the header's contents are only processed if ADD_H was not already defined.

### Inside add.cpp

This file contains the implementation of the ``add`` function.

1. ``#include "add.h"``:

   * Includes the add.h header file, described previously. The quotes ("add.h") indicate that the file is a user-defined header, typically located in the same directory.
  
2. ``int add(int a, int b)``:
 
    * Defines the add function, which takes two integers (a and b) as parameters and returns an integer.
  
3. ``return 0;``
    * A temporary return value (0) used as a placeholder. It is your job to implement this function properly!

### Inside main.cpp

This is the main executable of the program.

1. ``#include <iostream>``:

   * Includes the standard input/output library for using ``std::cout`` and ``std::cerr`` ([docs](https://en.cppreference.com/w/cpp/header/iostream)).

2. ``#include "add.h"``:

   * Same as before, requires the header.

3. ``int main(int argc, char* argv[])``:

    * The entry point of the program.
    * ``argc``: The number of command-line arguments.
    * ``argv``: An array of strings representing the arguments.

4. Argument Check:

   * Ensures the user provides exactly two arguments (besides the program name).
   * ``std::cerr``: Outputs an error message to the standard error stream.
   * ``return 1;``: Exits the program with an error code.

5. Convert Arguments to Integers

    * Converts the command-line arguments (strings) to integers using ``std::atoi`` ([docs](https://en.cppreference.com/w/cpp/string/byte/atoi))

6. Call the ``add`` Function:
   
    * Calls the ``add`` function to compute the sum of ``num1`` and ``num2``.

7. Display the result:

    * Outputs the result to the standard output stream using ``std::cout``.
  
8. ``return 0``
    
   * Indicates successful program execution.


## Tests

The ``tests`` folder contains the files [tests/test_add.cpp](tests/test_add.cpp). This program is a simple unit test for the add function. It uses assertions to verify that the function behaves correctly for various inputs.

1. ``#include <cassert>``:

   * Includes the C++ assertion library, which provides the assert macro.
   * ``assert`` is used to test conditions during runtime. If a condition is false, the program terminates with an error ([docs](https://en.cppreference.com/w/c/error/assert)).

2. ``#include <iostream>``:
   * Includes the standard input/output library for using std::cout to print messages to the console ([docs](https://en.cppreference.com/w/cpp/header/iostream)).

3. ``#include "../src/add.h"``:

   * Includes the add.h header file from the src directory.
  
4. ``void test_add()``:
   
    * A function that contains test cases to verify the correctness of the ``add`` function.

5. Test Cases with ``assert``:
   * Each assert checks if the ``add`` function produces the expected result for specific inputs.
   * For example, ``assert(add(2, 3) == 5)`` verifies that adding 2 and 3 returns 5.
   * If any assertion fails (i.e., the condition is false), the program terminates and reports the failed assertion.

6. ``std::cout << "All tests passed!" << std::endl;``:

    * If all assertions pass, this message is printed to indicate that all test cases were successful.

7. ``int main()``:
   
    * The entry point of the program. Calls the test_add function to run the test cases.

8. ``return 0;``:
    
    * Indicates successful program execution.


## Still more time?

* Try to complete your program, creating a basic calculator with functions for multiplication, subtraction and division. Make sure you create the corresponding unit tests.


[<-- back]prep-worksheet0.md#activities)