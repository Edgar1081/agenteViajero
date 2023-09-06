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

        void manage_w(int i, int j){
            if(i == j)
                return;
            int id_u = sol[i]->get_id();
            int id_v = sol[j]->get_id();
            if (id_u == id_v) {
                edges[id_u][id_v] = 0;
            } else {
                double w = bdd->edges(id_u, id_v);
                if(w!=-1)
                    L.push_back(w);
                edges[id_u][id_v] = w;
                if(max_edge < w)
                    max_edge = w;
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
                    double w = edges[i][j];
                    if(w == -1){
                        int city1_id = sol[i]->get_id();
                        int city2_id = sol[j]->get_id();
                        std::shared_ptr<City> city1 = bdd->get_city(city1_id);
                        std::shared_ptr<City> city2 = bdd->get_city(city2_id);
                        w = Cost::delta(city1->getLat(), city1->getLon(),
                                        city2->getLat(), city2->getLon());
                        edges[i][j] = w * max_edge;
                    }
                }
            }
        }

    public:
        Instance(int* _sol, std::shared_ptr<Bdd> _bdd, int _size) :
            bdd(_bdd), size(_size) {
            sol = new std::shared_ptr<City>[size];

            for(int i = 0; i < size; i++){
                sol[i] = bdd->get_city(_sol[i]);
            }

            normalizer = 0;
            edges = new double*[1092];
            for (int i = 0; i < size; i++) {
                edges[i] = new double[1092];
                for (int j = 0; j < size; j++) {
                    manage_w(i,j);
                }
            }
            L.sort();
            L.reverse();
            calc_norm();
            complete();
        }

        int get_edge(int vertex1, int vertex2) {
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


        ~Instance() {
            delete[] sol;

            for (int i = 0; i < size; i++) {
                delete[] edges[i];
            }
            delete[] edges;
        }
};
