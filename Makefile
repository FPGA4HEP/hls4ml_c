#COMMON_REPO := ../../../
COMMON_REPO := /home/centos/src/project_data/aws-fpga/SDAccel/examples/xilinx/

#--v--v--
#these need to be set by the user for their specific installation
HLS4ML_NAME := ereg_v1
HLS4ML_PROJ_TYPE := DENSE
#possible options are: DENSE, CONV1D
#--^--^--
ifeq ($(filter $(HLS4ML_PROJ_TYPE),DENSE CONV1D),)
$(error invalid HLS4ML_PROJ_TYPE, must be DENSE or CONV1D)
endif

include $(COMMON_REPO)/utility/boards.mk
include $(COMMON_REPO)/libs/xcl2/xcl2.mk
include $(COMMON_REPO)/libs/opencl/opencl.mk

# Host Application
host_SRCS=./src/host.cpp $(xcl2_SRCS)
host_HDRS=$(xcl2_HDRS)
host_CXXFLAGS=-I./src/ -I./src/nnet_utils/ $(xcl2_CXXFLAGS) $(opencl_CXXFLAGS) -DIS_$(HLS4ML_PROJ_TYPE) -DHLS4ML_DATA_DIR=./ -std=c++11
host_LDFLAGS=$(opencl_LDFLAGS) -I$(XILINX_VIVADO)/include/ -I$(XILINX_SDACCEL)/include/ -Wno-unknown-pragmas

# aws_hls4ml Kernels
aws_hls4ml_SRCS=./src/aws_hls4ml.cpp src/ereg_v1.cpp
aws_hls4ml_CLFLAGS=-k aws_hls4ml -DMYPROJ=ereg_v1 -DIS_$(HLS4ML_PROJ_TYPE) -I./src/ -I./src/weights -I./src/nnet_utils/ --xp "prop:solution.hls_pre_tcl=./config.tcl" --xp "prop:kernel.aws_hls4ml.kernel_flags=-std=c++11"

EXES=host
XCLBINS=aws_hls4ml

XOS=aws_hls4ml

aws_hls4ml_XOS=aws_hls4ml

# check
check_EXE=host
check_XCLBINS=aws_hls4ml

CHECKS=check

include $(COMMON_REPO)/utility/rules.mk
