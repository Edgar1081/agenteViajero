#include <string>
#include <memory>
#include "Bdd.h"

class Instance {
    private:
        int* sol;
        std::shared_ptr<Bdd> bdd;
        int size;
        int** edges;

        double complete() {}

    public:
        Instance(int* _sol, std::shared_ptr<Bdd> _bdd, int _size) :
            sol(_sol), bdd(_bdd), size(_size) {
            edges = new int*[size];
            for (int i = 0; i < size; i++) {
                edges[i] = new int[size];
                for (int j = i; j < size; j++) {
                    edges[i][j] = sol[j];
                    std::cout << edges[i][j];
                }
            std::cout<< std::endl;
            }
        }

        // Accessor function to get the edge value between two vertices
        int get_edge(int vertex1, int vertex2) {
            if (vertex1 < 0 || vertex1 >= size || vertex2 < 0 || vertex2 >= size) {
                return -1;
            }
            return edges[vertex1][vertex2];
        }

        // Accessor function to get the size of the instance
        int get_size() {
            return size;
        }

        void permute() {
            // Perform permutations on the solution
        }

        double max_distance() {
            // Calculate and return the maximum distance
            return 0.0; // Adjust as needed
        }

        ~Instance() {
            // Deallocate memory for the solution array
            delete[] sol;

            // Deallocate memory for the edges matrix
            for (int i = 0; i < size; i++) {
                delete[] edges[i];
            }
            delete[] edges;
        }
};
