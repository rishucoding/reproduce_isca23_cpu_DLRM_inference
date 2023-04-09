## You should be present in $TORCH_PATH
git restore c10/core/thread_pool*
git restore torch/csrc/utils/throughput_benchmark*
python setup.py clean
CMAKE_PREFIX_PATH=$CONDA_PREFIX BLAS=MKL USE_DISTRIBUTED=0 BUILD_TEST=0 BUILD_CAFFE2=0 REL_WITH_DEB_INFO=1 USE_CUDA=0 USE_NATIVE_ARCH=1 CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=0" python setup.py install

