#include "../Io.h"
#include "../Heuristic.h"
#include <memory>
#include <random>
#include <sqlite3.h>

int main(int argc, char *argv[]) {

    std::cout << std::setprecision(16);
    const std::string databasePath = "./data/tsp.db";
    std::shared_ptr<Bdd> bdd = std::make_shared<Bdd>(databasePath);
    std::unique_ptr<Io> input = std::make_unique<Io>(argv[1]);
    int size = input->get_size();
    int * ins = input->get_array();

    const char* integerStr = argv[2];
    int number = std::atoi(integerStr);

    bool print_all = true;

    std::mt19937 rng;
    rng.seed(number);
    std::uniform_int_distribution<int> distribution(1000, 1500);
    std::uniform_int_distribution<int> distributiontemp(1000*size, 8000*size);
    std::uniform_int_distribution<int> distPhi(95, 98);

    int lotes = distribution(rng);
    int temp = distributiontemp(rng);
    double eps = 0.0001;
    double epsP = 0.01;
    double phi = distPhi(rng)/100.0;
    int seed = number;


    std::cout << lotes << std::endl;
    std::cout << temp << std::endl;
    std::cout << eps << std::endl;
    std::cout << epsP << std::endl;
    std::cout << phi << std::endl;
    std::cout << seed << std::endl;

    std::shared_ptr<Instance> instance = std::make_shared<Instance>(ins, bdd, size, seed);
    std::shared_ptr<Heuristic> h;

    h = std::make_shared<Heuristic>(instance, lotes, temp, eps, epsP, phi, size, print_all);

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
