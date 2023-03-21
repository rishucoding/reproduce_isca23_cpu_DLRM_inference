# reproduce_isca23_cpu_DLRM_inference
Sharing the codebase and steps for artifact evaluation for ISCA 2023 paper


## Install Intel Vtune

* wget https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB
* sudo apt-key add GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB
* echo "deb https://apt.repos.intel.com/oneapi all main" | sudo tee /etc/apt/sources.list.d/oneAPI.list
* sudo apt update
* sudo apt install intel-oneapi-vtune
* source /opt/intel/oneapi/vtune/latest/env/vars.sh

## Install conda and setup env
* conda create --name dlrm_cpu python=3.9 ipython
* conda activate dlrm_cpu
* conda install -c conda-forge jemalloc
* conda install astunparse numpy ninja pyyaml setuptools cmake cffi typing_extensions future six requests dataclasses
* conda install mkl mkl-include
* pip install git+https://github.com/mlperf/logging
* pip install sklearn onnx lark-parser hypothesis tqdm scikit-learn

## Build PyTorch from source
* export CMAKE_PREFIX_PATH=${CONDA_PREFIX:-"$(dirname $(which conda))/../"}
* git clone --recursive -b v1.12.1 https://github.com/pytorch/pytorch
* cd pytorch
* export TORCH_PATH=$(pwd)
* REL_WITH_DEB_INFO=1 USE_NATIVE_ARCH=1 CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=0" USE_CUDA=0 python setup.py install


## Build IPEX
* cd ../
* git clone --recursive -b v1.12.300 https://github.com/intel/intel-extension-for-pytorch
* cd intel-extension-for-pytorch
* export IPEX_PATH=$(pwd)
* REL_WITH_DEB_INFO=1 USE_NATIVE_ARCH=1 CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=0" python setup.py install
  
## Build itt-python
* cd ../
* git clone https://github.com/NERSC/itt-python
* cd itt-python
* export VTUNE_PROFILER_DIR=/opt/intel/oneapi/vtune/latest
* python setup.py install --vtune=$VTUNE_PROFILER_DIR


## Setup DLRM inference
* cd ../
* git clone https://github.com/rishucoding/reproduce_isca23_cpu_DLRM_inference
* cd reproduce_isca23_cpu_DLRM_inference
* export DLRM_SYSTEM=$(pwd)
* git clone -b pytorch-r1.12-models https://github.com/IntelAI/models.git
* cd models
* export MODELS_PATH=$(pwd)
* cp $DLRM_SYSTEM/dlrm_patches/dlrm_data_pytorch.py models/recommendation/pytorch/dlrm/product/dlrm_data_pytorch.py
* cp $DLRM_SYSTEM/dlrm_patches/dlrm_s_pytorch.py models/recommendation/pytorch/dlrm/product/dlrm_s_pytorch.py
* cd $IPEX_PATH
* git apply $DLRM_SYSTEM/dlrm_patches/ipex.patch
* REL_WITH_DEB_INFO=1 USE_NATIVE_ARCH=1 CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=0" python setup.py install
* sudo apt-get install numactl

## Run DLRM inference  
* cd $MODELS_PATH
* cp $DLRM_SYSTEM/scripts/collect_1s.sh ./
* bash collect_1s.sh
NOTE: You can set the number of CPU cores to use, model configurations, and dataset paths in the collect_1s.sh script. 

## Enable MP-HT HyperThreading
* cd $TORCH_PATH 
* cp  $DLRM_SYSTEM/opt_designs/ht/torch_files/thread_pool.{cpp,h} $TORCH_PATH/c10/core/
* cp  $DLRM_SYSTEM/opt_designs/ht/torch_files/throughput_benchmark.{cpp,h} $TORCH_PATH/torch/csrc/utils/
* CMAKE_PREFIX_PATH=$CONDA_PREFIX BLAS=MKL USE_DISTRIBUTED=0 BUILD_TEST=0 BUILD_CAFFE2=0 REL_WITH_DEB_INFO=1 USE_CUDA=0 USE_NATIVE_ARCH=1 CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=0" python setup.py install


## Enable Software prefetch
* cp $DLRM_SYSTEM/opt_designs/prefetching/EmbeddingBagKrnl.cpp $IPEX_PATH/intel_extension_for_pytorch/csrc/aten/cpu/kernels/EmbeddingBagKrnl.cpp
* cd $IPEX_PATH
* REL_WITH_DEB_INFO=1 USE_NATIVE_ARCH=1 CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=0" python setup.py install

## Disable software prefetch
* cd $IPEX_PATH
* git restore intel_extension_for_pytorch/csrc/aten/cpu/kernels/EmbeddingBagKrnl.cpp
* REL_WITH_DEB_INFO=1 USE_NATIVE_ARCH=1 CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=0" python setup.py install

## Disable hardware prefetching
* git clone https://github.com/intel/msr-tools
* cd msr-tools
* sudo bash autogen.sh
* sudo ./wrmsr -a 0x1a4 15 #disables on all cores
* sudo ./wrmsr -a 0x1a4 0 #enables on all cores
* sudo ./wrmsr -p 0 0x1a4 15 #disables on core 0

## Disable turbo boost and set frequency
* bash $DLRM_SYSTEM/scripts/set_freq.sh











