#ifndef __EMBEDDED_PYTHON_TEST_H__
#define __EMBEDDED_PYTHON_TEST_H__

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>
using namespace std;

class EmbeddedPythonTest {
    PyObject* mModule;
    wchar_t*  mProgram;
    PyObject* mFunc;

    /** Call embedded python scipy convolve2D function
     * @param vector<vector<float>>& image input matrix image
     * @param vector<vector<float>>& filter input matrix filter
     * @return vector<vector<float>> scipy convolve2D matrix output
     */
    vector<vector<float>> getPyConv2DCall(vector<vector<float>>& image,
                                     vector<vector<float>>& filter) const;
    
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
    static int compareImages(vector<vector<float>>& a,
                              vector<vector<float>>& b);

public:
    EmbeddedPythonTest(int argc, char* argv[]);
    ~EmbeddedPythonTest();

    /** Collection of tests for few imageSize and filterSize
     * Creates and test random image using scipy routine
     * @return int status is 0 if input matrix values are close
     *             to equal within tolerance
     */
    int testRandomConv2DCall() const;

    /** Creates and test random image using scipy routine
     * @param int imageSize
     * @param int filterSize
     * @param bool printImages display matrix values in cout
     * @return int status is 0 if input matrix values are close
     *             to equal within tolerance
     */
    int testRandomConv2DCall(int imageSize, int filterSize,
                             bool printImages=false) const;
};

#endif // __EMBEDDED_PYTHON_TEST_H__
