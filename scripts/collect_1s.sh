#!/bin/bash -xe
export LD_PRELOAD=$CONDA_PREFIX/lib/libiomp5.so:$CONDA_PREFIX/lib/libjemalloc.so
export MALLOC_CONF="oversize_threshold:1,background_thread:true,metadata_thp:auto,dirty_decay_ms:9000000000,muzzy_decay_ms:9000000000"
export OMP_NUM_THREADS=1
export KMP_AFFINITY=granularity=fine,compact,1,0
export KMP_BLOCKTIME=1
PyGetCore='import sys; c=int(sys.argv[1]); print(",".join(str(2*i) for i in range(c)))'
#PyGetCore='import sys; c=int(sys.argv[1]); print(",".join(str(2*i + off) for off in (0, 48) for i in range(c)))'
BOT_MLP=1024-512-256-128
TOP_MLP=256-128-64-1
ROW=500000
DATA_GEN=random
for i in {1..1}; do
	C=$(python -c "$PyGetCore" "$i")
	numactl -C $C -m 0 $CONDA_PREFIX/bin/python -u $MODELS_PATH/models/recommendation/pytorch/dlrm/product/dlrm_s_pytorch.py --data-generation=$DATA_GEN --round-targets=True --learning-rate=1.0 --arch-mlp-bot=$BOT_MLP --arch-mlp-top=$TOP_MLP --arch-sparse-feature-size=128 --max-ind-range=40000000 --ipex-interaction --numpy-rand-seed=727 --inference-only --num-batches=64 --data-size 100000000 --num-indices-per-lookup 120 --num-indices-per-lookup-fixed=True --arch-embedding-size $ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW-$ROW --print-freq=10 --print-time --mini-batch-size=128 --share-weight-instance=$i | tee -a log_1s.txt
done
