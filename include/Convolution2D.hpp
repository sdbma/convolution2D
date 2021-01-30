#ifndef __CONVOLUTION2D__HPP_
#define __CONVOLUTION2D__HPP_

/*
 * The header file for class Convolution2D.
 * @author S Dutta
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
