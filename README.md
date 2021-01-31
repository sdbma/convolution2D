# Implementation of Convolution2D in C++
---

## Overview
Convolution has become an integral part of neural networks[[Convolution Neural Network]][wiki]. Image and video recognition, Natural Language Processing are some of the applications that use convolution. It is imperative that the convolution has to be as fast as possible.

The blog ['How are Convolutions Actually Performed Under The Hood?'][medium] explains the need for a fast convolution operator. The layers associated with convolution can take upto 90% of the execution time, and thus it is important for the convolution to be fast.

The blog above explains 2 tricks that PyTorch and TensorFlow use to make convolution significantly faster. The first trick is using im2col operation and second is memory striding

There are two implementations in the repository. 
1. Naive implementation
2. Fast convolution

The performance difference between the two can be visualized as below:
![alt text](Isolated.png "Title")

The emphasis of this repository was to only show the difference between naive and fast convolution. So, a number of assumptions have been made in order to not stray away from the point. The convolutions are on square image and square filters, same output mode and have stride and dilation value of 1. Other matrix multiplication methods, e.g. kn2row can also be explored in the future.

### Implementation
##### Naive Convolution
Naive convolution is easy to understand, we simply traverse the 2D input matrix and pull out "windows"the size of kernel. For each window, we do "matrix-multiplication" using a separate matrix-multiplication function. The matrix multiplication can be made efficient by moving column less frequently because column addresses are not contiguous. The code in the implementation achieves that by moving the columns only after full iteration of dot product.

##### Fast Convolution
In the naive implementation, we did matrix-multiply the kernel with each window of the input matrix. In contrast, the fast convolution uses im2col to vectorize the entire operation. The im2col is a technique where we take each window, flatten it out and stack them as columns in a matrix. Now, matrix multiplication is done on the flattened matrix and flattened kernel. This is much faster than naive implementation as can be seen in the performance graph above.

### Verification
There are many ways to do verification of the convolution, e.g. using C++ libraries like opencv2. However, the repository took the approach of importing embedded python module scipy2 and comparing the implementation results with signal.convolve2d method. The python module scipy is an ecosystem, a collection of open source software for scientific computing.

## Directory Structure
The directory structure for this project is as follows:

1. include/
   > contains the source and test include files 
2. src/
   > contains the implementation of convolution2D
3. tests/
   > contains unit test source code and unit test gold files
4. pytests/
   > execute embedded python code for verification

This directory contains a Makefile and a README.   

## How to run UNIT tests

In order to build the source code and run unit tests, run the following commands:   
```sh
$ make
$ make run
```

## How to run Python Embedded Tests

In order to verify the authored code with embedded python library scipy.signal method convolve2D(), please use the following make command.
```sh
$ make pytest
```

Please see the Makefile for instructions on how to get the required python configuration for C++ compilation, if the above make command has compilation issues.

## Executables
Two executables are created:   

* bin/unittest   
* pytests/pytest   

#### bin/testConv2D
Running just the command will print instructions on how to run it. 
* For running all the unit tests:   
```sh
$ bin/unittest -f tests/test.all
```
* For running one unit test:   
```sh
$ bin/unittest tests/test1.txt   
```
* For creating random image and running 2D convolution on it.   
```sh
$ bin/unittest -rand 7 3
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


Please contact [Shomit Dutta](mailto:shomitdutta@gmail.com) for follow-up questions. 


License
----

GNU General Public Licence


**Free Software, Hell Yeah!**

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [wiki]: <https://en.wikipedia.org/wiki/Convolutional_neural_network#Convolutional_layers>
   [medium]: <https://towardsdatascience.com/how-are-convolutions-actually-performed-under-the-hood-226523ce7fbf>

