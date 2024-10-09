#include <iostream>
#include "MyClass.h"

void MyClass::greet() {
    std::cout << "Functasfasfion 1!" << std::endl;
}

// Implementation of the create function
extern "C" MyClass* create() {
    return new MyClass();
}
