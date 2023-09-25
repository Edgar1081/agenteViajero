#include "../Io.h"
#include "../Heuristic.h"
#include <memory>
#include <random>
#include <sqlite3.h>

int main(int argc, char *argv[]) {

    bool print_all = false;

    double lotes = 0.0;
    double temp = 0.0;
    double eps = 0.0;
    double epstemp = 0.0;
    double phi = 0.0;
    bool useFlagT = false;
    int seed = -1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i+1];
        if (arg == "-t") {
            useFlagT = true;
            if (i + 7 < argc) {
                seed = std::stod(argv[i + 2]);
                lotes = std::stod(argv[i + 3]);
                temp = std::stod(argv[i + 4]);
                eps = std::stod(argv[i + 5]);
                epstemp = std::stod(argv[i + 6]);
                phi = std::stod(argv[i + 7]);
                i += 7;
            } else {
                std::cerr << "Error: Not enough arguments after '-t' flag." << std::endl;
                return 1;
            }
        }else{
            if (i + 5 < argc) {
                seed = std::stod(argv[i + 1]);
                lotes = std::stod(argv[i + 2]);
                temp = std::stod(argv[i + 3]);
                eps = std::stod(argv[i + 4]);
                phi = std::stod(argv[i + 5]);
                i += 6;
            } else {
                std::cerr << "Error: Not enough arguments" << std::endl;
                return 1;

            }
        }
    }

    std::cout << std::setprecision(16);
    const std::string databasePath = "./data/tsp.db";
    std::shared_ptr<Bdd> bdd = std::make_shared<Bdd>(databasePath);
    std::unique_ptr<Io> input = std::make_unique<Io>(argv[1]);
    int size = input->get_size();
    int * ins = input->get_array();

    std::shared_ptr<Instance> instance = std::make_shared<Instance>(ins, bdd, size, seed);
    std::shared_ptr<Heuristic> h;

    if(useFlagT){
        h = std::make_shared<Heuristic>(instance, lotes, temp, eps, epstemp, phi, size, print_all);
    }else {
        h = std::make_shared<Heuristic>(instance, lotes, temp, eps, phi, size, print_all);
    }

    auto [first, min] = h->apu();

    std::cout << "Eval : "  << instance->eval(min) << std::endl;
    std::cout << "SEED : "  << instance->get_seed() << std::endl;
    std::cout << "Lot  : "  << h->get_lotes() << std::endl;
    std::cout << "Temp : "  << h->get_temp() << std::endl;
    std::cout << "Eps  : "  << h->get_eps() << std::endl;
    std::cout << "EpsT : "  << h->get_eps_temp() << std::endl;
    std::cout << "Phi  : "  << h->get_phi() << std::endl;
    for(int i = 0; i<size; i++){
        std::cout << min[i]-> get_id();
        if(i != size-1)
            std::cout << ",";
    }
    return 0;
}
