module load gcc/8.1.0
module load cmake/3.15.3
module load boost/1.68
module load cuda/11.0

[ -d $PWD/build ] && rm -rf $PWD/build
mkdir $PWD/build
cd build
cmake \
  -DBOOST_ROOT=/share/apps/boost/1.68/no_mpi/ \
  -DNVML_LIBRARIES=/share/apps/cuda/11.0/lib64/stubs/libnvidia-ml.so \
  -DNVML_INCLUDE_DIRS=/share/apps/cuda/11.0/include/ \
  ..
make -j
cd -
