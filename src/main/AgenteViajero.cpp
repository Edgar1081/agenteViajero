#include <iostream>
#include <memory>
#include <sqlite3.h>
#include "Bdd.h"

int main() {



    const std::string databasePath = "/../data/tsp.db";  // Update with the correct path
    std::unique_ptr<Bdd> bdd = std::make_unique<Bdd>(databasePath);

    int cityId = 1;  // Replace with the desired city ID
    std::shared_ptr<City> city = bdd->getCity(cityId);

    int cityIddos = 7;  // Replace with the desired city ID
    std::shared_ptr<City> citydos = bdd->getCity(cityIddos);

    // Now you can work with the city instance using the smart pointer
    if (city) {
        // Example: Print the city's latitude and longitude
        std::cout << "City Latitude: " << city->getLat() << std::endl;
        std::cout << "City Longitude: " << city->getLon() << std::endl;
    } else {
        std::cout << "City not found." << std::endl;
    }

    if (citydos) {
        // Example: Print the city's latitude and longitude
        std::cout << "City Latitude: " << citydos->getLat() << std::endl;
        std::cout << "City Longitude: " << citydos->getLon() << std::endl;
    } else {
        std::cout << "City not found." << std::endl;
    }

    double delta = bdd->delta(city, citydos);

    std::cout.precision(17);
    std::cout << "e value: " << std::fixed << delta << std::endl;
    return 0; // Exit successfully
}
