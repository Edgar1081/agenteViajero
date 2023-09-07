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
        City(int _id, const std::string& _name, const std::string& _country,
             int _population, double _lat, double _lon)
            : id(_id), name(_name), country(_country),
              population(_population), lat(_lat), lon(_lon) {
        }

        ~City() {
        }

        int get_id() { return id; }
        std::string getName() const { return name; }
        std::string getCountry() const { return country; }
        int getPopulation() const { return population; }
        double get_lat() const { return lat; }
        double get_lon() const { return lon; }
};
