#include <iostream>
#include <math.h>
#include <memory>

class Cost {
private:

    static double C(int ulat, int ulon, int vlat, int vlon) {
        return 2 * atan2(sqrt(A(ulat, ulon, vlat, vlon)), sqrt(1 - A(ulat, ulon, vlat, vlon)));
    }

    static double A(int ulat, int ulon, int vlat, int vlon) {
        double vLat = (vlat) * (M_PI)/180;
        double vLon = (vlon) * (M_PI)/180;
        double uLat = (ulat) * (M_PI)/180;
        double uLon = (ulon) * (M_PI)/180;

        double a =  pow(sin((vLat - uLat) / 2), 2);
        double b = cos(vLat) * cos(uLat);
        double c =  pow(sin(((vLon - uLon) / 2)), 2);
        double d = b * c;
        return a + d;

    }
public:

    static double delta(int ulat, int ulon, int vlat, int vlon) {
        const int radius = 6373000;
        double res = radius * C(ulat, ulon, vlat, vlon);
        return res;
    }
};
