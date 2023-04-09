## Notes for using scripts: 

* Before using any script - source the paths.export file to load all the necessary environment variables

* collect_1s.sh : simple script to run standalone inference to test the working of setup.
* enable_MPHT.sh: It enables MP-HT design. You can use it in the $TORCH_PATH directory. 
* disable_MPHT.sh: It disables MP-HT design. You can use it in the $TORCH_PATH directory.
* enable_SWPF.sh: It enables software prefetch design. You can use it in the $IPEX_PATH directory. 
* disable_SWPF.sh: It disables software prefetch design. You can use it in the IPEX_PATH directory.
* enable_HWPF.sh: It enables intel hardware prefetcher. You can use it in the $HW_PF directory. 
* disable_HWPF.sh: It disables intel hardware prefetcher. You can use it in the $HW_PF directory.

* Steps to replicate figures is provided in the replicate_figures.md file. 

