#!/bin/bash -xe
echo "1" | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo
sudo cpupower frequency-set -u 2400000 -d 2400000
