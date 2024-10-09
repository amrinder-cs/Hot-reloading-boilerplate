#ifndef MYCLASS_H
#define MYCLASS_H
class MyClass {
public:
    virtual void greet();
};

// Function to create an instance of MyClass
extern "C" MyClass* create();

#endif // MYCLASS_H
