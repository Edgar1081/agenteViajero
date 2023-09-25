
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

    std::shared_ptr<Instance> instance = std::make_shared<Instance>(ins, bdd, size, 0);
    std::shared_ptr<City>* preSweep = instance->get_s();
    std::cout << "Pre sweep eval : "  << instance->eval(preSweep) << std::endl;

    for(int i = 0; i<size; i++){
        std::cout << preSweep[i]-> get_id();
        if(i != 39)
            std::cout << ",";
    }

    instance->set_min(instance->get_s());
    std::cout << std::endl;
    int improvement = instance->sweep();

    std::shared_ptr<City>* min = instance->get_min();

    if (improvement != 0){
        std::cout << "Pos sweep eval : "  << instance->eval(min) << std::endl;
        for(int i = 0; i<size; i++){
            std::cout << min[i]-> get_id();
            if(i != size-1)
                std::cout << ",";
        }
    }else{
        std::cout << "NO SWEEP IMPROVEMENT" << std::endl;
    }

    return 0;
}
