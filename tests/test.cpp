#include "Convolution2D.hpp"
#include "UnitTest.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>
using namespace std;

static
void printArgs() {
    cout << "Pass test file or test file list using -f option" << endl;
    cout << "$ testConv2D <test1.txt>" << endl;
    cout << "$ testConv2D -f <test1.list>" << endl;
    cout << "$ testConv2D -rand <imgSize> <filterSize>" << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printArgs();
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1],"-f") == 0) {
        // list of tests
        if (argc < 3) {
            printArgs();
            return EXIT_FAILURE;
        }
        ifstream my_file(argv[2]);
        if (!my_file.is_open()) {
            cerr << "Could not open the file - " << argv[2] << endl;
            return EXIT_FAILURE;
        }
        string line;
        while (getline(my_file, line)) {
            if(UnitTest::testUnitFile(line) != 0) {
                return EXIT_FAILURE;
            }
        }
        my_file.close();
        return EXIT_SUCCESS;
    } else if(strcmp(argv[1], "-rand") == 0) {
        if (argc < 4) {
            printArgs();
            return EXIT_FAILURE;
        }
        int i = stoi(argv[2]);
        int j = stoi(argv[3]);
        if(UnitTest::runRandomConv2D(i, j) != 0)
            return EXIT_FAILURE;
        else
            return EXIT_SUCCESS;
    } else {
        // only one test
        string test(argv[1]);
        if(UnitTest::testUnitFile(test) != 0) {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
}
