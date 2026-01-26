## Activity 3: Unit Tests

[<--back](./prep-worksheet1.md)

Unit tests are essential for verifying the correctness of individual components in your code. They help ensure that each function and class behaves as expected in isolation. 

In this activity, you will use a simple framework to define unit tests. [Catch2](https://github.com/catchorg/Catch2) is a header-only C++ testing framework that simplifies writing and running unit tests. It provides a set of macros for defining test cases, sections, and assertions, making tests easy to read and maintain. 


1) Follow the link to read more about [Catch2](https://github.com/catchorg/Catch2).

2) Have a look to the examples provided in Catch2 website and also the [``tests\test_CSVHALManager.cpp``](./tests/test_CSVHALManager.cpp) file provided. Then implement the unit tests for the `TemperatureSensor` in the  `test_TemperatureSensor.cpp` file.

3) Build the unit tests executable by running ``make test``. This will run the tests and display the results in the console.

The output should be:

```
t
[CSVHALManager] Device attached to port 0.

[CSVHALManager] Device attached to port 0.

[CSVHALManager] Device released from port 0.

===============================================================================
All tests passed (2 assertions in 1 test case)

===============================================================================
All tests passed (4 assertions in 1 test case)

```


[<--back](./prep-worksheet1.md)
