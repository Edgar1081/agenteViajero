#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <tuple>
#include <dirent.h>
#include <cmath>

class Analyzer {
private:
    static std::list<std::tuple<std::string, std::string, std::string>>
        readFilesInDirectory(std::string dirpath) {

        std::list<std::tuple<std::string, std::string, std::string>> fileContents;

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
                std::string line;
                std::string last_line;
                while (std::getline(file, line)) {
                    last_line = line;
                }
                std::string name = entry->d_name;
                fileContents.push_back(std::make_tuple(firstLine,last_line,name));
                file.close();
            } else {
                std::cerr << "Failed to open file: " << filePath << std::endl;
            }
        }

        closedir(dir);

        return fileContents;

    }
    static bool comp_tup(std::tuple<std::string, std::string, std::string> a,
                         std::tuple<std::string, std::string,std::string> b) {
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


    static void write_best(std::list<std::tuple<std::string,std::string,std::string>> l,
                           std::string dir, std::shared_ptr<Bdd> bdd, int size){
        for(auto tuple: l){
            int* array = Io::to_array(std::get<1>(tuple));
            std::shared_ptr<Instance> ins = std::make_shared<Instance>(array, bdd, size, 0);
            //std::cout << std::get<1>(ins->sweep1());
        }
    }

    public:

    static void write(std::shared_ptr<Heuristic> h,
               std::shared_ptr<Instance> instance,
               std::ofstream& outputFile, int size, double eval,
               std::shared_ptr<City>* min){
        outputFile << "Eval : "  << eval << std::endl;
        outputFile << "SEED : "  << instance->get_seed() << std::endl;
        outputFile << "Lot  : "  << h->get_lotes() << std::endl;
        outputFile << "Temp : "  << h->get_temp() << std::endl;
        outputFile << "Eps  : "  << h->get_eps() << std::endl;
        outputFile << "EpsT : "  << h->get_eps_temp() << std::endl;
        outputFile << "Phi  : "  << h->get_phi() << std::endl;
        for(int i = 0; i<size; i++){
            outputFile << min[i]-> get_id();
            if(i != size-1)
                outputFile << ",";
        }

    }

    static void sort(std::string dir, std::shared_ptr<Bdd> bdd, int size){
        std::list<std::tuple<std::string, std::string, std::string>> fileContents
            = readFilesInDirectory(dir);

        fileContents.sort(comp_tup);

        int firstpercent = std::round(fileContents.size()/10);
        fileContents.resize(firstpercent);
        write_best(fileContents, dir, bdd, size);
    }
};
