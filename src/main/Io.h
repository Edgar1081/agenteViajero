#include <sstream>
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <memory>

class Io {
    private:
        std::stringstream text;
        int size;
        int* instance;

    public:
        Io(const char* filename) {
            std::fstream file(filename);
            if (!file.is_open()) {
                std::cerr << "Failed to open the file: " << filename << std::endl;
                exit(1);
            }
            std::string line;
            while (std::getline(file, line)) {
                text << line;
            }

            int length = 0;
            std::string token;
            while (getline(text, token, ',')) {
                length++;
            }
            int* sol = new int[length];

            text.clear();
            text.seekg(0, std::ios::beg);

            int i = 0;
            while (std::getline(text, token, ',')) {
                int idVertice = std::stoi(token);
                sol[i++] = idVertice;
            }
            size = length;
            instance = sol;
            file.close();
        }

        static int * to_array(std::string& input){
            int length= 0;
            std::stringstream text;
            text << input;
            std::string token;
            while (getline(text, token, ',')) {
                length++;
            }
            int* sol = new int[length];

            text.clear();
            text.seekg(0, std::ios::beg);

            int i = 0;
            while (std::getline(text, token, ',')) {
                int idVertice = std::stoi(token);
                sol[i++] = idVertice;
            }
            return sol;
        }

        int get_size() {
            return size;
        }

        int* get_array() {
            return instance;
        }

        ~Io() {
            delete[] instance;
        }
};
