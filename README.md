# Convolution2D
---

### Overview
This document is organized under the following headers.

1. Directory Structure
2. How to Run Unit Tests
3. How to Run Python Embedded Tests
4. Executables
5. Implementation

## Directory Structure
The directory structure for this project is as follows:

1. include/
   > contains the include files:  
   > **Convolution2D.hpp**  
   > **EmbeddedPythonTest.hpp**  
   > **UnitTest.hpp**  
2. src/
   > contains the implementation of convolution2D:   
   > ** Convolution2D.cpp**   
3. tests/
   > contains unit test source code and test files:   
   > **UnitTest.cpp**   
   > **test\*.txt**   
   > **tests.all** - runs tests for all test\*.txt   
4. pytests/
   > execute embedded python code for verification:   
   > **EmbeddedPythonTest.cpp**   
   > **pytest.cpp** - contains main for running embed python tests   

This directory contains a Makefile and a README.   

## How to run UNIT tests

In order to build the source code and run unit tests, run the following commands:   
```sh
$ make
$ make run
```   
A successful verification will show the following text in console.   
```sh
$ make run
./bin/testConv2D -f tests/tests.all
 NAIVE CONV2D PASS: (7,3) tests/test1.txt
  FAST CONV2D PASS: (7,3) tests/test1.txt
 NAIVE CONV2D PASS: (7,1) tests/test2.txt
  FAST CONV2D PASS: (7,1) tests/test2.txt
 NAIVE CONV2D PASS: (7,5) tests/test3.txt
  FAST CONV2D PASS: (7,5) tests/test3.txt
 NAIVE CONV2D PASS: (7,7) tests/test4.txt
  FAST CONV2D PASS: (7,7) tests/test4.txt
 NAIVE CONV2D PASS: (7,9) tests/test5.txt
  FAST CONV2D PASS: (7,9) tests/test5.txt
 NAIVE CONV2D PASS: (64,11) tests/test6.txt
  FAST CONV2D PASS: (64,11) tests/test6.txt
 NAIVE CONV2D PASS: (16,11) tests/test7.txt
  FAST CONV2D PASS: (16,11) tests/test7.txt
 NAIVE CONV2D PASS: (32,9) tests/test8.txt
  FAST CONV2D PASS: (32,9) tests/test8.txt
```

## How to run Python Embedded Tests

In order to verify the authored code with embedded python library scipy.signal method convolve2D(), please use the following make command.
```sh
$ make pytest
```

A successful verification will have the following text in console.   
```sh
$ make pytest
cd ./pytests; ./pytest all ; cd ..

Comparing C++ conv() impl with python Scipy method.
---------------------------------------------------

Testing random images and filter

(ImageSize,FilterSize) = (7,3)
  NAIVE CONV PASSED.
   FAST CONV PASSED.
(ImageSize,FilterSize) = (7,5)
  NAIVE CONV PASSED.
   FAST CONV PASSED.
(ImageSize,FilterSize) = (7,1)
  NAIVE CONV PASSED.
   FAST CONV PASSED.
(ImageSize,FilterSize) = (7,7)
  NAIVE CONV PASSED.
   FAST CONV PASSED.
(ImageSize,FilterSize) = (16,7)
  NAIVE CONV PASSED.
   FAST CONV PASSED.
(ImageSize,FilterSize) = (32,11)
  NAIVE CONV PASSED.
   FAST CONV PASSED.
(ImageSize,FilterSize) = (64,11)
  NAIVE CONV PASSED.
   FAST CONV PASSED.
(ImageSize,FilterSize) = (55,9)
  NAIVE CONV PASSED.
   FAST CONV PASSED.
```
Please see the Makefile for instructions on how to get the required python configuration for C++ compilation, if the above make command has compilation issues.

## Executables
Two executables are created:   

* bin/testConv2D   
* pytests/pytest   

#### bin/testConv2D
Running just the command will print instructions on how to run it.    

* For running all the unit tests:   
```sh
$ bin/testConv2D -f tests/test.all
```
* For running one unit test:   
```sh
$ bin/testConv2D tests/test1.txt   
```
* For creating random image and running 2D convolution on it.   
```sh
$ bin/testConv2D -rand 7 3
```
#### pytests/pytest
This executable is specially created to verify the C++ authored conv2D implementation with python scipy.signal module convolve2d() method.    

* For running list of random tests and verifying them with scipy method.
```sh
$ cd pytests; ./pytest all ; cd ..
```

* For testing random image of a specific size of image and filter.
```sh
$ cd pytests; ./pytest 7 3 ; cd ..
```
## Implementation
class **Convolution2D** has the following methods:   

| Methods | Description |
| - | - |
| Constructor(imgSize, filterSize)  |  boilerplate size checking |
| convolve() | the first naive method that does 2D loop iteration |
| fastConvolve() | fast method after im2col copy |
| matrixMultipy() | modular method used in both the above methods |
| createRandImage() | creates random image matrix |
| createRandFilter() | creates random filter matrix |

class **EmbeddedPythonTest** has the following methods:   

| Methods | Description |
| - | - |
| Constructor | imports the python module "py_function.py" in pytests |
| testRandomConv2DCall(no args) | test many combinations of sizes of image and filter |
| testRandomConv2DCall(sizes) | test random image/filter for a particular size |
| getPyConv2DCall() | calls python scipy method from py_function.py |
| Other helper methods | floatCompare() etc. subroutine calls to assist above methods |

 class **UnitTest** has the following methods:   
 
 | Methods | Description |
 | - | - |
 | static testUnitFile() | reads input image, filter and output image from the file and compare the results with Convolution2D convolve method both fast and naive method |
 | static runRandomConv2D(sizes) | Creates random image and and filter and run 2D conv and prints the contents


##### Files containing main method()

###### **pytests/pytest.cpp**   

This always compares C++ authored implementation (both naive and fast) output results with python module scipy.signal convolve2d() results. 

The following command creates random images for list of various sizes and verifies the C++ authored implementation.   
```sh
$ cd pytests; ./pytests all; cd ..
```

The following command creates random images for selected image and filter size and verifies the authored implementation for those sizes.   
```sh
$ cd pytests; ./pytest 4 3; cd ..
```
   
###### **tests/test.cpp**
  
This compares with pre-selected unit test files, viz. tests/text\*.txt. The txt files have data for input image, filter and output image. This routine will pass input image and filter data to C++ authored implementation (both naive and fast) and compare the output received with test file output image data.    

To run all tests listed in tests/tests.all, use the following command:   
```sh
$ bin/testConv2D -f tests/tests.all
```

To run only one test.   
```sh
$ bin/testConv2D tests/test1.txt
```

The following command creates random image and filter with a specific size, and generates convolvedImage for both naive and fast method - compares them with each other, and prints them all.   
```sh
$ bin/testConv2D -rand <imageSize> <filterSize>
```
 
Please contact [Shomit Dutta](mailto:shomitdutta@gmail.com) for follow-up questions. 
