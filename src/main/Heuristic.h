#include "Instance.h"
#include <iostream>
#include <limits>
class Heuristic {
    private:
        std::shared_ptr<Instance> ins;
        int L;
        double init_temp;
        double epsilon;
        double phi;
    public:

        Heuristic(std::shared_ptr<Instance> _ins, int _L,
                  double _temp, double _epsilon, double _phi):
            ins(_ins), L(_L), init_temp(_temp),
            epsilon(_epsilon), phi(_phi){
        }


        std::tuple<double, std::shared_ptr<City>*> calcLote(double T,
            std::shared_ptr<City> * s){
            int c = 0;
            double r = 0;
            while(c < L){
                std::shared_ptr<City> * sP = ins->permute(s);
                double fs = ins->cost(s);
                double fsP = ins->cost(sP);
                if (fsP < (fs + T)){
                    s = sP;
                    c++;
                    r += fsP;
                }
            }
            return std::make_tuple(r/L, s);
        }

        void apu(double T, std::shared_ptr<City> * s){
            std::shared_ptr<City>* smin;
            double p = 0;
            while(p > epsilon){
                double q = std::numeric_limits<double>::max();
                while (p <= q){
                    q = p;
                    auto [r, sn] = calcLote(T, s);
                    p = r;
                    s = sn;

                }
                T = phi*T;
            }
        }
};
