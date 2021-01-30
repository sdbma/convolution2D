#ifndef __UNIT_TEST_HPP__
#define __UNIT_TEST_HPP__

#include <vector>
#include <string>
using namespace std;
class UnitTest {
    /**
     * Displays input matrix in std::cout
     * @param vector<vector<float>> image input matrix
     */
    static void printMatrix(vector<vector<float>>& image);

    /** Compare two floating point numbers
     * @param float a input value a
     * @param float b input value b
     * @param float eps tolerance value eps
     * @return bool true if inputs are with absolute and relative
     *                   tolerance of eps
     */
    static bool floatCompare(float a, float b, float eps = 0.0001);

    /** Compare two matrix images
     * @param vector<vector<float>>& a input matrix a
     * @param vector<vector<float>>& b input matrix b
     * @return int status is 0 if input matrix values are close
     *             to equal within tolerance
     */
    static int compareOutImages(vector<vector<float>>& expected,
                                  vector<vector<float>>& actual);

    UnitTest() {}
public:

    /** Create random image and filter and run 2D conv
     * @param int input image size
     * @param int input filter size
     */
    static int runRandomConv2D(int imgSz, int filterSz);

    /** Test the contents of one file
     *  - The file contains inputImage, filter and outputImage values
     * @param string testFile
     * @return int status is 0 if input matrix values are close
     *             to equal within tolerance
     */
    static int testUnitFile(string testFile);

    ~UnitTest() {}
};
#endif
