#include "Convolution2D.hpp"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <iostream>

/**
 * Constructor 
 * Verifies the size of filter and image and assumes square matrices.
 * @param imgSize size of image
 * @param filterSize size of filter
 */
Convolution2D::Convolution2D(int imgSize, int filterSize): 
                             mImgSize(imgSize), mFilterSize(filterSize) 
{
    if (filterSize <= 0 || filterSize > 11 || (filterSize % 2) == 0) {
        throw runtime_error(
         string("Fatal error: filter size should be in range 1-11 and odd"));
    }

    if (imgSize > 64 || imgSize <= 4) {
        throw runtime_error(
                string("Fatal error: image size  <= 4 and > 64 unsupported"));
    }
    srand(time(NULL));
}

/**
 * Matrix multiplication 
 * Modular code for efficiency with less frequent column moving
 * @param a input matrix A
 * @param b input matrix B
 * @return return matrix multplication of (A,B)
 */
vector<vector<float>>
Convolution2D::matrixMultiply(vector<vector<float>>& a,
                              vector<vector<float>>& b)
{
    vector<vector<float>> result;
    // boundary conditions
    if (a.empty() || b.empty() || a[0].empty() || b[0].empty())
        return result;
    // number of columns of A != number of rows of B
    if (a[0].size() != b.size())
        return result;
    // size of results = (row of A) * (column of B)
    result.resize(a.size(), vector<float>(b[0].size(), 0));
    
    // for efficient column moving, store column pointers
    // in an array. in next iteration, simply incr pointers
    vector<float*> colPtr;
    for (size_t i = 0; i < b.size(); ++i) 
        colPtr.push_back(&b[i][0]);

    // loop over output columns
    for (size_t col = 0; col < b[0].size(); ++col) {
        // dot product over rows
        for (size_t row = 0; row < a.size(); ++row) {
            result[row][col] = inner_product(a[row].begin(),
                    a[row].end(), colPtr.begin(), float(0.0), plus<float>(),
                    [] (float x, float* y) -> float { return x*(*y);});
        }
        // increment column pointers to next column
        transform(colPtr.begin(), colPtr.end(), colPtr.begin(),
                  [] (float* x) { return ++x;});
    }
    return result;
}

/**
 * Naive 2D convolution
 * Assume 'same' mode, i.e., input and output images are of same size
 * - 2D for loop with kernel operation using matrix multiplication
 * @param image input matrix image
 * @param filter input matrix filter
 * @return returns convolve2D output matrix
 */
vector<vector<float>> Convolution2D::convolve(vector<vector<float>>& image,
                                            vector<vector<float>>& filter)
{
    assert(filter.size() == mFilterSize);
    assert(filter[0].size() == mFilterSize);
    assert(image.size() == mImgSize);
    assert(image[0].size() == mImgSize);

    // allocate output image size
    vector<vector<float>> outImage(image.size(), vector<float>(image.size(), 0));
    // half the filter size
    register int hFltrSz = (mFilterSize+1)/2;
    // create 1xk^2 for matrix multiplication
    vector<vector<float>> flattenedFilter(1, 
                          vector<float>(mFilterSize*mFilterSize,0));
    for (size_t i = 0; i < mFilterSize; ++i) {
        copy_n(filter[i].begin(), mFilterSize, 
               flattenedFilter[0].begin() + i*mFilterSize);
    }
    
    // computing pixel by pixel
    for (int x = 0; x < mImgSize; ++x) {
        for (int y = 0; y < mImgSize; ++y) {
            // create k^2x1 image chunk for matrix multiplication
            vector<vector<float>> chunk(mFilterSize*mFilterSize, 
                                        vector<float>(1, float(0)));
            int startx = max(hFltrSz-1-x, 0);
            int starty = max(hFltrSz-1-y, 0);
            int endx = mFilterSize + min(mImgSize - x - hFltrSz,0);
            int endy = mFilterSize + min(mImgSize - y - hFltrSz,0);
            for (int i = startx; i < endx; ++i) {
                for (int j = starty; j < endy; ++j) {
                    chunk[i*mFilterSize + j][0] = 
                        image[x+i-hFltrSz+1][y+j-hFltrSz+1];
                }
            }
            vector<vector<float>> sum = matrixMultiply(flattenedFilter, chunk);
            outImage[x][y] = sum[0][0];
        }
    }
    return outImage;
}

