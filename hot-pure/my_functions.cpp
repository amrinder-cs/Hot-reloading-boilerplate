#include <iostream>

// Define a variable that will change with each compilation
extern "C" double sharedVariable = 0.5; // Initialize it

extern "C" double getSharedVariable() {
    return sharedVariable;
}
