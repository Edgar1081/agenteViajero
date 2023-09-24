
#include "../Io.h"
#include "../Heuristic.h"
#include <memory>
#include <random>
#include <sqlite3.h>

int main(int argc, char *argv[]) {
    std::cout << std::setprecision(16);
    const std::string databasePath = "./src/data/tsp.db";
    std::shared_ptr<Bdd> bdd = std::make_shared<Bdd>(databasePath);
    std::unique_ptr<Io> input = std::make_unique<Io>(argv[1]);
    int size = input->get_size();
    int * ins = input->get_array();

    std::shared_ptr<Instance> instance = std::make_shared<Instance>(ins, bdd, size, 0);
    instance->set_min(instance->get_s());
    instance->sweep();

    std::shared_ptr<City>* min = instance->get_min();

    for(int i = 0; i<40; i++){
        std::cout << min[i]-> get_id();
        if(i != 39)
            std::cout << ",";
    }

    return 0;
}
