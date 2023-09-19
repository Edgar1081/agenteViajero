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
        double actual_cost;
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
            actual_cost = first_cost();
            std::copy(sol, sol + size, sol_ant);
        }

        double get_edge(int vertex1, int vertex2) {
            return edges[vertex1][vertex2];
        }


        int get_size() {
            return size;
        }
        std::shared_ptr<City>* get_ant(){
            return sol_ant;
        }

        void restore(){
            std::copy(sol_ant, sol + size, sol);
            actual_cost = last_cost;
        }
        void permute() {
            int i = distribution(rng);
            int j = distribution(rng);
            while(j == i){
                j = distribution(rng);
            }

            std::copy(sol, sol + size, sol_ant);
            sol = swap(i,j, sol);
            last_cost = actual_cost;
            actual_cost = first_cost();
            //sol_ant = sol;
            //sol = swap(i,j,sol);
            /*
            std::shared_ptr<City>* newS = new std::shared_ptr<City>[size];
            std::copy(sol, sol + size, newS);
            std::copy(sol, sol + size, sol_ant);
            swap(i,j,newS);
            std::copy(newS, newS + size, sol);
            // Deallocate memory for newS
            delete[] newS;
            */
            double temp = actual_cost*normalizer;
            temp -= cost(i,j);
            temp += cost_recal(i,j);
            //actual_cost = temp/normalizer;
        }
        std::shared_ptr<City>* swap(int i, int j, std::shared_ptr<City>* s){
            std::shared_ptr<City> u = s[i];
            std::shared_ptr<City> v = s[j];
            s[i] = v;
            s[j] = u;
            return s;
        }

        double eval(int s []){
            double sum = 0;
            for(int i = 1; i < size; i++){
                int u = s[i-1];
                int v = s[i];
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
            return last_cost;
        }

        double get_cost(){
            return actual_cost;
        }

        double first_cost(){
            double sum = 0;
            for(int i = 1; i < size; i++){
                int u = sol[i-1] -> get_id();
                int v = sol[i] -> get_id();
                sum += edges[v][u];
            }
            return sum/normalizer;
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

        double cost(int i, int j){
            if(i < j)
                return cases(i,j);
            return cases(j, i);
        }

        double cost_recal(int i, int j){
            if(i < j)
                return cases_recal(i,j);
            return cases_recal(j,i);
        }

        double cases_recal(int i, int j){
            double recal = 0;
            if (i == 0)
                recal += edges[sol[i]->get_id()][sol[i+1]->get_id()];
            if (j == size-1)
                recal += edges[sol[j]->get_id()][sol[j-1]->get_id()];
            if(i == j-1 && i > 0 && j < size-1){
                recal += edges[sol[i]->get_id()][sol[i-1]->get_id()];
                recal += edges[sol[i]->get_id()][sol[j+1]->get_id()];
                return recal;
            }
            if(i == j-1 && i == 0){
                recal += edges[sol[i]->get_id()][sol[j]->get_id()];
                recal += edges[sol[i]->get_id()][sol[j+1]->get_id()];
                return recal;
            }
            if(i == j-1 && j == size-1){
                recal += edges[sol[i]->get_id()][sol[i-1]->get_id()];
                recal += edges[sol[i]->get_id()][sol[j]->get_id()];
                return recal;
            }
            if(i == j-1){
                recal += edges[sol[i]->get_id()][sol[i-1]->get_id()];
                recal += edges[sol[i]->get_id()][sol[j+1]->get_id()];
                return recal;
            }
            if(i > 0){
                recal += edges[sol[i]->get_id()][sol[i-1]->get_id()];
                recal += edges[sol[i]->get_id()][sol[i+1]->get_id()];
            }
            if(j < size-1){
                recal += edges[sol[j]->get_id()][sol[j-1]->get_id()];
                recal += edges[sol[j]->get_id()][sol[j+1]->get_id()];
            }
            return recal;
        }

        double cases(int i, int j){
            double  rest = 0;
            if (i == 0){
                rest += edges[sol_ant[i]->get_id()][sol_ant[i+1]->get_id()];
            }
            if (j == size-1)
                rest += edges[sol_ant[j]->get_id()][sol_ant[j-1]->get_id()];

            if(i == j-1 && i > 0 && j < size-1){
                rest += edges[sol_ant[i]->get_id()][sol_ant[i-1]->get_id()];
                rest += edges[sol_ant[i]->get_id()][sol_ant[j+1]->get_id()];
                return rest;
            }
            if(i == j-1 && i == 0){
                rest += edges[sol_ant[i]->get_id()][sol_ant[j]->get_id()];
                rest += edges[sol_ant[i]->get_id()][sol_ant[j+1]->get_id()];
                return rest;
            }
            if(i == j-1 && j == size-1){
                rest += edges[sol_ant[i]->get_id()][sol_ant[i-1]->get_id()];
                rest += edges[sol_ant[i]->get_id()][sol_ant[j]->get_id()];
                return rest;
            }
            if(i == j-1){
                rest += edges[sol_ant[i]->get_id()][sol_ant[i-1]->get_id()];
                rest += edges[sol_ant[i]->get_id()][sol_ant[j+1]->get_id()];
                return rest;
            }
            if(i > 0){
                rest += edges[sol_ant[i]->get_id()][sol_ant[i-1]->get_id()];
                rest += edges[sol_ant[i]->get_id()][sol_ant[i+1]->get_id()];
            }
            if(j < size-1){
                rest += edges[sol_ant[j]->get_id()][sol_ant[j-1]->get_id()];
                rest += edges[sol_ant[j]->get_id()][sol_ant[j+1]->get_id()];
            }
            return rest;
        }

        std::shared_ptr<City> * get_s(){
            return sol;
        }


        ~Instance() {
            //delete[] sol;
        }
};