/**
 * Fast 2D convolution
 * Assume 'same' mode, i.e., input and output images are of same size
 * - using im2col to create [k^2,n^2] image and then matrix multiplication
 * @param image input matrix image
 * @param filter input matrix filter
 * @return returns convolve2D output matrix
 */
vector<vector<float>> Convolution2D::fastConvolve(vector<vector<float>>& image,
                                                vector<vector<float>>& filter)
{
    assert(filter.size() == mFilterSize);
    assert(filter[0].size() == mFilterSize);
    assert(image.size() == mImgSize);
    assert(image[0].size() == mImgSize);

    vector<vector<float>> flattenedFilter(1, 
                          vector<float>(mFilterSize*mFilterSize,0));
    for (size_t i = 0; i < mFilterSize; ++i) {
        copy_n(filter[i].begin(), mFilterSize, 
               flattenedFilter[0].begin() + i*mFilterSize);
    }

    vector<vector<float>> inImage(mFilterSize*mFilterSize, 
                                   vector<float>(mImgSize*mImgSize, 0));
    vector<float*> colPtr;
    register int hFltrSz = (mFilterSize+1)/2;
    for (size_t i = 0; i < inImage.size(); ++i) {
        colPtr.push_back(&inImage[i][0]);
    }

    // loop over column element addresses
    for (int x = 0; x < mImgSize; ++x) {
        for (int y = 0; y < mImgSize; ++y) {
            vector<float> chunk(mFilterSize*mFilterSize, 0);
            int startx = max(hFltrSz-1-x, 0);
            int starty = max(hFltrSz-1-y, 0);
            int endx = mFilterSize + min(mImgSize - x - hFltrSz,0);
            int endy = mFilterSize + min(mImgSize - y - hFltrSz,0);
            for (int i = startx; i < endx; ++i) {
                for (int j = starty; j < endy; ++j) {
                    *colPtr[i*mFilterSize + j] = 
                        image[x+i-hFltrSz+1][y+j-hFltrSz+1];
                }
            }
            
            // move column pointer array to the next column
            transform(colPtr.begin(), colPtr.end(), colPtr.begin(),
                      [] (float* x) { return ++x;});
        }
    }
    // allocate output image size
    vector<vector<float>> result(mImgSize, vector<float>(mImgSize, 0));
    vector<vector<float>> outImage =  matrixMultiply(flattenedFilter, inImage);
    for (int i = 0; i < mImgSize; ++i) {
        copy_n(outImage[0].begin() + i*mImgSize, mImgSize, result[i].begin());
    }
    return result;
}

/**
 * Create Random Image using mImgSize
 * @return random image input matrix
 */
vector<vector<float>> Convolution2D::createRandImage()
{
    vector<vector<float>> image(mImgSize, vector<float>(mImgSize, 0));
    for (int i = 0; i < mImgSize; ++i) {
        for (int j = 0; j < mImgSize; ++j) {
            image[i][j] = rand()%10 + ((float)(rand()%10000))/10000;
        }
    }
    return image;
}

/**
 * Create Random Filter using mFilterSize
 * @return random filter input matrix
 */
vector<vector<float>> Convolution2D::createRandFilter()
{
    vector<vector<float>> filter(mFilterSize, vector<float>(mFilterSize, 0));
    for (int i = 0; i < mFilterSize; ++i) {
        for (int j = 0; j < mFilterSize; ++j) {
            filter[i][j] = rand()%10 + ((float)(rand()%10000))/10000;
        }
    }
    return filter;
}
