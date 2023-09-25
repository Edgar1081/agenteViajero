#include "../Io.h"
#include "../Heuristic.h"
#include "../Analyzer.h"
#include <memory>
#include <sys/stat.h>
#include <random>
#include <sqlite3.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " [-s|-f] <integer>" << std::endl;
        return 1;
    }
    std::cout << std::setprecision(16);

    const char* flag = argv[1];
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

    struct stat st;
    if (stat(dir.c_str(), &st) != 0) {
        int dirResult = mkdir(dir.c_str(), 0777);
        if (dirResult != 0) {
            std::cerr << "Failed to create directory." << std::endl;
            return 1;
        }
    }

    for(int i = 1; i < number+1; i++){
        std::string filename = std::to_string(i);
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

        outputFile << std::setprecision(16);
        outputFile << "Eval : "  << instance->eval(min) << std::endl;
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
        outputFile.close();
    }
    return 0;
}
