#include "Instance.h"
#include <algorithm>
#include <iostream>
#include <limits>
class Heuristic {
    private:
        std::shared_ptr<Instance> ins;
        int L;
        double init_temp;
        double epsilon;
        double epsilonP;
        double phi;
        int size;

        /*
        double temp_init(std::shared_ptr<City> * s, double T, double P){
            double p = percent_accepted(s, T);
            double T1;
            double T2;
            if(fabs( P - p)  <= epsilonP ){
                return T;
            }
            if (p < P) {
                while (p < P) {
                    T = 2*T;
                    p = percent_accepted(s, T);
                }
                T1 = T/2;
                T2 = T;
            }else{
                while (p > P) {
                    T = T/2;
                    p = percent_accepted(s, T);
                }
                T1 = T;
                T2 = 2*T;
            }
            return binary_search(s, T1, T2, P);
        }


        double percent_accepted(std::shared_ptr<City> * s, double T){
            int c = 0;
            std::shared_ptr<City> * sP;
            for (int i = 0; i < 1600; i++) {
                sP = ins->permute(s);
                if (ins->cost(sP) < (ins->cost(s) + T)){
                    c ++;
                    s = sP;
                }
            }
            return c/1600;
        }

        double binary_search(std::shared_ptr<City> * s,
                             double T1, double T2, double P){
            double Tm = (T1 + T2) /2;
            if ((T2 - T1) < epsilonP) {
                return Tm;
            }
            double p = percent_accepted(s, Tm);
            if (p > P) {
                return binary_search(s, T1, Tm, P);
            }else{
                return binary_search(s, Tm, T2, P);
            }
        }
        */

    public:

        Heuristic(std::shared_ptr<Instance> _ins, int _L,
                  double _temp, double _epsilon, double _epsilonP, double _phi, int _size):
            ins(_ins), L(_L), init_temp(_temp),
            epsilon(_epsilon), epsilonP(_epsilonP), phi(_phi), size(_size){

        }

        std::tuple<double, std::shared_ptr<City>*, std::shared_ptr<City>*>
        calcLote() {
            int c = 0;
            double r = 0;
            std::shared_ptr<City>* s = ins->get_s();
            std::shared_ptr<City>* smin = s;
            double min_cost = ins->get_cost();
            while (c < L) {
                double fs = ins->get_cost();
                ins->permute();
                std::shared_ptr<City>* sP = ins->get_s();
                double fsP = ins->get_cost();
                if (fsP < (fs + init_temp)) {
                    s = sP;
                    c++;
                    r += fsP;
                    std::cout << fsP << std::endl;
                }else{
                    ins->restore();
                }
                if (fsP < min_cost) {
                    smin = sP;
                    min_cost = fsP;
                }
            }
            return std::make_tuple(r / L, s, smin);
        }

        void print_sol(std::shared_ptr<City>* sP){
            for(int i = 0; i<size; i++){
                std::cout << sP[i]-> get_id() << "  ";
            }
            std::cout << std::endl;
        }

        std::shared_ptr<City> * apu(){
            std::shared_ptr<City>* smin = new std::shared_ptr<City>[size];
            std::shared_ptr<City>* s = ins->get_s();
            std::copy(s, s + size, smin);
            int c = 0;
            double p = 0;
            while(init_temp > epsilon){
                double q = std::numeric_limits<double>::max();
                while (p <= q /*&& c < max*/){
                    q = p;
                    auto [r, sn, sminN] = calcLote();
                    p = r;
                    s = sn;
                    smin = sminN;
                    c++;
                }
                init_temp = phi*init_temp;
            }
            return s;
        }
};
