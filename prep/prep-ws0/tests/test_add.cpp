#include <cassert>
#include <iostream>
#include "../src/add.h"

void test_add() {
    // Test cases
    assert(add(2, 3) == 5);
    assert(add(-1, 1) == 0);
    assert(add(0, 0) == 0);
    assert(add(-5, -5) == -10);
    assert(add(100, 200) == 300);

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    test_add();
    return 0;
}