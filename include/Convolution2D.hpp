#ifndef __CONVOLUTION2D__HPP_
#define __CONVOLUTION2D__HPP_

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
 * The header file for class Convolution2D.
 */
#include <vector>
using namespace std;
 
/** Contains method to create random image, random filter,
 *  convolve2D directly, fast convolve2D using im2col method
 */
class Convolution2D {
    int mImgSize; /** Row or column size of image. Assume square matrix */
    int mFilterSize; /** Row/column size of filter. Assume square matrix*/

    /** Matrix multiplication of two matrices
     * @param vector<vector<float>>& a input matrix A
     * @param vector<vector<float>>& b input matrix B
     * @return vector<vector<float>>  matrix multiplication of A & B
     */
    vector<vector<float>> matrixMultiply(vector<vector<float>>& a,
                                         vector<vector<float>>& b);

public:
    Convolution2D(int imgSize, int filterSize);
    ~Convolution2D() {}

    /** 2D convolution of image and filter
     * @param vector<vector<float>>& image input matrix image
     * @param vector<vector<float>>& filter input matrix filter
     * @return vector<vector<float>> 2D convolution results
     */
    vector<vector<float>> convolve(vector<vector<float>>& image, 
                                   vector<vector<float>>& filter);

    /** 2D fast convolution of image and filter using im2col
     * @param vector<vector<float>>& image input matrix image
     * @param vector<vector<float>>& filter input matrix filter
     * @return vector<vector<float>> 2D convolution results
     */
    vector<vector<float>> fastConvolve(vector<vector<float>>& image, 
                                       vector<vector<float>>& filter);

    /** Create random image using mImgSize
     * @return vector<vector<float>> random image
     */
    vector<vector<float>> createRandImage();

    /** Create random filter using mFilterSize
     * @return vector<vector<float>> random filter
     */
    vector<vector<float>> createRandFilter();
};
#endif
