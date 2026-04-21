# UFCFVK-15-2 Internet of Things - Worksheet 1 Journal

## 2026-02-10

Today I started working on Activity 1.1 by reading the assignment specification and understanding how the Hardware Abstraction Layer (HAL) works. My main task was to create a GPSSensor class that interfaces with the CSVHALManager to read simulated GPS data. I studied the ISensor interface and understood that I needed to implement getId(), getDimension(), and format() methods. I also updated ebikeClient.cpp to connect the sensor and print readings. I encountered an issue understanding how the HAL reads CSV data, so I consulted the prep worksheet examples in /opt/iot/include/hal/. I learned that the HAL uses port IDs and dimensions to determine which columns to read from the CSV file.

*Reflection: Initially I tried to implement the sensor without fully reading the HAL documentation. Next time I would read all provided interfaces first before writing any code.*

## 2026-02-17

Today I worked on implementing the GPSSensor class and connecting it to the HAL. I discovered that the HAL constructor only takes the number of ports as an argument, and the CSV file is loaded separately via initialise(). This was different from what I initially assumed. I also found that the format() method receives raw bytes from the HAL and needs to parse them as semicolon-separated values. I debugged this by writing small test programs to understand what the HAL returns for different port and dimension combinations. I found that using dimension 2 with the correct port returns two values separated by semicolons representing lat and lon. Resources: HAL header files in /opt/iot/include/hal/, prep worksheet examples.

## 2026-03-01

Today I completed Activity 1.2 by writing unit tests using the Catch2 framework. I created GPSSensorTest.cpp with two test cases covering sensor ID/dimensions and GPS format validation. I also created ebikeClientTest.cpp covering HAL port management and log output format. The main challenge was that calling hal.read() directly in tests caused Column index out of range exceptions. I solved this by testing the format() method directly with known raw byte inputs instead of going through the HAL, which also makes the tests more focused and reliable. I learned that unit tests should isolate the component being tested rather than testing the full integration stack.