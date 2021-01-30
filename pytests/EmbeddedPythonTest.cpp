#include <EmbeddedPythonTest.hpp>
#include <Convolution2D.hpp>
#include <stdexcept>
#include <string>
#include <iostream>


/**
 * Constructor
 * Checks the existence of embedded python call
 *   - opens module "py_function.py" that exists in pytests directory
 *   - checks whether "my_conv" method is callable
 * @param argc int passed from main function 
 * @param argv[] char* passed from main function
 */
EmbeddedPythonTest::EmbeddedPythonTest(int argc, char* argv[]) {
    PyObject *pName;
    wchar_t *mProgram = Py_DecodeLocale(argv[0], NULL);
    if (mProgram == NULL) {
        throw runtime_error(
                string("Fatal error: cannot decode argv[0]\n"));
    }
    // informs the interpreter about paths to 
    // python runtime libraries
    Py_SetProgramName(mProgram);  /* optional but recommended */
 
    // initialize the python interpreter
    Py_Initialize();
    PyRun_SimpleString( "import os, sys \n"
                        "sys.path.append(os.getcwd()) \n");
    pName = PyUnicode_DecodeFSDefault("py_function");
    if (pName == NULL) {
        throw runtime_error(
                string("Fatal error: cannot decode \"py_function\"\n"));
    }

    // script is loaded using import
    mModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (mModule == NULL) {
        throw runtime_error(
                string("Fatal error: Import failed for \"py_function\""));
    }

    // function argument
    mFunc = PyObject_GetAttrString(mModule, "my_conv");
    if (mFunc == 0) {
        throw runtime_error(
                string("Fatal error: Function not found for \"my_conv\""));
    }
    if(!PyCallable_Check(mFunc)) {
        throw runtime_error(
                string("Fatal error: Function not callable for \"my_conv\""));
    }
}

/**
 * Destructor
 * Free resources
 */
EmbeddedPythonTest::~EmbeddedPythonTest() {
    Py_DECREF(mFunc);
    Py_DECREF(mModule);
    // shuts down the interpreter
    Py_FinalizeEx();
    //PyMem_RawFree(mProgram);
}

// Utility method that prints vector when mismatches occur
static
void printVector(vector<vector<float>>& image) {
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

/** Compare two matrix images
 * @param vector<vector<float>>& a input matrix a
 * @param vector<vector<float>>& b input matrix b
 * @return int status is 0 if input matrix values are close
 *             to equal within tolerance
 */
int
EmbeddedPythonTest::compareImages(vector<vector<float>>& a,
                                  vector<vector<float>>& b)
{
    if (a.size() != b.size() || a[0].size() != b[0].size())
        return -1;
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < a[0].size(); ++j) {
            if (!floatCompare(a[i][j], b[i][j])) {
                cerr << "Float Compare Fail (" << a[i][j] << " != " 
                     << b[i][j] << ")" << endl;
                cerr << "Printing both images for comparison" << endl;
                printVector(a);
                printVector(b);
                return -1;
            }
        }
    }
    return 0;
}

/** Compare two floating point numbers
 * @param float a input value a
 * @param float b input value b
 * @param float eps tolerance value eps
 * @return bool true if inputs are with absolute and relative
 *                   tolerance of eps
 */
