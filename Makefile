CC=g++
INCLUDE=-I$(PWD)/include
CFLAGS=-g -std=c++11 -I$(INCLUDE)

TARGET=conv2DTest
SRC=./src
TESTS=./tests
BUILDDIR=./build
BINDIR=./bin
INCLUDE=./include
MKDIR_P = mkdir -p
PYTESTS=./pytests

all: $(BINDIR) $(BINDIR)/unittest

$(BINDIR)/unittest: $(BUILDDIR)/Convolution2D.o $(BUILDDIR)/UnitTest.o $(BUILDDIR)/test.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SRC)/%.cpp 
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: $(TESTS)/%.cpp 
	$(CC) -c $(CFLAGS) $< -o $@

# Please call the command "python3-config --cflags" 
# to get the necessary cflags
$(BUILDDIR)/%.o: $(PYTESTS)/%.cpp 
	$(CC) -c $(CFLAGS) $< -o $@ -I/usr/include/python3.7m -I/usr/include/python3.7m -Wno-unused-result -Wsign-compare -g -fdebug-prefix-map=/build/python3.7-3.7.3=. -specs=/usr/share/dpkg/no-pie-compile.specs -fstack-protector -Wformat -Werror=format-security -DNDEBUG -g -fwrapv -O3 -Wall -fPIC

# Please call the command "python3-config --ldflags" 
# to get the necessary cflags
$(PYTESTS)/pytest: $(BUILDDIR)/pytest.o $(BUILDDIR)/EmbeddedPythonTest.o $(BUILDDIR)/Convolution2D.o
	$(CC) $(CFLAGS) $^ -o $@ -L/usr/lib/python3.7/config-3.7m-x86_64-linux-gnu -L/usr/lib -lpython3.7m -lcrypt -lpthread -ldl -lutil -lm -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions

$(BINDIR):
	$(MKDIR_P} $(OUTDIR)

clean:
	rm -rf $(BUILDDIR)/* $(BINDIR)/*

run: $(BINDIR)/unittest
	$(BINDIR)/unittest -f tests/tests.all

pytest: $(PYTESTS)/pytest
	cd $(PYTESTS); ./pytest all ; cd ..

.PHONY: directories clean run pytest
