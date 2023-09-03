#include <iostream>
#include <string>
#include <memory>

class Io {
private:
    int *array;
    int size;

public:
    Io(int argc, char *argv[]) {
        size = argc - 1;
        array = new int[size];

        for (int i = 1; i < argc; ++i) {
            array[i - 1] = std::stoi(argv[i]);
        }
    }

    ~Io() {
        delete[] array;
    }
};
