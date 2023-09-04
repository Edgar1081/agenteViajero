#include <memory>
#include <math.h>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include "City.h"

class Bdd {
    private:
        sqlite3 *db;
        const std::string& route;
        int rc;

        double C(const std::shared_ptr<City>& u, const std::shared_ptr<City>& v) {
            return 2 * atan2(sqrt(A(u, v)), sqrt(1 - A(u, v)));
        }

        double A(const std::shared_ptr<City>& u, const std::shared_ptr<City>& v) {
            double vLat = (v->getLat()) * (M_PI)/180;
            double vLon = (v->getLon()) * (M_PI)/180;
            double uLat = (u->getLat()) * (M_PI)/180;
            double uLon = (u->getLon()) * (M_PI)/180;

            double a =  pow(sin((vLat - uLat) / 2), 2);
            double b = cos(vLat) * cos(uLat);
            double c =  pow(sin(((vLon - uLon) / 2)), 2);
            double d = b * c;
            return a + d;

        }
    public:
        Bdd(const std::string& route) : db(nullptr), route(route), rc(-1) {
            int result = sqlite3_open(route.c_str(), &db);
            if (result != SQLITE_OK) {
                std::cerr << "SQLite error: " << sqlite3_errmsg(db) << std::endl;
            }
        }

        ~Bdd() {
            std::cout << "Destructor called" << std::endl;
            sqlite3_close(db);  // Close the database in the destructor
        }

        void printData() {
            std::cout << "Data: " << route << std::endl;
        }

        void getAllCities() {
            const char *query = "SELECT * FROM cities;";
            sqlite3_stmt *stmt;

            rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);  // Initialize stmt, not res
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int id = sqlite3_column_int(stmt, 0);
                std::cout << "ID: " << id << std::endl;
            }
        
            sqlite3_finalize(stmt);
        }

        void getAllLatitudes() {
            const char *query = "SELECT latitude FROM cities;";
            sqlite3_stmt *stmt;

            rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);  // Initialize stmt, not res
        
            while(sqlite3_step(stmt) == SQLITE_ROW){
                std::cout << sqlite3_column_double(stmt, 0) << std::endl;
            }

            sqlite3_finalize(stmt);  // Clean up the prepared statement
        }


        void getAllLongitudes() {
            const char *query = "SELECT longitude FROM cities;";
            sqlite3_stmt *stmt;

            rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
        
            while(sqlite3_step(stmt) == SQLITE_ROW){
                std::cout << sqlite3_column_double(stmt, 0) << std::endl;
            }

            sqlite3_finalize(stmt);
            sqlite3_close(db);
        }

        double edges(int i, int j) {

            std::cout << i << ", ";
            std::cout << j << " " << std::endl;
            std::string id_1Str = std::to_string(i);
            std::string id_2Str = std::to_string(j);
            std::string query = "SELECT * FROM connections WHERE ";
            std::string id1 = "id_city_1 = " + id_1Str + " AND ";
            std::string id2 = "id_city_2 = " + id_2Str + ";";
            query = query + id1 + id2;
            const char* queryChar = query.c_str();
            sqlite3_stmt *stmt;

            rc = sqlite3_prepare_v2(db, queryChar, -1, &stmt, 0);

            double distance = 0;

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                distance = sqlite3_column_double(stmt, 2);
            } else {
                return -1;
            }


            sqlite3_finalize(stmt);
            return distance;
        }


        double delta(const std::shared_ptr<City>& u, const std::shared_ptr<City>& v) {
            const int radius = 6373000;
            double res = radius * C(u, v);
            return res;

        }

        std::unique_ptr<City> getCity(int id) {
            std::string idStr = std::to_string(id);

            // Construct the query string
            std::string queryString = "SELECT * FROM cities WHERE id = " + idStr + ";";

            // Convert the query string to a const char* when needed
            const char* query = queryString.c_str();

            sqlite3_stmt *stmt;

            rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

            double lat = 0;
            double lon = 0;

            while (sqlite3_step(stmt) == SQLITE_ROW) {
                lat = sqlite3_column_double(stmt, 4);
                lon = sqlite3_column_double(stmt, 5);
                std::cout << lat << " " << lon << std::endl;
            }

            sqlite3_finalize(stmt);
        
            return std::make_unique<City>(0, "xd", "xdxd", 0, lat, lon);
        }
};
