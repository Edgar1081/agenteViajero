#include <string>
#include <list>
#include <iomanip>
#include <memory>
#include <random>
#include "Bdd.h"
#include "Cost.h"

class Instance {
    private:
        std::shared_ptr<City>* sol;
        std::shared_ptr<City>* sol_ant;
        std::list<double> L;
        double normalizer;
        double max_edge = 0;
        std::shared_ptr<Bdd> bdd;
        int size;
        double edges[1093][1093];
        std::mt19937 rng;
        std::uniform_int_distribution<int> distribution;
        double actual_sum;
        double actual_cost;
        double last_sum;
        double last_cost;

        void manage_w(int i, int j) {
            int id_u = sol[i]->get_id();
            int id_v = sol[j]->get_id();
            if (id_u == id_v) {
                edges[id_u][id_v] = 0;
            } else {
                double w = bdd->edges(id_u, id_v);
                if (w != -1) {
                    L.push_back(w);
                }
                edges[id_u][id_v] = edges[id_v][id_u] = w;
                if (max_edge < w) {
                    max_edge = w;
                }
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

        void calc_norm(){
            int length = static_cast<int>(L.size());
            if(length < size){
                for (const double& element : L)
                    normalizer += element;
            }else{
                auto it = L.begin();
                for (int i = 0; i < size-1 && it != L.end(); ++i, ++it) {
                    normalizer += *it;
                }
            }
        }

    public:
        Instance(int* _sol, std::shared_ptr<Bdd> _bdd, int _size, unsigned int seed) :
            bdd(_bdd), size(_size),  rng(seed), distribution(0, size-1){

            sol = new std::shared_ptr<City>[size];
            sol_ant = new std::shared_ptr<City>[size];
            for (int i = 0; i < size; i++) {
                sol[i] = bdd->get_city(_sol[i]);
                sol_ant[i] = sol[i];
            }

            for (int i = 0; i < 1093; i++) {
                for (int j = 0; j < 1093; j++) {
                    edges[i][j] = 0;
                }
            }

            normalizer = 0;
            for (int i = 0; i < size; i++) {
                for (int j = i+1; j < size; j++) {
                    manage_w(i, j);
                }
            }
            L.sort();
            L.reverse();
            calc_norm();
            complete();
            first_cost();
            last_sum = actual_sum;
            std::copy(sol, sol + size, sol_ant);
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
            actual_cost = last_cost;
        }
        std::tuple<std::shared_ptr<City>*,int, int> permute() {
            int i = distribution(rng);
            int j = distribution(rng);

            while( i == j )
                j = distribution(rng);

            std::copy(sol, sol+size, sol_ant);
            swap(i,j, sol);

            last_sum = actual_sum;
            modify_cost(i ,j);
            //first_cost();
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
        }

        double cost_eval(std::shared_ptr<City>* array){
            double sum = 0;
            for(int i = 1; i < size; i++){
                int u = array[i-1] -> get_id();
                int v = array[i] -> get_id();
                sum += edges[v][u];
            }
            return sum/normalizer;
        }

        void modify_cost(int i, int j) {
            double sum = 0;
            double res = 0;

            int ii = std::min(i,j);
            int jj = std::max(i,j);

            int city_i = sol_ant[ii]->get_id();
            int city_j = sol_ant[jj]->get_id();

            int li = -1;
            int di = sol_ant[ii+1]->get_id();

            int lj = sol_ant[jj-1]->get_id();
            int dj = -1;

            if (ii != 0)
                li = sol_ant[ii-1]->get_id();
            if (jj != size-1)
                dj = sol_ant[jj+1]->get_id();

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

            actual_sum -= res;
            actual_sum += sum;
        }

        std::shared_ptr<City> * get_s(){
            return sol;
        }

        ~Instance() {
            delete[] sol;
            delete[] sol_ant;
        }
};
