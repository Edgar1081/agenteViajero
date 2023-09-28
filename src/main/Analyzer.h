#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <tuple>
#include <dirent.h>
#include <cmath>
#include <sys/stat.h>
#include "Edges.h"
#include "Heuristic.h"
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
                size_t dotPos = name.find('.');
                std::string just_name = name.substr(0, dotPos);
                fileContents.push_back(std::make_tuple(firstLine,last_line,just_name));
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

        if(l.size() < 1)
            return;

        std::string dirSweep = dir+"sweeped";
        int dirResult = mkdir(dirSweep.c_str(), 0777);
        if (dirResult != 0) {
            std::cerr << "Failed to create directory." << std::endl;
        }

        double min = std::numeric_limits<double>::max();
        std::string mintsp;

        int* ins = Io::to_array(std::get<1>(l.front()));

        std::shared_ptr<Edges> ed = std::make_shared<Edges>(ins, bdd, size);

        double norm = ed->get_norm();
        double max = ed->get_max();

        for(auto tuple: l){
            std::string name = std::get<2>(tuple) + ".tsp";
            std::string nameS = name;

            std::shared_ptr<Instance> instance =
                std::make_shared<Instance>(ins, size, 0, norm, max, bdd);

            int imp = instance->sweep1();
            double cost = instance->get_cost();
            nameS = "sN" + std::to_string(imp) + "_" + name;

            std::string filepath = dirSweep+"/"+nameS;

            std::ofstream outputFile(filepath);
            if (!outputFile.is_open()) {
                std::cerr << "Failed to open file for writing." << std::endl;
            }

            if(cost < min){
                min = cost;
                mintsp = nameS;
            }

            outputFile << std::setprecision(16);
            std::shared_ptr<City> * min = instance->get_s();
            for(int i = 0; i<size; i++){
                outputFile << min[i] -> get_id();
                if(i != size-1)
                    outputFile << ",";
            }

        }

        std::string best = dirSweep+"/best_"+ mintsp;
        std::string minfile = dirSweep+"/"+ mintsp;

        if (std::rename(minfile.c_str(), best.c_str()) == 0) {
        } else {
            std::cerr << "Error renaming the file." << std::endl;
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
