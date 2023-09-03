#include "Io.h"
#include "Instance.h"
#include "Bdd.h"
#include <memory>
#include <sqlite3.h>

int main(int argc, char *argv[]) {

    std::unique_ptr<Io> input = std::make_unique<Io>(argc, argv);

    const std::string databasePath = "/../data/tsp.db";
    std::unique_ptr<Bdd> bdd = std::make_unique<Bdd>(databasePath);

    int cityId = 1;
    std::shared_ptr<City> city = bdd->getCity(cityId);

    int cityIddos = 7;
    std::shared_ptr<City> citydos = bdd->getCity(cityIddos);

    double delta = bdd->delta(city, citydos);

    return 0;
}
