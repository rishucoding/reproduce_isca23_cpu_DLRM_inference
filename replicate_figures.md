Note: the scripts for obtaining the latency numbers are present in scripts directory. Also, the 1core and 24core scripts are separated.
Note: carefully grep the latency values obtained in the print_out.log file. Ensure to remove the .log files after every run. 

## Steps to replicate Figure 13
1. To collect baseline latency values: run scripts/fig13/core/normal_run_1c.sh and scripts/fig13/24core/normal_run_24.sh
2. To collect latency values with hardware prefetcher disabled: first disable the hardware prefetcher using steps shown in README.md, and then use (1)
3. To collect latency values with software prefetcher enabled: ensure hardware prefetcher is enabled, then enable software prefetch using the steps shown in README.md, and then use (1)
4. To collect latency values for DP-HT: ensure to disable software prefetch and enable hardware prefetcher, run scripts/fig13/1core/dp_ht_1c.sh and scripts/fig13/24core/dp_ht_24c.sh
5. To collect latency values for MP-HT: ensure to disable software prefetch and enable hardware prefetcher, enable MP-HT using the steps shown in README.md, and run scripts/fig13/1core/mp_ht_1c.sh and scripts/fig13/24core/mp_ht_24c.sh.
6. To collect latency values for integrated scheme: ensure hardware prefetcher is enabled, enable software prefetcher, and MP-HT scheme, and run scripts/fig13/1core/mp_ht_1c.sh and scripts/fig13/24core/mp_ht_24c.sh.


## Steps to replicate Figure 14
1. To collect baseline latency values: run scripts/fig14core/normal_run_1c.sh and scripts/fig1424core/normal_run_24.sh
2. To collect latency values with hardware prefetcher disabled: first disable the hardware prefetcher using steps shown in README.md, and then use (1)
3. To collect latency values with software prefetcher enabled: ensure hardware prefetcher is enabled, then enable software prefetch using the steps shown in README.md, and then use (1)
4. To collect latency values for DP-HT: ensure to disable software prefetch and enable hardware prefetcher, run scripts/fig14/1core/dp_ht_1c.sh and scripts/fig14/24core/dp_ht_24c.sh
5. To collect latency values for MP-HT: ensure to disable software prefetch and enable hardware prefetcher, enable MP-HT using the steps shown in README.md, and run scripts/fig14/1core/mp_ht_1c.sh and scripts/fig14/24core/mp_ht_24c.sh.
6. To collect latency values for integrated scheme: ensure hardware prefetcher is enabled, enable software prefetcher, and MP-HT scheme, and run scripts/fig14/1core/mp_ht_1c.sh and scripts/fig14/24core/mp_ht_24c.sh.

## Steps to replicate Figure 12
1. cp $DLRM_SYSTEM/dlrm_patches/dlrm_s_pytorch_emb_only.py models/recommendation/pytorch/dlrm/product/dlrm_s_pytorch.py
2. To collect baseline latency values: run scripts/fig13/core/normal_run_1c.sh and scripts/fig13/24core/normal_run_24.sh
3. To collect latency values with hardware prefetcher disabled: first disable the hardware prefetcher using steps shown in README.md, and then use (2). 
4. To collect latency values with software prefetcher enabled: ensure hardware prefetcher is enabled, then enable software prefetch using the steps shown in README.md, and then use (2)
