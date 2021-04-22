
CXX ?= $(shell which g++)

BOOSTROOT ?= /share/apps/boost/1.68/no_mpi
CUDAROOT ?= /share/apps/cuda/11.0

WARNINGS := \
	-Wall -Wextra

BOOST_FLAGS := \
	-L$(BOOSTROOT)/lib \
	-L$(BOOSTROOT)/lib64 \
	-I$(BOOSTROOT)/include \
	$(BOOSTROOT)/lib/libboost_system.so \
	$(BOOSTROOT)/lib/libboost_iostreams.so \
	-DBOOST_ERROR_CODE_HEADER_ONLY

# NVML libraries are often under lib64/stubs for some reason
CUDA_FLAGS := \
	-L$(CUDAROOT)/lib \
	-L$(CUDAROOT)/lib64 \
	-L$(CUDAROOT)/lib64/stubs \
	-I$(CUDAROOT)/include \
	-lnvidia-ml

# Directories to be added to the rpath are in this variable, even though they
# logically belong with the cuda and boost variables above.
RPATH_FLAGS := \
	-Wl,-rpath,. \
	-Wl,-rpath,$(BOOSTROOT)/lib \
	-Wl,-rpath,$(BOOSTROOT)/lib64 \
	-Wl,-rpath,$(CUDAROOT)/lib \
	-Wl,-rpath,$(CUDAROOT)/lib64 \
	-Wl,-rpath,$(CUDAROOT)/lib64/stubs

CFLAGS := \
	$(RPATH_FLAGS) \
	$(WARNINGS) \
	$(CUDA_FLAGS) \
	$(BOOST_FLAGS) \
	-lpthread

SRC := driver.cpp NVMLWatcher.cpp
BIN := perses
PREFIX ?= $(shell pwd)/install

all: clean perses

perses:
	$(CXX) $(SRC) -o $(BIN) $(CFLAGS)

install: all
	if [ ! -d $(PREFIX)/bin ]; then \
		mkdir -p $(PREFIX)/bin; \
	fi
	install $(BIN) $(PREFIX)/bin

clean:
	$(RM) $(BIN)
