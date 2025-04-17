#include <iostream>

#include "IO_manager.h"
#include "args_parsing.h"
#include "iteration.h"
#include "scripts.h"
#include "structures.h"

int main(int argc, char** argv) {
    try {
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        Args CLI_data = parseArgs(argc, argv);
        if (CLI_data.debug) {
            std::cout << "input file: " << CLI_data.input << "\n";
            std::cout << "output path: " << CLI_data.output << "\n";
            std::cout << "max_iter: " << CLI_data.max_iter << "\n";
            std::cout << "freq: " << CLI_data.freq << "\n";
        }

        TSVData data = readTSV(CLI_data.input);
        if (CLI_data.debug) {
            for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < (*data.matrix).size_x; ++j) {
                    std::cout << (*data.matrix)[i][j] << "\t";
                }
                std::cout << "\n";
            }
        }

        iterateLoop(data.matrix, &data.iter_data, CLI_data);

        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        std::cout << "\n\nExecution time: " << elapsed.count() << " seconds\n";

    } catch (const std::bad_alloc& e) {
        std::cerr << "Error: not enough RAM." << e.what() << std::endl;
        return 1;
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } 

    return 0;
}
