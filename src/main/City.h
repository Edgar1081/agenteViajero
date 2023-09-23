#include <string>

class City {
    private:
        int id;
        double lat;
        double lon;

    public:
        City(int _id,double _lat, double _lon)
            : id(_id),lat(_lat), lon(_lon) {
        }

        ~City() {
        }

        int get_id() { return id; }
        double get_lat() const { return lat; }
        double get_lon() const { return lon; }
};
