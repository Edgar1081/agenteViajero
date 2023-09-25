#include "../Io.h"
#include "../Modify_svg.h"
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
    std::shared_ptr<City>* min = instance->get_s();
    std::list<std::tuple<double,double>> L = instance->get_path(instance, size);
    Modify_svg::draw_map(L);

    return 0;
}
