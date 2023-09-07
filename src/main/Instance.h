#include <string>
#include <list>
#include <iomanip>
#include <memory>
#include "Bdd.h"
#include "Cost.h"

class Instance {
    private:
        std::shared_ptr<City>* sol;
        std::list<double> L;
        double normalizer;
        double max_edge = 0;
        std::shared_ptr<Bdd> bdd;
        int size;
        double** edges;

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
                edges[id_u][id_v] = w;

                if (max_edge < w) {
                    max_edge = w;
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

        void complete() {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    int city1_id = sol[i]->get_id();
                    int city2_id = sol[j]->get_id();
                    double w = edges[city1_id][city2_id];
                    if(w == -1){
                        w = Cost::delta(sol[i]->get_lat(), sol[i]->getLon(),
                                        sol[j]->get_lat(), sol[j]->getLon());
                        edges[city1_id][city2_id] = w * max_edge;
                    }
                }
            }
        }

    public:
        Instance(int* _sol, std::shared_ptr<Bdd> _bdd, int _size) :
            bdd(_bdd), size(_size) {
            sol = new std::shared_ptr<City>[size];

            for (int i = 0; i < size; i++) {
                sol[i] = bdd->get_city(_sol[i]);
            }
            edges = new double*[1093];
            for (int i = 0; i < 1093; i++) {
                edges[i] = new double[1093];
                for (int j = 0; j < 1093; j++) {
                    edges[i][j] = 0.0;
                }
            }

            normalizer = 0;
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    manage_w(i, j);
                }
            }

            L.sort();
            L.reverse();
            calc_norm();
            complete();
        }

        double get_edge(int vertex1, int vertex2) {
            return edges[vertex1][vertex2];
        }

        int get_size() {
            return size;
        }

        void permute() {
        }

        double get_max_edge() {
            return max_edge;
        }

        double get_normalizer(){
            return normalizer;
        }


        double cost(){
            double sum = 0;
            for(int i = 1; i < size; i++){
                int v = sol[i] -> get_id();
                int u = sol[i-1] -> get_id();
                std::cout << u << ", " << v;
                std::cout << ": " << edges[u][v] << std::endl;
                sum += edges[u][v];
            }
            return sum/normalizer;
        }


        ~Instance() {
            delete[] sol;

            for (int i = 0; i < size; i++) {
                delete[] edges[i];
            }
            delete[] edges;
        }
};
