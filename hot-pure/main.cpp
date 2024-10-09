#include <iostream>
#include <dlfcn.h>
#include <sys/stat.h>
#include <unistd.h>

time_t getLastModificationTime(const char* filename) {
    struct stat statbuf;
    if (stat(filename, &statbuf) == 0) {
        return statbuf.st_mtime;
    }
    return 0;
}

int main() {
    const char* libPath = "./libmy_functions.so";
    time_t lastModTime = getLastModificationTime(libPath);
    void* handle = nullptr;

    typedef double (*FuncPtrDouble)();
    FuncPtrDouble getSharedVariable = nullptr;

    double lastValue = 0.0;

    while (true) {
        // Check for file modification
        time_t newModTime = getLastModificationTime(libPath);
        if (newModTime != lastModTime) {
            lastModTime = newModTime;

            // Reload the library
            if (handle) {
                dlclose(handle);
            }

            handle = dlopen(libPath, RTLD_LAZY);
            if (!handle) {
                std::cerr << "Cannot open library: " << dlerror() << std::endl;
                continue;
            }

            // Load symbols
            getSharedVariable = (FuncPtrDouble)dlsym(handle, "getSharedVariable");
            if (!getSharedVariable) {
                std::cerr << "Cannot load symbol: " << dlerror() << std::endl;
                continue;
            }

            // Get the new value of the shared variable
            double newValue = getSharedVariable();
            if (newValue != lastValue) {
                std::cout << "Variable changed: " << newValue << std::endl;
                lastValue = newValue; // Update the last value
            }
        }

        sleep(1); // Check every second
    }

    // Cleanup (unreachable in this example)
    if (handle) {
        dlclose(handle);
    }
    return 0;
}
