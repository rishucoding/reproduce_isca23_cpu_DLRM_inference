## You should be present in $TORCH_PATH
cp $DLRM_SYSTEM/opt_designs/ht/torch_files/thread_pool.{cpp,h} $TORCH_PATH/c10/core/
cp $DLRM_SYSTEM/opt_designs/ht/torch_files/throughput_benchmark.{cpp,h} $TORCH_PATH/torch/csrc/utils/
python setup.py clean
CMAKE_PREFIX_PATH=$CONDA_PREFIX BLAS=MKL USE_DISTRIBUTED=0 BUILD_TEST=0 BUILD_CAFFE2=0 REL_WITH_DEB_INFO=1 USE_CUDA=0 USE_NATIVE_ARCH=1 CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=0" python setup.py install
cp $DLRM_SYSTEM/dlrm_patches/dlrm_s_pytorch_mpht.py models/recommendation/pytorch/dlrm/product/dlrm_s_pytorch.py