bool     
EmbeddedPythonTest::floatCompare(float a, float b, float eps) {
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

/** Collection of tests for few imageSize and filterSize
 * Creates and test random image using scipy routine
 * @return int status is 0 if input matrix values are close
 *             to equal within tolerance
 */
int
EmbeddedPythonTest::testRandomConv2DCall() const {
    cout << endl;
    cout << "Comparing C++ conv() impl with python Scipy method." << endl;
    cout << "---------------------------------------------------" << endl;
    cout << endl;
    cout << "Testing random images and filter" << endl << endl;
    cout << "(ImageSize,FilterSize) = (7,3)" << endl;
    if(testRandomConv2DCall(7,3) != 0)
        return -1;
    cout << "(ImageSize,FilterSize) = (7,5)" << endl;
    if(testRandomConv2DCall(7,5)!=0)
        return -1;
    cout << "(ImageSize,FilterSize) = (7,1)" << endl;
    if(testRandomConv2DCall(7,1)!=0)
        return -1;

    cout << "(ImageSize,FilterSize) = (7,7)" << endl;
    if(testRandomConv2DCall(7,7)!=0)
        return -1;

    cout << "(ImageSize,FilterSize) = (16,7)" << endl;
    if(testRandomConv2DCall(16,7)!=0)
        return -1;
    cout << "(ImageSize,FilterSize) = (32,11)" << endl;
    if(testRandomConv2DCall(32,11)!=0)
        return -1;

    cout << "(ImageSize,FilterSize) = (64,11)" << endl;
    if(testRandomConv2DCall(64,11)!=0)
        return -1;

    cout << "(ImageSize,FilterSize) = (55,9)" << endl;
    if(testRandomConv2DCall(55,9)!=0)
        return -1;
    return 0;
}

// flip Kernel before call to scipy.signal convolve2D method
static
vector<vector<float>> flipFilter(vector<vector<float>>& matrix)
{
    vector<vector<float>> result(matrix.size(),
                                 vector<float>(matrix[0].size(), 0));
    for (size_t i = 0; i<matrix.size(); i++) {
        for (size_t j = 0; j<matrix[0].size(); j++) {
             result[matrix.size()-1-i][matrix[0].size()-1-j] = matrix[i][j];
        }
    }
    return result;
}


/** Creates and test random image using scipy routine
 * @param int imageSize
 * @param int filterSize
 * @param bool printImages display matrix values in cout
 * @return int status is 0 if input matrix values are close
 *             to equal within tolerance
 */
int
EmbeddedPythonTest::testRandomConv2DCall(int imgSize, int filterSize,
                                         bool printImages) const 
{
    Convolution2D conv2d(imgSize,filterSize);
    vector<vector<float>> inImg = conv2d.createRandImage();
    vector<vector<float>> filter = conv2d.createRandFilter();
    vector<vector<float>> outImg = conv2d.convolve(inImg, filter);
    vector<vector<float>> outImgFast = conv2d.fastConvolve(inImg, filter);
    if (printImages) {
        cout << "Input Image " << endl;
        printVector(inImg);
        cout << "Input Filter " << endl;
        printVector(filter);
        cout << "Output Image " << endl;
        printVector(outImg);
    }
    vector<vector<float>> flippedFilter = flipFilter(filter);
    vector<vector<float>> pyOutImg = getPyConv2DCall(inImg, flippedFilter);
    int status = compareImages(outImg, pyOutImg);
    if (status == 0) {
        cout << "  NAIVE CONV PASSED." << endl;
    } else {
        cout << "  NAIVE CONV FAILED." << endl;
        return -1;
    }
    status = compareImages(outImgFast, pyOutImg);
    if (status == 0)
        cout << "   FAST CONV PASSED." << endl;
    else
        cout << "   FAST CONV FAILED." << endl;
    return status;
}

/** Call embedded python scipy convolve2D function
 * @param vector<vector<float>>& image input matrix image
 * @param vector<vector<float>>& filter input matrix filter
 * @return vector<vector<float>> scipy convolve2D matrix output
 */
vector<vector<float>>
EmbeddedPythonTest::getPyConv2DCall(vector<vector<float>>& inImage,
                                   vector<vector<float>>& filter) const
{
    vector<vector<float>> result;
    PyObject *pArgTuple, *pValue;
    pArgTuple = PyTuple_New(2);
    // Transfer the C++ vector to a python tuple
    PyObject* pXVec = PyList_New(inImage.size()*inImage[0].size());
    for (size_t i = 0; i < inImage.size(); ++i) {
        for (size_t j = 0; j < inImage[0].size(); ++j) {
            pValue = PyFloat_FromDouble(inImage[i][j]);
            if (!pValue) {
                Py_DECREF(pXVec);
                cerr << "Cannot convert array value" << endl;
                return result;
            }
            PyList_SetItem(pXVec, i*inImage.size()+j, pValue);
        }
    }
    PyObject* pYVec = PyList_New(filter.size()*filter[0].size());
    for (size_t i = 0; i < filter.size(); ++i) {
        for (size_t j = 0; j < filter[0].size(); ++j) {
            pValue = PyFloat_FromDouble(filter[i][j]);
            if (!pValue) {
                Py_DECREF(pYVec);
                cerr << "Cannot convert array value" << endl;
                return result;
            }
            PyList_SetItem(pYVec, i*filter.size() + j, pValue);
        }
    }
    PyTuple_SetItem(pArgTuple, 0, pXVec);
    PyTuple_SetItem(pArgTuple, 1, pYVec);
    pValue = PyObject_CallObject(mFunc, pArgTuple);
    Py_DECREF(pArgTuple);
    Py_DECREF(pXVec);
    Py_DECREF(pYVec);
    if (pValue != NULL) {
        if (PyList_Check(pValue)) {
            size_t sz = PyList_Size(pValue);
            if (sz != inImage.size()*inImage[0].size()) {
                Py_DECREF(pValue);
                cerr << "Output image size: (" << sz << ")unexpected" << endl;
                return result;
            }
            result.resize(inImage.size(), vector<float>(inImage.size(), 0));
            for (size_t i = 0; i < inImage.size(); ++i) {
                for (size_t j = 0; j < inImage[0].size(); ++j) {
                    PyObject* pval = 
                        PyList_GetItem(pValue, i*inImage.size()+j);
                    //cout << "Result of call: " << value << endl;
                    result[i][j] = PyFloat_AS_DOUBLE(pval);
                }
            }
        }
        Py_DECREF(pValue);
    } else {
        PyErr_Print();
        cerr << "Call failed" << endl;
        return result;
    }
    return result;
}
