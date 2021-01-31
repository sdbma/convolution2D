/*
 * This file is part of the github distribution (https://github.com/sdbma).
 * Copyright (c) 2021 Shomit Dutta.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License <http://www.gnu.org/licenses/> for more details.
 *
 * Methods for unit test.
 */
#include "UnitTest.hpp"
#include "Convolution2D.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>
#include <chrono>
using namespace std;

/**
 * Displays input matrix in std::cout
 * @param vector<vector<float>> image input matrix
 */
void 
UnitTest::printMatrix(vector<vector<float>>& image) {
    cout << "[";
    for (auto it : image) {
        cout << "[";
        for (auto ij : it) {
            cout << ij << " ";
        }
        cout << "]" << endl;
    }
    cout << "]" << endl;
}

/** Compare two floating point numbers
 * @param float a input value a
 * @param float b input value b
 * @param float eps tolerance value eps
 * @return bool true if inputs are with absolute and relative
 *                   tolerance of eps
 */
bool
UnitTest::floatCompare(float a, float b, float eps) {
    float diff = fabs(a - b);
    // absolute tolerance
    if (diff < eps)
        return true;
    // relative tolerance
    float largest = (fabs(a) > fabs(b)) ? a : b;
    if (diff < fabs(largest)*eps)
        return true;
    return false;
}

/** Compare two floating point numbers
 * @param float a input value a
 * @param float b input value b
 * @param float eps tolerance value eps
 * @return bool true if inputs are with absolute and relative
 *                   tolerance of eps
 */
int
UnitTest::compareOutImages(vector<vector<float>>& expected,
                      vector<vector<float>>& actual)
{
    if (expected.size() != actual.size() || 
            expected[0].size() != actual[0].size()) {
        cerr << "Expected and actual images have different sizes" << endl;
        return -1;
    }
    for (int i = 0; i < expected.size(); ++i) {
        for (int j = 0; j < expected[0].size(); ++j) {
            if (!UnitTest::floatCompare(expected[i][j],actual[i][j])) {
                cout << "Expected Image ==> " << endl;
                UnitTest::printMatrix(expected);
                cout << "Actual Image ==> " << endl;
                UnitTest::printMatrix(actual);
                return -1;
            }
        }
    }
    return 0;
}

// utility function to split line given a delimiter
static
vector<string> split_string(string& line, char delimiter) {
    vector<string> result;
    string token;
    istringstream tokenStream(line);
    while(getline(tokenStream, token, delimiter))
        result.push_back(token);
    return result;
}

// utility function to parse matrix values from a file
// using the above routine of splitting line using delimiter
static
bool parseMatrixValues(vector<vector<float>>& vv, ifstream& my_file) {
    string line;
    for (int i = 0; i < vv.size(); ++i) {
        getline(my_file, line);
        vector<string> nk = split_string(line, ' ');
        if (nk.size() < vv[0].size()) {
            cerr << "Missing data for row " << i << endl;
            return false;
        }
        for (int j = 0; j < vv[0].size(); ++j) 
            vv[i][j] = stof(nk[j]);
    }
    return true;
}

/** Create random image and filter and run 2D conv
 * @param int input image size
 * @param int input filter size
 */
int
UnitTest::runRandomConv2D(int imgSize, int filterSize) {
    Convolution2D conv2d(imgSize,filterSize);
    vector<vector<float>> inImg = conv2d.createRandImage();
    vector<vector<float>> filter = conv2d.createRandFilter();
    vector<vector<float>> outImg = conv2d.convolve(inImg, filter);
    vector<vector<float>> outImgFast = conv2d.fastConvolve(inImg, filter);
    cout << "Input Image " << endl;
    printMatrix(inImg);
    cout << "Input Filter " << endl;
    printMatrix(filter);
    cout << "Output Image " << endl;
    printMatrix(outImg);
    int status = compareOutImages(outImg, outImgFast);
    if (status != 0) {
        cout << "Conv2D output different between naive and fast" << endl;
    } else {
        cout << "Conv2D output same between naive and fast" << endl;
    }
    return status;
}

/** Test the contents of one file
 *  - The file contains inputImage, filter and outputImage values
 * @param string testFile
 * @return int status is 0 if input matrix values are close
 *             to equal within tolerance
 */
int 
UnitTest::testUnitFile(string testFile) {
    ifstream my_file(testFile);
    if (!my_file.is_open()) {
        cerr << "Could not open the file - " << testFile << endl;
        return -1;
    }

    try {
        string line;
        getline(my_file, line);
        int imgSize = stoi(line.c_str());
        getline(my_file, line);
        int filterSize = stoi(line.c_str());
        vector<vector<float>> img(imgSize, vector<float>(imgSize, 0));
        vector<vector<float>> outImg(imgSize, vector<float>(imgSize, 0));
        vector<vector<float>> filter(filterSize, vector<float>(filterSize, 0));
        if (!parseMatrixValues(img, my_file)) {
            cerr << "Could not obtain input image data" << endl;
            return -1;
        }
        if (!parseMatrixValues(filter, my_file)) {
            cerr << "Could not obtain filter data" << endl;
            return -1;
        }
        if (!parseMatrixValues(outImg, my_file)) {
            cerr << "Could not obtain output image data" << endl;
            return -1;
        }
        Convolution2D conv2d(imgSize,filterSize);
        vector<vector<float>> outImg2 = conv2d.convolve(img, filter);
        if(UnitTest::compareOutImages(outImg, outImg2) != 0) {
            cout << " NAIVE CONV2D FAIL: (" << imgSize << "," 
                 << filterSize << ") " << testFile << endl;
        } else {
            cout << " NAIVE CONV2D PASS: (" << imgSize << "," 
                 << filterSize << ") " << testFile << endl;
        }
        vector<vector<float>> outImg3 = conv2d.fastConvolve(img, filter);
        if(UnitTest::compareOutImages(outImg, outImg3) != 0) {
            cout << "  FAST CONV2D FAIL: (" << imgSize << "," 
                 << filterSize << ") " << testFile << endl;
        } else {
            cout << "  FAST CONV2D PASS: (" << imgSize << "," 
                 << filterSize << ") " << testFile << endl;
        }
    } catch (...) {
        cerr << "Could not parse the file - " << testFile << endl;
        my_file.close();
        return -1;
    }
    my_file.close();
    return 0;
}
