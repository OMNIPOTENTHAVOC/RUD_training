#include <iostream>
#include <thread> // For std::this_thread::sleep_for
#include <chrono> // For std::chrono::seconds

// Function to print the binary representation of a number
void printBinary(int number) {
    for (int i = 7; i >= 0; --i) {
        std::cout << ((number >> i) & 1); // Shift the bits and mask with 1
    }
    std::cout << std::endl;
}

int main() {
    // Move motor 1 forward at half speed
    printBinary(127); // Motor 1 forward (half speed)
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for 2 seconds

    // Stop the motor
    printBinary(0); // Stop motor 1
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for 2 seconds

    // Move motor 1 in reverse at half speed
    printBinary(192); // Motor 1 reverse (half speed)
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for 2 seconds

    // Stop the motor again
    printBinary(0); // Stop motor 1
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for 2 seconds

    return 0;
}

