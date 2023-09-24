#include "Instance.h"
#include <algorithm>
#include <iostream>
#include <limits>
class Heuristic {
    private:
        std::shared_ptr<Instance> ins;
        int L;
        double temp_calc = 0;
        double init_temp;
        double epsilon;
        double epsilonP;
        double phi;
        int size;

        void temp_init(double T, double P){
            double p = percent_accepted(T);
            double T1;
            double T2;
            if(std::abs( P - p)  <= epsilonP ){
                init_temp = T;
                temp_calc = T;
                return;
            }
            if (p < P) {
                while (p < P) {
                    T = 2*T;
                    p = percent_accepted(T);
                }
                T1 = T/2;
                T2 = T;
            }else{
                while (p > P) {
                    T = T/2;
                    p = percent_accepted(T);
                }
                T1 = T;
                T2 = 2*T;
            }
            double temp = binary_search(T1, T2, P);
            init_temp = temp;
            temp_calc = temp;
            ins->reset_rng();
        }

        double percent_accepted(double T){
            int c = 0;
            for (int i = 0; i < 1600; i++) {
                double last_cost = ins->get_cost();
                auto [sP, x, y] = ins->permute();
                if (ins->get_cost() < (last_cost + T)){
                    c ++;
                }else{
                    ins->restore(x,y);
                }
            }
            return c/1600;
        }

        double binary_search(double T1, double T2, double P){
            double Tm = (T1 + T2) /2;
            if ((T2 - T1) < epsilonP) {
                return Tm;
            }
            double p = percent_accepted(Tm);
            if (p > P) {
                return binary_search(T1, Tm, P);
            }else{
                return binary_search(Tm, T2, P);
            }
        }

    public:
        Heuristic(std::shared_ptr<Instance> _ins, int _L,
                  double _temp, double _epsilon, double _epsilonP, double _phi, int _size):
            ins(_ins), L(_L), epsilon(_epsilon), epsilonP(_epsilonP), phi(_phi), size(_size){
            temp_init(_temp, 0.9);
            for(int i = 0; i < size; i++)
                std::cout << ins->get_s()[i]->get_id() << ", ";
            std::cout << "CALC_TEMP: " << init_temp << " SEED: "
                      << ins->get_seed() <<std::endl;
        }

        Heuristic(std::shared_ptr<Instance> _ins, int _L,
                  double _temp, double _epsilon, double _phi, int _size):
            ins(_ins), L(_L), init_temp(_temp),
            epsilon(_epsilon), phi(_phi), size(_size){
            for(int i = 0; i < size; i++)
                std::cout << ins->get_s()[i]->get_id() << ", ";
            std::cout << "INIT_TEMP: " << init_temp << " SEED: "
                      << ins->get_seed()  <<std::endl;
        }

        std::tuple<double, std::shared_ptr<City>*, std::shared_ptr<City>*>
        calcLote(double min_cost, std::shared_ptr<City> * smin) {
            int c = 0;
            double r = 0;
            std::shared_ptr<City>* s = ins->get_s();
            while (c < L) {
                double fs = ins->get_cost();
                auto [sP, i, j] = ins->permute();
                double fsP = ins->get_cost();
                if (fsP < (fs + init_temp)) {
                    s = sP;
                    c++;
                    r += fsP;
                    std::cout << fsP << std::endl;
                }else{
                    ins->restore(i , j);
                }
                if (fsP < min_cost) {
                    smin = sP;
                    min_cost = fsP;
                }
            }
            return std::make_tuple(r/L, s, smin);
        }

        std::tuple<std::shared_ptr<City> *, std::shared_ptr<City> *> apu(){
            int max = (size)*(size-1);
            std::shared_ptr<City>* s = ins->get_s();
            std::shared_ptr<City>* solmin = s;
            double min_cost = ins->get_cost();
            double p = 0;
            int c = 0;
            while(init_temp > epsilon){
                double q = std::numeric_limits<double>::max();
                while (p <= q && (c < max)){
                    q = p;
                    auto [r, sn, sminN] = calcLote(min_cost, solmin);
                    p = r;
                    s = sn;
                    solmin = sminN;
                    c++;
                }
                init_temp = phi*init_temp;
            }
            return std::make_tuple(s, solmin);
        }
};
