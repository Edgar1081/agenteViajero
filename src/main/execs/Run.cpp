#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <random>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " [-s|-f] <integer>" << std::endl;
        return 1;
    }

    const char* flag = argv[1];
    const char* integerStr = argv[2];
    int number = std::atoi(integerStr);
    const char* insS = argv[3];
    int ins = std::atoi(insS);

    std::mt19937 rng;
    rng.seed(number);
    std::uniform_int_distribution<int> distribution(1000, 1500);
    std::uniform_int_distribution<int> distPhi(90, 95);


    if (std::strcmp(flag, "-s") == 0) {
        for (int i = 1; i < number + 1; i++) {
            const char* comm = "./release/reproduce ";
            const char* inp = "./data/input/";
            char insN[50];
            snprintf(insN, sizeof(insN), "%d", ins);
            const char* extin = ".tsp -t ";
            char seed[50];
            snprintf(seed, sizeof(seed), "%d ", i);
            char lote[50];
            snprintf(lote, sizeof(lote), "%d", distribution(rng));
            char temp[50];
            snprintf(temp, sizeof(temp), "%d", distribution(rng));
            const char* eps = "0.0001";
            const char* epsP = "0.01";
            char phi[50];
            snprintf(phi, sizeof(phi), "%.2f", static_cast<double>(distPhi(rng)) / 100.0);

            const char* dir = " > ./results/";
            char file[50];
            snprintf(file, sizeof(file), "%d", i);
            const char* ext = ".txt";

            char ins[500];
            snprintf(ins, sizeof(ins), "%s%s%s%s%s%s %s %s %s %s%s%s%s",
                     comm, inp, insN, extin,
                     seed, lote, temp, eps, epsP, phi, dir, file, ext);

            std::cout << ins << std::endl;

            const char* command = ins;
            system(command);
        }

    } else if (std::strcmp(flag, "-f") == 0) {
        int intValue = std::atoi(integerStr);
        std::cout << "Flag: -f, Integer: " << intValue << std::endl;
    } else {
        std::cerr << "Invalid flag. Use -s or -f." << std::endl;
        return 1;
    }

    return 0;
}
