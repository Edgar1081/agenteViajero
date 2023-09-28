#ifndef INSTANCE_H
#define INSTANCE_H

#include <string>
#include <list>
#include <iomanip>
#include <memory>
#include <random>
#include <algorithm>
#include "City.h"
#include "Bdd.h"
#include "Cost.h"

class Instance {
    private:
        std::shared_ptr<City>* sol;
        std::shared_ptr<City>* sol_ant;
        std::list<double> L;
        int size;
        int seed;
        double edges[1093][1093];
        double normalizer;
        double max_edge = 0;
        std::shared_ptr<Bdd> bdd;
        std::mt19937 rng;
        std::uniform_int_distribution<int> distribution;
        double actual_sum;
        double last_sum;
        std::shared_ptr<City>* init_sol;


        void manage_w(int i, int j) {
            int id_u = sol[i]->get_id();
            int id_v = sol[j]->get_id();
            if (id_u == id_v) {
                edges[id_u][id_v] = 0;
            } else {
                int ii = std::min(id_u, id_v);
                int jj = std::max(id_u, id_v);
                double w = bdd->edges(ii, jj);
                edges[id_u][id_v] = edges[id_v][id_u] = w;
            }
        }

        void complete() {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    int city1_id = sol[i]->get_id();
                    int city2_id = sol[j]->get_id();
                    double w = edges[city1_id][city2_id];
                    if(w == -1){
                        w = Cost::delta(sol[i]->get_lat(), sol[i]->get_lon(),
                                        sol[j]->get_lat(), sol[j]->get_lon());
                        edges[city1_id][city2_id] =
                            edges[city2_id][city1_id] = w * max_edge;
                    }
                }
            }
        }

    public:
        Instance(int* _sol, int _size,
                 unsigned int _seed,
                 double _normalizer, double _max_edge,
                 std::shared_ptr<Bdd> _bdd) :
            size(_size),normalizer(_normalizer),
            max_edge(_max_edge), bdd(_bdd),distribution(0, size-1){
            rng.seed(_seed);
            seed = _seed;
            sol = new std::shared_ptr<City>[size];
            sol_ant = new std::shared_ptr<City>[size];
            init_sol = new std::shared_ptr<City>[size];
            for (int i = 0; i < size; i++) {
                sol[i] = bdd->get_city(_sol[i]);
                sol_ant[i] = sol[i];
                init_sol[i] = sol[i];
            }


            for (int i = 0; i < 1093; i++) {
                for (int j = 0; j < 1093; j++) {
                    edges[i][j] = 0;
                }
            }

            for (int i = 0; i < size; i++) {
                for (int j = i+1; j < size; j++) {
                    manage_w(i, j);
                }
            }


            if(seed != 0){
                std::shuffle(sol, sol + size, rng);
                std::copy(sol, sol + size, sol_ant);
                rng.seed(seed);
            }

            complete();
            first_cost();
            last_sum = actual_sum;
        }

        int get_seed(){
            return seed;
        }


        int first_ran(){
            return distribution(rng);
        }

        void reset_rng(){
            rng.seed(seed);
            std::copy(init_sol, init_sol+size, sol);
            std::copy(init_sol, init_sol+size, sol_ant);
            first_cost();
        }

        int get_size() {
            return size;
        }

        std::shared_ptr<City>* get_ant(){
            return sol_ant;
        }

        void restore(int i ,int j){
            swap(i,j, sol);
            actual_sum = last_sum;
        }

        std::tuple<std::shared_ptr<City>*,int, int> permute() {
            int i = distribution(rng);
            int j = distribution(rng);
            return permute(i,j);
        }
        std::tuple<std::shared_ptr<City>*,int, int> permute(int i, int j) {
            std::copy(sol, sol+size, sol_ant);
            swap(i,j, sol);

            last_sum = actual_sum;
            actual_sum += modify_cost(i ,j, sol_ant);
            return std::make_tuple(sol,i,j);
        }

        std::shared_ptr<City>* swap(int i, int j, std::shared_ptr<City>* s){
            std::shared_ptr<City> u = s[i];
            std::shared_ptr<City> v = s[j];
            s[i] = v;
            s[j] = u;
            return s;
        }

        double eval(std::shared_ptr<City>* s){
            double sum = 0;
            for(int i = 1; i < size; i++){
                int u = s[i-1]->get_id();
                int v = s[i]->get_id();
                sum += edges[v][u];
            }
            return sum/normalizer;
        }

        int sweep(){
            int c = 0;
            while(sweep_once2()){c++;}
            return c;
        }

        bool sweep_once2(){
            for(int i = 0; i < size; i++){
                for(int j = 0; j < size; j++){
                    if(i == j)
                        continue;
                    permute(i,j);
                    if(get_cost() < get_last_cost()){
                        std::cout << "Sweep improvement : "  << get_cost() << std::endl;
                        return true;
                    }else{
                        restore(i,j);
                    }
                }
            }
            return false;
        }


        int sweep1(){
            int c = 0;
            std::string total;
            while(sweep_once1()){c++;}
            return c;
        }

        bool sweep_once1(){
            std::string imp;
            for(int i = 0; i < size; i++){
                for(int j = 0; j < size; j++){
                    if(i == j)
                        continue;
                    permute(i,j);
                    if(get_cost() < get_last_cost()){
                        return true;
                    }else{
                        restore(i,j);
                    }
                }
            }
            return false;
        }

        double get_max_edge() {
            return max_edge;
        }

        double get_normalizer(){
            return normalizer;
        }

        double get_last_cost(){
            return last_sum/normalizer;
        }

        double get_cost(){
            return actual_sum/normalizer;
        }

        void first_cost(){
            double sum = 0;
            for(int i = 1; i < size; i++){
                int u = sol[i-1] -> get_id();
                int v = sol[i] -> get_id();
                sum += edges[v][u];
            }
            actual_sum = sum;
            last_sum = sum;
        }

        double modify_cost(int i, int j, std::shared_ptr<City>* array) {
            if(i == j)
                return 0;
            double sum = 0;
            double res = 0;

            int ii = std::min(i,j);
            int jj = std::max(i,j);

            int city_i = array[ii]->get_id();
            int city_j = array[jj]->get_id();

            int li = -1;
            int di = array[ii+1]->get_id();

            int lj = array[jj-1]->get_id();
            int dj = -1;

            if (ii != 0)
                li = array[ii-1]->get_id();
            if (jj != size-1)
                dj = array[jj+1]->get_id();

            if(li != -1){
                res += edges[li][city_i];
                sum += edges[li][city_j];
            }

            if(dj != -1){
                res += edges[city_j][dj];
                sum += edges[city_i][dj];
            }


            if( std::abs(ii-jj) != 1){
                sum += edges[city_j][di];
                sum += edges[city_i][lj];
                res += edges[city_i][di];
                res += edges[city_j][lj];
            }

            return sum - res;
        }

        std::shared_ptr<City> * get_s(){
            return sol;
        }

        std::shared_ptr<City> * get_init_s(){
            return init_sol;
        }

        std::list<double> get_L(){
            return L;
        }

        std::list<std::tuple<double, double>>
        get_path(std::shared_ptr<Instance> ins, int size){
            std::shared_ptr<City>* min = ins->get_s();
            std::list<std::tuple<double,double>> points;
            for(int i = 0; i < size; i++){
                std::shared_ptr<City> c = min[i];
                double lat = c->get_lat();
                double lon = c->get_lon();
                points.push_back(std::make_tuple(lat,lon));
            }
            return points;
        }

        ~Instance() {
            delete[] sol;
            delete[] sol_ant;
            delete[] init_sol;
        }
};
#endif
