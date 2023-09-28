#include "Cost.h"
#include "Bdd.h"
#include <list>

class Edges{
    private:
        double edges[1093][1093];
        std::shared_ptr<City>* sol;
        std::shared_ptr<Bdd> bdd;
        double normalizer;
        std::list<double> L;
        double max_edge = 0;
        int size;

        void manage_w(int i, int j) {
            int id_u = sol[i]->get_id();
            int id_v = sol[j]->get_id();
            if (id_u == id_v) {
                edges[id_u][id_v] = 0;
            } else {
                int ii = std::min(id_u, id_v);
                int jj = std::max(id_u, id_v);
                double w = bdd->edges(ii, jj);
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
        Edges(int* _sol, std::shared_ptr<Bdd> _bdd, int _size):
            bdd(_bdd), size(_size){

            sol = new std::shared_ptr<City>[size];
            for (int i = 0; i < size; i++)
                sol[i] = bdd->get_city(_sol[i]);


            for (int i = 0; i < 1093; i++)
                for (int j = 0; j < 1093; j++)
                    edges[i][j] = 0;

            normalizer = 0;
            for (int i = 0; i < size; i++)
                for (int j = i+1; j < size; j++)
                    manage_w(i, j);

            L.sort();
            L.reverse();
            calc_norm();
            complete();
        }

        double (*get_edges())[1093] {
            return edges;
        }
        double get_norm(){
            return normalizer;
        }

        double get_max(){
            return max_edge;
        }

        std::shared_ptr<City>* get_sol(){
            std::shared_ptr<City>* c = new std::shared_ptr<City>[size];
            std::copy(sol, sol+size, c);
            return c;
        }
        ~Edges() {
            delete[] sol;
        }


};
