#include <string>

class City {
private:
    int id;
    std::string name;
    std::string country;
    int population;
    double lat;
    double lon;

public:
    // Constructors
    City(int _id, const std::string& _name, const std::string& _country,
         int _population, double _lat, double _lon)
        : id(_id), name(_name), country(_country),
          population(_population), lat(_lat), lon(_lon) {}

    // Accessors
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getCountry() const { return country; }
    int getPopulation() const { return population; }
    double getLat() const { return lat; }
    double getLon() const { return lon; }

    // Other methods can be added as needed
};
