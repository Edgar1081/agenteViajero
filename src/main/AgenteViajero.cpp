#include "Io.h"
#include "Instance.h"
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
    std::shared_ptr<City> u = bdd->get_city(1);
    std::shared_ptr<City> v = bdd->get_city(7);
    double max = instance->get_max_edge();
    double norm = instance->get_normalizer();
    double cost = instance->cost();
    std::cout << std::setprecision(15);
    std::cout <<  "Maximum: " << max << std::endl;
    std::cout <<  "Normalizer: " << norm << std::endl;
    std::cout <<  "Eval: " << cost << std::endl;
    return 0;
}
