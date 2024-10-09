#include <iostream>
#include <dlfcn.h>
#include <sys/stat.h>
#include <unistd.h>

int temp;
class MyClass {
public:
    virtual void greet();
};

// Function to get the last modification time of a file
time_t getLastModificationTime(const char* filename) {
    struct stat statbuf;
    if (stat(filename, &statbuf) == 0) {
        return statbuf.st_mtime;
    }
    return 0;
}

typedef MyClass* (*CreateMyClass)();
typedef void (*Greet)(MyClass*);

int main() {
    const char* libPath = "./libMyClass.so";
    time_t lastModTime = getLastModificationTime(libPath);
    void* handle = nullptr;
    CreateMyClass create = nullptr;

    while (true) {
        // Check for file modification
        time_t newModTime = getLastModificationTime(libPath);
        if (newModTime != lastModTime) {
            lastModTime = newModTime;

            // Reload the library
            if (handle) {
                dlclose(handle); // Close the old library
            }

            handle = dlopen(libPath, RTLD_LAZY);
            if (!handle) {
                std::cerr << "Cannot open library: " << dlerror() << std::endl;
                continue; // Skip to the next iteration
            }

            create = (CreateMyClass)dlsym(handle, "create");
            if (!create) {
                std::cerr << "Cannot load symbol 'create': " << dlerror() << std::endl;
                continue; // Skip to the next iteration
            }

            std::cout << "Library changed, reloading..." << std::endl;
        }

        // Use the dynamically loaded class
        if (create) {
            MyClass* myObject = create();
            myObject->greet();
            delete myObject;
        }



        sleep(1); // Check every second
    }

    // Cleanup (unreachable in this example)
    if (handle) {
        dlclose(handle);
    }
    return 0;
}
