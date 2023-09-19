#include "Io.h"
#include "Heuristic.h"
#include <memory>
#include <random>
#include <sqlite3.h>

int main(int argc, char *argv[]) {
    std::cout << std::setprecision(15);
    const std::string databasePath = "./src/data/tsp.db";
    std::shared_ptr<Bdd> bdd = std::make_shared<Bdd>(databasePath);
    std::unique_ptr<Io> input = std::make_unique<Io>(argv[1]);
    int size = input->get_size();
    int * ins = input->get_array();
    int seed = std::stoi(argv[2]);
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> distribution(0, size);
    std::shared_ptr<Instance> instance = std::make_shared<Instance>(ins, bdd, size, seed);

    std::shared_ptr<Heuristic> h =
        std::make_shared<Heuristic>(instance, 60, 800000, 0.001, 0.01, 0.95, size);
    std::shared_ptr<City> * first = h->apu();
    /*
    for(int i = 0; i<40; i++){
        std::cout << first[i]-> get_id() << " ";
    }
    */
}
