## You should be present in $IPEX_PATH
cp $DLRM_SYSTEM/opt_designs/prefetching/EmbeddingBagKrnl.cpp $IPEX_PATH/intel_extension_for_pytorch/csrc/aten/cpu/kernels/EmbeddingBagKrnl.cpp
REL_WITH_DEB_INFO=1 USE_NATIVE_ARCH=1 CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=0" python setup.py install
