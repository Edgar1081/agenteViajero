#include "../Io.h"
#include "../Heuristic.h"
#include "../Analyzer.h"
#include <memory>
#include <sys/stat.h>
#include <random>
#include <sqlite3.h>
#include <unistd.h>
#include <dirent.h>

bool directoryExists(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

bool removeDirectory(const std::string& path) {
    DIR* dir = opendir(path.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir))) {
            if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
                std::string entryPath = path + "/" + entry->d_name;
                if (entry->d_type == DT_DIR) {
                    if (!removeDirectory(entryPath))
                        return false;
                } else {
                    if (remove(entryPath.c_str()) != 0)
                        return false;
                }
            }
        }
        closedir(dir);
    }
    return rmdir(path.c_str()) == 0;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " [-s|-f] <integer>" << std::endl;
        return 1;
    }
    std::cout << std::setprecision(16);

    std::string flag = argv[1];
    const char* integerStr = argv[2];
    int number = std::atoi(integerStr);
    const char* insS = argv[3];
    int insC = std::atoi(insS);

    std::string in = "x";
    if(insC == 150)
        in = "./data/input/150.tsp";
    if(insC == 40)
        in = "./data/input/40.tsp";

    const std::string databasePath = "./data/tsp.db";
    std::shared_ptr<Bdd> bdd = std::make_shared<Bdd>(databasePath);
    std::unique_ptr<Io> input = std::make_unique<Io>(in.c_str());
    int size = input->get_size();
    int * ins = input->get_array();

    std::mt19937 rng;
    rng.seed(number);
    std::uniform_int_distribution<int> distribution(1000, 1500);
    std::uniform_int_distribution<int> distributiontemp(1000*size, 8000*size);
    std::uniform_int_distribution<int> distPhi(95, 98);

    std::string directory = "./results";
    std::string caso = "x";
    if(insC == 150)
        caso = "150/";
    if(insC == 40)
        caso = "40/";

    std::string dir = directory + caso;

    if (directoryExists(dir)) {
        if (!removeDirectory(dir)) {
            std::cerr << "Failed to remove directory. " << dir << std::endl;
            return 1;
        }
    }

    int dirResult = mkdir(dir.c_str(), 0777);
    if (dirResult != 0) {
        std::cerr << "Failed to create directory." << std::endl;
        return 1;
    }

    int i = 1;
    int c = 1;
    while(i < number+1){
        std::string filename = std::to_string(c);
        std::string filext = filename + ".txt";
        std::string filepath = dir + "/" + filext;

        std::ofstream outputFile(filepath);
        if (!outputFile.is_open()) {
            std::cerr << "Failed to open file for writing." << std::endl;
            return 1;
        }

        std::shared_ptr<Instance> instance = std::make_shared<Instance>(ins, bdd, size, i);
        int lotes = distribution(rng);
        int temp = distributiontemp(rng);
        double eps = 0.0001;
        double epsP = 0.01;
        double phi = distPhi(rng)/100.0;

        std::shared_ptr<Heuristic> h
            = std::make_shared<Heuristic>(instance, lotes, temp, eps, epsP, phi, size, false);


        auto [first, min] = h->apu();
        double eval = instance->eval(min);

        outputFile << std::setprecision(16);
        if(flag == "-s"){
            i++;
            Analyzer::write(h, instance, outputFile, size, eval, min);
        }

        c++;
        if(flag == "-f")
            if (instance->eval(min) < 1){
                i++;
                Analyzer::write(h, instance, outputFile, size, eval, min);
            }

        outputFile.close();
    }

    Analyzer::sort(dir, bdd, size);

    return 0;
}
