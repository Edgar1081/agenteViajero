#ifndef BDD_H
#define BDD_H


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

    public:
        Bdd(const std::string& route) : db(nullptr), route(route), rc(-1) {
            sqlite3_initialize();
            int result = sqlite3_open(route.c_str(), &db);
            if (result != SQLITE_OK) {
                std::cerr << "SQLite error: " << sqlite3_errmsg(db) << std::endl;
            }
        }

        ~Bdd() {
            if (db) {
                sqlite3_close(db);
                db = nullptr;
            }
        }

        void printData() {
            std::cout << "Data: " << route << std::endl;
        }

        void getAllCities() {
            const char *query = "SELECT * FROM cities;";
            sqlite3_stmt *stmt;

            rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int id = sqlite3_column_int(stmt, 0);
                std::cout << "ID: " << id << std::endl;
            }
        
            sqlite3_finalize(stmt);
        }

        void getAllLatitudes() {
            const char *query = "SELECT latitude FROM cities;";
            sqlite3_stmt *stmt;

            rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
        
            while(sqlite3_step(stmt) == SQLITE_ROW){
                std::cout << sqlite3_column_double(stmt, 0) << std::endl;
            }

            sqlite3_finalize(stmt);
        }


        void getAllLongitudes() {
            const char *query = "SELECT longitude FROM cities;";
            sqlite3_stmt *stmt;

            rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
        
            while(sqlite3_step(stmt) == SQLITE_ROW){
                std::cout << sqlite3_column_double(stmt, 0) << std::endl;
            }

            sqlite3_finalize(stmt);
        }

        double edges(int i, int j) {
            std::string id_1Str = std::to_string(i);
            std::string id_2Str = std::to_string(j);
            std::string query = "SELECT * FROM connections WHERE ";
            std::string id1 = "id_city_1 = " + id_1Str + " AND ";
            std::string id2 = "id_city_2 = " + id_2Str + ";";
            query = query + id1 + id2;
            const char* queryChar = query.c_str();
            sqlite3_stmt *stmt;

            rc = sqlite3_prepare_v2(db, queryChar, -1, &stmt, 0);
            double distance = -1;

            if (sqlite3_step(stmt) == SQLITE_ROW)
                distance = sqlite3_column_double(stmt, 2);
            sqlite3_finalize(stmt);
            return distance;
        }

        std::shared_ptr<City> get_city(int id) {
            std::string idStr = std::to_string(id);
            std::string queryString = "SELECT * FROM cities WHERE id = " + idStr + ";";
            const char* query = queryString.c_str();
            sqlite3_stmt *stmt;
            rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

            double lat = 0;
            double lon = 0;
            int idbdd = -1;

            while (sqlite3_step(stmt) == SQLITE_ROW) {
                idbdd = sqlite3_column_int(stmt, 0);
                lat = sqlite3_column_double(stmt, 4);
                lon = sqlite3_column_double(stmt, 5);
            }

            std::shared_ptr<City> city = std::make_shared<City>(idbdd,lat, lon);
            sqlite3_finalize(stmt);
            return city;
        }
};

#endif
