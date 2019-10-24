# hls4ml on Alveo U200 (HLS C/C++ Kernel)

Setup tools, licenses, check connection to FPGA card

Check out packages
```bash
# check out SDAccel example projects
git clone https://github.com/Xilinx/SDAccel_Examples -b b2884db9768d6589ae094cd06d9b491b3bd39816
# check out hls4ml
git clone https://github.com/hls-fpga-machine-learning/hls4ml -b tutorial
# check out hls4ml_c SDAccel project
git clone https://github.com/FPGA4HEP/hls4ml_c -b alveo
```

Create hls4ml project
```bash
cd ~/hls4ml
pip install . --user
cd example-models
export FAVOURITEMODEL=1layer
hls4ml convert -c keras-config-${FAVOURITEMODEL}.yml
```

Compile SDAccel project
```bash
cd ~/hls4ml_c
make check TARGETS=sw_emu DEVICES=xilinx_u200_xdma_201830_1 all # software emulation
make check TARGETS=hw_emu DEVICES=xilinx_u200_xdma_201830_1 all # hardware emulation
make TARGETS=hw DEVICES=xilinx_u200_xdma_201830_1 all # build
```

Run project
```bash
./host xclbin/alveo_hls4ml.hw.xilinx_u200_xdma_201830_1.xclbin
```