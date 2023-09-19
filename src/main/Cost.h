#include <iostream>
#include <cmath>
#include <memory>

class Cost {

    private:
        static double degreesToRadians(double degrees) {
            double radians = degrees * (M_PI / 180);
            return radians;
        }

    public:

        static double delta(double ulat, double ulon, double vlat, double vlon) {
            const int radius = 6373000;
            double vLat = degreesToRadians(vlat);
            double vLon = degreesToRadians(vlon);
            double uLat = degreesToRadians(ulat);
            double uLon = degreesToRadians(ulon);

            double latdiff = (vLat-uLat)/2;
            double latdiffSin = sin(latdiff);

            double londiff = (vLon-uLon)/2;
            double londiffSin = sin(londiff);

            double a =  pow(latdiffSin, 2) +
                cos(uLat) * cos(vLat) *

                pow(londiffSin, 2);
            double c = 2 * atan2(sqrt(a), sqrt(1-a));
            return radius * c;
        }
};
