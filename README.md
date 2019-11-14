# hls4ml on Alveo U250 (HLS C/C++ Kernel)

Setup tools, licenses, check connection to FPGA card

Check out packages
```bash
# check out hls4ml_c SDAccel project
git clone https://github.com/FPGA4HEP/hls4ml_c -b hcal_model
```
Compile SDAccel project
```bash
cd hls4ml_c
make check TARGETS=sw_emu DEVICES=xilinx_u250_xdma_201830_2 all # software emulation
make check TARGETS=hw_emu DEVICES=xilinx_u250_xdma_201830_2 all # hardware emulation
make TARGETS=hw DEVICES=xilinx_u250_xdma_201830_2 all # build
```

Run project
```bash
./host 
```