#include "Io.h"
#include "Heuristic.h"
#include <memory>
#include <random>
#include <sqlite3.h>

int main(int argc, char *argv[]) {
    const std::string databasePath = "./src/data/tsp.db";
    std::shared_ptr<Bdd> bdd = std::make_shared<Bdd>(databasePath);
    std::unique_ptr<Io> input = std::make_unique<Io>(argv[1]);
    int size = input->get_size();
    int * ins = input->get_array();
    int seed = std::stoi(argv[2]);
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> distribution(0, size);
    std::shared_ptr<Instance> instance = std::make_shared<Instance>(ins, bdd, size, seed);
    double max = instance->get_max_edge();
    double norm = instance->get_normalizer();


    std::shared_ptr<City> * first = instance->get_s();
    double cost = instance->cost(first);
    std::cout << std::setprecision(15);
    std::cout <<  "Maximum: " << max << std::endl;
    std::cout <<  "Normalizer: " << norm << std::endl;
    std::cout <<  "Eval: " << cost << std::endl;

    std::shared_ptr<Heuristic> h =
        std::make_shared<Heuristic>(instance, 40, 8, 0.001, 0.01, 0.8);

    std::shared_ptr<City> * iter = h->apu(8, first, 40);
    //double cost3 = instance->cost(iter);
    //std::cout <<  "Eval 3: " << cost3 << std::endl;


    //for(int i = 0; i<40; i++){
    //    std::cout << iter[i]-> get_id() << " ";
    //}

    return 0;
}
