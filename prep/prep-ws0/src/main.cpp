#include <iostream>
#include "add.h"

int main(int argc, char* argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <num1> <num2>" << std::endl;
        return 1; // Exit with an error code
    }

    // Convert command-line arguments to integers
    int num1 = std::atoi(argv[1]);
    int num2 = std::atoi(argv[2]);

    // Call the add function and display the result
    int result = add(num1, num2);
    std::cout << "The sum of " << num1 << " and " << num2 << " is " << result << "." << std::endl;

    return 0; // Exit successfully
}