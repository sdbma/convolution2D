#define PY_SSIZE_T_CLEAN
#include "EmbeddedPythonTest.hpp"
#include <stdexcept>
#include <iostream>
#include <cstdlib>

using namespace std;

static
void printArgs() {
    cout << "Verify using python module scipy convolve2d method." << endl;
    cout << "Test all in-house random cases." << endl;
    cout << "$ pytest all" << endl;
    cout << "Test random image and filter of size argv[1] and "
            "argv[2] respectively." << endl;
    cout << "$ pytest 7 3" << endl;
}

/**
 * There are two ways to run this program.
 * $ pytest all
 * Runs pre-chosen image and filter dimensions, creates random images
 * and filters and verifies both naive and fast convolution with
 * python scipy routine convolve2d.
 *
 * $pytest 4 3
 * Numeric arguments assigned to imageSize and filterSize respectively.
 * Random image and filter is created and verification for both naive
 * and fast convolution done similarly.
 */
int
main(int argc, char *argv[])
{
    if (argc < 2) {
        printArgs();
        return -1;
    }
    bool alltests = false;
    int imgSize = -1;
    int filterSize = -1;
    if (strncmp(argv[1],"all",3) == 0) {
        alltests = true;
    } else {
        imgSize = atoi(argv[1]);
        if (argc < 3) {
            printArgs();
            return -1;
        }
        filterSize = atoi(argv[2]);
        if (imgSize <= 0 || filterSize <= 0) {
            printArgs();
            return -1;
        }
    }
    try {
        const EmbeddedPythonTest emb(argc, argv);
        if (alltests) {
            if (emb.testRandomConv2DCall() != 0)
                return -1;
        } else {
            if (emb.testRandomConv2DCall(imgSize, filterSize, true) != 0)
                return -1;
        }
        return 0;
    } catch (exception& exc) {
        cout << exc.what() << endl;
        return -1;
    }
}
