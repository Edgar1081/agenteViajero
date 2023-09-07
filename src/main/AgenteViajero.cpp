#include "Io.h"
#include "Instance.h"
#include <memory>
#include <sqlite3.h>

int main(int argc, char *argv[]) {
    const std::string databasePath = "./src/data/tsp.db";
    std::shared_ptr<Bdd> bdd = std::make_shared<Bdd>(databasePath);
    std::unique_ptr<Io> input = std::make_unique<Io>(argv[1]);
    int size = input->get_size();
    int * ins = input->get_array();
    std::shared_ptr<Instance> instance = std::make_shared<Instance>(ins, bdd, size);
    double max = instance->get_max_edge();
    double norm = instance->get_normalizer();
    double cost = instance->cost();
    std::cout << std::setprecision(15)<< max << std::endl;
    std::cout << norm << std::endl;
    std::cout << cost << std::endl;
    return 0;
}
