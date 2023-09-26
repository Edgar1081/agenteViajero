#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <tuple>
#include <dirent.h>
#include <cmath>

class Analyzer {
private:
    static std::list<std::tuple<std::string, std::string>>
        readFilesInDirectory(std::string dirpath) {

        std::list<std::tuple<std::string, std::string>> fileContents;

        DIR* dir = opendir(dirpath.c_str());
        if (!dir) {
            std::cerr << "Failed to open directory: " << dirpath << std::endl;
            return fileContents;
        }

        struct dirent* entry;
        while ((entry = readdir(dir))) {
            std::string filePath = dirpath + "/" + entry->d_name;
            std::ifstream file(filePath);
            if (entry->d_type != DT_REG)
                continue;
            if (file.is_open()) {
                std::string firstLine;
                std::getline(file, firstLine);

                std::string content;
                std::string line;
                while (std::getline(file, line)) {
                    content += line + "\n";
                }

                file.close();
                fileContents.push_back(std::make_tuple(firstLine, content));
            } else {
                std::cerr << "Failed to open file: " << filePath << std::endl;
            }
        }

        closedir(dir);

        return fileContents;

    }
    static bool comp_tup(std::tuple<std::string, std::string> a, std::tuple<std::string, std::string> b) {
        double evalA = quit_eval(std::get<0>(a));
        double evalB = quit_eval(std::get<0>(b));
        return evalA < evalB;
    }

    static double quit_eval(std::string eval){

        std::string substr = "Eval : ";

        size_t position = eval.find(substr);

        if (position != std::string::npos) {
            eval.erase(position, substr.length());
        } else {
            std::cout << "Substring not found in the original string." << std::endl;
        }
        return std::stod(eval);
    }
    public:

    static void sort(std::string dir){
        std::list<std::tuple<std::string, std::string>> fileContents
            = readFilesInDirectory(dir);

        fileContents.sort(comp_tup);

        int firstpercent = std::round(fileContents.size()/10);
        fileContents.resize(firstpercent);

        for (auto tuple : fileContents) {
            std::cout << "First Line: " << std::get<0>(tuple) << std::endl;
            std::cout << "Content:\n" << std::get<1>(tuple) << std::endl;
        }
    }
};
