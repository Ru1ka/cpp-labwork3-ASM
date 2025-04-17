#include "args_parsing.h"

Args parseArgs(int argc, char** argv) {
    Args result;
    for (int i = 1; i < argc; i++) {
        if (stringBegginingEquality(argv[i], "--")) {
            // Long argument
            if (stringBegginingEquality(argv[i], "--input=")) {
                result.input = argv[i] + 8;
            } else if (stringBegginingEquality(argv[i], "--output=")) {
                result.output = argv[i] + 9;
            } else if (stringBegginingEquality(argv[i], "--max-iter=")) {
                if (!cstringIsNumber(argv[i] + 11)) {
                    throw std::invalid_argument("Invalid long argument --max-iter");
                }
                result.max_iter = atoi(argv[i] + 7);
            } else if (stringBegginingEquality(argv[i], "--freq=")) {
                if (!cstringIsNumber(argv[i] + 7)) {
                    throw std::invalid_argument("Invalid long argument --freq");
                }
                result.freq = atoi(argv[i] + 7);
            } else if (stringBegginingEquality(argv[i], "--debug")) {
                result.debug = true;
            }
        } else if (stringBegginingEquality(argv[i], "-")) {
            // Short argument
            ++i;
            if (argv[i] == nullptr) {
                throw std::invalid_argument("Invalid short argument");
            }
            if (strictStringEquality(argv[i - 1], "-i")) {
                result.input = argv[i];
            } else if (strictStringEquality(argv[i - 1], "-o")) {
                result.output = argv[i];
            } else if (strictStringEquality(argv[i - 1], "-m")) {
                if (!cstringIsNumber(argv[i])) {
                    throw std::invalid_argument("Invalid argument -m");
                }
                result.max_iter = atoi(argv[i]);
            } else if (strictStringEquality(argv[i - 1], "-f")) {
                if (!cstringIsNumber(argv[i])) {
                    throw std::invalid_argument("Invalid argument -f");
                }
                result.freq = atoi(argv[i]);
            } else {
                throw std::invalid_argument("Invalid short argument");
            }
        } else {
            // Positional argument
            throw std::invalid_argument("There's no positional arguments");
        }
    }
    // Validation
    if (result.input == nullptr) {
        throw std::invalid_argument("input argument is requiered");
    } else if (result.output == nullptr) {
        throw std::invalid_argument("output argument is requiered");
    }

    return result;
}