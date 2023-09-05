#include <string>
#include <list>
#include <iomanip>
#include <memory>
#include "Bdd.h"
#include "Cost.h"

class Instance {
    private:
        int* sol;
        double normalizer;
        double* heavy_list;
        double max_edge = 0;
        std::shared_ptr<Bdd> bdd;
        int size;
        double** edges;

        void manage_w(int i, int j){
            std::list<double> L;
            if (i == j) {
                edges[i][j] = 0;
            } else {
                int city1_id = sol[i];
                int city2_id = sol[j];
                double w = bdd->edges(city1_id, city2_id);
                if(w!=-1)
                    L.push_back(w);
                edges[i][j] = w;
                if(max_edge < w)
                    max_edge = w;
            }
            normalizer = 0;
            for (const double& element : myList) {
                normalizer+= element;
            }
        }

        void complete() {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    double w = edges[i][j];
                    if(w == -1){
                        int city1_id = sol[i];
                        int city2_id = sol[j];
                        std::shared_ptr<City> city1 = bdd->getCity(city1_id);
                        std::shared_ptr<City> city2 = bdd->getCity(city2_id);
                        w = Cost::delta(city1->getLat(), city1->getLon(),
                                        city2->getLat(), city2->getLon());
                        edges[i][j] = w * max_edge;
                    }
                }
            }
        }

    public:
        Instance(int* _sol, std::shared_ptr<Bdd> _bdd, int _size) :
            sol(_sol), bdd(_bdd), size(_size) {
            edges = new double*[size];
            for (int i = 0; i < size; i++) {
                edges[i] = new double[size];
                for (int j = 0; j < size; j++) {
                    manage_w(i,j);
                }
            }
            complete();
        }

        int get_edge(int vertex1, int vertex2) {
            //std::cout << sol[vertex1] << ", " << sol[vertex2] << std::endl;
            //std::cout << edges[vertex1][vertex2] << std::endl;
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
